//
// Created by rocklct on 1/29/18.
//


#pragma once

#include "sandstorm/task/TaskInfo.h"
#include "sandstorm/base/NetAddress.h"
#include <memory>
#include <thread>
#include <map>
#include <functional>

namespace sandstorm {
    namespace message {
        class CommandClient;
    }


    namespace collector {

        class OutputQueue;

        class OutputItem;

        class TaskQueue;

        typedef std::pair<std::string, std::string> TaskPathName;

        class DispatchTaskInfo {
        private:
            int32_t _taskType;
            std::vector<sandstorm::task::PathInfo> _globalPaths;
            std::vector<std::vector<sandstorm::task::PathInfo>> _randomPaths;
            std::vector<std::map<std::string, std::vector<sandstorm::task::PathInfo>>> _fieldPaths;
        };

        class OutputDispatcher {
        public:
            typedef std::function<void(sandstorm::task::ExecutorPosition)> AskFieldCallback;

            OutputDispatcher();

            OutputDispatcher(std::shared_ptr<OutputQueue> queue);

            void SetQueue(std::shared_ptr<OutputQueue> queue) {
                _queue = queue;
            }

            std::shared_ptr<OutputQueue> GetQueue() const {
                return _queue;
            }

            void SetSelfAddress(const base::NetAddress &selfAddress) {
                _selfAddress = selfAddress;
            }

            void SetSelfSpoutCount(int32_t selfSpoutCount) {
                _selfSpoutCount = selfSpoutCount;
            }

            void SetSelfTasks(std::vector<std::shared_ptr<hurricane::collector::TaskQueue>> selfTasks) {
                _selfTasks = selfTasks;
            }

            void SetTaskInfos(const std::vector<hurricane::task::TaskInfo> &taskInfos);

            void SetPresidentClient(message::CommandClient *presidentClient);

            void Start();

            void SetTaskFields(const std::map<std::string, const std::vector<std::string> *> &taskFields) {
                _taskFields = taskFields;
            }

            void SetTaskFieldsMap(const std::map<std::string, const std::map<std::string, int32_t> *> &taskFieldsMap) {
                _taskFieldsMap = taskFieldsMap;
            }


        private:

            void MainThread();

            void ProcessPath(const task::TaskInfo &taskInfo, const task::PathInfo &path, OutputItem *outputItem);

            void SendTupleTo(OutputItem *outputItem, const task::ExecutorPosition &executorPosition);

            void AskField(TaskPathName taskPathName, const std::string &fieldValue, AskFieldCallback callback);


            std::shared_ptr<OutputQueue> _queue;
            std::thread _thread;
            sandstorm::base::NetAddress _selfAddress;
            int32_t _selfSpoutCount;

            std::vector<sandstorm::task::TaskInfo> _taskInfos;

            std::vector<std::shared_ptr<sandstorm::collector::TaskQueue>> _selfTasks;

            std::shared_ptr<sandstorm::message::CommandClient> _presidentClient;

            std::map<std::string, sandstorm::message::CommandClient *> _commandClients;

            std::map<TaskPathName, std::map<std::string, sandstorm::task::ExecutorPosition>> _fieldsDestination;

            std::map<std::string, const std::vector<std::string> *> _taskFields;

            std::map<std::string, const std::map<std::string, int32_t> *> _taskFieldsMap;

        };


    }

}

