#include "lang.hpp"
#include "utils.hpp"
#include <sstream>
#include <string>

TemplateStructure *parse_template(std::string source) {
    std::vector<std::string> libLines;
    std::vector<std::string> codeLines;

    std::string line;
    bool inLib = false;
    std::istringstream stream(source);
    while (std::getline(stream, line)) {
        if (to_lower(trim(line)) == "lib {") {
            inLib = true;
        } else if (to_lower(trim(line)) == "}") {
            inLib = false;
        } else if (inLib) {
            libLines.push_back(line);
        } else {
            codeLines.push_back(line);
        }
    }
    return new TemplateStructure{libLines, codeLines};
}
