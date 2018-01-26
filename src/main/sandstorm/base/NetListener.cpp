//
// Created by Rocklct on 2017/12/26.
//



#include "sandstorm/base/NetListener.h"

#include "sandstorm/network/Eventqueue.h"
#include "sandstorm/network/EventQueueLoop.h"
#include "sandstorm/network/epoll/EpollLoop.h"

#include <iostream>
#include <thread>
#include <chrono>

const int DATA_BUFFER_SIZE = 65535;

void NetListener::StartListen() {
    sandstorm::network::EpollLoop::Get()->Start();

    _server.Listen(_host.GeHost(), _host.GetPort());
    _server.OnConnect([this](sandstorm::network::IStream *stream) {
        stream->OnData([stream, this](const char *buf, int64_t size) mutable {
            this->_receiver(dynamic_cast<sandstorm::network::TcpStream *>(stream));
        });
    });

}