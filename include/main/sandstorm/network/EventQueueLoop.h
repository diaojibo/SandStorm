//
// Created by rocklct on 1/26/18.
//

#pragma once

#include "sandstorm/network/Loop.h"
#include <memory>

const int32_t MAX_EVENT_COUNT = 5000;
const int32_t MAX_RECV_BUFF = 65535;


namespace sandstorm {
    namespace network {
        class EventQueue;

        class BaseEvent;

        class EventQueueLoop : public Loop {

        public:

            EventQueueLoop(EventQueue *queue);

        protected:
            virtual void _Run();

            virtual void OnEvent(std::shared_ptr<BaseEvent> event) = 0;


        private:
            EventQueue *_queue;
        };
    }
}
