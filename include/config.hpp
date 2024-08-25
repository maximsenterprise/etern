#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

enum class VCS {
    Mercurial,
    Git,
    None
};

// A classic configuration has to have this values:
// VCS - Version Control System of preference
// Custom Template Directory
class Config {
public:
    VCS version;
    std::string template_dir;
    bool fancyLetters;

    bool isFirst;

    Config(VCS version, std::string template_dir, bool fancyLetters, bool isFirst) : version(version), template_dir(template_dir), fancyLetters(fancyLetters), isFirst(isFirst) {}
};

Config getConfig();
Config setup();
std::string setup_in();
void success(std::string message);

#endif