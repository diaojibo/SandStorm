//
// Created by rocklct on 1/30/18.
//

#pragma once

#include "sandstorm/util/Socket.h"
#include "sandstorm/base/NetAddress.h"

#include <cstdint>
#include <functional>
#include <memory>

namespace sandstorm {
    namespace util {

        class NetConnector {

        public:
            typedef std::function<void(const SocketError &)> ConnectCallback;
            typedef std::function<void(char *resultBuffer, int32_t readSize, const SocketError &)> DataReceiver;


            NetConnector(const sandstorm::base::NetAddress &host);

            const sandstorm::base::NetAddress &GetHost() const {
                return _host;
            }

            void SetHost(const sandstorm::base::NetAddress &host) {
                _host = host;
            }

            void Connect();

            void Connect(ConnectCallback callback);

            int32_t SendAndReceive(const char *buffer, int32_t size, char *resultBuffer, int32_t resultSize);

            void SendAndReceive(const char *buffer, int32_t size, DataReceiver receiver);

            void Close();

        private:
            sandstorm::base::NetAddress _host;
            std::shared_ptr<TcpClient> _client;
        };
    }
}