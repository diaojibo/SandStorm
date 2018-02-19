//
// Created by rocklct on 2/18/18.
//

#include "sandstorm/service/ManagerContext.h"

namespace sandstorm {
    namespace service {
        using sandstorm::base::Variant;
        using sandstorm::base::Variants;
        using sandstorm::base::Serializable;

        ManagerContext::ManagerContext() : _spoutCount(0), _boltCount(0) {
        }

        void ManagerContext::Serialize(base::Variants &variants) const {
            Variant::Serialize(variants, _id);
            Variant::Serialize(variants, _spoutCount);
            Variant::Serialize(variants, _boltCount);
            Variant::Serialize(variants, _freeSpouts);
            Variant::Serialize(variants, _freeBolts);
            Variant::Serialize(variants, _busySpouts);
            Variant::Serialize(variants, _busyBolts);
            Variant::Serialize(variants, _taskInfos);

        }

        void ManagerContext::Deserialize(Variants::const_iterator &it) {
            Variant::Deserialize(it, _id);
            Variant::Deserialize(it, _spoutCount);
            Variant::Deserialize(it, _boltCount);
            Variant::Deserialize(it, _freeSpouts);
            Variant::Deserialize(it, _freeBolts);
            Variant::Deserialize(it, _busySpouts);
            Variant::Deserialize(it, _busyBolts);
            Variant::Deserialize(it, _taskInfos);
        }

    }
}