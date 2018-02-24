//
// Created by rocklct on 1/29/18.
//

#pragma once

#include "sandstorm/util/Socket.h"
#include "sandstorm/base/NetAddress.h"
#include <functional>
#include <memory>

namespace sandstorm {
    namespace util {
        typedef std::function<void(std::shared_ptr<TcpConnection> connection)>
                ConnectionCallback;

        typedef std::function<void(std::shared_ptr<TcpConnection> connection,
                                   const char *buffer, int32_t size)> DataReceiver;


        class NetListener {

        public:
            NetListener(const sandstorm::base::NetAddress& host);

            const sandstorm::base::NetAddress &GetHost() const {
                return _host;
            }

            void SetHost(const sandstorm::base::NetAddress &host) {
                _host = host;
            }

            void StartListen();

            void DataThreadMain(std::shared_ptr<TcpConnection> connection);

            void OnConnection(ConnectionCallback callback);


        private:
            sandstorm::base::NetAddress _host;
            std::shared_ptr<TcpServer> _server;
            ConnectionCallback _connectionCallback;
        };
    }
}