//
// Created by rocklct on 1/27/18.
//


#include "sandstorm/base/NetConnector.h"
#include "sandstorm/network/Network.h"

#include <thread>
#include <chrono>

void NetConnector::Connect() {
    _client = sandstorm::network::TcpClient::Connect(_host.GeHost(), _host.GetPort(), nullptr);

}

int32_t NetConnector::SendAndReceive(const char *buffer, int32_t size, char *resultBuffer, int32_t resultSize) {


    //send data to the server
    _client->Send(sandstorm::network::ByteArray(buffer, size));

    bool receiveData = false;


    //set the datahandler in the connection
    _client->OnData([&receiveData, &resultSize, &resultBuffer](const char *buf, int64_t size) {
        if (resultSize > size) {
            resultSize = size;
        }

        //after EpollLoop finish reading the data from server
        // OnData function will be called and the data will be copied to
        //the resultBuffer using memcpy below
        memcpy(resultBuffer, buf, resultSize);

        //if the data come,the variable will be set true
        receiveData = true;
    });


    // wait for the data
    while (!receiveData) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return resultSize;

}