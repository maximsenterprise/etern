#include "config.hpp"
#include "help.hpp"
#include "new.hpp"
#include "utils.hpp"
#include <cstring>
#include <iostream>
#include <vector>

bool FANCY = true;
Config user_configuration = Config(VCS::None, "", false, true);

int main(int argc, char *argv[]) {
    Config conf = getConfig();
    user_configuration = conf;
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
    } else if (argc == 2) {
        if (strcmp(argv[1], "setup") == 0) {
            Config user_conf = setup();
            serialize_configuration(user_conf);
            FANCY = user_conf.fancyLetters;
            user_configuration = user_conf;
            return 0;
        } else if (strcmp(argv[1], "--version") == 0) {
            std::cout << "Etern" << std::endl;
            std::cout << "Version 1.0.0 Release" << std::endl;
            std::cout << "Created by Maxims Enterprise" << std::endl;
        } else if (strcmp(argv[1], "run") == 0) {
            if (!fileExists("./.etern/run.hast")) {
                error("No run file found. Check that your project is runnable "
                      "and that .etern/run.hast exists");
            }
            if (system("cd .etern && cp run.hast ../ && cd .. && hast") != 0) {
                system("rm run.hast");
                error("Could not run project.");
            }
            success("Project ran successfully");
            system("rm run.hast");

        } else {
            std::cout << "Etern" << std::endl;
            std::cout << "Created by Maxims Enterprise" << std::endl;
            std::cout << "----------------------------" << std::endl;

            std::cout << "Invalid input. First argument is incorrect"
                      << std::endl;
            std::cout << argv[1] << " is not a valid argument" << std::endl
                      << std::endl;
            printHelp();
            return 1;
        }
    } else {
        std::vector<std::string> args;

        for (int i = 0; i < argc; i++) {
            args.push_back(std::string(argv[i]));
        }

        if (strcmp(argv[1], "new") == 0) {
            new_proj(args, argc);
        } else if (strcmp(argv[1], "remove") == 0) {
            if (args[2] == "--all") {
                if (system("sudo rm -rf ./*") == 0) {
                    success("All files removed");
                } else {
                    error("Could not remove all files");
                }
            } else if (args[2] == "--vcs") {
                if (user_configuration.version == VCS::None) {
                    error("No VCS to remove");
                } else if (user_configuration.version == VCS::Git) {
                    if (system("sudo rm -rf .git") == 0) {
                        success("VCS removed");
                    } else {
                        error("Could not remove VCS");
                    }
                } else if (user_configuration.version == VCS::Mercurial) {
                    if (system("sudo rm -rf .hg") == 0) {
                        success("VCS removed");
                    } else {
                        error("Could not remove VCS");
                    }
                }
            }
        } else {
            std::cout << "Etern" << std::endl;
            std::cout << "Created by Maxims Enterprise" << std::endl;
            std::cout << "----------------------------" << std::endl;

            std::cout << "Invalid input. First argument is incorrect"
                      << std::endl;
            std::cout << argv[1] << " is not a valid argument" << std::endl
                      << std::endl;
            printHelp();
            return 1;
        }
    }
    return 0;
}
