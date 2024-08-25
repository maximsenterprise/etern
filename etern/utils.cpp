#include "utils.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <cctype>

std::string bold(std::string text) {
    return "\033[1m" + text + "\033[0m";
}

std::string italic(std::string text) {
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