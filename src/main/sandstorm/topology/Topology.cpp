//
// Created by rocklct on 2/22/18.
//

#include "sandstorm/topology/Topology.h"

namespace sandstorm {
    namespace topology {
        Topology::Topology(const std::string& name) : _name(name) {
        }

        sandstorm::spout::SpoutDeclarer& Topology::SetSpout(const std::string& spoutName, sandstorm::spout::ISpout* spout) {
            _spoutDeclarers.insert({ spoutName, sandstorm::spout::SpoutDeclarer(spoutName, spout) });

            return _spoutDeclarers[spoutName];
        }

        sandstorm::bolt::BoltDeclarer& Topology::SetBolt(const std::string& boltName, sandstorm::bolt::IBolt* bolt) {
            _boltDeclarers.insert({ boltName, sandstorm::bolt::BoltDeclarer(boltName, bolt) });

            return _boltDeclarers[boltName];
        }
    }
}