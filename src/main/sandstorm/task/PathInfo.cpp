//
// Created by rocklct on 2/18/18.
//

#include "sandstorm/task/TaskInfo.h"
#include "sandstorm/base/Variant.h"

namespace sandstorm {
    namespace task {

        using sandstorm::base::Variant;
        using sandstorm::base::Variants;
        using sandstorm::base::Serializable;

        void PathInfo::Serialize(base::Variants &variants) const {
            Variant::Serialize(variants, _groupMethod);
            Variant::Serialize(variants, _destinationTask);
            Variant::Serialize(variants, _fieldName);
            Variant::Serialize(variants, _destinationExecutors);
        }

        void PathInfo::Deserialize(Variants::const_iterator &it) {
            Variant::Deserialize(it, _groupMethod);
            Variant::Deserialize(it, _destinationTask);
            Variant::Deserialize(it, _fieldName);
            Variant::Deserialize(it, _destinationExecutors);
        }

        void ExecutorPosition::Serialize(base::Variants &variants) const {
            Variant::Serialize(variants, _manager);
            Variant::Serialize(variants, _executorIndex);
        }

        void ExecutorPosition::Deserialize(Variants::const_iterator &it) {
            Variant::Deserialize(it, _manager);
            Variant::Deserialize(it, _executorIndex);
        }


    }
}