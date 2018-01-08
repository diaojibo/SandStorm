//
// Created by Rocklct on 2017/12/28.
//

#pragma once

#include "sandstorm/base/Values.h"

#include <string>
#include <memory>
#include <map>

namespace sandstorm {
    namespace spout {
        class ISpout;
    }

    namespace bolt {
        class IBolt;
    }

    namespace topology {
        class ITopology {
            ~ITopology() {}

            virtual void SetSpout(std::map <std::string, std::shared_ptr<sandstorm::spout::ISpout>> spouts) = 0;

            virtual void SetBolts(std::map <std::string, std::shared_ptr<sandstorm::bolt::IBolt>> bolts) = 0;

            virtual void SetNetwork(std::map <std::string, std::vector<std::string>> network) = 0;

            virtual void EmitFrom(const std::string &src, const base::Values &values) = 0;

            virtual const std::map<std::string,std::shared_ptr<sandstorm::spout::ISpout>>& GetSpouts() const = 0;
            virtual const std::map<std::string,std::shared_ptr<sandstorm::bolt::IBolt>>& GetBolts() const = 0;
            virtual const std::map<std::string,std::vector<std::string>>& GetNetwork() const = 0;

            virtual void Start() = 0;
        };
    }
}