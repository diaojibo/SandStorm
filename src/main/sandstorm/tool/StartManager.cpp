//
// Created by rocklct on 2/23/18.
//


#include "sandstorm/service/Manager.h"


#include "sandstorm/util/Configuration.h"
#include "sandstorm/base/Constants.h"

#include <iostream>
#include <string>
#include <csignal>

using namespace std;

void StartManager(const std::string &configFileName);

static void ProcessSignal(int signalNumber);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    StartManager(argv[1]);

    return EXIT_SUCCESS;
}

void StartManager(const std::string &configFileName) {
    sandstorm::util::Configuration managerConfiguration;
    managerConfiguration.Parse(configFileName);

    std::cout << managerConfiguration.GetProperty(sandstorm::CONF_KEY_PRESIDENT_HOST);
    std::cout << managerConfiguration.GetIntegerProperty(sandstorm::CONF_KEY_PRESIDENT_PORT);
    std::cout << managerConfiguration.GetProperty(sandstorm::CONF_KEY_MANAGER_HOST);
    std::cout << managerConfiguration.GetIntegerProperty(sandstorm::CONF_KEY_MANAGER_PORT);


    signal(SIGINT, ProcessSignal);

    sandstorm::service::Manager manager(managerConfiguration);
    manager.JoinPresident([&manager](const sandstorm::message::Response &response) {
        if (response.GetStatus() != sandstorm::message::Response::Status::Successful) {
            std::cout << "Can't join president.";
            std::cout << "Exit with failure.";

            exit(EXIT_FAILURE);
        } else {
            std::cout << "Join successfully";
        }

        manager.StartListen();
    });
}


static void ProcessSignal(int signalNumber) {
    std::cout << "Receive signal number: " << signalNumber;
    std::cout << "Exit";
    exit(0);
}