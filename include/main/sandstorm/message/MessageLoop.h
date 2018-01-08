//
// Created by Rocklct on 2017/12/25.
//

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>

namespace sandstorm {
    namespace message {
        class Message;

        // Message Queue
        class MessageLoop {
        public:
            typedef std::function<void(Message *)> MessageHandler;

            MessageLoop();

            MessageLoop(const MessageLoop &) = delete;

            const MessageLoop &operator=(const MessageLoop &) = delete;

            template<class ObjectType, class MethodType>
            void MessageMap(int messageType, ObjectType *self, MethodType method) {
                MessageMap(messageType, std::bind(method, self, std::placeholders::_1));
            };

            void MessageMap(int messageType, MessageHandler handler) {
                _messageHandlers.insert({messageType, handler});
            }

            void Run();

            void Stop();

            void PostMessage(Message *message);

        private:
            std::map<int, MessageHandler> _messageHandlers;
            uint64_t _threadId;
        };

        class MessageLoopManager {
        public:
            static MessageLoopManager &GetInstance() {
                static MessageLoopManager manager;
                return manager;
            }

            MessageLoopManager(const MessageLoopManager &) = delete;

            const MessageLoopManager &operator=(const MessageLoopManager &) = delete;

            void Register(const std::string &name, MessageLoop *loop) {
                _messageLoops.insert({name, std::shared_ptr<MessageLoop>(loop)});
            }

            void PostMessage(const std::string &name, Message *message) {
                auto messageLoopPair = _messageLoops.find(name);
                if (messageLoopPair != _messageLoops.end()) {
                    messageLoopPair->second->PostMessage(message);
                }
            }

        private:
            MessageLoopManager() {}

            std::map <std::string, std::shared_ptr<MessageLoop>> _messageLoops;

        };
    }
}