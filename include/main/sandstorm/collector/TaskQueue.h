//
// Created by rocklct on 1/29/18.
//

#pragma once

#include "sandstorm/base/BlockingQueue.h"
#include "sandstorm/base/Values.h"

namespace sandstorm {
    namespace collector {
        class TaskItem {
        public:
            inline TaskItem(int32_t taskIndex, const base::Tuple &tuple);

            int32_t GetTaskIndex() const {
                return _taskIndex;
            }

            const base::Tuple &GetTuple() const {
                return _tuple;
            }


        private:
            int32_t _taskIndex;
            base::Tuple _tuple;
        };

        class TaskQueue : public base::BlockingQueue<TaskItem *> {
        };
    }
}