//
// Created by Rocklct on 2017/12/24.
//

#pragma once

#include "../base/ITask.h"

namespace SandStorm {
    namespace base {
        class OutputCollector;

        class Values;
    }

    namespace bolt {
        class IBolt : public base::ITask {
        public:
            virtual void Prepare(base::OutputCollector &outputCollector) = 0;

            virtual void CleanUp()=0;

            virtual void Execute(const base::Values &values)=0;

            virtual IBolt *clone() const = 0;
        };
    }
}