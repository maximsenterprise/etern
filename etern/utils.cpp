#include "utils.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <cctype>
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
    const char* home = std::getenv("HOME");
    std::string finalStr = "";
    if (name[0] == '~') {
        finalStr = home + name.substr(1);
    }
    else {
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