#pragma once

#include "sandstorm/base/Fields.h"


namespace sandstorm {
    namespace base {
        class ITask {
        public:

            struct Strategy {
                enum Values {
                    Global = 0,
                    Random = 1,
                    Group = 2
                };
            };

            virtual ~ITask() {}

            virtual Fields DeclareFields() const =0;

            Strategy::Values GetStrategy() {
                return _strategy;
            }

            void SetStrategy(Strategy::Values strategy) {
                _strategy = strategy;
            }

        private:
            Strategy::Values _strategy;

        };
    }
}