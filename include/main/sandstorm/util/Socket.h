//
// Created by rocklct on 1/29/18.
//

#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include <cstdio>
#include <cerrno>

#define SOCKET int32_t
#define closesocket close
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define LPSOCKADDR sockaddr*

#define DISPLAY_SOCKET_ERROR(message) perror(message)


#include "sandstorm/util/SocketError.h"

#include <string>
#include <functional>
#include <exception>
#include <cstdint>

namespace sandstorm {
    namespace util {
        class Socket {
        public:
            typedef std::function<void(int32_t sentSize, const SocketError &error)> SendCallback;
            typedef std::function<void(char *buf, int32_t size, const SocketError &error)> ReceiveCallback;

            Socket();

            ~Socket();

            SOCKET GetSocket() const {
                return _socket;
            }

            int32_t Send(const char *buf, size_t size);

            void SendAsync(const char *buf, size_t size, SendCallback callback);

            int32_t Receive(char *buf, size_t size);

            bool ReceiveAsync(char *buf, size_t size);

            void OnData(ReceiveCallback callback);


        protected:
            SOCKET _socket;
            ReceiveCallback _receiveCallback;

        };


        class TcpConnection : public Socket {
        public:
            TcpConnection(SOCKET socket, sockaddr_in clientAddress);

        private:
            sockaddr_in _clientAddress;
        };


        class TcpServer : public Socket {
        public:
            TcpServer();

            void Listen(const std::string &address, int32_t port);

            TcpConnection *Accept();
        };

        class TcpClient : public Socket {
        public:
            TcpClient();

            void Connect(const std::string &address, int32_t port);
        };
    }
}