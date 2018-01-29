//
// Created by rocklct on 1/29/18.
//

#pragma once

#include "sandstorm/base/NetAddress.h"
#include "sandstorm/base/Variant.h"
#include <string>
#include <cstdint>
#include <list>
#include <map>

namespace sandstorm {
    namespace service {
        class ManagerContext;
    }

    namespace task {
        class ExecutorPosition : public sandstorm::base::Serializable {

            ExecutorPosition();

            ExecutorPosition(const sandstorm::base::NetAddress &manager, int32_t executorIndex);

            const sandstorm::base::NetAddress &GetManager() const {
                return _manager;
            }

            void SetManager(const sandstorm::base::NetAddress &manager) {
                _manager = manager;
            }

            int32_t GetExecutorIndex() const {
                return _executorIndex;
            }

            void SetExecutorIndex(int32_t executorIndex) {
                _executorIndex = executorIndex;
            }

            virtual void Serialize(sandstorm::base::Variants &variants) const override;

            virtual void Deserialize(sandstorm::base::Variants::const_iterator &it) override;

        private:
            sandstorm::base::NetAddress _manager;
            int32_t _executorIndex;
        };

        class PathInfo : public sandstorm::base::Serializable {
        public:

            struct GroupMethod {
                enum {
                    Invalid = 0,
                    Global,
                    Field,
                    Random
                };
            };

            PathInfo();

            int32_t GetGroupMethod() const {
                return _groupMethod;
            }

            void SetGroupMethod(int32_t groupMethod) {
                _groupMethod = groupMethod;
            }

            void SetDestinationTask(const std::string &taskName) {
                _destinationTask = taskName;
            }

            const std::string &GetTaskName() const {
                return _destinationTask;
            }

            const std::string &GetFieldName() const {
                return _fieldName;
            }

            void SetFieldName(const std::string &fieldName) {
                _fieldName = fieldName;
            }


            const std::vector<ExecutorPosition> &GetDestinationExecutors() const {
                return _destinationExecutors;
            }

            void SetDestinationExecutors(const std::vector<ExecutorPosition> &executors) {
                _destinationExecutors = executors;
            }

            virtual void Serialize(hurricane::base::Variants &variants) const override;

            virtual void Deserialize(hurricane::base::Variants::const_iterator &it) override;

        private:
            int32_t _groupMethod;
            std::string _destinationTask;
            std::string _filedName;
            std::vector<ExecutorPosition> _destinationExecutors;
        };


        class TaskInfo : public sandstorm::base::Serializable {
        public:

            TaskInfo();

            const std::string &GetTopologyName() const {
                return _topologyName;
            }

            void SetTopologyName(const std::string &topologyName) {
                _topologyName = topologyName;
            }

            const std::string &GetTaskName() const {
                return _taskName;
            }

            void SetTaskName(const std::string &taskName) {
                _taskName = taskName;
            }

            const std::list<PathInfo> &GetPaths() const {
                return _paths;
            }

            void SetPaths(const std::list<PathInfo> &paths) {
                _paths = paths;
            }

            void AddPath(const PathInfo &path) {
                _paths.push_back(path);
            }

            const hurricane::service::ManagerContext *GetManagerContext() const {
                return _managerContext;
            }

            hurricane::service::ManagerContext *GetManagerContext() {
                return _managerContext;
            }

            void SetManagerContext(hurricane::service::ManagerContext *context) {
                _managerContext = context;
            }

            int32_t GetExecutorIndex() const {
                return _executorIndex;
            }

            void SetExecutorIndex(int32_t executorIndex) {
                _executorIndex = executorIndex;
            }

            virtual void Serialize(hurricane::base::Variants &variants) const override;

            virtual void Deserialize(hurricane::base::Variants::const_iterator &it) override;

        private:
            std::string _topologyName;
            std::string _taskName;
            std::list<PathInfo> _paths;

            sandstorm::service::ManagerContext *_managerContext;
            int32_t _executorIndex;
        };
    }
}
