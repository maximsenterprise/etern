
#include "new.hpp"
#include "utils.hpp"
#include "config.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>

void new_proj(std::vector<std::string> args, int arg_count) {
    std::string lang = "";
    std::string name = "";
    bool create_in_new = true;
    std::string from_repo = "";
    std::string new_repo = "";
    bool use_vcs = true;
    bool begin_commited = false;
    bool lib = false;

    if (arg_count < 4) {
        error("Not enough arguments for 'etern new'. [lang] [name]");
    }

    if (args[2].starts_with("-")) {
        error("Language should not start with '-'");
    }
    else {
        lang = args[2];
    }

    if (args[3].starts_with("-")) {
        error("Project Name should not start with '-'");
    }
    else {
        name = args[3];
    }

    for (int i = 4; i < arg_count; i++) {
        std::string opt = get_arg(args[i], args, i);
        if (opt == "lib") {
            lib = true;
        }
        else if (opt == "current") {
            create_in_new = false;
        }
        else if (opt == "no_vcs") {
            use_vcs = false;
        }
        else if (opt.starts_with("from")) {
            opt.erase(0, 5);
            from_repo = opt;
            i++;
        }
        else if (opt == "commit") {
            begin_commited = true;
        }
    }
    
    Project* proj = new Project();
    proj->name = name;
    proj->lang = lang;
    proj->create_in_new = create_in_new;
    proj->from_repo = from_repo;
    proj->use_vcs = use_vcs;
    proj->lib = lib;
    proj->begin_commit = begin_commited;
    proj->new_repo = new_repo;
    if (create_in_new) {
        std::string cwd = std::filesystem::current_path();
        create_dir(cwd + "/" + name);
        proj->proj_path = cwd + "/" + name;
    }
    else {
        std::string cwd = std::filesystem::current_path();
        proj->proj_path = cwd;
    }

    if (lang == "c++" || lang == "cpp") {
        cpp(proj);
    }
    else if (lang == "csharp" || lang == "c#") {
        csharp(proj);
    }
    else if (lang == "javascript" || lang == "js") {
        javascript(proj);
    }
    else if (lang == "c") {
        c(proj);
    }
    else if (lang == "c_c++" || lang == "c_cpp") {
        c_cpp(proj);
    }
    else {
        error("No language " + lang + " try again.");
    }
}

void cpp(Project* proj) {
    std::cout << bold("Which compiler do you use?") << std::endl;
    std::cout << "Type in the exact command you use (clang++, g++) [Default is g++]" << std::endl;
    std::string cpp_compiler = setup_in();
    if (cpp_compiler == "") {
        cpp_compiler = "g++";
    }
    std::cout << bold("Do you want extra compiler flags?") << std::endl;
    std::cout << "Defaults are '-I include -std=c++17" << std::endl;
    std::string flags = setup_in();
    flags.append(" -I include -std=c++17");

    create_dir(proj->proj_path + "/include");
    create_dir(proj->proj_path + "/bin");
    create_dir(proj->proj_path + "/src");
    std::string makefile = "CXX = " + cpp_compiler + "\n"
                            "CXX_ARGS = " + flags + "\n"
                            "BIN = ./bin/" + proj->name + "\n"
                            "SRC = src\n"
                            ".PHONY: all clean\n"
                            "SRCS = $(shell find $(SRC) -name \"*.cpp\")\n"
                            "all:\n"
                            "   $(CXX) $(CXX_ARGS) $(SRCS) -o $(BIN)\n"
                            "clean:\n"
                            "   rm $(BIN)\n";
    std::ofstream makefile_file(proj->proj_path + "/Makefile");
    if (!makefile_file) {
        error("Cannot create file Makefile");
    }
    makefile_file << makefile;
    makefile_file.close();

    boilerplate(proj);
}

void c_cpp(Project* proj) {
    std::cout << bold("Which C++ compiler do you use?") << std::endl;
    std::cout << "Type in the exact command you use (clang++, g++) [Default is g++]" << std::endl;
    std::string cpp_compiler = setup_in();
    if (cpp_compiler == "") {
        cpp_compiler = "g++";
    }
    std::cout << bold("Which C compiler do you use?") << std::endl;
    std::cout << "Type in the exact command you use (clang, gcc) [Default is gcc]" << std::endl;
    std::string c_compiler = setup_in();
    if (c_compiler == "") {
        c_compiler = "gcc";
    }
    std::cout << bold("Do you want extra compiler flags for C++?") << std::endl;
    std::cout << "Defaults are '-I include -std=c++17" << std::endl;
    std::string flags = setup_in();
    flags.append(" -I include -std=c++17");

    std::cout << bold("Do you want extra compiler flags for C?") << std::endl;
    std::cout << "Defaults are '-I include -std=c89" << std::endl;
    std::string c_flags = setup_in();
    c_flags.append(" -I include -std=c89");

    create_dir(proj->proj_path + "/include");
    create_dir(proj->proj_path + "/bin");
    create_dir(proj->proj_path + "/src");
    std::string makefile = "CXX = " + cpp_compiler + "\n"
                            "CXX_ARGS = " + flags + "\n"
                            "CC = " + c_compiler + "\n"
                            "C_ARGS = " + c_flags + "\n"
                            "BIN = ./bin/" + proj->name + "\n"
                            "BIN_CXX = ./bin/cxx.o" + "\n"
                            "BIN_C = ./bin/c.o" + "\n"
                            "SRC = src\n"
                            ".PHONY: all clean\n"
                            "SRCS = $(shell find $(SRC) -name \"*.cpp\")\n"
                            "C_SRCS = $(shell find $(SRC) -name \"*.c\")\n"
                            "all:\n"
                            "   $(CXX) $(CXX_ARGS) $(SRCS) -c -o $(BIN_C)\n"
                            "   $(CC) $(C_ARGS) $(C_SRCS) -c -o $(BIN_CXX)\n"
                            "   $(CXX) $(CXX_ARGS) $(BIN_C) $(BIN_CXX) -o $(BIN)\n"
                            "clean:\n"
                            "   rm $(BIN)\n";
    std::ofstream makefile_file(proj->proj_path + "/Makefile");
    if (!makefile_file) {
        error("Cannot create file Makefile");
    }
    makefile_file << makefile;
    makefile_file.close();

    boilerplate(proj);
}

void c(Project* proj) {
    std::cout << bold("Which compiler do you use?") << std::endl;
    std::cout << "Type in the exact command you use (clang, gcc) [Default is gcc]" << std::endl;
    std::string c_compiler = setup_in();
    if (c_compiler == "") {
        c_compiler = "gcc";
    }
    std::cout << bold("Do you want extra compiler flags?") << std::endl;
    std::cout << "Defaults are '-I include -std=c89" << std::endl;
    std::string flags = setup_in();
    flags.append(" -I include -std=c89");

    create_dir(proj->proj_path + "/include");
    create_dir(proj->proj_path + "/bin");
    create_dir(proj->proj_path + "/src");
    std::string makefile = "C = " + c_compiler + "\n"
                            "C_ARGS = " + flags + "\n"
                            "BIN = ./bin/" + proj->name + "\n"
                            "SRC = src\n"
                            ".PHONY: all clean\n"
                            "SRCS = $(shell find $(SRC) -name \"*.c\")\n"
                            "all:\n"
                            "   $(C) $(C_ARGS) $(SRCS) -o $(BIN)\n"
                            "clean:\n"
                            "   rm $(BIN)\n";
    std::ofstream makefile_file(proj->proj_path + "/Makefile");
    if (!makefile_file) {
        error("Cannot create file Makefile");
    }
    makefile_file << makefile;
    makefile_file.close();

    boilerplate(proj);
}

void csharp(Project* proj) {
    if (system(("dotnet new console -n" + proj->name).c_str()) != 0) {
        error("The dotnet suite is not installed on this computer");
    } 
    boilerplate(proj);
}

void javascript(Project* proj) {
    if (system("npm init -y") != 0) {
        error("The NodeJS interpreter is not installed on this computer");
    }
    boilerplate(proj);
}

void boilerplate(Project* proj) {
    create_dir(proj->proj_path + "/.etern");
}

std::string get_arg(std::string raw_arg, std::vector<std::string> args, int index) {
    if (raw_arg == "--lib") {
        return "lib";
    }
    else if (raw_arg == "-n") {
        return "current";
    }
    else if (raw_arg == "--no-vcs") {
        return "no_vcs";
    }
    else if (raw_arg == "--from") {
        if (index + 1 == args.size()) {
            error("From needs a repo to get started");
        }
        else {
            return "from " + args[index + 1];
        }
    }
    else if (raw_arg == "--commit") {   
        return "commit";
    }
    else {
        error("Invalid argument for 'new' command. (--lib, -n, --no-vcs, --from [repo] or --commit)");
    }
}


