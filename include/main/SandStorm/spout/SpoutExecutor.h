//
// Created by Rocklct on 2017/12/26.
//

# pragma once

#include "../base/Executor.h"
#include "ISpout.h"

#include <iostream>

namespace sandstorm {
    namespace topology {
        class ITopology;
    }

    namespace message {
        class SupervisorCommander;
    }

    namespace spout {
        class SpoutOutputCollector;

        class SpoutExecutor : public base::Executor<spout::ISpout> {
        public:
            SpoutExecutor() :
                    base::Executor<spout::ISpout>(), _needToStop(false) {

            }

            void StopTask() override;

            void OnCreate() override;

            void OnStop() override;

            void SetExecutorIndex(int executorIndex) {
                _executorIndex = executorIndex;
            }

            void SetCommander(message::SupervisorCommander *commander);

            void RandomDestination(SpoutOutputCollector *outputCollector);

            void GroupDestination(SpoutOutputCollector *outputCollector, int fieldIndex);

        private:
            topology::ITopology *_topology;
            bool _needToStop;
            message::SupervisorCommander *_commander;
            int _executorIndex;
        };
    }
}
