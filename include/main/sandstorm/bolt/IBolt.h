//
// Created by Rocklct on 2017/12/24.
//

#pragma once

#include "sandstorm/task/ITask.h"
#include "sandstorm/base/Values.h"

namespace sandstorm {
    namespace base {
        class OutputCollector;

        class Values;
    }

    namespace bolt {
        class IBolt : public sandstorm::task::ITask {
        public:

            virtual IBolt *Clone() = 0;

            virtual void Execute(const sandstorm::base::Tuple &tuple) = 0;
        };
    }
}