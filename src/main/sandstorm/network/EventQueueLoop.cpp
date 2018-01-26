//
// Created by rocklct on 1/26/18.
//

#include "sandstorm/network/EventQueueLoop.h"
#include "sandstorm/network/Eventqueue.h"

namespace sandstorm {
    namespace network {
        EventQueueLoop::EventQueueLoop(EventQueue *queue) : _queue(queue) {}

        void EventQueueLoop::_Run() {
            while (true) {
                std::shared_ptr<BaseEvent> event = _queue->GetEvent();
                if (!event) {
                    continue;
                }

                OnEvent(event);
            }
        }
    }
}
