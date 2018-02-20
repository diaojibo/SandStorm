//
// Created by rocklct on 2/18/18.
//

#include "sandstorm/task/TaskInfo.h"

namespace sandstorm {
    namespace task {
        using sandstorm::base::Variant;
        using sandstorm::base::Variants;
        using sandstorm::base::Serializable;

        ExecutorPosition::ExecutorPosition() : _executorIndex(-1) {
        }

        ExecutorPosition::ExecutorPosition(const sandstorm::base::NetAddress &manager, int32_t executorIndex) :
                _manager(manager), _executorIndex(executorIndex) {
        }

        PathInfo::PathInfo() : _groupMethod(GroupMethod::Invalid) {}

        TaskInfo::TaskInfo() : _managerContext(nullptr), _executorIndex(-1) {
        }

        void TaskInfo::Serialize(base::Variants &variants) const {
            Variant::Serialize(variants, _topologyName);
            Variant::Serialize(variants, _taskName);
            Variant::Serialize(variants, _paths);
            Variant::Serialize(variants, _executorIndex);
        }

        void TaskInfo::Deserialize(base::Variants::const_iterator &it) {
            Variant::Deserialize(it, _topologyName);
            Variant::Deserialize(it, _taskName);
            Variant::Deserialize(it, _paths);
            Variant::Deserialize(it, _executorIndex);
        }
    }
}