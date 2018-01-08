//
// Created by Rocklct on 2017/12/26.
//

#pragma once

#include <string>

namespace sandstorm {
    namespace base {
        class NetAddress {
        public:
            NetAddress(const std::string& host,int port):_host(host),_port(port){

            }

            const std::string& GeHost() const {
                return _host;
            }

            void SetHost(const std::string& host){
                _host = host;
            }

            int GetPort() const {
                return _port;
            }

            void SetPort(int port){
                _port = port;
            }

        private:
            std::string _host;
            int _port;
        };
    }
}