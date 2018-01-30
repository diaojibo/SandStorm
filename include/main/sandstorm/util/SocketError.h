//
// Created by rocklct on 1/29/18.
//

#pragma once

#include <string>

namespace sandstorm {
    namespace util {
        class SocketError {
        public:

            enum class Type {
                NoError,
                ConnectError,
                SendError,
                RecvError
            };

            SocketError() : _type(Type::NoError) {}

            SocketError(Type type, const std::string &message)
                    : _type(type), _message(message) {}


            Type GetType() const {
                return _type;
            }

            const char *what() const {
                return _message.c_str();
            }

            const std::string GetMessage() const {
                return _message;
            }

        private:
            Type _type;
            std::string _message;
        };
    }
}