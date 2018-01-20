//
// Created by rocklct on 1/19/18.
//

#ifndef SANDSTORM_EPOLLCONNECTION_H
#define SANDSTORM_EPOLLCONNECTION_H

#include "sandstorm/network/Net.h"

#include "sandstorm/network/epoll/EpollStream.h"

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>


namespace sandstorm {
    namespace network {
        class EpollLoop;

        class EpollConnection : public EpollStream {
        public:
            EpollConnection(NativeSocket nativeSocket) : EpollStream(nativeSocket) {}

            virtual ~EpollConnection() {}

            EpollConnection(const EpollConnection &connection) = delete;
        };

        typedef std::shared_ptr<EpollConnection> EpollConnectionPtr;
    }
}


#endif //SANDSTORM_EPOLLCONNECTION_H
