//
// Created by rocklct on 1/22/18.
//

#ifndef SANDSTORM_NETWORK_H
#define SANDSTORM_NETWORK_H

#include "sandstorm/network/epoll/EpollLoop.h"
#include "sandstorm/network/epoll/EpollServer.h"
#include "sandstorm/network/epoll/EpollClient.h"
#include "sandstorm/network/epoll/EpollStream.h"
#include "sandstorm/network/epoll/EpollConnection.h"


namespace sandstorm {
    namespace network {
        typedef EpollServer TcpServer;
        typedef EpollConnection TcpConnection;
        typedef EpollClient TcpClient;
        typedef EpollStream TcpStream;
    }
}


#endif //SANDSTORM_NETWORK_H
