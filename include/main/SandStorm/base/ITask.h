#pragma once

#include "Fields.h"


namespace sandstorm {
    namespace base {
        class ITask {
        public:
            virtual ~ITask() {}

            virtual Fields DeclareFields() const =0;
        };
    }
}