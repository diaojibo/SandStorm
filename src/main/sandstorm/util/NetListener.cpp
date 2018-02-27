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

            sandstorm::network::EpollLoop::Get()->Start();

            _server.Listen(_host.GetHost(), _host.GetPort());

            std::cout << "Listen on " << _host.GetHost() << ":" << _host.GetPort() << std::endl;

        }

        void NetListener::OnConnection(ConnectionCallback callback) {
            _connectionCallback = callback;
            _server.OnConnect(callback);

        }


    }
}