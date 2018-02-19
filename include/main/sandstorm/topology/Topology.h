//
// Created by rocklct on 2/18/18.
//

#pragma once

#include "sandstorm/spout/SpoutDeclarer.h"
#include "sandstorm/bolt/BoltDeclarer.h"

#include <memory>
#include <map>
#include <string>

namespace sandstorm {
    namespace spout {
        class ISpout;
    }

    namespace bolt {
        class IBolt;
    }


    namespace topology {
        class Topology {
        public:
            Topology(const std::string &name);

            sandstorm::spout::SpoutDeclarer &SetSpout(const std::string &spoutName, sandstorm::spout::ISpout *spout);

            sandstorm::bolt::BoltDeclarer &SetBolt(const std::string &boltName, sandstorm::bolt::IBolt *bolt);

            const std::string &GetName() const {
                return _name;
            }

            const std::map<std::string, sandstorm::spout::SpoutDeclarer> &GetSpoutDeclarers() const {
                return _spoutDeclarers;
            }

            const std::map<std::string, sandstorm::bolt::BoltDeclarer> &GetBoltDeclarers() const {
                return _boltDeclarers;
            }

        private:
            std::string _name;
            std::map<std::string, sandstorm::spout::SpoutDeclarer> _spoutDeclarers;
            std::map<std::string, sandstorm::bolt::BoltDeclarer> _boltDeclarers;
        };

    }
}
