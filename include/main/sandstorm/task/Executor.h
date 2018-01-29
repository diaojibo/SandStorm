//
// Created by rocklct on 1/29/18.
//

#pragma once

namespace sandstorm {
    namespace task {
        class Executor {
        public:
            struct MessageType {
                enum {
                    OnTuple
                };
            };

            virtual ~Executor() {}

            virtual void Start() = 0;
        };
    }
}