//
// Created by rocklct on 1/24/18.
//

#pragma once

#include <string>

namespace sandstorm {
    const std::string NODE_PRESIDENT = "president";
    const std::string NODE_MANAGER = "manager";

    const std::string CONF_KEY_PRESIDENT_HOST = "president.host";
    const std::string CONF_KEY_PRESIDENT_PORT = "president.port";
    const std::string CONF_KEY_MANAGER_COUNT = "president.manager.count";
    const std::string CONF_KEY_MANAGER_NAME = "manager.name";
    const std::string CONF_KEY_MANAGER_HOST = "manager.host";
    const std::string CONF_KEY_MANAGER_PORT = "manager.port";
    const std::string CONF_KEY_TOPOLOGY_NAME = "topology.name";
    const std::string CONF_KEY_SPOUT_COUNT = "manager.spout.num";
    const std::string CONF_KEY_BOLT_COUNT = "manager.bolt.num";
    const std::string CONF_KEY_SPOUT_FLOW_PARAM = "spout.flow.param";

}
