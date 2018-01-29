//
// Created by rocklct on 1/29/18.
//

#pragma once

#include "sandstorm/task/Executor.h"
#include "sandstorm/collector/TaskQueue.h"
#include <thread>
#include <memory>

namespace sandstorm {
    namespace bolt {
        class IBolt;
    }

    namespace collector {
        class TaskQueue;
    }

    namespace task {
        class BoltExecutor : public Executor {
        public:
            BoltExecutor();

            ~BoltExecutor() {}

            void Start();

            std::shared_ptr<sandstorm::bolt::IBolt> GetBolt() {
                return _bolt;
            }

            void SetBolt(sandstorm::bolt::IBolt *bolt) {
                _bolt.reset(bolt);
            }

            void SetTaskQueue(std::shared_ptr<collector::TaskQueue> taskQueue) {
                _taskQueue = taskQueue;
            }


        private:
            void StartLoop();

            std::thread _thread;
            std::shared_ptr<sandstorm::bolt::IBolt> _bolt;
            std::shared_ptr<sandstorm::collector::TaskQueue> _taskQueue;

        };
    }
}