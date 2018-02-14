//
// Created by rocklct on 2/14/18.
//

#include "sandstorm/collector/OutputCollector.h"
#include "sandstorm/collector/OutputQueue.h"
#include "sandstorm/base/Values.h"

#include <iostream>

namespace sandstorm {
    namespace collector {
        OutputCollector::OutputCollector() : _taskIndex(-1) {}

        OutputCollector::OutputCollector(
                int32_t taskIndex, const std::string &taskName, std::shared_ptr<OutputQueue> queue) :
                _taskIndex(taskIndex), _taskName(taskName), _queue(queue) {
        }

        void OutputCollector::Emit(const sandstorm::base::Tuple &tuple) {
            if (_taskIndex != -1) {
                _queue->Push(new OutputItem(_taskIndex, tuple, _taskName));
            }
        }
    }
}