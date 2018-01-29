//
// Created by rocklct on 1/28/18.
//

#pragma once

namespace sandstorm {
    namespace base {
        NetAddress::NetAddress() : _port(0) {
        }

        NetAddress::NetAddress(const std::string &host, int32_t port) : _host(host), _port(port) {
        }

        void NetAddress::Serialize(Variants &variants) const {
            Variant::Serialize(variants, _host);
            Variant::Serialize(variants, _port);
        }

        void NetAddress::Deserialize(Variants::const_iterator &it) {
            Variant::Deserialize(it, _host);
            Variant::Deserialize(it, _port);
        }
    }
}