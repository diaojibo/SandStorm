//
// Created by rocklct on 2/18/18.
//

#pragma once

#include "sandstorm/task/TaskDeclarer.h"
#include <memory>
#include <string>
#include <vector>
#include <map>


namespace sandstorm {
    namespace spout {
        class ISpout;

        class SpoutDeclarer : public sandstorm::task::TaskDeclarer {
        public:
            SpoutDeclarer() = default;

            SpoutDeclarer(const std::string &spoutName, ISpout *spout);

            SpoutDeclarer &ParallismHint(int32_t parallismHint) {
                SetParallismHint(parallismHint);

                return *this;
            }

            std::shared_ptr<ISpout> GetSpout() const {
                return _spout;
            }

            const std::vector<std::string> &GetFields() const {
                return _fields;
            }

            const std::map<std::string, int32_t> &GetFieldsMap() const {
                return _fieldsMap;
            }

        private:
            std::shared_ptr<ISpout> _spout;
            std::vector<std::string> _fields;
            std::map<std::string, int32_t> _fieldsMap;
        };


    }
}