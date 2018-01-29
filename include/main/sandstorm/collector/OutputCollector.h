//
// Created by rocklct on 1/28/18.
//

#pragma once

#include <memory>
#include <string>

namespace sandstorm {
    namespace base {
        class Tuple;
    }

    namespace collector {
        class OutputQueue;

        class OutputCollector {
        public:
            OutputCollector();

            OutputCollector(int32_t taskIndex, const std::string &taskName,
                            std::shared_ptr<OutputQueue> queue);

            void SetQueue(std::shared_ptr<OutputQueue> queue) {

            }

        private:

            int32_t _taskIndex;
            std::string _taskName;
            std::shared_ptr<OutputQueue> _queue;
        };
    }
}