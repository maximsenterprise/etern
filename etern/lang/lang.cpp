#include "lang.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>
#include <vector>

void run_template(std::string source, Project *project) {
    TemplateStructure *structure = parse_template(source);
    Template temp = parse_instructions(structure);
    if (project->lib) {

    } else {
        for (Instruction instr : temp.instructions) {
            std::cout << instr.operand << " " << instr.mode << " "
                      << instr.value << std::endl;
        }
    }
}

Template parse_instructions(TemplateStructure *structure) {
    Template eternTemplate;
    eternTemplate.instructions = {};
    eternTemplate.libInstructions = {};
    for (size_t i = 0; i < structure->codeLines.size(); i++) {
        Instruction instr;
        std::string line = structure->codeLines[i];
        std::vector<std::string> parts = split(line, ' ');
        std::cout << parts[0] << std::endl;
        std::cout << parts[1] << std::endl;
        std::cout << parts[2] << std::endl;
        instr.operand = parts[0];
        instr.mode = parts[1];
        instr.value = parts[2];
    }

    for (size_t i = 0; i < structure->libLines.size(); i++) {
        Instruction instr;
        std::string line = structure->libLines[i];
        std::vector<std::string> parts = split(line, ' ');
        instr.operand = parts[0];
        instr.mode = parts[1];
        instr.value = parts[2];
    }
    return eternTemplate;
}
