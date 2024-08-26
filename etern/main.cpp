#include <iostream>
#include "help.hpp"
#include "config.hpp"
#include "utils.hpp"
#include "new.hpp"
#include <vector>
#include <cstring>

bool FANCY = true;
Config user_configuration = Config(VCS::None, "", false, true);

int main(int argc, char* argv[]) {
    Config conf = getConfig();
    if (conf.isFirst) {
        Config user_conf = setup();
        serialize_configuration(user_conf);
        FANCY = user_conf.fancyLetters;
        user_configuration = user_conf;
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
    else if (argc == 2) {
        if (strcmp(argv[1], "setup") == 0) {
            Config user_conf = setup();
            serialize_configuration(user_conf);
            FANCY = user_conf.fancyLetters;
            user_configuration = user_conf;
            return 0;
        }
        else {
            std::cout << "Etern" << std::endl;
            std::cout << "Created by Maxims Enterprise" << std::endl;
            std::cout << "----------------------------" << std::endl;
            
            std::cout << "Invalid input. First argument is incorrect" << std::endl;
            std::cout << argv[1] << " is not a valid argument" << std::endl << std::endl;
            printHelp();
            return 1;
        }
    }
    else {
        std::vector<std::string> args;

        for (int i = 0; i < argc; i++) {
            args.push_back(std::string(argv[i]));
        }

        if (strcmp(argv[1], "new") == 0) {
            new_proj(args, argc);
        }
    }
    return 0;
}