//
// Created by rocklct on 1/29/18.
//

#pragma once

#include <functional>

namespace sandstorm {
    namespace util {
        class NetConnector;
    }

    namespace message {
        class Command;

        class Response;

        class CommandError {
        public:
            enum class Type {
                NoError,
                SendError,
                ConnectError
            };

            CommandError() : _type(Type::NoError) {}

            CommandError(Type type, const std::string &message) :
                    _type(type), _message(message) {
            }

            Type GetType() const {
                return _type;
            }


            const char *what() const {
                return _message.c_str();
            }

            const std::string &GetMessage() const {
                return _message;
            }

        private:
            Type _type;
            std::string _message;
        };

        class CommandClient {
            typedef std::function<void(const CommandError &)> ConnectCallback;
            typedef std::function<void(const Response &response, const CommandError &)> SendCommandCallback;

            CommandClient(sandstorm::util::NetConnector *connector);

            ~CommandClient();

            void Connect(ConnectCallback callback);

            void SendCommand(const Command &command, SendCommandCallback callback);

            sandstorm::util::NetConnector *GetConnector() {
                return _connector;
            }

            const sandstorm::util::NetConnector *GetConnector() const {
                return _connector;
            }

        private:
            sandstorm::util::NetConnector *_connector;
        };
    }
}