//
// Created by rocklct on 1/21/18.
//

#ifndef SANDSTORM_COMMONUTILS_H
#define SANDSTORM_COMMONUTILS_H

#include <fcntl.h>
#include <cstdint>

namespace sandstorm {
    namespace network {
        int32_t SetNonBlocking(int32_t sockfd);
    }
}


#endif //SANDSTORM_COMMONUTILS_H
