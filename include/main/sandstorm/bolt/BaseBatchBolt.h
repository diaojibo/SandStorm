//
// Created by rocklct on 1/28/18.
//

#pragma once

#include "sandstorm/base/Values.h"
#include "sandstorm/base/Fields.h"

namespace sandstorm {
    namespace base {
        class BatchOutputCollector;
    }

    namespace bolt {
        class BaseBatchBolt {
        private:
            void *_id;
            int32_t _count = 0;

        public:
            virtual void Prepare(base::BatchOutputCollector &collector, void *id)=0;

            virtual void Execute(const base::Values &values) = 0;

            virtual void FinishBatch() = 0;

            virtual base::Fields DeclareOutputFields() =0;
        };
    }
}