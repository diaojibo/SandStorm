//
// Created by rocklct on 1/19/18.
//

#ifndef SANDSTORM_EPOLLSTREAM_H
#define SANDSTORM_EPOLLSTREAM_H

#include "sandstorm/network/Net.h"

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

namespace sandstorm {
    namespace network {
        class EpollLoop;

        class EpollStream : public BasicStream {
        public:
            EpollStream(NativeSocket nativeSocket) : BasicStream(nativeSocket) {}

            virtual ~EpollStream() {}

            EpollStream(const EpollStream &stream) = delete;

            virtual int32_t Receive(char *buffer, int32_t bufferSize, int32_t &readSize) override;

            virtual int32_t Send(const ByteArray &byteArray) override;

            uint32_t GetEvents() const {
                return _events;
            }

            void SetEvents(uint32_t events) {
                _events = events;
            }

            void OnData(DataHandler handler) override {
                _dataHandler = handler;
            }

            DataHandler GetDataHandler() override {
                return _dataHandler;
            }


        private:
            uint32_t _events;
            DataHandler _dataHandler;
        };

        typedef std::shared_ptr<EpollStream> EpollStreamPtr;
    }
}


#endif //SANDSTORM_EPOLLSTREAM_H
