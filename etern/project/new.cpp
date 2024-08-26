
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
    std::string repo_name = "";
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
    proj->repo_name = repo_name;
    if (create_in_new) {
        std::string cwd = std::filesystem::current_path();
        create_dir(cwd + "/" + name);
        proj->proj_path = cwd + "/" + name;
    }
    else {
        std::string cwd = std::filesystem::current_path();
        proj->proj_path = cwd;
    }

    if (proj->from_repo != "") {
        if (create_in_new) {
            create_dir(proj->proj_path);
        }
        if (user_configuration.version == VCS::Git) {
            if (system(("git clone " + from_repo + " " + proj->proj_path).c_str()) != 0) {
                error("Cloning with Git failed");
            }
        }
        else if (user_configuration.version == VCS::Mercurial) {
            if (system(("hg clone " + from_repo + " " + proj->proj_path).c_str()) != 0) {
                error("Cloning with Mercurial failed");
            }
        }
        boilerplate(proj);
        return;
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
    if (proj->lib) {
        create_dir(proj->proj_path + "/lib");
    }
    else {
        create_dir(proj->proj_path + "/bin");
    }
    create_dir(proj->proj_path + "/src");
    std::string makefile = "";
    if (!proj->lib) {
        makefile = "CXX = " + cpp_compiler + "\n"
                            "CXX_ARGS = " + flags + "\n"
                            "BIN = ./bin/" + proj->name + "\n"
                            "SRC = src\n"
                            ".PHONY: all clean\n"
                            "SRCS = $(shell find $(SRC) -name \"*.cpp\")\n"
                            "all:\n"
                            "   $(CXX) $(CXX_ARGS) $(SRCS) -o $(BIN)\n"
                            "clean:\n"
                            "   rm $(BIN)\n";
    }
    else {
        makefile = "CXX = " + cpp_compiler + "\n"
                    "CXX_ARGS = " + flags + " -shared " + "\n"
                    "LIB = ./lib/lib" + proj->name + "\n"
                    "SRC = src\n"
                    ".PHONY: all clean\n"
                    "SRCS = $(shell find $(SRC) -name \"*.cpp\")\n"
                    "all:\n"
                    "   $(CXX) $(CXX_ARGS) $(SRCS) -o $(LIB)\n"
                    "clean:\n"
                    "   rm $(BIN)\n";
    }
    std::ofstream makefile_file(proj->proj_path + "/Makefile");
    if (!makefile_file) {
        error("Cannot create file Makefile");
    }
    makefile_file << makefile;
    makefile_file.close();
    create_file((proj->proj_path + "/src/main.cpp"));

    init_vcs(proj);
    boilerplate(proj);
}

void init_vcs(Project* proj) {
    if (proj->use_vcs == false) {
        return;
    }
    if (user_configuration.version == VCS::Git) {
        system(("git init " + proj->proj_path).c_str());
    }
    else {
        system(("hg init " + proj->proj_path).c_str());
    }

    if (proj->begin_commit) {
        if (user_configuration.version == VCS::Mercurial) {
            error("This function is not available for Heptapod right now.");
        }
        else {
            system(("cd " + proj->proj_path + " && " + "git add .").c_str());
            system(("cd " + proj->proj_path + " && " + "git commit -m \"Initial Commit\"").c_str());
            system(("cd " + proj->proj_path + " && " + "git branch -M main").c_str());
        }
    }
}

void python(Project* proj) {
    create_dir(proj->proj_path);
    create_file(proj->proj_path + "/main.py");
    boilerplate(proj);
    init_vcs(proj);
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
    if (proj->lib) {
        create_dir(proj->proj_path + "/lib");
    } else {
        create_dir(proj->proj_path + "/bin");
    }
    create_dir(proj->proj_path + "/src");

    std::string makefile;
    if (!proj->lib) {
        makefile = "CXX = " + cpp_compiler + "\n"
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
    } else {
        makefile = "CXX = " + cpp_compiler + "\n"
                    "CXX_ARGS = " + flags + " -shared " + "\n"
                    "CC = " + c_compiler + "\n"
                    "C_ARGS = " + c_flags + " -shared " + "\n"
                    "LIB = ./lib/lib" + proj->name + "\n"
                    "SRC = src\n"
                    ".PHONY: all clean\n"
                    "SRCS = $(shell find $(SRC) -name \"*.cpp\")\n"
                    "C_SRCS = $(shell find $(SRC) -name \"*.c\")\n"
                    "all:\n"
                    "   $(CXX) $(CXX_ARGS) $(SRCS) -c -o $(LIB)\n"
                    "   $(CC) $(C_ARGS) $(C_SRCS) -c -o $(LIB)\n"
                    "clean:\n"
                    "   rm $(LIB)\n";
    }
    
    std::ofstream makefile_file(proj->proj_path + "/Makefile");
    if (!makefile_file) {
        error("Cannot create file Makefile");
    }
    makefile_file << makefile;
    makefile_file.close();

    create_file((proj->proj_path + "/src/main.cpp"));

    init_vcs(proj);
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
    if (proj->lib) {
        create_dir(proj->proj_path + "/lib");
    } else {
        create_dir(proj->proj_path + "/bin");
    }
    create_dir(proj->proj_path + "/src");

    std::string makefile;
    if (!proj->lib) {
        makefile = "CC = " + c_compiler + "\n"
                    "C_ARGS = " + flags + "\n"
                    "BIN = ./bin/" + proj->name + "\n"
                    "SRC = src\n"
                    ".PHONY: all clean\n"
                    "SRCS = $(shell find $(SRC) -name \"*.c\")\n"
                    "all:\n"
                    "   $(CC) $(C_ARGS) $(SRCS) -o $(BIN)\n"
                    "clean:\n"
                    "   rm $(BIN)\n";
    } else {
        makefile = "CC = " + c_compiler + "\n"
                    "C_ARGS = " + flags + " -shared " + "\n"
                    "LIB = ./lib/lib" + proj->name + "\n"
                    "SRC = src\n"
                    ".PHONY: all clean\n"
                    "SRCS = $(shell find $(SRC) -name \"*.c\")\n"
                    "all:\n"
                    "   $(CC) $(C_ARGS) $(SRCS) -o $(LIB)\n"
                    "clean:\n"
                    "   rm $(LIB)\n";
    }

    std::ofstream makefile_file(proj->proj_path + "/Makefile");
    if (!makefile_file) {
        error("Cannot create file Makefile");
    }
    makefile_file << makefile;
    makefile_file.close();
    create_file((proj->proj_path + "/src/main.c"));

    init_vcs(proj);
    boilerplate(proj);
}

void csharp(Project* proj) {
    if (!proj->create_in_new && proj->lib) {
        if (system("dotnet new classlib") != 0) {
            error("The dotnet suite is not installed on this computer");
        }
    }
    else if (!proj->create_in_new && !proj->lib) {
        if (system("dotnet new console") != 0) {
            error("The dotnet suite is not installed on this computer");
        }
    }
    else if (proj->lib) {
        if (system(("dotnet new classlib -n " + proj->name).c_str()) != 0) {
            error("The dotnet suite is not installed on this computer");
        }
    }
    else if (system(("dotnet new console -n" + proj->name).c_str()) != 0) {
        error("The dotnet suite is not installed on this computer");
    } 
    init_vcs(proj);
    boilerplate(proj);
}

void javascript(Project* proj) {
    if (proj->lib) {
        error("This template does not support libraries");
    }
    if (system("npm init -y") != 0) {
        error("The NodeJS interpreter is not installed on this computer");
    }
    create_file((proj->proj_path + "/main.js"));
    init_vcs(proj);
    boilerplate(proj);
}

void erlang(Project* proj) {
    if (proj->lib) {
        if (system(("rebar3 new lib " + proj->name).c_str()) != 0) {
            error("Rebar3 for Erlang is not installed on this computer");
        }
    }
    else if (system(("rebar3 new app " + proj->name).c_str()) != 0) {
        error("Rebar3 for Erlang is not installed on this computer");
    }
    init_vcs(proj);
    boilerplate(proj);
}

void java(Project* proj) {
    if (proj->lib) {
        error("This template does not support libraries");
    }
    std::cout << bold("Etern uses Maven for Java Projects, is that ok?") << std::endl;
    std::cout << "Yes or No? (yes, no)" << std::endl;
    std::string opt = setup_in();
    if (opt != "yes" || opt != "y") {
        return;
    }
    std::cout << bold("What's your organitation?") << std::endl;
    std::cout << "i.e (com.maximsenterprise)" << std::endl;
    std::string org = setup_in();
    if (system(("mvn archetype:generate -DgroupId=" + org + "-DartifactId=" + proj->name + "-DarchetypeArtifactId=maven-archetype-quickstart -DinteractiveMode=false").c_str()) != 0) {
        error("Maven project initialitzation failed. Check that Maven is installed");
    }
    init_vcs(proj);
    boilerplate(proj);
}

void rust(Project* proj) {
    if (user_configuration.version == VCS::Git) {
        if (proj->lib) {
            if (system(("cargo new " + proj->name + " --lib").c_str()) != 0) {
                error("Initialization failed. Check that cargo is installed and setted up");
            }
        }
        else if (system(("cargo init " + proj->name).c_str()) != 0) {
            error("Initialization failed. Check that cargo is installed and setted up");
        }
        boilerplate(proj);
    }
    else if (user_configuration.version == VCS::Mercurial) {
        if (proj->lib) {
            if (system(("cargo new " + proj->name + " --lib --vcs hg").c_str()) != 0) {
                error("Initialization failed. Check that cargo is installed and setted up");
            }
        }
        else if (system(("cargo init " + proj->name + " --vcs hg").c_str()) != 0) {
            error("Initialization failed. Check that cargo is installed and setted up");
        }
        boilerplate(proj);
    }
}

void ada(Project* proj) {
    if (proj->lib) {
        error("This template does not support libraries");
    }
    if (proj->create_in_new) {
        create_dir(proj->proj_path);
    }
    create_file(proj->proj_path + "/proj.apr");
    create_file(proj->proj_path + "/main.adb");
    init_vcs(proj);
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

