//
// Created by rocklct on 2/22/18.
//

#include "sandstorm/topology/TopologyLoader.h"
#include "sandstorm/base/Library.h"

namespace sandstorm {
    namespace topology {

        const std::string &GET_TOPOLOGY_INTERFACE = "GetTopology";

        TopologyLoader &TopologyLoader::GetInstance() {
            static TopologyLoader instance;

            return instance;
        }

        std::shared_ptr<Topology> TopologyLoader::GetTopology(const std::string &topologyName) {
            if (_libraryHandles.find(topologyName) == _libraryHandles.end()) {
                LibraryHandle libraryHandle = SandstormLibraryLoad(topologyName);
                _libraryHandles[topologyName] = libraryHandle;
                TopologyGetter topologyGetter =
                        SandstormLibraryGetSymbol<TopologyGetter>(libraryHandle, GET_TOPOLOGY_INTERFACE);

                _topologies[topologyName].reset(topologyGetter());
            }

            return _topologies[topologyName];
        }

    }
}