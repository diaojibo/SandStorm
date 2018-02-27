//
// Created by rocklct on 2/18/18.
//

#include "sandstorm/service/President.h"
#include "sandstorm/util/NetConnector.h"
#include "sandstorm/message/CommandClient.h"
#include "sandstorm/util/Configuration.h"
#include "sandstorm/topology/Topology.h"
#include "sandstorm/topology/TopologyLoader.h"
#include "sandstorm/base/Constants.h"


#include <iostream>
#include <thread>
#include <chrono>
#include <map>
#include <list>
#include <cassert>


namespace sandstorm {
    namespace service {
        const int32_t MAX_HEARTBEAT_FAILED_TIMES = 5;

        President::President(const sandstorm::base::NetAddress &host) :
                CommandServer(new sandstorm::util::NetListener(host)),
                _presidentHost(host),
                _managerCount(0),
                _submitted(false) {
            OnConnection(std::bind(&President::OnConnect, this, std::placeholders::_1));
            OnCommand(sandstorm::message::Command::Type::Join, this, &President::OnJoin);
            OnCommand(sandstorm::message::Command::Type::AskField, this, &President::OnAskField);
            OnCommand(sandstorm::message::Command::Type::OrderId, this, &President::OnOrderId);
        }

        President::President(const sandstorm::util::Configuration &configuration) :
                President(sandstorm::base::NetAddress(
                        configuration.GetProperty(CONF_KEY_PRESIDENT_HOST),
                        configuration.GetIntegerProperty(CONF_KEY_PRESIDENT_PORT))) {
            _managerCount = configuration.GetIntegerProperty(CONF_KEY_MANAGER_COUNT);
            _configuration.reset(new sandstorm::util::Configuration(configuration));


            std::cout << "Need managers: " << _managerCount << std::endl;
        }


        // The function to be called after the connection
        void President::OnConnect(ManagerContext *context) {
        }

        void President::OnJoin(ManagerContext *context, const sandstorm::message::Command &command,
                               sandstorm::message::CommandServer<sandstorm::message::BaseCommandServerContext>::Responsor Responsor) {
            std::string joinerType = command.GetArgument(0).GetStringValue();
            std::string managerHost = command.GetArgument(1).GetStringValue();
            int32_t managerPort = command.GetArgument(2).GetInt32Value();

            std::cout << "Join node: " << joinerType << std::endl;

            ManagerContext managerContext;
            base::Variants::const_iterator currentIterator = command.GetArguments().cbegin() + 3;
            managerContext.Deserialize(currentIterator);

            std::cout << "Manager name: " << managerContext.GetId() << std::endl;
            std::cout << "Host: " << managerHost << std::endl;
            std::cout << "Port: " << managerPort << std::endl;
            std::cout << "Spout count: " << managerContext.GetSpoutCount() << std::endl;
            std::cout << "Bolt count: " << managerContext.GetBoltCount() << std::endl;
            std::cout << "Task info count: " << managerContext.GetTaskInfos().size() << std::endl;
            std::cout << "Free spout count: " << managerContext.GetFreeSpouts().size() << std::endl;
            std::cout << "Free bolt count: " << managerContext.GetFreeBolts().size() << std::endl;
            std::cout << "Busy spout count: " << managerContext.GetBusySpouts().size() << std::endl;
            std::cout << "Busy bolt count: " << managerContext.GetBusyBolts().size() << std::endl;

            managerContext.SetNetAddress(sandstorm::base::NetAddress(
                    managerHost, managerPort));
            managerContext.PrepareTaskInfos();
            _managers.push_back(managerContext);

            // Response
            sandstorm::message::Response response(sandstorm::message::Response::Status::Successful);
            response.AddArgument({NODE_PRESIDENT});

            Responsor(response);

            // Initialize command clients
            sandstorm::base::NetAddress managerAddress(managerHost,
                                                       managerPort);
            sandstorm::util::NetConnector *managerConnector =
                    new sandstorm::util::NetConnector(managerAddress);
            sandstorm::message::CommandClient *managerCommandClient =
                    new sandstorm::message::CommandClient(managerConnector);

            _managerClients.insert({managerContext.GetId(),
                                    std::shared_ptr<sandstorm::message::CommandClient>(managerCommandClient)});

            SendHeartbeat(managerContext.GetId(), 0);

            // Initialize topology
            if (_managers.size() == _managerCount) {
                std::string topologyName = _configuration->GetProperty(CONF_KEY_TOPOLOGY_NAME);
                sandstorm::topology::Topology *topology =
                        sandstorm::topology::TopologyLoader::GetInstance().GetTopology(topologyName).get();
                SubmitTopology(topology);
            }
        }

        void President::OnAskField(ManagerContext *context, const sandstorm::message::Command &command,
                                   sandstorm::message::CommandServer<sandstorm::message::BaseCommandServerContext>::Responsor Responsor) {
            std::string sourceTaskName = command.GetArgument(0).GetStringValue();
            std::string destTaskName = command.GetArgument(1).GetStringValue();
            TaskPathName taskPathName = {sourceTaskName, destTaskName};
            std::string fieldValue = command.GetArgument(2).GetStringValue();

            auto taskPairIter = _fieldsDestinations.find(taskPathName);
            if (taskPairIter == _fieldsDestinations.end()) {
                _fieldsDestinations.insert({taskPathName, std::map<std::string, task::ExecutorPosition>()});
                taskPairIter = _fieldsDestinations.find(taskPathName);
            }

            std::map<std::string, task::ExecutorPosition> &destinations = taskPairIter->second;
            auto destinationPairIter = destinations.find(fieldValue);
            if (destinationPairIter == destinations.end()) {
                std::vector<task::ExecutorPosition> &candidates = _fieldsCandidates[taskPathName];
                int32_t positionIndex = rand() % candidates.size();

                destinations.insert({fieldValue, candidates[positionIndex]});
                destinationPairIter = destinations.find(fieldValue);
            }

            task::ExecutorPosition destination = destinationPairIter->second;
            base::Variants destinationVariants;
            destination.Serialize(destinationVariants);

            sandstorm::message::Response response(sandstorm::message::Response::Status::Successful);
            response.AddArguments(destinationVariants);

            Responsor(response);
        }

        void President::OnOrderId(ManagerContext *context, const sandstorm::message::Command &command,
                                  sandstorm::message::CommandServer<sandstorm::message::BaseCommandServerContext>::Responsor Responsor) {
            std::string topologyName = command.GetArgument(0).GetStringValue();

            if (_orderIds.find(topologyName) == _orderIds.end()) {
                _orderIds[topologyName] = 1;
            }

            int64_t orderId = _orderIds[topologyName];
            _orderIds[topologyName] = orderId + 1;

            sandstorm::message::Response response(sandstorm::message::Response::Status::Successful);
            response.AddArgument(orderId);

            Responsor(response);
        }

        std::list<task::TaskInfo> President::GetAllSpoutTasks(
                const std::map<std::string, sandstorm::spout::SpoutDeclarer> &spoutDeclarers,
                sandstorm::topology::Topology *topology) {
            std::list<sandstorm::task::TaskInfo> originSpoutTasks;
            for (const auto &spoutPair : spoutDeclarers) {
                sandstorm::spout::SpoutDeclarer spoutDeclarer = spoutPair.second;
                std::cout << "Spout " << spoutDeclarer.GetTaskName() << std::endl;
                std::cout << "ParallismHint: " << spoutDeclarer.GetParallismHint() << std::endl;

                int32_t parallismHint = spoutDeclarer.GetParallismHint();
                for (int32_t taskIndex = 0; taskIndex != parallismHint; ++taskIndex) {
                    sandstorm::task::TaskInfo taskInfo;
                    taskInfo.SetTopologyName(topology->GetName());
                    taskInfo.SetTaskName(spoutDeclarer.GetTaskName());

                    originSpoutTasks.push_back(taskInfo);
                }
            }

            return originSpoutTasks;
        }

        std::map<std::string, std::vector<sandstorm::task::TaskInfo *>>
        President::AllocateSpoutTasks(std::list<sandstorm::task::TaskInfo> &originSpoutTasks) {
            std::map<std::string, std::vector<sandstorm::task::TaskInfo *>> nameToSpoutTasks;
            // Allocate task for every manager
            for (ManagerContext &managerContext : _managers) {
                if (!originSpoutTasks.size()) {
                    break;
                }

                while (true) {
                    if (!originSpoutTasks.size()) {
                        break;
                    }

                    // If useNextSpout return -1, the spout slots is used up
                    int32_t spoutIndex = managerContext.useNextSpout();
                    if (spoutIndex == -1) {
                        break;
                    }

                    // Put the spout task into spout slot
                    sandstorm::task::TaskInfo taskInfo = originSpoutTasks.front();
                    taskInfo.SetManagerContext(&managerContext);
                    taskInfo.SetExecutorIndex(managerContext.GetExecutorIndex(
                            ManagerContext::ExecutorType::Spout, spoutIndex));
                    originSpoutTasks.pop_front();
                    managerContext.SetSpoutTaskInfo(spoutIndex, taskInfo);

                    // Insert the spout task pointer into mapper
                    std::string taskName = taskInfo.GetTaskName();
                    auto spoutTasksPair = nameToSpoutTasks.find(taskName);
                    if (spoutTasksPair == nameToSpoutTasks.end()) {
                        nameToSpoutTasks.insert({taskName, std::vector<sandstorm::task::TaskInfo *>()});
                        spoutTasksPair = nameToSpoutTasks.find(taskName);
                    }

                    spoutTasksPair->second.push_back(&(managerContext.GetSpoutTaskInfo(spoutIndex)));
                }
            }

            return nameToSpoutTasks;
        }

        std::list<task::TaskInfo> President::GetAllBoltTasks(sandstorm::topology::Topology *topology,
                                                             const std::map<std::string, sandstorm::bolt::BoltDeclarer> &boltDeclarers) {
            std::list<sandstorm::task::TaskInfo> originBoltTasks;
            for (const auto &boltPair : boltDeclarers) {
                sandstorm::bolt::BoltDeclarer boltDeclarer = boltPair.second;
                std::cout << "Bolt " << boltDeclarer.GetTaskName() << std::endl;
                std::cout << "Source: " << boltDeclarer.GetSourceTaskName() << std::endl;
                std::cout << "ParallismHint: " << boltDeclarer.GetParallismHint() << std::endl;

                int32_t parallismHint = boltDeclarer.GetParallismHint();
                for (int32_t taskIndex = 0; taskIndex != parallismHint; ++taskIndex) {
                    sandstorm::task::TaskInfo taskInfo;
                    taskInfo.SetTopologyName(topology->GetName());
                    taskInfo.SetTaskName(boltDeclarer.GetTaskName());

                    originBoltTasks.push_back(taskInfo);
                }
            }

            return originBoltTasks;
        }

        std::map<std::string, std::vector<sandstorm::task::TaskInfo *>>
        President::AllocateBoltTasks(std::list<sandstorm::task::TaskInfo> &originBoltTasks) {
            std::map<std::string, std::vector<sandstorm::task::TaskInfo *>> nameToBoltTasks;
            // Allocate bolt tasks
            for (ManagerContext &managerContext : _managers) {
                if (!originBoltTasks.size()) {
                    break;
                }

                while (true) {
                    if (!originBoltTasks.size()) {
                        break;
                    }

                    // If useNextBolt return -1, the bolt slots is used up
                    int32_t boltIndex = managerContext.useNextBolt();
                    if (boltIndex == -1) {
                        break;
                    }

                    // Put the bolt task into bolt slot
                    sandstorm::task::TaskInfo taskInfo = originBoltTasks.front();
                    taskInfo.SetManagerContext(&managerContext);
                    taskInfo.SetExecutorIndex(managerContext.GetExecutorIndex(
                            ManagerContext::ExecutorType::Bolt, boltIndex));
                    originBoltTasks.pop_front();
                    managerContext.SetBoltTaskInfo(boltIndex, taskInfo);

                    // Insert the bolt task pointer into mapper
                    std::string taskName = taskInfo.GetTaskName();
                    auto boltTasksPair = nameToBoltTasks.find(taskName);
                    if (boltTasksPair == nameToBoltTasks.end()) {
                        nameToBoltTasks.insert({taskName, std::vector<sandstorm::task::TaskInfo *>()});
                        boltTasksPair = nameToBoltTasks.find(taskName);
                    }

                    boltTasksPair->second.push_back(&(managerContext.GetBoltTaskInfo(boltIndex)));
                }
            }

            return nameToBoltTasks;
        }

        std::vector<task::TaskInfo *> President::FindTask(
                const std::map<std::string, std::vector<task::TaskInfo *>> &nameToBoltTasks,
                const std::map<std::string, std::vector<task::TaskInfo *>> &nameToSpoutTasks,
                const std::string &sourceTaskName) {
            auto spoutTaskPair = nameToSpoutTasks.find(sourceTaskName);
            if (spoutTaskPair != nameToSpoutTasks.end()) {
                return spoutTaskPair->second;
            }

            auto boltTaskPair = nameToBoltTasks.find(sourceTaskName);
            if (boltTaskPair != nameToBoltTasks.end()) {
                return boltTaskPair->second;
            }

            return std::vector<task::TaskInfo *>();
        }

        std::vector<task::TaskInfo *> President::FindTask(
                const std::map<std::string, std::vector<task::TaskInfo *>> &nameToBoltTasks,
                const std::string &sourceTaskName) {
            auto boltTaskPair = nameToBoltTasks.find(sourceTaskName);
            if (boltTaskPair != nameToBoltTasks.end()) {
                return boltTaskPair->second;
            }

            return std::vector<task::TaskInfo *>();
        }

        void President::ShowTaskInfos(const std::vector<sandstorm::task::TaskInfo> &taskInfos) {
            for (const sandstorm::task::TaskInfo &taskInfo : taskInfos) {
                if (!taskInfo.GetManagerContext()) {
                    continue;
                }

                std::cout << "    Manager: " << taskInfo.GetManagerContext()->GetId() << std::endl;
                std::cout << "    Exectuor index: " << taskInfo.GetExecutorIndex() << std::endl;
                std::cout << "    Paths: " << std::endl;
                const std::list<sandstorm::task::PathInfo> &paths = taskInfo.GetPaths();

                for (const sandstorm::task::PathInfo &path : paths) {
                    std::cout << "      Path: " << std::endl;
                    int32_t groupMethod = path.GetGroupMethod();
                    std::cout << "        Group method: " << groupMethod << std::endl;
                    if (path.GetGroupMethod() == sandstorm::task::PathInfo::GroupMethod::Global) {
                        std::cout << "        Destination host: " <<
                                  path.GetDestinationExecutors()[0].GetManager().GetHost() << std::endl;
                        std::cout << "        Destination port: " <<
                                  path.GetDestinationExecutors()[0].GetManager().GetPort() << std::endl;
                        std::cout << "        Destination executor index: " <<
                                  path.GetDestinationExecutors()[0].GetExecutorIndex() << std::endl;
                    }
                }
            }
        }

        void President::SyncWithManagers() {
            for (ManagerContext &managerContext : _managers) {
                std::string managerId = managerContext.GetId();
                std::cout << "Sync meta data with supervisr: " << managerId << std::endl;
                std::shared_ptr<sandstorm::message::CommandClient> managerClient =
                        _managerClients[managerId];

                managerClient->GetConnector()->Connect([managerId, managerClient, &managerContext, this]
                                                               (const util::SocketError &) {
                    sandstorm::message::Command command(sandstorm::message::Command::Type::SyncMetadata);

                    // 1 means President to Manager
                    // 2 means Manager to President
                    command.AddArgument({1});

                    base::Variants managerContextVariants;
                    managerContext.Serialize(managerContextVariants);
                    command.AddArguments(managerContextVariants);
                    managerClient->SendCommand(command,
                                               [managerId, this](const sandstorm::message::Response &response,
                                                                 const message::CommandError &error) -> void {
                                                   if (error.GetType() != message::CommandError::Type::NoError) {
                                                       std::cout << error.what() << std::endl;
                                                       return;
                                                   }

                                                   if (response.GetStatus() ==
                                                       sandstorm::message::Response::Status::Successful) {
                                                       std::cout << "Sync with " << managerId << " successfully."
                                                                 << std::endl;
                                                   } else {
                                                       std::cout << "Sync with " << managerId << " failed."
                                                                 << std::endl;
                                                   }
                                               });
                });
            }
        }

        void President::StartConnectionChecker() {
            _connectionChecker = std::thread(&President::CheckConnections, this);
        }

        void President::CheckConnections() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(10));

                for (ManagerContext &managerContext : _managers) {
                    std::cout << "Check " << managerContext.GetId() << std::endl;
                    SendHeartbeat(managerContext.GetId(), 0);
                }
            }
        }

        void President::ShowManagerTaskInfos() {
            std::cout << "================ Allocate result ================" << std::endl;
            for (ManagerContext &managerContext : _managers) {
                std::cout << managerContext.GetId() << std::endl;
                std::cout << "  Host: " << managerContext.GetNetAddress().GetHost() << std::endl;
                std::cout << "  Port: " << managerContext.GetNetAddress().GetPort() << std::endl;

                std::cout << "  Tasks: " << std::endl;
                const std::vector<sandstorm::task::TaskInfo> &taskInfos =
                        managerContext.GetTaskInfos();
                ShowTaskInfos(taskInfos);
            }
        }

        void President::CalculateTaskPaths(
                const std::map<std::string, std::vector<sandstorm::task::TaskInfo *>> &nameToBoltTasks,
                const std::map<std::string, sandstorm::bolt::BoltDeclarer> &boltDeclarers,
                const std::map<std::string, std::vector<sandstorm::task::TaskInfo *>> &nameToSpoutTasks) {
            for (const auto &boltPair : boltDeclarers) {
                sandstorm::bolt::BoltDeclarer boltDeclarer = boltPair.second;
                // No setted source task
                if (boltDeclarer.GetSourceTaskName().empty()) {
                    continue;
                }

                std::string sourceTaskName = boltDeclarer.GetSourceTaskName();
                std::vector<sandstorm::task::TaskInfo *> sourceTasks =
                        FindTask(nameToBoltTasks, nameToSpoutTasks, sourceTaskName);

                std::string destTaskName = boltDeclarer.GetTaskName();
                std::vector<sandstorm::task::TaskInfo *> destTasks =
                        FindTask(nameToBoltTasks, destTaskName);

                std::vector<task::ExecutorPosition> destExecutorPositions;
                for (sandstorm::task::TaskInfo *destTask : destTasks) {
                    destExecutorPositions.push_back(task::ExecutorPosition(
                            destTask->GetManagerContext()->GetNetAddress(),
                            destTask->GetExecutorIndex()
                    ));
                }

                if (boltDeclarer.GetGroupMethod() == task::TaskDeclarer::GroupMethod::Global) {
                    for (sandstorm::task::TaskInfo *sourceTask : sourceTasks) {
                        int32_t destTaskIndex = rand() % destTasks.size();
                        sandstorm::task::TaskInfo *destTask = destTasks[destTaskIndex];

                        sandstorm::task::PathInfo pathInfo;
                        pathInfo.SetGroupMethod(sandstorm::task::PathInfo::GroupMethod::Global);
                        pathInfo.SetDestinationTask(destTask->GetTaskName());
                        pathInfo.SetDestinationExecutors({task::ExecutorPosition(
                                destTask->GetManagerContext()->GetNetAddress(),
                                destTask->GetExecutorIndex()
                        )});

                        sourceTask->AddPath(pathInfo);
                    }
                } else if (boltDeclarer.GetGroupMethod() == task::TaskDeclarer::GroupMethod::Field) {
                    // Resolve the destination by field when run task.
                    for (sandstorm::task::TaskInfo *sourceTask : sourceTasks) {
                        sandstorm::task::PathInfo pathInfo;
                        pathInfo.SetGroupMethod(sandstorm::task::PathInfo::GroupMethod::Field);
                        pathInfo.SetDestinationTask(destTaskName);
                        pathInfo.SetFieldName(boltDeclarer.GetGroupField());

                        sourceTask->AddPath(pathInfo);
                    }

                    TaskPathName taskPathName = {sourceTaskName, destTaskName};
                    _fieldsCandidates[taskPathName] = destExecutorPositions;
                } else if (boltDeclarer.GetGroupMethod() == task::TaskDeclarer::GroupMethod::Random) {
                    // Resolve the destination by field when run task.
                    for (sandstorm::task::TaskInfo *sourceTask : sourceTasks) {
                        sandstorm::task::PathInfo pathInfo;
                        pathInfo.SetGroupMethod(sandstorm::task::PathInfo::GroupMethod::Random);
                        pathInfo.SetDestinationTask(destTaskName);
                        pathInfo.SetDestinationExecutors(destExecutorPositions);

                        sourceTask->AddPath(pathInfo);
                    }
                } else {
                    std::cout << "Unsupported group method occured" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }

        void President::ShowManagerMetadata() {
            std::cout << "================ Manager metadata ================" << std::endl;
            for (ManagerContext &managerContext : _managers) {
                std::cout << "Manager name: " << managerContext.GetId() << std::endl;
                std::cout << "  Spout count: " << managerContext.GetSpoutCount() << std::endl;
                std::cout << "  Bolt count: " << managerContext.GetBoltCount() << std::endl;
                std::cout << "  Task info count: " << managerContext.GetTaskInfos().size() << std::endl;
                std::cout << "  Free spout count: " << managerContext.GetFreeSpouts().size() << std::endl;
                std::cout << "  Free bolt count: " << managerContext.GetFreeBolts().size() << std::endl;
                std::cout << "  Busy spout count: " << managerContext.GetBusySpouts().size() << std::endl;
                std::cout << "  Busy bolt count: " << managerContext.GetBusyBolts().size() << std::endl;
            }
        }

        void President::SubmitTopology(sandstorm::topology::Topology *topology) {
            std::cout << "Submit topology: " << topology->GetName() << std::endl;

            _orderIds[topology->GetName()] = 0;

            const std::map<std::string, sandstorm::spout::SpoutDeclarer> &spoutDeclarers =
                    topology->GetSpoutDeclarers();
            const std::map<std::string, sandstorm::bolt::BoltDeclarer> &boltDeclarers =
                    topology->GetBoltDeclarers();

            // Allocate task and send to manager
            std::list<sandstorm::task::TaskInfo> originSpoutTasks =
                    GetAllSpoutTasks(spoutDeclarers, topology);
            std::map<std::string, std::vector<sandstorm::task::TaskInfo *>> nameToSpoutTasks =
                    AllocateSpoutTasks(originSpoutTasks);

            std::list<sandstorm::task::TaskInfo> originBoltTasks =
                    GetAllBoltTasks(topology, boltDeclarers);
            std::map<std::string, std::vector<sandstorm::task::TaskInfo *>> nameToBoltTasks =
                    AllocateBoltTasks(originBoltTasks);

            CalculateTaskPaths(nameToBoltTasks, boltDeclarers, nameToSpoutTasks);
            ShowManagerTaskInfos();
            ShowManagerMetadata();
            SyncWithManagers();
            _submitted = true;
            StartConnectionChecker();
        }

        void President::SendHeartbeat(const std::string managerId, int32_t sendTimes) {
            std::cout << "Sending heartbeat to " << managerId << std::endl;

            std::shared_ptr<sandstorm::message::CommandClient> commandClient =
                    _managerClients.at(managerId);

            commandClient->GetConnector()->Connect([commandClient, managerId, sendTimes, this]
                                                           (const util::SocketError &error) {
                if (error.GetType() != util::SocketError::Type::NoError) {
                    std::cout << "Sendtimes: " << sendTimes << std::endl;
                    if (sendTimes >= MAX_HEARTBEAT_FAILED_TIMES) {
                        if (!_submitted) {
                            return;
                        }

                        std::cout << "Lost connection from " << managerId << std::endl;
                        return;
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    this->SendHeartbeat(managerId, sendTimes + 1);

                    return;
                }

                std::cout << "Connected to " << managerId << std::endl;
                sandstorm::message::Command command(sandstorm::message::Command::Type::Heartbeat);

                commandClient->SendCommand(command,
                                           [managerId, sendTimes, commandClient, this](
                                                   const sandstorm::message::Response &response,
                                                   const message::CommandError &error) {
                                               if (error.GetType() != message::CommandError::Type::NoError) {
                                                   std::cout << error.what() << std::endl;
                                               }

                                               if (response.GetStatus() ==
                                                   sandstorm::message::Response::Status::Successful) {
                                                   std::cout << managerId << " alived." << std::endl;
                                               } else {
                                                   std::cout << "Sendtimes: " << sendTimes << std::endl;
                                                   if (sendTimes >= MAX_HEARTBEAT_FAILED_TIMES) {
                                                       if (!_submitted) {
                                                           return;
                                                       }

                                                       std::cout << "Lost connection from " << managerId << std::endl;
                                                       return;
                                                   }

                                                   commandClient->GetConnector()->Close();
                                                   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                                                   this->SendHeartbeat(managerId, sendTimes + 1);

                                                   return;
                                               }
                                           });
            });
        }
    }
}