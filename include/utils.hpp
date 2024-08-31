#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

std::string bold(std::string text);
std::string italic(std::string text);
bool fileExists(const std::string &name);
std::string trim(const std::string &str);
std::string to_lower(const std::string &str);
void error(const std::string &str);
void create_dir(const std::string &route);
void create_file(const std::string &route);
void run();
std::vector<std::string> list_directory(const std::string &route);

extern bool FANCY;

#endif // UTILS_HPP
