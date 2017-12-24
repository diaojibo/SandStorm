//
// Created by Rocklct on 2017/12/24.
//

#pragma once

#include "Values.h"
#include "../message/SupervisorCommander.h"

namespace SandStorm {
    namespace topology {
        class ITopology;
    }

    namespace base {
        class OutputCollector {
        public:
            struct Strategy {
                enum Values {
                    Global = 0,
                    Random = 1,
                    Group = 2
                };
            };

            OutputCollector(const std::string &src, int strategy) :
                    _src(src), _strategy(strategy), _commander(nullptr) {}

            virtual void Emit(const Values &values);

            void SetCommander(SandStorm::messgae::SupervisorCommander *commander) {
                if (_commander) {
                    delete _commander;
                }
                _commander = commander;
            }

            void SetTaskIndex(int taskIndex){
                _taskIndex = taskIndex;
            }

            void SetGroupField(int groupField){
                _groupField = groupField;
            }

            int GetGroupField() const {
                return _groupField;
            }

            virtual void RandomDestination() = 0;
            virtual void GroupDestination() = 0;

        private:
            std::string _src;
            int _strategy;
            int _taskIndex;
            int _groupField;
            SandStorm::message::ManagerCommander *_commander;
        };
    }

}
