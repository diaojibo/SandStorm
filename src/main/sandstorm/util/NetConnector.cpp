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

        void NetConnector::Connect(ConnectCallback callback) {
            try {
                Connect();
                callback(SocketError());
            }
            catch (const SocketError &e) {
                callback(e);
            }
        }

        void NetConnector::Connect() {
            if (!_client.get()) {
                _client = sandstorm::network::EpollClient::Connect(_host.GetHost(),
                                                                   _host.GetPort(), nullptr);
            }
        }


        int32_t NetConnector::SendAndReceive(const char *buffer, int32_t size, char *resultBuffer, int32_t resultSize) {

            _client->Send(sandstorm::network::ByteArray(buffer, size));

            bool receiveData = false;
            _client->OnData([&receiveData, &resultSize, &resultBuffer]
                                    (const char *buf, int64_t size) {
                if (resultSize > size) {
                    resultSize = size;
                }

                memcpy(resultBuffer, buf, resultSize);
                receiveData = true;
            });

            while (!receiveData) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            return resultSize;
        }


        void NetConnector::Close() {
            _client.reset();
        }

    }
}