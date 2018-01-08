//
// Created by Rocklct on 2017/12/24.
//

#pragma once

#include "sandstorm/base/ITask.h"

namespace sandstorm {
    namespace base {
        class OutputCollector;
    }

    namespace spout {
        class ISpout : public base::ITask {
        public:
            virtual void Open(base::OutputCollector &outputCollector) = 0;

            virtual void Close() = 0;

            virtual void Execute() = 0;

            virtual ISpout *Clone() const = 0;
        };
    }
}
