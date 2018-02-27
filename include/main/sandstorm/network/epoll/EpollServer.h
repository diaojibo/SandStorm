//
// Created by rocklct on 1/19/18.
//

#ifndef SANDSTORM_EPOLLSERVER_H
#define SANDSTORM_EPOLLSERVER_H

#include "sandstorm/network/PackageDataSink.h"
#include "sandstorm/network/epoll/EpollConnection.h"

namespace sandstorm {
    namespace network {
        class EpollServer : public BasicServer<EpollConnectionPtr> {
        public:

            typedef std::function<void(std::shared_ptr<sandstorm::network::EpollConnection> connection)>
                    ConnectionCallback;

            EpollServer() {}

            virtual ~EpollServer() {}

            int32_t Listen(const std::string &host, int32_t port, int32_t backlog = 20) override;

            void OnConnect(ConnectionCallback handler) {
                _connectHandler = handler;
            }

            void OnDisconnect(DisconnectHandler handler) {
                _disconnectIndication = handler;
            }

            EpollConnectionPtr Accept(int32_t sockfd);

        private:
            int32_t _Bind(const std::string &host, int32_t port);

            DataSink *_dataSink;

            //Connect Function
            ConnectionCallback _connectHandler;

            //Disconnect Function
            DisconnectHandler _disconnectIndication;

        };
    }
}


#endif //SANDSTORM_EPOLLSERVER_H
