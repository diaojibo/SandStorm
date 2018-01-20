//
// Created by rocklct on 1/18/18.
//

#ifndef SANDSTORM_EXENDIAN_H
#define SANDSTORM_EXENDIAN_H

#include <cstdint>
#include <endian.h>


namespace sandstorm {
    namespace network {
        inline uint64_t ConvertHostToNetworkLongLong(uint64_t hostll) {
            return htobe64(hostll);
        }

        inline uint32_t ConvertHostToNetworkLong(uint32_t hostl) {
            return htobe32(hostl);
        }

        inline uint16_t ConverHostToNetworkShort(uint16_t hosts) {
            return htobe16(hosts);
        }

        inline uint64_t ConvertNetworkToHostLongLong(uint64_t networkll) {
            return be64toh(networkll);
        }

        inline uint32_t ConvertNetworkToHostLong(uint32_t networkl) {
            return be32toh(networkl);
        }

        inline uint16_t ConvertNetworkToHostShort(uint16_t networks) {
            return be16toh(networks);
        }
    }
}


#endif //SANDSTORM_EXENDIAN_H
