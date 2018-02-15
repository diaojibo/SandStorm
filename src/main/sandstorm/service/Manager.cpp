//
// Created by rocklct on 2/15/18.
//


#include "sandstorm/service/Manager.h"
#include "sandstorm/message/CommandClient.h"
#include "sandstorm/util/NetConnector.h"
#include "sandstorm/util/Configuration.h"
#include "sandstorm/topology/Topology.h"
#include "sandstorm/topology/TopologyLoader.h"
#include "sandstorm/task/SpoutExecutor.h"
#include "sandstorm/task/BoltExecutor.h"
#include "sandstorm/spout/ISpout.h"
#include "sandstorm/bolt/IBolt.h"
#include "sandstorm/collector/OutputCollector.h"
#include "sandstorm/collector/OutputQueue.h"
#include "sandstorm/collector/TaskQueue.h"
#include "sandstorm/base/Constants.h"

namespace sandstorm {
    namespace service {
        static Manager *GlobalInstance = nullptr;

        void Manager::SetGlobalInstance(Manager *manager) {
            GlobalInstance = manager;
        }

        Manager *Manager::GetGlobalInstance() {
            return GlobalInstance;
        }

        Manager::Manager(const sandstorm::util::Configuration &configuration) :
                CommandServer(new sandstorm::util::NetListener(sandstorm::base::NetAddress(
                        configuration.GetProperty(CONF_KEY_MANAGER_HOST),
                        configuration.GetIntegerProperty(CONF_KEY_MANAGER_PORT)))),
                _host(configuration.GetProperty(CONF_KEY_MANAGER_HOST)),
                _port(configuration.GetIntegerProperty(CONF_KEY_MANAGER_PORT)) {
            _managerConfiguration.reset(new sandstorm::util::Configuration(configuration));
            _name = configuration.GetProperty(CONF_KEY_MANAGER_NAME);

            InitPresidentConnector();
            InitSelfContext();
            ReserveExecutors();
            InitEvents();
        }

        void Manager::InitPresidentConnector() {
            sandstorm::base::NetAddress presidentAddress(_managerConfiguration->GetProperty(CONF_KEY_PRESIDENT_PORT),
                                                         _managerConfiguration->GetIntegerProperty(
                                                                 CONF_KEY_PRESIDENT_PORT));
            _presidentConnector = new sandstorm::util::NetConnector(presidentAddress);
            _presidentClient = new sandstorm::message::CommandClient(_presidentConnector);
        }

        void Manager::ReserveExecutors() {
            _spoutExecutors.resize(_managerConfiguration->GetIntegerProperty(CONF_KEY_SPOUT_COUNT));
            _boltExecutors.resize(_managerConfiguration->GetIntegerProperty(CONF_KEY_BOLT_COUNT));
            _spoutCollectors.resize(_managerConfiguration->GetIntegerProperty(CONF_KEY_SPOUT_COUNT));
            _boltCollectors.resize(_managerConfiguration->GetIntegerProperty(CONF_KEY_BOLT_COUNT));
            _boltTaskQueues.resize(_managerConfiguration->GetIntegerProperty(CONF_KEY_BOLT_COUNT));

            for (auto &boltTask : _boltTaskQueues) {
                boltTask.reset(new collector::TaskQueue);
            }

            _outputDispatcher.SetQueue(std::shared_ptr<collector::OutputQueue>(
                    new collector::OutputQueue()));
            _outputDispatcher.SetSelfAddress(sandstorm::base::NetAddress(_host, _port));
            _outputDispatcher.SetSelfTasks(_boltTaskQueues);
            _outputDispatcher.SetSelfSpoutCount(static_cast<int32_t>(_spoutExecutors.size()));

            sandstorm::base::NetAddress presidentAddress(
                    _managerConfiguration->GetProperty(CONF_KEY_PRESIDENT_HOST),
                    _managerConfiguration->GetIntegerProperty(CONF_KEY_PRESIDENT_PORT));
            _presidentConnector = new sandstorm::util::NetConnector(presidentAddress);
            _presidentClient = new sandstorm::message::CommandClient(_presidentConnector);
            _outputDispatcher.SetPresidentClient(_presidentClient);

            _outputDispatcher.Start();
        }

        void Manager::InitEvents() {
            OnConnection(std::bind(&Manager::OnConnect, this, std::placeholders::_1));
            OnCommand(sandstorm::message::Command::Type::Heartbeat, this, &Manager::OnHeartbeat);
            OnCommand(sandstorm::message::Command::Type::SyncMetadata, this, &Manager::OnSyncMetadata);
            OnCommand(sandstorm::message::Command::Type::SendTuple, this, &Manager::OnSendTuple);
        }

        void Manager::InitTaskFieldsMap() {
            const std::map<std::string, sandstorm::spout::SpoutDeclarer> &spoutDeclarers =
                    _topology->GetSpoutDeclarers();
            for (const auto &spoutDeclarerPair : spoutDeclarers) {
                const spout::SpoutDeclarer &spoutDeclarer = spoutDeclarerPair.second;

                _taskFields[spoutDeclarer.GetTaskName()] = &spoutDeclarer.GetFields();
                _taskFieldsMap[spoutDeclarer.GetTaskName()] = &spoutDeclarer.GetFieldsMap();
            }

            const std::map<std::string, sandstorm::bolt::BoltDeclarer> &boltDeclarers =
                    _topology->GetBoltDeclarers();
            for (const auto &boltDeclarerPair : boltDeclarers) {
                const bolt::BoltDeclarer &boltDeclarer = boltDeclarerPair.second;

                _taskFields[boltDeclarer.GetTaskName()] = &boltDeclarer.GetFields();
                _taskFieldsMap[boltDeclarer.GetTaskName()] = &boltDeclarer.GetFieldsMap();
            }

            _outputDispatcher.SetTaskFields(_taskFields);
            _outputDispatcher.SetTaskFieldsMap(_taskFieldsMap);
        }

        void Manager::OnConnect(ManagerContext *context) {
        }

        void Manager::JoinPresident(JoinPresidentCallback callback) {
            sandstorm::message::CommandClient *commandClient = _presidentClient;

            _presidentConnector->Connect([commandClient, callback, this](const util::SocketError &) {
                sandstorm::message::Command command(sandstorm::message::Command::Type::Join);
                command.AddArgument({NODE_MANAGER});
                command.AddArgument({this->_host});
                command.AddArgument({this->_port});
                std::vector<sandstorm::base::Variant> context;
                _selfContext->Serialize(context);
                command.AddArguments(context);

                commandClient->SendCommand(command, [callback](const sandstorm::message::Response &response,
                                                               const message::CommandError &error) {
                    if (error.GetType() != message::CommandError::Type::NoError) {
                        std::cout << error.what();
                        return;
                    }

                    callback(response);
                });
            });
        }

        void Manager::OnHeartbeat(ManagerContext *context, const message::Command &command,
                                  sandstorm::message::CommandServer<sandstorm::message::BaseCommandServerContext>::Responsor Responsor) {
            sandstorm::message::Response response(sandstorm::message::Response::Status::Successful);
            response.AddArgument({_name});

            Responsor(response);
        }

        void Manager::OnSyncMetadata(ManagerContext *context, const message::Command &command,
                                     message::CommandServer<sandstorm::message::BaseCommandServerContext>::Responsor Responsor) {
            const std::vector<sandstorm::base::Variant> &arguments = command.GetArguments();

            int32_t syncMethod = arguments[0].GetInt32Value();
            if (syncMethod != 1) {
                sandstorm::message::Response response(sandstorm::message::Response::Status::Failed);
                Responsor(response);

                return;
            }

            sandstorm::message::Response response(sandstorm::message::Response::Status::Successful);
            base::Variants::const_iterator currentIterator = arguments.cbegin() + 1;
            _selfContext->Deserialize(currentIterator);

            OwnManagerTasks();
            _outputDispatcher.SetTaskInfos(_selfContext->GetTaskInfos());

            ShowManagerMetadata();
            ShowTaskInfos();

            std::string topologyName = _managerConfiguration->GetProperty(CONF_KEY_TOPOLOGY_NAME);
            _topology = sandstorm::topology::TopologyLoader::GetInstance().GetTopology(topologyName);

            InitTaskFieldsMap();
            InitExecutors();

            Responsor(response);
        }

        void Manager::OnSendTuple(ManagerContext *context, const message::Command &command,
                                  message::CommandServer<sandstorm::message::BaseCommandServerContext>::Responsor Responsor) {
            const base::Variants &arguments = command.GetArguments();
            base::Variants::const_iterator it = arguments.cbegin();

            base::NetAddress sourceAddress;
            sourceAddress.Deserialize(it);

            task::ExecutorPosition destination;
            destination.Deserialize(it);

            base::Tuple tuple;
            tuple.Deserialize(it);
            tuple.SetFields(_taskFields[tuple.GetSourceTask()]);
            tuple.SetFieldsMap(_taskFieldsMap[tuple.GetSourceTask()]);

            int32_t executorIndex = destination.GetExecutorIndex();
            int32_t boltIndex = executorIndex - _selfContext->GetSpoutCount();

            std::shared_ptr<sandstorm::collector::TaskQueue> taskQueue = _boltTaskQueues[boltIndex];
            collector::TaskItem *taskItem =
                    new collector::TaskItem(executorIndex, tuple);
            taskQueue->Push(taskItem);

            sandstorm::message::Response response(sandstorm::message::Response::Status::Successful);
            Responsor(response);
        }

        sandstorm::message::CommandClient *Manager::CreatePresidentClient() {
            sandstorm::base::NetAddress presidentAddress(_managerConfiguration->GetProperty(CONF_KEY_PRESIDENT_PORT),
                                                         _managerConfiguration->GetIntegerProperty(
                                                                 CONF_KEY_PRESIDENT_PORT));
            sandstorm::util::NetConnector *presidentConnector =
                    new sandstorm::util::NetConnector(presidentAddress);
            sandstorm::message::CommandClient *presidentClient =
                    new sandstorm::message::CommandClient(presidentConnector);

            return presidentClient;
        }

        std::string Manager::GetTopologyName() const {
            return _topology->GetName();
        }

        void Manager::InitSelfContext() {
            this->_selfContext.reset(new ManagerContext);
            _selfContext->SetId(_name);
            _selfContext->SetSpoutCount(_managerConfiguration->GetIntegerProperty(CONF_KEY_SPOUT_COUNT));
            _selfContext->SetBoltCount(_managerConfiguration->GetIntegerProperty(CONF_KEY_BOLT_COUNT));
            _selfContext->SetTaskInfos(std::vector<sandstorm::task::TaskInfo>(
                    _selfContext->GetSpoutCount() + _selfContext->GetBoltCount()));

            std::set<int32_t> freeSpouts;
            int spoutCount = _selfContext->GetSpoutCount();
            for (int32_t spoutIndex = 0; spoutIndex != spoutCount; ++spoutIndex) {
                freeSpouts.insert(spoutIndex);
            }
            _selfContext->SetFreeSpouts(freeSpouts);

            std::set<int32_t> freeBolts;
            int boltCount = _selfContext->GetBoltCount();
            for (int32_t boltIndex = 0; boltIndex != boltCount; ++boltIndex) {
                freeBolts.insert(boltIndex);
            }
            _selfContext->SetFreeBolts(freeBolts);
        }

        void Manager::InitSpoutExecutors() {
            std::cout << "DEBUG:Init spout executors";
            const std::map<std::string, sandstorm::spout::SpoutDeclarer> &spoutDeclarers =
                    _topology->GetSpoutDeclarers();
            std::set<int32_t> busySpouts = _selfContext->GetBusySpouts();
            for (int32_t spoutIndex : busySpouts) {
                sandstorm::task::TaskInfo &spoutTask = _selfContext->GetSpoutTaskInfo(spoutIndex);
                std::string taskName = spoutTask.GetTaskName();
                const sandstorm::spout::SpoutDeclarer &spoutDeclarer = spoutDeclarers.at(taskName);

                std::shared_ptr<collector::OutputQueue> outputQueue = _outputDispatcher.GetQueue();
                collector::OutputCollector *collector = new collector::OutputCollector(spoutIndex,
                                                                                       taskName, outputQueue);
                _spoutCollectors[spoutIndex].reset(collector);

                spout::ISpout *spout = spoutDeclarer.GetSpout()->Clone();
                spout->Prepare(_spoutCollectors[spoutIndex]);

                std::shared_ptr<task::SpoutExecutor> spoutExecutor(new task::SpoutExecutor);
                spoutExecutor->SetSpout(spout);
                int32_t flowParam = _managerConfiguration->GetIntegerProperty(CONF_KEY_SPOUT_FLOW_PARAM);
                spoutExecutor->SetFlowParam(flowParam);
                _spoutExecutors[spoutIndex] = spoutExecutor;
            }
        }

        void Manager::InitBoltExecutors() {
            std::cout << "DEBUG: Init bolt executors";
            const std::map<std::string, sandstorm::bolt::BoltDeclarer> &boltDeclarers =
                    _topology->GetBoltDeclarers();
            std::set<int32_t> busyBolts = _selfContext->GetBusyBolts();
            int32_t spoutCount = _selfContext->GetSpoutCount();
            for (int32_t boltIndex : busyBolts) {
                std::cout << boltIndex;


                sandstorm::task::TaskInfo &boltTask = _selfContext->GetBoltTaskInfo(boltIndex);
                std::string taskName = boltTask.GetTaskName();
                const sandstorm::bolt::BoltDeclarer &boltDeclarer = boltDeclarers.at(taskName);

                std::shared_ptr<collector::OutputQueue> outputQueue = _outputDispatcher.GetQueue();
                collector::OutputCollector *collector = new collector::OutputCollector(
                        spoutCount + boltIndex, taskName, outputQueue);
                _boltCollectors[boltIndex].reset(collector);

                bolt::IBolt *bolt = boltDeclarer.GetBolt()->Clone();
                bolt->Prepare(_boltCollectors[boltIndex]);

                std::shared_ptr<task::BoltExecutor> boltExecutor(new task::BoltExecutor);
                _boltExecutors[boltIndex] = boltExecutor;
                boltExecutor->SetTaskQueue(_boltTaskQueues[boltIndex]);
                boltExecutor->SetBolt(bolt);
            }
        }

        void Manager::InitExecutors() {
            InitSpoutExecutors();
            InitBoltExecutors();

            std::set<int32_t> busyBolts = _selfContext->GetBusyBolts();
            std::set<int32_t> busySpouts = _selfContext->GetBusySpouts();

            for (int32_t boltIndex : busyBolts) {
                _boltExecutors[boltIndex]->Start();
            }

            for (int32_t spoutIndex : busySpouts) {
                _spoutExecutors[spoutIndex]->Start();
            }
        }

        void Manager::OwnManagerTasks() {
            std::vector<sandstorm::task::TaskInfo> &taskInfos = _selfContext->GetTaskInfos();
            for (sandstorm::task::TaskInfo &taskInfo : taskInfos) {
                taskInfo.SetManagerContext(_selfContext.get());
            }
        }

        void Manager::ShowManagerMetadata() {
            std::cout << "Manager name: " << _selfContext->GetId();
            std::cout << "  Spout count: " << _selfContext->GetSpoutCount();
            std::cout << "  Bolt count: " << _selfContext->GetBoltCount();
            std::cout << "  Task info count: " << _selfContext->GetTaskInfos().size();
            std::cout << "  Free spout count: " << _selfContext->GetFreeSpouts().size();
            std::cout << "  Free bolt count: " << _selfContext->GetFreeBolts().size();
            std::cout << "  Busy spout count: " << _selfContext->GetBusySpouts().size();
            std::cout << "  Busy bolt count: " << _selfContext->GetBusyBolts().size();
        }

        void Manager::ShowTaskInfos() {
            const std::vector<sandstorm::task::TaskInfo> &taskInfos = _selfContext->GetTaskInfos();
            for (const sandstorm::task::TaskInfo &taskInfo : taskInfos) {
                if (!taskInfo.GetManagerContext()) {
                    continue;
                }

                std::cout << "    Manager: " << taskInfo.GetManagerContext()->GetId();
                std::cout << "    Exectuor index: " << taskInfo.GetExecutorIndex();
                std::cout << "    Task name: " << taskInfo.GetTaskName();
                std::cout << "    Paths: ";
                const std::list<sandstorm::task::PathInfo> &paths = taskInfo.GetPaths();

                for (const sandstorm::task::PathInfo &path : paths) {
                    std::cout << "      Path: ";
                    int32_t groupMethod = path.GetGroupMethod();
                    std::cout << "        Group method: " << groupMethod;

                    if (path.GetGroupMethod() == sandstorm::task::PathInfo::GroupMethod::Global) {
                        std::cout << "        Destination host: " <<
                                  path.GetDestinationExecutors()[0].GetManager().GetHost();
                        std::cout << "        Destination port: " <<
                                  path.GetDestinationExecutors()[0].GetManager().GetPort();
                        std::cout << "        Destination executor index: " <<
                                  path.GetDestinationExecutors()[0].GetExecutorIndex();
                    }
                }
            }
        }
    }
}