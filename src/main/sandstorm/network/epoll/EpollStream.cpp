//
// Created by rocklct on 1/20/18.
//


#include "sandstorm/network/epoll/EpollStream.h"
#include "sandstorm/network/epoll/EpollLoop.h"

#include "sandstorm/network/ByteArray.h"

#include <unistd.h>

namespace sandstorm {
    namespace network {
        int32_t EpollStream::Receive(char *buffer, int32_t bufferSize, int32_t &readSize) {
            readSize = 0;
            int32_t nread = 0;
            NativeSocketEvent ev;

            while (nread = read(GetNativeSocket(), buffer + readSize, bufferSize - 1) > 0) {
                readSize += nread;
            }

            return nread;
        }

        int32_t EpollStream::Send(const ByteArray &byteArray) {
            struct epoll_event ev;
            NativeSocket clientSocket = GetNativeSocket();

            if (EpollLoop::Get()->ModifyEpollEvents(_events | EPOLLOUT, clientSocket)) {
                //Epoll fail

                std::cout << "Epoll failed" << std::endl;
            }

            const char *buf = byteArray.data();
            int32_t size = byteArray.size();
            int32_t n = size;

            while (n > 0) {
                int32_t nwrite;
                nwrite = write(clientSocket, buf + size - n, n);
                if (nwrite < n) {
                    if (nwrite == -1 && errno != EAGAIN) {
                        perror("write error");
                        break;
                    }
                }

                n -= nwrite;
            }


        }
    }
}