#include <iostream>
#include "help.hpp"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Etern" << std::endl;
        std::cout << "Created by Maxims Enterprise" << std::endl;
        std::cout << "----------------------------" << std::endl;

        std::cout << "Invalid input. No arguments" << std::endl << std::endl;
        printHelp();
    }
}