//
// Created by rocklct on 1/20/18.
//


#include "sandstorm/network/epoll/EpollLoop.h"

#include <signal.h>
#include <cassert>

namespace sandstorm {
    namespace network {
        using namespace std::placeholders;

        EpollLoop *EpollLoop::Get() {
            static EpollLoop epollLoop;
            return &epollLoop;
        }

        EpollLoop::EpollLoop() {


            // block SIGPIPE signal
            sigset_t set;
            sigemptyset(&set);
            sigaddset(&set, SIGPIPE);
            sigprocmask(SIG_BLOCK, &set, NULL);

            _Initialize();

        }

        EpollLoop::~EpollLoop() {
            _shutdown = true;
        }

        void EpollLoop::AddServer(NativeSocket socket, EpollServer *server) {
            _servers.insert({socket, server});
        }

        void EpollLoop::AddStream(EpollStreamPtr stream) {
            _stream[stream->GetNativeSocket()] = stream;
        }

        int32_t EpollLoop::AddEpollEvents(int32_t events, int32_t fd) {
            NativeSocketEvent ev;
            ev.events = events;
            ev.data.fd = fd;

            return epoll_ctl(_eventfd, EPOLL_CTL_ADD, fd, &ev);
        }

        int32_t EpollLoop::ModifyEpollEvents(int32_t events, int32_t fd) {
            NativeSocketEvent ev;
            ev.events = events;
            ev.data.fd = fd;

            return epoll_ctl(_eventfd, EPOLL_CTL_MOD, fd, &ev);
        }

        void EpollLoop::_Initialize() {
            _eventfd = epoll_create(MAX_EVENT_COUNT);
            if (_eventfd == -1) {
                assert(0);
            }
        }

        void EpollLoop::_Run() {
            auto func = std::bind(&EpollLoop::_EpollThread, this);
            std::thread listenThread(func);
            listenThread.detach();
        }

        void EpollLoop::_EpollThread() {
            NativeSocketEvent events[MAX_EVENT_COUNT];

            while (!_shutdown) {
                int32_t nfds;

                //The number of events to be handled
                nfds = epoll_wait(_eventfd, events, MAX_EVENT_COUNT, -1);

                if (-1 == nfds) {
                    exit(EXIT_FAILURE);
                }

                _HandleEvent(_eventfd, events, nfds);
            }
        }

        void EpollLoop::_HandleEvent(int32_t eventfd, struct epoll_event *events, int32_t nfds) {

            for (int32_t i = 0; i < nfds; ++i) {
                int32_t fd;
                fd = events[i].data.fd;

                //The event is from a server listen
                if (_servers.find(fd) != _servers.end()) {
                    _Accept(eventfd, fd);
                    continue;
                }

                //The event is from a client connection
                int32_t n = 0;
                if (events[i].events & EPOLLIN) {
                    _Read(eventfd, fd, events[i].events);
                }

                if (events[i].events & EPOLLOUT) {

                }


            }

        }

        int32_t EpollLoop::_Accept(int32_t eventfd, int32_t listenfd) {
            EpollServer *server = _servers.find(listenfd)->second;
            EpollConnectionPtr connection = server->Accept(eventfd);

            if (connection != nullptr) {
                _stream[connection->GetNativeSocket()] = connection;
            }
        }

        void EpollLoop::_Read(int32_t eventfd, int32_t fd, uint32_t events) {
            EpollStreamPtr stream = _stream[fd];

            char buffer[BUFSIZ];
            int32_t readSize;
            int32_t nread = stream->Receive(buffer, BUFSIZ, readSize);

            stream->SetEvents(events);

            if ((nread == -1 && errno != EAGAIN) || readSize == 0) {
                _stream.erase(fd);

                return;
            }

            _Enqueue(stream, buffer, readSize);

        }

        void EpollLoop::_Enqueue(EpollStreamPtr connection, const char *buf, int64_t nread) {
            if (connection->GetDataHandler()) {
                connection->GetDataHandler()(buf, nread);
            }
        }

    }
}