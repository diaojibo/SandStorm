#pragma once

#include "Fields.h"


namespace SandStorm {
    namespace base {
        class ITask {
        public:
            virtual ~ITask() {}

            virtual Fields DeclareFields() const =0;
        };
    }
}