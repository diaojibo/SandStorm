//
// Created by rocklct on 2/14/18.
//


#include "sandstorm/collector/OutputDispatcher.h"
#include "sandstorm/collector/OutputQueue.h"
#include "sandstorm/collector/TaskQueue.h"
#include "sandstorm/util/StringUtil.h"
#include "sandstorm/message/CommandClient.h"
#include "sandstorm/message/Command.h"
#include "sandstorm/util/NetConnector.h"

namespace sandstorm {
    namespace collector {
        OutputDispatcher::OutputDispatcher() : _selfSpoutCount(0) {
        }

        OutputDispatcher::OutputDispatcher(std::shared_ptr<OutputQueue> queue) : _queue(queue), _selfSpoutCount(0) {
        }

        void OutputDispatcher::SetTaskInfos(const std::vector<task::TaskInfo> &taskInfos) {
            _taskInfos = taskInfos;
        }

        void OutputDispatcher::SetPresidentClient(sandstorm::message::CommandClient *presidentClient) {
            _presidentClient.reset(presidentClient);
        }

        void OutputDispatcher::Start() {
            _thread = std::thread(&OutputDispatcher::MainThread, this);
        }

        void OutputDispatcher::MainThread() {
            while (true) {
                OutputItem *outputItem = nullptr;
                if (_queue->Pop(outputItem)) {
                    int32_t taskIndex = outputItem->GetTaskIndex();
                    task::TaskInfo taskInfo = _taskInfos[taskIndex];
                    for (const task::PathInfo &pathInfo : taskInfo.GetPaths()) {
                        ProcessPath(taskInfo, pathInfo, outputItem);
                    }

                    delete outputItem;
                    outputItem = nullptr;
                }
            }
        }

        void OutputDispatcher::ProcessPath(const task::TaskInfo &taskInfo, const task::PathInfo &path,
                                           OutputItem *outputItem) {
            std::string sourceTaskName = taskInfo.GetTaskName();
            std::string destTaskName = path.GetTaskName();

            outputItem->GetTuple().SetSourceTask(sourceTaskName);
            outputItem->GetTuple().SetDestTask(destTaskName);

            int32_t groupMethod = path.GetGroupMethod();
            if (groupMethod == task::PathInfo::GroupMethod::Global) {
                const task::ExecutorPosition &executorPosition = path.GetDestinationExecutors()[0];

                SendTupleTo(outputItem, executorPosition);
            } else if (groupMethod == task::PathInfo::GroupMethod::Random) {
                int32_t destCount = static_cast<int32_t>(path.GetDestinationExecutors().size());
                int32_t destIndex = rand() % destCount;

                const task::ExecutorPosition &executorPosition = path.GetDestinationExecutors()[destIndex];

                SendTupleTo(outputItem, executorPosition);
            } else if (groupMethod == task::PathInfo::GroupMethod::Field) {
                TaskPathName taskPathName = {sourceTaskName, destTaskName};

                auto taskPairIter = _fieldsDestinations.find(taskPathName);
                if (taskPairIter == _fieldsDestinations.end()) {
                    _fieldsDestinations.insert({taskPathName, std::map<std::string, task::ExecutorPosition>()});
                    taskPairIter = _fieldsDestinations.find(taskPathName);
                }

                const std::map<std::string, task::ExecutorPosition> &destinations = taskPairIter->second;
                int32_t fieldIndex = this->_taskFieldsMap[sourceTaskName]->at(path.GetFieldName());
                std::string fieldValue = outputItem->GetTuple()[fieldIndex].GetStringValue();
                auto fieldDestIter = destinations.find(fieldValue);

                if (fieldDestIter == destinations.end()) {
                    AskField(taskPathName, fieldValue,
                             [taskPathName, outputItem, fieldValue, this](
                                     task::ExecutorPosition executorPosition) -> void {
                                 _fieldsDestinations[taskPathName].insert({fieldValue, executorPosition});
                                 SendTupleTo(outputItem, executorPosition);
                             });
                } else {
                    const task::ExecutorPosition &executorPosition = fieldDestIter->second;
                    SendTupleTo(outputItem, executorPosition);
                }
            }
        }

        void OutputDispatcher::SendTupleTo(OutputItem *outputItem, const task::ExecutorPosition &executorPosition) {
            sandstorm::base::NetAddress destAddress = executorPosition.GetManager();
            std::string destIdentifier = destAddress.GetHost() + ":" + Int2String(destAddress.GetPort());
            std::string selfIdentifier = _selfAddress.GetHost() + ":" + Int2String(_selfAddress.GetPort());

            if (destIdentifier == selfIdentifier) {
                int32_t executorIndex = executorPosition.GetExecutorIndex();
                int32_t boltIndex = executorIndex - _selfSpoutCount;

                std::shared_ptr<sandstorm::collector::TaskQueue> taskQueue = _selfTasks[boltIndex];
                TaskItem *taskItem = new TaskItem(outputItem->GetTaskIndex(), outputItem->GetTuple());
                taskQueue->Push(taskItem);
            } else {
                std::map<std::string, message::CommandClient *>::iterator commandClientPair =
                        _commandClients.find(destIdentifier);
                if (commandClientPair == _commandClients.end()) {
                    util::NetConnector *connector = new util::NetConnector(destAddress);
                    message::CommandClient *commandClient = new message::CommandClient(connector);
                    _commandClients.insert({destIdentifier, commandClient});

                    commandClientPair = _commandClients.find(destIdentifier);
                }

                message::CommandClient *commandClient = commandClientPair->second;

                commandClient->GetConnector()->Connect([
                                                               outputItem, commandClient, destIdentifier, executorPosition, this]
                                                               (const util::SocketError &socketError) {
                    sandstorm::message::Command command(sandstorm::message::Command::Type::SendTuple);

                    base::Variants commandVariants;
                    _selfAddress.Serialize(commandVariants);
                    executorPosition.Serialize(commandVariants);
                    outputItem->GetTuple().Serialize(commandVariants);

                    command.AddArguments(commandVariants);

                    try {
                        commandClient->SendCommand(command,
                                                   [destIdentifier, this](const sandstorm::message::Response &response,
                                                                          const message::CommandError &error) -> void {
                                                       if (error.GetType() != message::CommandError::Type::NoError) {
                                                           return;
                                                       }

                                                       if (response.GetStatus() ==
                                                           sandstorm::message::Response::Status::Successful) {
                                                       } else {
                                                       }
                                                   });
                    }
                    catch (util::SocketError &e) {
                    }
                });
            }
        }

        void OutputDispatcher::AskField(TaskPathName taskPathName,
                                        const std::string &fieldValue, OutputDispatcher::AskFieldCallback callback) {
            _presidentClient->Connect([taskPathName, fieldValue, callback, this](const message::CommandError &) {
                sandstorm::message::Command command(sandstorm::message::Command::Type::AskField);
                command.AddArgument(taskPathName.first);
                command.AddArgument(taskPathName.second);
                command.AddArgument(fieldValue);

                _presidentClient->SendCommand(command,
                                              [callback](const sandstorm::message::Response &response,
                                                         const message::CommandError &error) {
                                                  if (error.GetType() != message::CommandError::Type::NoError) {
                                                      std::cout << error.what() << std::endl;
                                                      return;
                                                  }

                                                  task::ExecutorPosition destination;
                                                  const base::Variants respArguments = response.GetArguments();

                                                  base::Variants::const_iterator argIter = respArguments.cbegin();
                                                  destination.Deserialize(argIter);

                                                  callback(destination);
                                              });
            });
        }
    }
}
