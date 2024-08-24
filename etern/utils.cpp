#include "utils.hpp"

std::string bold(std::string text) {
    return "\033[1m" + text + "\033[0m";
}

std::string italic(std::string text) {
    return "\033[3m" + text + "\033[0m";
}
