//
// Created by rocklct on 2/22/18.
//

#include "sandstorm/util/NetListener.h"

#include <iostream>
#include <thread>
#include <chrono>

namespace sandstorm {
    namespace util {
        const int32_t DATA_BUFFER_SIZE = 65535;

        NetListener::NetListener(const sandstorm::base::NetAddress &host) :
                _host(host) {
        }

        void NetListener::StartListen() {
            _server = std::make_shared<TcpServer>();

            _server->Listen(_host.GetHost(), _host.GetPort());
            std::cout << "Listen on " << _host.GetHost() << ":" << _host.GetPort();

            while (true) {
                std::shared_ptr<TcpConnection> connection = std::shared_ptr<TcpConnection>(_server->Accept());

                std::cout << "A client is connected";

                std::thread dataThread(std::bind(&NetListener::DataThreadMain, this, std::placeholders::_1),
                                       connection);
                dataThread.detach();
            }
        }

        void NetListener::DataThreadMain(std::shared_ptr<TcpConnection> connection) {
            int32_t _lostTime = 0;

            _connectionCallback(connection);

            try {
                char buffer[DATA_BUFFER_SIZE];
                while (true) {
                    bool successful = connection->ReceiveAsync(buffer, DATA_BUFFER_SIZE);

                    if (!successful) {
                        break;
                    }
                }
            }
            catch (const std::exception &e) {
                std::cout << e.what();
            }
        }

        void NetListener::OnConnection(ConnectionCallback callback) {
            _connectionCallback = callback;
        }
    }
}