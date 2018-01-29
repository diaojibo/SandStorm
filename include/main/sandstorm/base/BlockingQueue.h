//
// Created by rocklct on 1/27/18.
//

#pragma once

#include <condition_variable>
#include <string>
#include <queue>
#include <chrono>

namespace sandstorm {
    namespace base {

        template<class T>
        class BlockingQueue {
        public:
            BlockingQueue();

            virtual ~BlockingQueue();

            void Push(const T &element);

            bool Pop(T &element, int32_t milliseconds = 0);

            bool Peek(T &element) const;

            bool Empty() const;

            int32_t GetSize() const {
                std::unique_lock<std::mutex> locker(_mutex);

                return _queue.size();
            }

        private:
            mutable std::mutex _mutex;
            std::condition_variable _emptyCv;
            std::queue<T> _queue;
        };
    }
}

#include "sandstorm/base/BlockingQueue.tcc"