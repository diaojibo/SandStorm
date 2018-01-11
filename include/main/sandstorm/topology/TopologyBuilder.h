//
// Created by rocklct on 1/11/18.
//


#pragma once

#include "sandstorm/topology/SimpleTopology.h"

#include <memory>
#include <map>
#include <vector>
#include <string>

namespace sandstorm {
    namespace topology {
        class TopologyBuilder {

            void SetSpout(const std::string &name, spout::ISpout *spout);

            void SetBolt(const std::string &name, bolt::IBolt *bolt, const std::string &prev);

            SimpleTopology *Build();


        private:
            std::map<std::string, std::shared_ptr<spout::ISpout>> _spouts;
            std::map<std::string, std::shared_ptr<bolt::IBolt>> _bolts;
            std::map<std::string, std::vector<std::string>> _networks;
        };
    }
}