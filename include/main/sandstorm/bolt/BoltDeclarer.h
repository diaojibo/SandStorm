//
// Created by rocklct on 1/28/18.
//

#pragma once

#include "sandstorm/task/TaskDeclarer.h"
#include <memory>
#include <string>
#include <map>
#include <vector>

namespace sandstorm {
    namespace bolt {
        class IBolt;

        class BoltDeclarer : public sandstorm::task::TaskDeclarer {

        public:
            BoltDeclarer() = default;

            BoltDeclarer(const std::string &boltName, IBolt *bolt);

            BoltDeclarer &ParallismHint(int32_t parallismHint);

            BoltDeclarer &Global(const std::string &sourceTaskName);

            BoltDeclarer &Field(const std::string &sourceTaskName, const std::string &groupField);

        private:
            std::shared_ptr<IBolt> _bolt;
            std::string _groupField;
            std::vector<std::string> _fields;
            std::map<std::string, int32_t> _fieldsMap;
        };
    }
}