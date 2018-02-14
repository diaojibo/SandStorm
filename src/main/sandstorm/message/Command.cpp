//
// Created by rocklct on 2/14/18.
//

#include "sandstorm/message/Command.h"
#include "sandstorm/base/DataPackage.h"

#include <iostream>

namespace sandstorm {
    namespace message {
        Command::Command(int32_t type) : _type(type) {
        }

        Command::Command(int32_t type, std::vector<sandstorm::base::Variant> &arguments) :
                _type(type), _arguments(arguments) {
        }

        void Command::AddArgument(const sandstorm::base::Variant &argument) {
            _arguments.push_back(argument);
        }

        void Command::AddArguments(const std::vector<sandstorm::base::Variant> &arguments) {
            for (const sandstorm::base::Variant &argument : arguments) {
                _arguments.push_back(argument);
            }
        }

        void Command::Deserialize(const sandstorm::base::ByteArray &data) {
            sandstorm::base::DataPackage dataPackage;
            dataPackage.Deserialize(data);

            if (!dataPackage.GetVariants().size()) {
                std::cout << "Data package error";
            }

            _arguments = dataPackage.GetVariants();
            _type = _arguments[0].GetInt32Value();
            _arguments.erase(_arguments.begin());
        }

        sandstorm::base::ByteArray Command::Serialize() const {
            sandstorm::base::DataPackage dataPackage;

            dataPackage.AddVariant(sandstorm::base::Variant(_type));
            for (const sandstorm::base::Variant &argument : _arguments) {
                dataPackage.AddVariant(argument);
            }

            return dataPackage.Serialize();
        }

        Response::Response(int32_t status) : _status(status) {
        }

        Response::Response(int32_t status, std::vector<sandstorm::base::Variant> &arguments) :
                _status(status), _arguments(arguments) {
        }

        void Response::AddArguments(const std::vector<sandstorm::base::Variant> &arguments) {
            for (const sandstorm::base::Variant &argument : arguments) {
                _arguments.push_back(argument);
            }
        }

        void Response::AddArgument(const sandstorm::base::Variant &argument) {
            _arguments.push_back(argument);
        }

        void Response::Deserialize(const sandstorm::base::ByteArray &data) {
            sandstorm::base::DataPackage dataPackage;
            dataPackage.Deserialize(data);

            if (!dataPackage.GetVariants().size()) {
                std::cout << "Data package error";
            }

            _arguments = dataPackage.GetVariants();
            _status = _arguments[0].GetInt32Value();
            _arguments.erase(_arguments.begin());
        }

        sandstorm::base::ByteArray Response::Serialize() const {
            sandstorm::base::DataPackage dataPackage;

            dataPackage.AddVariant(sandstorm::base::Variant(_status));
            for (const sandstorm::base::Variant &argument : _arguments) {
                dataPackage.AddVariant(argument);
            }

            return dataPackage.Serialize();
        }

    }
}