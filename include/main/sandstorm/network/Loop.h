//
// Created by rocklct on 1/19/18.
//

#pragma once

namespace sandstorm {
    namespace network {
        class Loop {
        public:
            virtual ~Loop() {}

            void Start() {
                _Run();
            }

        protected:
            virtual void _Run()=0;
        };

    }
}
