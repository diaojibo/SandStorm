//
// Created by Rocklct on 2017/12/27.
//

#pragma once

#include "NetAddress.h"
#include "Meshy.h"

#include <cstdint>
#include <memory>

class NetConnector {

public:
    NetConnector(const sandstorm::base::NetAddress &host) : _host(host) {

    }

    const sandstorm::base::NetAddress& GetHost const {
        return _host;
    }

    void SetHost(const sandstorm::base::NetAddress& host){
        _host = host;
    }

    void Connect();

    int32_t SendAndReceive(const char* buffer,int32_t size,char* resultBuffer,int32_t resultSize);


private:
    sandstorm::base::NetAddress _host;
    std::shared_ptr <TcpClient> _client;
};
