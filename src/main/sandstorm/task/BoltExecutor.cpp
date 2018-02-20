//
// Created by rocklct on 2/18/18.
//

#include "sandstorm/task/BoltExecutor.h"
#include "sandstorm/bolt/IBolt.h"
#include "sandstorm/collector/TaskQueue.h"

namespace sandstorm {
    namespace task {
        BoltExecutor::BoltExecutor() {
        }

        void BoltExecutor::Start() {
            // It is managed by main thread, so we don't need to detach it
            _thread = std::thread(&BoltExecutor::StartLoop, this);
        }

        void BoltExecutor::StartLoop() {
            collector::TaskItem *taskItem;

            while (_taskQueue->Pop(taskItem)) {
                _bolt->Execute(taskItem->GetTuple());

                delete taskItem;
                taskItem = nullptr;
            }
        }

    }
}