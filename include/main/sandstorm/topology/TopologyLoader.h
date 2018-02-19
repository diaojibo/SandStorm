//
// Created by rocklct on 2/18/18.
//

#pragma once

#include "sandstorm/topology/Topology.h"
#include "sandstorm/base/Library.h"

#include <map>
#include <string>
#include <memory>

namespace sandstorm {
    namespace topology {
        class Topology;

        class TopologyLoader {
        public:
            typedef Topology *(*TopologyGetter)();

            static TopologyLoader &GetInstance();

            std::shared_ptr<Topology> GetTopology(const std::string &name);

        private:
            TopologyLoader() {}

            TopologyLoader(const TopologyLoader &loader) = delete;

            const TopologyLoader &operator=(const TopologyLoader &loader) = delete;

            std::map<std::string, std::shared_ptr<Topology>> _topologies;
            std::map<std::string, LibraryHandle> _libraryHandles;
        };
    }
}
