//
// Created by rocklct on 1/29/18.
//

#pragma once

#include "sandstorm/util/NetListener.h"
#include "sandstorm/message/Command.h"
#include "sandstorm/base/ByteArray.h"

namespace sandstorm {
    namespace util {
        class TcpConnection;
    }

    namespace message {
        class BaseCommandServerContext {
        public:
            const std::string &GetId() const {
                return _id;
            }

            void SetId(const std::string &id) {
                _id = id;
            }

        private:
            std::string _id;
        };


        template<class CommandServerContext>
        class CommandServer {

        public:
            typedef std::function<void(const Response &response)> Responsor;
            typedef std::function<void(CommandServerContext *context)> ConnectHandler;
            typedef std::function<void(CommandServerContext *context, const Command &command,
                                       Responsor)> CommandHandler;

            CommandServer(sandstorm::util::NetListener *listener = nullptr);

            virtual ~CommandServer();

            void StartListen();

            void OnConnection(ConnectHandler handler);


            template<class ObjectType, class HandlerType>
            void OnCommand(int32_t commandType, ObjectType *self, HandlerType handler) {
                OnCommand(commandType, std::bind(handler, self, std::placeholders::_1, std::placeholders::_2,
                                                 std::placeholders::_3));
            }

            void OnCommand(int32_t commandType, CommandHandler handler);

            void Response(sandstorm::util::TcpConnection *connection, const Response &response);

            void SetListener(sandstorm::util::NetListener *listener) {
                _listener = listener;
            }


        private:
            sandstorm::util::NetListener *_listener;
            std::map<int32_t, CommandHandler> _commandHandlers;
            ConnectHandler _connectHandler;

        };
    }
}

#include "sandstorm/message/CommandServer.tcc"