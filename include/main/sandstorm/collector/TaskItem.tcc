//
// Created by rocklct on 1/29/18.
//

#pragma once

namespace hurricane {
    namespace collector {

        TaskItem::TaskItem(int32_t taskIndex, const base::Tuple &tuple) :
                _taskIndex(taskIndex), _tuple(tuple) {
        }

    }
}