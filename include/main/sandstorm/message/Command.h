//
// Created by rocklct on 1/29/18.
//

#pragma once

#include <string>
#include <vector>

#include "sandstorm/base/ByteArray.h"
#include "sandstorm/base/Variant.h"

namespace sandstorm {
    namespace message {
        class Command {
        public:
            struct Type {
                enum {
                    Invalid = 0,
                    Join,
                    Heartbeat,
                    SyncMetadata,
                    SendTuple,
                    AskField,
                    OrderId
                };
            };

            Command(int32_t type = Type::Invalid);

            Command(int32_t type, std::vector<sandstorm::base::Variant> &arguments);

            void AddArgument(const sandstorm::base::Variant &argument);

            void AddArguments(const std::vector<sandstorm::base::Variant> &arguments);

            void Deserialize(const sandstorm::base::ByteArray &data);

            sandstorm::base::ByteArray Serialize() const;

            int32_t GetType() const {
                return _type;
            }

            void SetType(int32_t type) {
                _type = type;
            }

            sandstorm::base::Variant GetArgument(int32_t index) const {
                return _arguments[index];
            }

            int32_t GetArgumentCount() const {
                return static_cast<int32_t>(_arguments.size());
            }

            const std::vector<sandstorm::base::Variant> &GetArguments() const {
                return _arguments;
            }

        private:
            int32_t _type;
            std::vector<sandstorm::base::Variant> _arguments;
        };

        class Response {
        public:
            struct Status {
                enum {
                    Failed = 0,
                    Successful = 1
                };
            };

            Response(int32_t status = Status::Failed);

            Response(int32_t status, std::vector<sandstorm::base::Variant> &arguments);

            void AddArguments(const std::vector<sandstorm::base::Variant> &arguments);

            void AddArgument(const sandstorm::base::Variant &argument);

            void Deserialize(const sandstorm::base::ByteArray &data);

            sandstorm::base::ByteArray Serialize() const;

            int32_t GetStatus() const {
                return _status;
            }

            void SetStatus(int32_t status) {
                _status = status;
            }

            sandstorm::base::Variant GetArgument(int32_t index) const {
                return _arguments[index];
            }

            int32_t GetArgumentCount() const {
                return static_cast<int32_t>(_arguments.size());
            }

            const std::vector<sandstorm::base::Variant> &GetArguments() const {
                return _arguments;
            }

        private:
            int32_t _status;
            std::vector<sandstorm::base::Variant> _arguments;

        };
    }
}