//
// Created by rocklct on 1/19/18.
//

#ifndef SANDSTORM_EPOLLLOOP_H
#define SANDSTORM_EPOLLLOOP_H

#include "sandstorm/network/Loop.h"
#include "sandstorm/network/epoll/EpollStream.h"
#include "sandstorm/network/epoll/EpollConnection.h"
#include "sandstorm/network/epoll/EpollServer.h"
#include "sandstorm/network/epoll/EpollClient.h"

#include "sandstorm/network/Net.h"
#include "sandstorm/network/DataSink.h"
#include "sandstorm/network/Common.h"

#include <map>
#include <memory>
#include <thread>
#include <string>

#include <sys/epoll.h>

namespace sandstorm {
    namespace network {
        class EventQueue;

        class EpollServer;

        class EpollLoop : public Loop {

        public:
            static EpollLoop *Get();

            virtual ~EpollLoop() override;

            void AddServer(NativeSocket socket, EpollServer *server);

            void AddStream(EpollStreamPtr stream);

            int32_t AddEpollEvents(int32_t events, int32_t fd);

            int32_t ModifyEpollEvents(int32_t events, int32_t fd);

        protected:
            EpollLoop();

            virtual void _Run() override;

        private:

            void _Initialize();

            void _EpollThread();

            void _HandleEvent(int32_t eventfd, struct epoll_event *events, int32_t nfds);

            int32_t _Accept(int32_t eventfd, int32_t listenfd);

            void _Read(int32_t eventfd, int32_t fd, uint32_t events);

            void _Enqueue(EpollStreamPtr connection, const char *buf, int64_t nread);


            int32_t _eventfd;
            bool _shutdown;

            std::map<NativeSocket, EpollServer *> _servers;
            std::map<NativeSocket, EpollStreamPtr> _stream;
        };
    }
}


#endif //SANDSTORM_EPOLLLOOP_H
