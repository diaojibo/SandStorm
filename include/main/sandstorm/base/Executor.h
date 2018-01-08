//
// Created by Rocklct on 2017/12/25.
//


#pragma once

#include "sandstorm/base/ITask.h"
#include "../message/MessageLoop.h"

#include <thread>
#include <memory>
#include <vector>
#include <iostream>
#include <mutex>
#include <functional>

namespace sandstorm {
    namespace base {
        template<class TaskType>
        class Executor {
        public:
            enum class Status {
                Stopping,
                Running
            };

            Executor() : _status(Status::Stopping) {

            }

            virtual ~Executor() {}

            void StartTask(const std::string &taskName, TaskType *task) {
                _taskName = taskName;
                _task = std::shared_ptr<TaskType>(task);

                _thread = std::thread(std::bind(&Executor::StartThread, this));
            }

            virtual void StopTask() {
                _messageLoop.Stop();
            }

            Status GetStatus() const {
                return _status;
            }

            const std::string &GetTaskName() const {
                return _taskName;
            }

        protected:
            virtual void OnCreate() = 0;

            virtual void OnStop() =  0;

            std::shared_ptr <TaskType> _task;
            sandstorm::message::MessageLoop _messageLoop;
        private:

            void StartThread() {
                _status = Status::Running;

                OnCreate();
                _messageLoop.Run();
                OnStop();

                _status = Status::Stopping;
            }

            std::thread _thread;
            Status _status;
            std::string _taskName;
        };
    }
}