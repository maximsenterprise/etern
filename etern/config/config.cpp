#include <json/yyjson.h>
#include "config.hpp"
#include "utils.hpp"
#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

using namespace std;

Config getConfig() {
    if (fileExists("~/.etern")) {
        // Return a first config
        return Config(VCS::None, "", true, false);
    }
    else {
        return Config(VCS::None, "", true, true);
    }
}

Config setup() {
    cout << bold("Welcome to Etern") << endl;
    cout << "We need to do a couple of configurations before you begin." << endl;
    cout << "Your preferences will be recorded in ~/.etern (Use 'exit' anytime to exit)" << endl << endl;

    // Version Control System
    cout << italic(bold("Which VCS (Version Control System) you use?")) << endl;
    cout << "Mercurial or Git? (mercurial, hg, git)" << endl;
    std::string vcs_in = setup_in();
    VCS vcs = VCS::None;

    while (true) {
        if (to_lower(trim(vcs_in)) == "exit" || to_lower(trim(vcs_in)) == "end") {
            std::exit(0);
        }
        else if (to_lower(trim(vcs_in)) == "git") {
            success("Git is now your VCS of preference");
            vcs = VCS::Git;
            break;
        }
        else if (to_lower(trim(vcs_in)) == "mercurial" || to_lower(trim(vcs_in)) == "hg") {
            success("Mercurial is now your VCS of preference");
            vcs = VCS::Mercurial;
            break;
        }
        else {
            std::cout << std::endl;
            std::cout << "Your input does not correspond to a valid input (git, mercurial or hg)" << std::endl;
            cout << italic(bold("Which VCS (Version Control System) you use?")) << endl;   
            cout << "Mercurial or Git? (mercurial, hg, git)" << endl;
            vcs_in = setup_in();
        }
    }

    // Fancy letters
    cout << italic(bold("Do you want italics and bolds (colors will not be changed)?")) << endl;
    cout << "Yes or No? (yes, no)" << endl;
    std::string fancy_in = to_lower(trim(setup_in()));
    bool fancy_letters = true;

    while (true) {
        if (fancy_in == "exit" || fancy_in == "end") {
            std::exit(0);
        }
        else if (fancy_in == "y" || fancy_in == "yes") {
            success("We'll show italics and bolds");
            break;   
        }
        else if (fancy_in == "n" || fancy_in == "no") {
            fancy_letters = false;
            success("We won't show italics and bolds");
            break;
        }
        else {
            std::cout << std::endl;
            std::cout << "Your input is not a valid input (must be 'yes' or 'no')" << std::endl;
            cout << italic(bold("Do you want italics and bolds (colors will not be changed)?")) << endl;
            cout << "Yes or No? (yes, no)" << endl;
            fancy_in = to_lower(trim(setup_in()));
        }
    }

    // Template directory
    cout << italic(bold("Which directory do you want for you custom templates?")) << endl;
    cout << "Default is '~/.eternTemplates" << endl;
    std::string temp_in = to_lower(trim(setup_in()));
    std::string temp = "";

    if (temp_in == "exit" || temp_in == "end") {
        std::exit(0);
    }

    if (temp_in == "") {
        std::string home = std::getenv("HOME");
        temp = home + "/.eternTemplates";
    }
    else {
        std::string home = std::getenv("HOME");
        if (temp_in[0] == '~') {
            temp = home + temp.substr(0, 1);
        }
    }

    if (fileExists(temp)) {
        success("Templates directory is now: " + temp);
    }
    else {
        #ifdef _WIN32
        if (CreateDirectory(temp.c_str(), NULL)) {
            success("Created Templates directory is: " + temp);
        }
        else {
            std::cout << "Cannot create folder: " + temp << std::endl;
            std::exit(1);
        }
        #else
        if (mkdir(temp.c_str(), 0755) == 0) {
            success("Created Templates directory is: " + temp);
        }
        else {
            std::cout << "Cannot create folder: " + temp << std::endl;
            std::exit(1);
        }
        #endif
    }

    return Config(vcs, temp, fancy_letters, false);
    
}

std::string setup_in() {
    std::cout << "> ";
    std::cout << "\x1b[34m" << "\033[3m";
    std::string option;
    char c = std::cin.get();
    if (c == '\n') {
        option == "";
    }
    else {
        std::cin.putback(c);
        while (true) {
            char c = std::cin.get();
            if (c == '\n') {
                break;
            }
            option.push_back(c);
        }
    }
    std::cout << "\033[0m";
    return option;
}

void success(std::string message) {
    std::cout << "\x1b[32m";
    std::cout << bold(message) << std::endl << std::endl;
}