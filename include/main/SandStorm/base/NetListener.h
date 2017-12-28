//
// Created by Rocklct on 2017/12/26.
//

#pragma once

#include "NetAddress.h"
#include "net.h"
#include "Meshy.h"

#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <functional>
#include <cstdint>

typedef std::function<
void(meshy::TcpStream
* connection,
const char *buffer, int32_t
size)>
DataReceiver;

class NetListener {
public:
    NetListener(const sandstorm::base::NetAddress &host) : _host(host) {

    }

    const sandstorm::base::NetAddress& GetHost const {
        return _host;
    }

    void SetHost(const sandstorm::base::NetAddress& host){
        _host = host;
    }

    void StartListen();

    void DataThreadMain(std::shared_ptr<TcpConnection> connection);

    void OnData(DataReceiver receiver){
        _receiver = receiver;
    }


private:
    sandstorm::base::NetAddress _host;
    DataReceiver _receiver;
    meshy::TcpServer _server;
};