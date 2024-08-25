#include <iostream>
#include "help.hpp"
#include "config.hpp"
#include "utils.hpp"
#include <vector>

int main(int argc, char* argv[]) {
    Config conf = getConfig();
    if (conf.isFirst) {
        Config user_conf = setup();
        serialize_configuration(user_conf);
        return 0;
    }
    if (argc == 1) {
        std::cout << "Etern" << std::endl;
        std::cout << "Created by Maxims Enterprise" << std::endl;
        std::cout << "----------------------------" << std::endl;

        std::cout << "Invalid input. No arguments" << std::endl << std::endl;
        printHelp();
        return 1;
    }
    return 0;
}