//
// Created by Rocklct on 2017/12/24.
//

#pragma once

#include "sandstorm/base/NetAddress.h"
#include "sandstorm/base/NetConnector.h"
#include "sandstorm/base/Values.h"
#include <string>

namespace sandstorm {
    namespace message {
        class SupervisorCommander {
        public:
            SupervisorCommander(const sandstorm::base::NetAddress &nimbusAddress,
                                const std::string &supervisorName) :
                    _nimbusAddress(nimbusAddress), _supervisorName(supervisorName) {}

            void Connect() {
                if (!_connector.get()) {
                    _connector = std::make_shared<NetConnector>(_nimbusAddress);
                    _connector->Connect();
                }
            }

            void Join();

            void Alive;

            void SendTuple(int taskIndex, const base::Values &values);

            void RandomDestination(const std::string srcType, int srcIndex,
                                   std::string *host, int *port, int *destIndex, int fieldIndex);

            const std::string GetSupervisorName() const {
                return _supervisorName;
            }

        private:
            sandstorm::base::NetAddress _nimbusAddress;
            std::string _supervisorName;
            std::shared_ptr<NetConnector> _connector;

        };
    }
}