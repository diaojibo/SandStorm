//
// Created by rocklct on 2/18/18.
//


#include "sandstorm/task/SpoutExecutor.h"
#include "sandstorm/spout/ISpout.h"
#include <chrono>

namespace sandstorm {
    namespace task {
        const int32_t BASIC_FLOW_PARAM = 1000 * 1000;

        SpoutExecutor::SpoutExecutor() {

        }

        void SpoutExecutor::Start() {
            // It is managed by main thread, so we don't need to detach it
            _thread = std::thread(&SpoutExecutor::MainLoop, this);
        }

        void SpoutExecutor::MainLoop() {
            int32_t flowTime = BASIC_FLOW_PARAM / _flowParam;
            while (true) {
                _spout->NextTuple();
                std::this_thread::sleep_for(std::chrono::microseconds(flowTime));
            }
        }
    }
}
