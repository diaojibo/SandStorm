//
// Created by rocklct on 2/22/18.
//

#include "sandstorm/util/Socket.h"

#include <iostream>

namespace sandstorm {
    namespace util {
        Socket::Socket() {

        }

        Socket::~Socket() {
            closesocket(_socket);
        }

        int32_t Socket::Send(const char *buf, size_t size) {
            struct sockaddr addr;
            int32_t iSend = static_cast<int32_t>(sendto(_socket, buf, static_cast<int32_t>(size), 0, &addr,
                                                        sizeof(addr)));
            if (iSend == SOCKET_ERROR) {
                throw SocketError(SocketError::Type::SendError, "send() Failed");
            }

            return iSend;
        }


        void Socket::SendAsync(const char *buf, size_t size, SendCallback callback) {
            try {
                int32_t sentSize = Send(buf, size);
                callback(sentSize, SocketError());
            }
            catch (const SocketError &e) {
                std::cout << e.what() << std::endl;
                callback(0, e);
            }
        }

        int32_t Socket::Receive(char *buf, size_t size) {
            int32_t iLen = static_cast<int32_t>(recv(_socket, buf, static_cast<int32_t>(size), 0));

            if (iLen == SOCKET_ERROR) {
                throw SocketError(SocketError::Type::RecvError, "recv() Failed");
            }

            return iLen;
        }

        bool Socket::ReceiveAsync(char *buf, size_t size) {
            try {
                int32_t receivedSize = Receive(buf, size);

                if (!receivedSize) {
                    return false;
                }

                _receiveCallback(buf, receivedSize, SocketError());
                return true;
            }
            catch (const SocketError &e) {
                _receiveCallback(nullptr, 0, e);
            }

            return false;
        }

        void Socket::OnData(ReceiveCallback callback) {
            _receiveCallback = callback;
        }


        /* TcpConnection */

        TcpConnection::TcpConnection(SOCKET socket, sockaddr_in clientAddress) :
                _clientAddress(clientAddress) {
            _socket = socket;
        }


        /* Tcp Server */

        TcpServer::TcpServer() {
            _socket = socket(AF_INET, SOCK_STREAM, 0);
            if (_socket == INVALID_SOCKET) {
                DISPLAY_SOCKET_ERROR("socket() Failed.");
            }
        }

        void TcpServer::Listen(const std::string &address, int32_t port) {
            sockaddr_in serverAddress;

            serverAddress.sin_family = AF_INET;
            serverAddress.sin_port = htons(port);
            serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

            if (bind(_socket, (const LPSOCKADDR) &serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
                DISPLAY_SOCKET_ERROR("bind() Failed");
            }

            if (listen(_socket, 5) == SOCKET_ERROR) {
                DISPLAY_SOCKET_ERROR("lisiten() Failed");
            }
        }

        TcpConnection *TcpServer::Accept() {
            sockaddr_in clientAddress;

            socklen_t length = sizeof(clientAddress);

            SOCKET clientSocket = accept(_socket, (struct sockaddr *) &clientAddress, &length);

            if (clientSocket == INVALID_SOCKET) {
                DISPLAY_SOCKET_ERROR("accept() Failed");
            }


            std::cout << "Accepted client IP: " << inet_ntoa(clientAddress.sin_addr) << ",port:["
                      << ntohs(clientAddress.sin_port) << "]" << std::endl;


            return new TcpConnection(clientSocket, clientAddress);
        }


        /* TcpClient */


        TcpClient::TcpClient() {
            _socket = socket(AF_INET, SOCK_STREAM, 0);

            if (_socket == INVALID_SOCKET) {
                DISPLAY_SOCKET_ERROR("socket() Failed");
            }
        }

        void TcpClient::Connect(const std::string &address, int32_t port) {
            sockaddr_in serverAddress;

            serverAddress.sin_family = AF_INET;
            serverAddress.sin_port = htons(port);


            serverAddress.sin_addr.s_addr = inet_addr(address.c_str());

            int32_t ret = connect(_socket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));

            if (ret == INVALID_SOCKET) {
                DISPLAY_SOCKET_ERROR("connect() Failed");
                throw SocketError(SocketError::Type::ConnectError, "connect() Failed");
            }
        }
    }
}