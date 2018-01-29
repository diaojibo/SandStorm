//
// Created by rocklct on 1/27/18.
//

#include "sandstorm/base/OutputCollector.h"
#include "sandstorm/topology/ITopology.h"

#include <iostream>

namespace sandstorm {
    namespace base {
        void OutputCollector::Emit(const Values &values) {
            if(_strategy == Strategy::Global){
                if(_commander){
                    _commander->SendTuple(this->_taskIndex,values);
                }
            }
            else if(_strategy == Strategy::Random){
                this->RandomDestination();
                _commander->SendTuple(this->_taskIndex,values);
            } else if(_strategy == Strategy::Group){
                this->GroupDestination();
                _commander->SendTuple(this->_taskIndex,values);
            }
        }
    }
}