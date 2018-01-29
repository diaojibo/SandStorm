//
// Created by rocklct on 1/28/18.
//

#pragma once

#include "sandstorm/bolt/BaseBatchBolt.h"
#include "sandstorm/bolt/TransactionAttempt.h"

namespace sandstorm {
    namespace base {
        class BatchOutputCollector;
    }

    namespace bolt {
        class BaseTransactionalBolt : public BaseBatchBolt {
        public:
            virtual void Prepare(base::BatchOutputCollector &collector, TransactionAttempt *attempt)=0;

            void Prepare(base::BatchOutputCollector &collector, void *id) {}
        };
    }
}