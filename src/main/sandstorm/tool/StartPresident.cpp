//
// Created by rocklct on 2/23/18.
//

#include "sandstorm/service/President.h"

#include "sandstorm/topology/TopologyLoader.h"
#include "sandstorm/util/Configuration.h"
#include "sandstorm/base/Constants.h"

#include <iostream>
#include <string>
#include <csignal>
#include <thread>

using namespace std;

void StartPresident(const std::string &configFileName);

static void ProcessSignal(int signalNumber);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    std::cout << "start" << std::endl;

    StartPresident(argv[1]);

    return EXIT_SUCCESS;
}

void StartPresident(const std::string &configFileName) {
    sandstorm::util::Configuration presidentConfigratuion;
    presidentConfigratuion.Parse(configFileName);


    signal(SIGINT, ProcessSignal);

    sandstorm::service::President president(presidentConfigratuion);
    president.StartListen();
}


static void ProcessSignal(int signalNumber) {
    std::cout << "Receive signal number: " << signalNumber << std::endl;
    std::cout << "Exit" << std::endl;
    exit(0);
}