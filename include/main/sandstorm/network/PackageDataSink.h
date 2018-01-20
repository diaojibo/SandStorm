//
// Created by rocklct on 1/19/18.
//

#pragma once

#include "sandstorm/network/DataSink.h"
#include "sandstorm/network/ByteArray.h"
#include "sandstorm/network/utils/ThreadPool.h"


namespace sandstorm {
    namespace network {
        class EventQueue;

        class BaseEvent;

        class PackageDataSink : public DataSink {
        public:
            PackageDataSink(EventQueue *eventQueue);

            ~PackageDataSink();

            int32_t Write(IStream *stream, const char *buf, int64_t bytes) override;

        private:
            EventQueue *_eventQueue;
            ThreadPool<BaseEvent> *_threadPool;
            ByteArray _data;
            int32_t _totalSize;
        };
    }
}