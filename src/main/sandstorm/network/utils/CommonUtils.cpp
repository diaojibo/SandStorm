//
// Created by rocklct on 1/21/18.
//


#include "sandstorm/network/utils/CommonUtils.h"

#include <cstdio>

namespace sandstorm {
    namespace network {
        int32_t sandstorm::network::SetNonBlocking(int32_t sockfd) {
            int32_t opts;

            opts = fcntl(sockfd, F_GETFL);
            if (opts < 0) {
                perror("fcntl(F_GETFL\n");
                return -1;
            }

            opts = (opts | O_NONBLOCK);
            if (fcntl(sockfd, F_SETFL, opts) < 0) {
                perror("fcntl(F_SETFL)\n");
                return -1;
            }

            return 0;
        }
    }
}