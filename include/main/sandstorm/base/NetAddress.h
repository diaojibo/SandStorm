//
// Created by Rocklct on 2017/12/26.
//

#pragma once

#include <string>
#include "sandstorm/base/Variant.h"

namespace sandstorm {
    namespace base {
        class NetAddress : public Serializable {
        public:
            inline NetAddress();

            inline NetAddress(const std::string &host, int32_t port);

            inline void Serialize(Variants &variants) const override;

            inline void Deserialize(Variants::const_iterator &it) override;

            const std::string &GetHost() const {
                return _host;
            }

            void SetHost(const std::string &host) {
                _host = host;
            }

            int32_t GetPort() const {
                return _port;
            }

            void SetPort(int32_t port) {
                _port = port;
            }

        private:
            std::string _host;
            int32_t _port;
        };
    }
}