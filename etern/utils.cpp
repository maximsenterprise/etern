#include "utils.hpp"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

std::string bold(std::string text) {
    if (!FANCY) {
        return text;
    }
    return "\033[1m" + text + "\033[0m";
}

std::string italic(std::string text) {
    if (!FANCY) {
        return text;
    }
    return "\033[3m" + text + "\033[0m";
}

bool fileExists(const std::string &name) {
    const char *home = std::getenv("HOME");
    std::string finalStr = "";
    if (name[0] == '~') {
        finalStr = home + name.substr(1);
    } else {
        finalStr = name;
    }
    std::ifstream infile(finalStr);
    return infile.good();
}

std::string trim(const std::string &str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
        return "";
    }

    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

std::string to_lower(const std::string &str) {
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return lower_str;
}

void error(const std::string &str) {
    std::cout << "\033[31m" << bold(str) << "\033[0m" << std::endl;
    std::exit(1);
}

void create_dir(const std::string &route) {
    if (fileExists(route)) {
        return;
    }
#ifdef _WIN32
    if (!CreateDirectory(route.c_str(), NULL)) {
        error("Cannot create directory");
    }
#else
    if (mkdir(route.c_str(), 0755) != 0) {
        error("Cannot create directory in specified location");
    }
#endif
}

void create_file(const std::string &route) {
    std::ofstream file(route);
    if (!file) {
        error("File could not be created");
    }
    file.close();
}

void run() {
    if (!fileExists("/.etern")) {
        error("Your project cannot be runned, initialize a etern project here "
              "before");
        return;
    }
    if (fileExists("/.etern/run.hast")) {
        if (system("cd ./etern && hast") != 0) {
            error("Command failed to execute. Check that Hast is installed");
        }
    } else {
        error("Your project cannot be runned, check that ~/.etern/run.hast is "
              "safe and that your project is runnable");
    }
}

std::vector<std::string> list_directory(const std::string &route) {
    std::vector<std::string> files = {};
    if (!fileExists(route)) {
        error("Directory does not exist; Thus cannot be listed");
    } else {
        namespace fs = std::filesystem;
        try {
            for (const auto &entry : fs::directory_iterator(route)) {
                files.push_back(entry.path());
            }
        } catch (const fs::filesystem_error &err) {
            error(std::string("Error while listing files: ") + err.what());
        }
    }
    return files;
}
