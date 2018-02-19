//
// Created by rocklct on 2/18/18.
//

#pragma once

#include "sandstorm/task/ITask.h"
#include "sandstorm/base/Values.h"

namespace sandstorm {
    namespace spout {
        class ISpout : public sandstorm::task::ITask {
        public:
            virtual ISpout *Clone()=0;

            virtual void NextTuple()=0;
        };
    }
}