//
// Created by rocklct on 1/21/18.
//

#include "sandstorm/network/epoll/EpollServer.h"
#include "sandstorm/network/epoll/EpollLoop.h"
#include "sandstorm/network/utils/CommonUtils.h"


#include <cstdint>
#include <cassert>

#ifdef DISABLE_ASSERT
#ifdef assert
#undef assert
#endif

#define assert(x)
#endif

namespace sandstorm {
    namespace network {
        int32_t EpollServer::_Bind(const std::string &host, int32_t port) {

            int32_t listenfd;
            if (listenfd = socket(AF_INET, SOCK_STREAM, 0) < 0) {
                exit(1);
            }

            SetNativeSocket(listenfd);
            int32_t option = 1;

            //set socket option and approve to reuse address
            setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

            //make socket non-blocking
            SetNonBlocking(listenfd);

            NativeSocketAddress addr;
            bzero(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(host.c_str());

            int32_t errorCode = bind(listenfd, (struct sockaddr *) &addr, sizeof(addr));

            if (errorCode < 0) {
                assert(0);
                return errorCode;
            }

            return 0;

        }


        int32_t EpollServer::Listen(const std::string &host, int32_t port, int32_t backlog) {
            _Bind(host, port);

            int32_t listenfd = GetNativeSocket();

            int32_t errorCode = listen(listenfd, backlog);

            if (-1 == errorCode) {
                assert(0);
                return errorCode;
            }

            errorCode = EpollLoop::Get()->AddEpollEvents(EPOLLIN, listenfd);

            if (errorCode == -1) {
                assert(0);
                return errorCode;
            }

            EpollLoop::Get()->AddServer(listenfd, this);
        }

        EpollConnectionPtr EpollServer::Accept(int32_t sockfd) {
            int32_t conn_sock = 0;
            int32_t addrlen = 0;
            int32_t remote = 0;

            int32_t listenfd = GetNativeSocket();
            while ((conn_sock = accept(listenfd, (struct sockaddr *) &remote, (socklen_t *) &addrlen)) > 0) {
                SetNonBlocking(conn_sock);

                NativeSocketEvent ev;
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;

                if (epoll_ctl(sockfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                    perror("epoll_ctl: add");
                    exit(EXIT_FAILURE);
                }

                EpollConnectionPtr connection = std::make_shared<EpollConnection>(conn_sock);

                //execute OnConnect function
                if (_connectHandler) {
                    _connectHandler(connection.get());
                }

                return connection;

            }

            if (conn_sock == -1) {
                if (errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR) {
                    perror("accept");
                }
            }

            return EpollConnectionPtr(nullptr);

        }
    }
}