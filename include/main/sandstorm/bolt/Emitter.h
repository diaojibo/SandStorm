//
// Created by rocklct on 1/28/18.
//

#pragma once

#include "sandstorm/bolt/IBolt.h"


namespace sandstorm {
    namespace bolt {
        class Emitter : public IBolt {
        public:
            virtual void Prepare(base::OutputCollector &outputCollector) {
                _outputCollector = &outputCollector;
            }

            virtual void Cleanup() {

            }

            virtual void Execute(const base::Values &values) {

            }

        private:
            base::OutputCollector *_outputCollector;
        };
    }
}
