//
// Created by rocklct on 1/17/18.
//

#ifndef SANDSTORM_THREADPOOL_H
#define SANDSTORM_THREADPOOL_H

#include "sandstorm/network/utils/ConcurrentQueue.h"

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#define MIN_THREADS 10

namespace sandstorm {
    namespace network {
        template<class Type>
        class ThreadPool {
            ThreadPool &operator=(const ThreadPool &) = delete;

            ThreadPool(const ThreadPool &other) = delete;

        public:
            ThreadPool(int32_t threads, std::function<void(Type &record)> handler);

            virtual ~ThreadPool();

            void Submit(Type record);

        private:
            bool _shutdown;
            int32_t _threads;
            std::function<void(Type &record)> _handler;
            std::vector<std::thread> _workers;
            ConcurrentQueue<Type> _tasks;
        };
    }
}

#include "sandstorm/network/template/utils/thread_pool.tcc"


#endif //SANDSTORM_THREADPOOL_H
