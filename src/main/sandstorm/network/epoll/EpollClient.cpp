//
// Created by rocklct on 1/22/18.
//

#include "sandstorm/network/epoll/EpollClient.h"
#include "sandstorm/network/utils/CommonUtils.h"
#include "sandstorm/network/epoll/EpollLoop.h"

#include <unistd.h>

namespace sandstorm {
    namespace network {
        void EpollClient::Connect(const std::string &host, int32_t port) {
            struct sockaddr_in serv_addr;

            bzero((char *) &serv_addr, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = inet_addr(host.c_str());
            serv_addr.sin_port = htons(port);

            SetNonBlocking(GetNativeSocket());

            connect(GetNativeSocket(), (
                    struct sockaddr *) &serv_addr, sizeof(serv_addr));
        }

        EpollClientPtr EpollClient::Connect(const std::string &ip, int32_t port, DataSink *dataSink) {
            int32_t clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            //Connect
            EpollClientPtr client = EpollClientPtr(new EpollClient(clientSocket));

            client->SetDataSink(dataSink);
            client->Connect(ip, port);


            EpollLoop *epollLoop = EpollLoop::Get();

            client->_events = EPOLLIN | EPOLLET;

            if (epollLoop->AddEpollEvents(client->_events, clientSocket) == -1) {
                perror("epoll_ctl:add");
                std::cout << "epoll_ctl:add" << std::endl;
                exit(EXIT_FAILURE);
            }

            epollLoop->AddStream(client);

            return client;

        }

        int32_t EpollClient::Receive(char *buffer, int32_t bufferSize, int32_t &readSize) {
            readSize = 0;
            int32_t nread = 0;
            NativeSocketEvent ev;

            while ((nread = read(GetNativeSocket(), buffer + readSize, bufferSize - 1)) > 0) {
                readSize += nread;
            }

            return nread;
        }

        int32_t EpollClient::Send(const ByteArray &byteArray) {
            struct epoll_event ev;
            NativeSocket clientSocket = GetNativeSocket();

            if (EpollLoop::Get()->ModifyEpollEvents(_events | EPOLLOUT, clientSocket)) {
                perror("FATAL epoll_ctl:mod failed");
                std::cout << "FATAL epoll_ctl:mod failed" << std::endl;
            }

            const char *buf = byteArray.data();
            int32_t size = byteArray.size();
            int32_t n = size;

            while (n > 0) {
                int32_t nwrite;
                nwrite = write(clientSocket, buf + size - n, n);
                if (nwrite < n) {
                    if (nwrite == -1 && errno != EAGAIN) {
                        perror("FATAL write data to peer failed!");
                        std::cout << "FATAL write data to peer failed!" << std::endl;
                        break;
                    }
                }
                n -= nwrite;
            }

            return 0;
        }


    }
}

