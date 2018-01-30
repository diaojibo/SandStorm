//
// Created by rocklct on 1/29/18.
//

#pragma once

#include "sandstorm/base/BlockingQueue.h"
#include "sandstorm/base/Values.h"

namespace sandstorm {
    namespace collector {

        class OutputItem {

        public:
            inline OutputItem(int32_t taskIndex, const base::Tuple &tuple,
                              const std::string &taskName);

            int32_t GetTaskIndex() const {
                return _taskIndex;
            }

            sandstorm::base::Tuple &GetTuple() {
                return _tuple;
            }

            const base::Tuple &GetTuple() const {
                return _tuple;
            }


        private:
            int32_t _taskIndex;
            sandstorm::base::Tuple _tuple;
        };

        class OutputQueue : public base::BlockingQueue<OutputItem *> {
        };
    }
}

#include "sandstorm/collector/OutputItem.tcc"