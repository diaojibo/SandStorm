//
// Created by rocklct on 1/28/18.
//

#pragma once

#include "sandstorm/collector/OutputCollector.h"

#include <vector>
#include <memory>

namespace sandstorm {
    namespace task {
        class ITask {
        public:
            virtual void Prepare(std::shared_ptr<sandstorm::collector::OutputCollector> outputCollector) = 0;

            virtual void Cleanup() = 0;

            virtual std::vector<std::string> DeclareFields() = 0;
        };

    }
}