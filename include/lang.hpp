#ifndef LANG_HPP
#define LANG_HPP

#include "new.hpp"
#include <string>
#include <vector>

void run_template(std::string source, Project *project);

class TemplateStructure {
  public:
    std::vector<std::string> libLines;
    std::vector<std::string> codeLines;
};

TemplateStructure *parse_template(std::string source);

class Instruction {
  public:
    std::string operand;
    std::string mode;
    std::string value;
};

class Template {
  public:
    std::vector<Instruction> instructions;
    std::vector<Instruction> libInstructions;
};

Template parse_instructions(TemplateStructure *templateStructure);

#endif // LANG_HPP
