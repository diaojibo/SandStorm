//
// Created by rocklct on 2/15/18.
//

#include "sandstorm/message/CommandClient.h"
#include "sandstorm/message/Command.h"
#include "sandstorm/util/NetConnector.h"
#include "sandstorm/base/ByteArray.h"

#include <iostream>

namespace sandstorm {
    namespace message {
        CommandClient::CommandClient(sandstorm::util::NetConnector *connector) : _connector(connector) {
        }

        CommandClient::~CommandClient() {
            if (_connector) {
                delete _connector;
                _connector = nullptr;
            }
        }

        void CommandClient::Connect(CommandClient::ConnectCallback callback) {
            this->_connector->Connect([callback](const util::SocketError &error) {
                if (error.GetType() != util::SocketError::Type::NoError) {
                    callback(CommandError(CommandError::Type::ConnectError,
                                          error.what()));
                    return;
                }

                callback(CommandError());
            });
        }

        void CommandClient::SendCommand(const Command &command, SendCommandCallback callback) {
            try {
                sandstorm::base::ByteArray commandBytes = command.Serialize();

                _connector->SendAndReceive(commandBytes.data(), static_cast<int32_t>(commandBytes.size()), [callback]
                        (char *resultBuffer, int32_t readSize, const util::SocketError &error) {
                    sandstorm::message::Response response;

                    if (error.GetType() != util::SocketError::Type::NoError) {
                        callback(response, CommandError(CommandError::Type::SendError,
                                                        error.what()));
                        return;
                    }

                    sandstorm::base::ByteArray commandBytes(resultBuffer, readSize);
                    response.Deserialize(commandBytes);
                    callback(response, CommandError());
                });
            }
            catch (const char *message) {
                std::cout << message;
            }
        }


    }
}