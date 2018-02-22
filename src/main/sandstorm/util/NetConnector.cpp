//
// Created by rocklct on 2/22/18.
//

#include "sandstorm/util/NetConnector.h"

#include <iostream>

namespace sandstorm {
    namespace util {
        const int32_t RECEIVE_BUFFER_SIZE = 65535;

        NetConnector::NetConnector(const sandstorm::base::NetAddress &host) :
                _host(host) {
        }

        void NetConnector::Connect() {
            if (!_client.get()) {
                _client = std::make_shared<TcpClient>();

                try {
                    _client->Connect(_host.GetHost(), _host.GetPort());
                }
                catch (const SocketError &e) {
                    _client.reset();
                    throw e;
                }
            }
        }

        void NetConnector::Connect(ConnectCallback callback) {
            try {
                Connect();
                callback(SocketError());
            }
            catch (const SocketError &e) {
                callback(e);
            }
        }

        int32_t NetConnector::SendAndReceive(const char *buffer, int32_t size, char *resultBuffer, int32_t resultSize) {
            _client->Send(buffer, size);
            return _client->Receive(resultBuffer, resultSize);
        }

        void NetConnector::SendAndReceive(const char *buffer, int32_t size, DataReceiver receiver) {
            _client->SendAsync(buffer, size, [this, receiver](int32_t sentSize, const SocketError &error) {
                if (error.GetType() != SocketError::Type::NoError) {
                    receiver(nullptr, 0, error);
                    return;
                }

                char resultBuffer[RECEIVE_BUFFER_SIZE];
                int32_t readSize = _client->Receive(resultBuffer, RECEIVE_BUFFER_SIZE);

                if (!readSize) {
                    receiver(resultBuffer, readSize,
                             SocketError(SocketError::Type::RecvError, "Receive 0 bytes from client"));
                    return;
                }

                receiver(resultBuffer, readSize, SocketError());
            });
        }

        void NetConnector::Close() {
            _client.reset();
        }

    }
}