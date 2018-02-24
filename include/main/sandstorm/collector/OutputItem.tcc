//
// Created by rocklct on 1/29/18.
//

#pragma once

namespace sandstorm {
    namespace collector {

        OutputItem::OutputItem(int32_t taskIndex, const base::Tuple &tuple, const std::string &taskName) :
                _taskIndex(taskIndex), _tuple(tuple) {
            _tuple.SetSourceTask(taskName);
        }

    }
}