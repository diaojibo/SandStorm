//
// Created by rocklct on 1/20/18.
//

#ifndef SANDSTORM_EPOLLCLIENT_H
#define SANDSTORM_EPOLLCLIENT_H

#include "sandstorm/network/epoll/EpollStream.h"
#include "sandstorm/network/Net.h"
#include "sandstorm/network/DataSink.h"

#include <memory>

namespace sandstorm {
    namespace network {
        class EpollClient;

        typedef std::shared_ptr<EpollClient> EpollClientPtr;

        class EpollClient : public EpollStream, public IConnectable {

        public:
            EpollClient(const EpollClient &client) = delete;

            virtual ~EpollClient() {}

            virtual int32_t Receive(char *buffer, int32_t bufferSize, int32_t &readSize) override;

            virtual int32_t Send(const ByteArray &byteArray) override;

            uint32_t GetEvents() const {
                return _events;
            }

            void SetEvents(uint32_t events) {
                _events = events;
            }

            void Connect(const std::string &host, int32_t port) override;

            static EpollClientPtr Connect(const std::string &ip, int32_t port, DataSink *dataSink);

        private:
            EpollClient(NativeSocket clientSocket) : EpollStream(clientSocket) {
                this->SetNativeSocket(clientSocket);
            }


            uint32_t _events;
        };
    }
}


#endif //SANDSTORM_EPOLLCLIENT_H
