//
// Created by rocklct on 2/18/18.
//


#include "sandstorm/task/TaskDeclarer.h"

namespace sandstorm {
    namespace task {
        TaskDeclarer::TaskDeclarer() :
                _type(Type::Invalid), _groupMethod(GroupMethod::Global), _parallismHint(1) {
        }

    }
}