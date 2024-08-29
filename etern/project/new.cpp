
#include "new.hpp"
#include "config.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
    bool run = true;

    if (arg_count < 4) {
        error("Not enough arguments for 'etern new'. [lang] [name]");
    }

    if (args[2].starts_with("-")) {
        error("Language should not start with '-'");
    } else {
        lang = args[2];
    }

    if (args[3].starts_with("-")) {
        error("Project Name should not start with '-'");
    } else {
        name = args[3];
    }

    for (int i = 4; i < arg_count; i++) {
        std::string opt = get_arg(args[i], args, i);
        if (opt == "lib") {
            lib = true;
        } else if (opt == "current") {
            create_in_new = false;
        } else if (opt == "no_vcs") {
            use_vcs = false;
        } else if (opt.starts_with("from")) {
            opt.erase(0, 5);
            from_repo = opt;
            i++;
        } else if (opt == "commit") {
            begin_commited = true;
        } else if (opt == "no-run") {
            run = false;
        }
    }

    Project *proj = new Project();
    proj->name = name;
    proj->lang = lang;
    proj->create_in_new = create_in_new;
    proj->from_repo = from_repo;
    proj->use_vcs = use_vcs;
    proj->lib = lib;
    proj->begin_commit = begin_commited;
    proj->new_repo = new_repo;
    proj->repo_name = repo_name;
    proj->run = run;
    if (create_in_new) {
        std::string cwd = std::filesystem::current_path();
        create_dir(cwd + "/" + name);
        proj->proj_path = cwd + "/" + name;
    } else {
        std::string cwd = std::filesystem::current_path();
        proj->proj_path = cwd;
    }

    if (proj->from_repo != "") {
        if (create_in_new) {
            create_dir(proj->proj_path);
        }
        if (user_configuration.version == VCS::Git) {
            if (system(("git clone " + from_repo + " " + proj->proj_path)
                           .c_str()) != 0) {
                error("Cloning with Git failed");
            }
        } else if (user_configuration.version == VCS::Mercurial) {
            if (system(("hg clone " + from_repo + " " + proj->proj_path)
                           .c_str()) != 0) {
                error("Cloning with Mercurial failed");
            }
        }
        boilerplate(proj);
        return;
    }

    Shared::proj = proj;
    if (lang == "c++" || lang == "cpp") {
        cpp(proj);
    } else if (lang == "csharp" || lang == "c#") {
        csharp(proj);
    } else if (lang == "javascript" || lang == "js") {
        javascript(proj);
    } else if (lang == "c") {
        c(proj);
    } else if (lang == "c_c++" || lang == "c_cpp") {
        c_cpp(proj);
    } else if (lang == "ada") {
        ada(proj);
    } else if (lang == "fsharp" || lang == "f#") {
        fsharp(proj);
    } else if (lang == "python" || lang == "py") {
        python(proj);
    } else if (lang == "crystal") {
        crystal(proj);
    } else if (lang == "html") {
        html(proj);
    } else if (lang == "electron") {
        electron(proj);
    } else if (lang == "flutter") {
        flutter(proj);
    } else if (lang == "elixir") {
        elixir(proj);
    } else if (lang == "go") {
        go(proj);
    } else if (lang == "haskell") {
        haskell(proj);
    } else if (lang == "lua") {
        lua(proj);
    } else if (lang == "nim") {
        nim(proj);
    } else if (lang == "ocaml") {
        ocaml(proj);
    } else if (lang == "ruby") {
        ruby(proj);
    } else if (lang == "rails") {
        rails(proj);
    } else if (lang == "scala") {
        scala(proj);
    } else if (lang == "spark") {
        spark(proj);
    } else if (lang == "swift") {
        swift(proj);
    } else if (lang == "zig") {
        zig(proj);
    } else {
        error("No language " + lang + " try again.");
    }
}

void cpp(Project *proj) {
    std::cout << bold("Which compiler do you use?") << std::endl;
    std::cout
        << "Type in the exact command you use (clang++, g++) [Default is g++]"
        << std::endl;
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
    } else {
        create_dir(proj->proj_path + "/bin");
    }
    create_dir(proj->proj_path + "/src");
    std::string makefile = "";
    if (!proj->lib) {
        makefile = "CXX = " + cpp_compiler +
                   "\n"
                   "CXX_ARGS = " +
                   flags +
                   "\n"
                   "BIN = ./bin/" +
                   proj->name +
                   "\n"
                   "SRC = src\n"
                   ".PHONY: all clean\n"
                   "SRCS = $(shell find $(SRC) -name \"*.cpp\")\n"
                   "all:\n"
                   "   $(CXX) $(CXX_ARGS) $(SRCS) -o $(BIN)\n"
                   "clean:\n"
                   "   rm $(BIN)\n";
    } else {
        makefile = "CXX = " + cpp_compiler +
                   "\n"
                   "CXX_ARGS = " +
                   flags + " -shared " +
                   "\n"
                   "LIB = ./lib/lib" +
                   proj->name +
                   "\n"
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

    proj->run_cmd = "make\n(run) ./bin/" + proj->name;
    init_vcs(proj);
    boilerplate(proj);
}

void init_vcs(Project *proj) {
    if (proj->use_vcs == false) {
        return;
    }
    if (user_configuration.version == VCS::Git) {
        system(("git init " + proj->proj_path).c_str());
    } else {
        system(("hg init " + proj->proj_path).c_str());
    }

    if (proj->begin_commit) {
        if (user_configuration.version == VCS::Mercurial) {
            error("This function is not available for Heptapod right now.");
        } else {
            system(("cd " + proj->proj_path + " && " + "git add .").c_str());
            system(("cd " + proj->proj_path + " && " +
                    "git commit -m \"Initial Commit\"")
                       .c_str());
            system(("cd " + proj->proj_path + " && " + "git branch -M main")
                       .c_str());
        }
    }
}

void python(Project *proj) {
    create_dir(proj->proj_path);
    create_file(proj->proj_path + "/main.py");
    proj->run_cmd = "python3 " + proj->proj_path + "/main.py";
    boilerplate(proj);
    init_vcs(proj);
}

void c_cpp(Project *proj) {
    std::cout << bold("Which C++ compiler do you use?") << std::endl;
    std::cout
        << "Type in the exact command you use (clang++, g++) [Default is g++]"
        << std::endl;
    std::string cpp_compiler = setup_in();
    if (cpp_compiler == "") {
        cpp_compiler = "g++";
    }
    std::cout << bold("Which C compiler do you use?") << std::endl;
    std::cout
        << "Type in the exact command you use (clang, gcc) [Default is gcc]"
        << std::endl;
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
        makefile = "CXX = " + cpp_compiler +
                   "\n"
                   "CXX_ARGS = " +
                   flags +
                   "\n"
                   "CC = " +
                   c_compiler +
                   "\n"
                   "C_ARGS = " +
                   c_flags +
                   "\n"
                   "BIN = ./bin/" +
                   proj->name +
                   "\n"
                   "BIN_CXX = ./bin/cxx.o" +
                   "\n"
                   "BIN_C = ./bin/c.o" +
                   "\n"
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
        makefile = "CXX = " + cpp_compiler +
                   "\n"
                   "CXX_ARGS = " +
                   flags + " -shared " +
                   "\n"
                   "CC = " +
                   c_compiler +
                   "\n"
                   "C_ARGS = " +
                   c_flags + " -shared " +
                   "\n"
                   "LIB = ./lib/lib" +
                   proj->name +
                   "\n"
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
    proj->run_cmd = "make\n(run) ./bin/" + proj->name;
    boilerplate(proj);
}

void crystal(Project *proj) {
    if (proj->lib) {
        if (system(("crystal init app " + proj->name).c_str()) != 0) {
            error("Crystal is not installed on this computer");
        }
    } else if (system(("crystal init app " + proj->name).c_str()) != 0) {
        error("Crystal is not installed on this computer");
    }
    init_vcs(proj);
    proj->run_cmd = "crystal run " + proj->proj_path + "/src/main.cr";
    boilerplate(proj);
}

void html(Project *proj) {
    if (proj->lib) {
        error("This template does not support libraries");
    }
    create_dir(proj->proj_path);
    create_file(proj->proj_path + "/index.html");
    init_vcs(proj);
    proj->run = false;
    boilerplate(proj);
}

void flutter(Project *proj) {
    if (proj->lib) {
        error("This template does not support libraries");
    }
    if (system(("flutter create " + proj->name).c_str()) != 0) {
        error("Flutter is not installed on this computer");
    }
    init_vcs(proj);
    proj->run_cmd = "flutter run";
    boilerplate(proj);
}

void electron(Project *proj) {
    if (proj->lib) {
        error("This template does not support libraries");
    }
    if (system(("npx create-electron-app" + proj->name).c_str()) != 0) {
        error("The create-electron-app package is not installed on this "
              "computer");
    }
    init_vcs(proj);
    proj->run_cmd = "npm start";
    boilerplate(proj);
}

void fsharp(Project *proj) {
    if (proj->lib) {
        if (system(("dotnet new classlib -lang F# -n " + proj->name).c_str()) !=
            0) {
            error("The dotnet suite is not installed on this computer");
        }
    } else if (system(
                   ("dotnet new console -lang F# -n " + proj->name).c_str()) !=
               0) {
        error("The dotnet suite is not installed on this computer");
    }
    init_vcs(proj);
    proj->run_cmd = "dotnet run";
    boilerplate(proj);
}

void go(Project *proj) {
    if (proj->lib) {
        if (system(("go mod init " + proj->name).c_str()) != 0) {
            error("Go is not installed on this computer");
        }
    } else if (system(("go mod init " + proj->name).c_str()) != 0) {
        error("Go is not installed on this computer");
    }
    init_vcs(proj);
    proj->run_cmd = "go run " + proj->proj_path + "/src/main.go";
    boilerplate(proj);
}

void haskell(Project *proj) {
    std::cout << bold("Do you want to use Stack or Cabal?") << std::endl;
    std::cout
        << "Type in the exact command you use (stack, cabal) [Default is stack]"
        << std::endl;
    std::string haskell_compiler = setup_in();
    if (haskell_compiler == "") {
        haskell_compiler = "stack";
    }
    if (proj->lib && haskell_compiler == "cabal") {
        if (system(("cabal init --non-interactive --package-name=" +
                    proj->name + " --lib")
                       .c_str()) != 0) {
            error("Cabal is not installed on this computer");
        }
    } else if (haskell_compiler == "cabal") {
        if (system(("cabal init --non-interactive --package-name=" +
                    proj->name + " --exe")
                       .c_str()) != 0) {
            error("Cabal is not installed on this computer");
        }
    } else if (proj->lib && haskell_compiler == "stack") {
        if (system(("stack new " + proj->name + " --template lib").c_str()) !=
            0) {
            error("Stack is not installed on this computer");
        }
    } else if (haskell_compiler == "stack") {
        if (system(
                ("stack new " + proj->name + " --template simple").c_str()) !=
            0) {
            error("Stack is not installed on this computer");
        }
    }
    init_vcs(proj);
    proj->run_cmd = haskell_compiler + " run";
    boilerplate(proj);
}

void lua(Project *proj) {
    if (proj->lib) {
        error("This template does not support libraries");
    }
    create_dir(proj->proj_path);
    create_file(proj->proj_path + "/main.lua");
    init_vcs(proj);
    proj->run_cmd = "lua " + proj->proj_path + "/main.lua";
    boilerplate(proj);
}

void nim(Project *proj) {
    if (proj->lib) {
        if (system(("nimble init " + proj->name).c_str()) != 0) {
            error("Nim is not installed on this computer");
        }
    } else if (system(("nimble init " + proj->name + " -t exe").c_str()) != 0) {
        error("Nim is not installed on this computer");
    }
    init_vcs(proj);
    proj->run_cmd = "nimble run";
    boilerplate(proj);
}

void ocaml(Project *proj) {
    if (proj->lib) {
        if (system(("dune init lib " + proj->name).c_str()) != 0) {
            error("Dune is not installed on this computer");
        }
    } else if (system(("dune init exe " + proj->name).c_str()) != 0) {
        error("Dune is not installed on this computer");
    }
    init_vcs(proj);
    proj->run_cmd = "dune exec";
    boilerplate(proj);
}

void ruby(Project *proj) {
    if (proj->lib) {
        if (system(("bundle gem " + proj->name).c_str()) != 0) {
            error("Ruby is not installed on this computer");
        }
    } else if (system(("bundle gem " + proj->name).c_str()) != 0) {
        error("Ruby is not installed on this computer");
    }
    init_vcs(proj);
    proj->run_cmd = "ruby " + proj->proj_path + "/bin/" + proj->name;
    boilerplate(proj);
}

void rails(Project *proj) {
    if (proj->lib) {
        error("This template does not support libraries");
    }
    if (system(("rails new " + proj->name).c_str()) != 0) {
        error("Rails is not installed on this computer");
    }
    init_vcs(proj);
    proj->run_cmd = "rails server";
    boilerplate(proj);
}

void scala(Project *proj) {
    if (proj->lib) {
        error("This template does not support libraries");
    }
    if (system(("sbt new scala/scala-seed.g8 -o " + proj->name).c_str()) != 0) {
        error("SBT is not installed on this computer");
    }
    init_vcs(proj);
    proj->run_cmd = "sbt run";
    boilerplate(proj);
}

void spark(Project *proj) {
    if (proj->lib) {
        error("This template does not support libraries");
    }
    create_dir(proj->proj_path);
    if (system(
            ("cd " + proj->proj_path + "&& spark new " + proj->name).c_str()) !=
        0) {
        error("Spark is not installed on this computer");
    }
    init_vcs(proj);
    proj->run_cmd = "spark run";
    boilerplate(proj);
}

void swift(Project *proj) {
    if (proj->lib) {
        if (system(("swift package init --type library --name " + proj->name)
                       .c_str()) != 0) {
            error("Swift is not installed on this computer");
        }
    }
    if (system(("swift package init --type executable --name " + proj->name)
                   .c_str()) != 0) {
        error("Swift is not installed on this computer");
    }
    init_vcs(proj);
    proj->run_cmd = "swift run";
    boilerplate(proj);
}

void zig(Project *proj) {
    if (proj->lib) {
        if (system(("zig init-lib " + proj->name).c_str()) != 0) {
            error("Zig is not installed on this computer");
        }
    } else if (system(("zig init-exe " + proj->name).c_str()) != 0) {
        error("Zig is not installed on this computer");
    }
    init_vcs(proj);
    proj->run_cmd = "zig build run";
    boilerplate(proj);
}

void c(Project *proj) {
    std::cout << bold("Which compiler do you use?") << std::endl;
    std::cout
        << "Type in the exact command you use (clang, gcc) [Default is gcc]"
        << std::endl;
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
        makefile = "CC = " + c_compiler +
                   "\n"
                   "C_ARGS = " +
                   flags +
                   "\n"
                   "BIN = ./bin/" +
                   proj->name +
                   "\n"
                   "SRC = src\n"
                   ".PHONY: all clean\n"
                   "SRCS = $(shell find $(SRC) -name \"*.c\")\n"
                   "all:\n"
                   "   $(CC) $(C_ARGS) $(SRCS) -o $(BIN)\n"
                   "clean:\n"
                   "   rm $(BIN)\n";
    } else {
        makefile = "CC = " + c_compiler +
                   "\n"
                   "C_ARGS = " +
                   flags + " -shared " +
                   "\n"
                   "LIB = ./lib/lib" +
                   proj->name +
                   "\n"
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

void csharp(Project *proj) {
    if (!proj->create_in_new && proj->lib) {
        if (system("dotnet new classlib") != 0) {
            error("The dotnet suite is not installed on this computer");
        }
    } else if (!proj->create_in_new && !proj->lib) {
        if (system("dotnet new console") != 0) {
            error("The dotnet suite is not installed on this computer");
        }
    } else if (proj->lib) {
        if (system(("dotnet new classlib -n " + proj->name).c_str()) != 0) {
            error("The dotnet suite is not installed on this computer");
        }
    } else if (system(("dotnet new console -n" + proj->name).c_str()) != 0) {
        error("The dotnet suite is not installed on this computer");
    }
    init_vcs(proj);
    boilerplate(proj);
}

void javascript(Project *proj) {
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

void erlang(Project *proj) {
    if (proj->lib) {
        if (system(("rebar3 new lib " + proj->name).c_str()) != 0) {
            error("Rebar3 for Erlang is not installed on this computer");
        }
    } else if (system(("rebar3 new app " + proj->name).c_str()) != 0) {
        error("Rebar3 for Erlang is not installed on this computer");
    }
    init_vcs(proj);
    boilerplate(proj);
}

void java(Project *proj) {
    if (proj->lib) {
        error("This template does not support libraries");
    }
    std::cout << bold("Etern uses Maven for Java Projects, is that ok?")
              << std::endl;
    std::cout << "Yes or No? (yes, no)" << std::endl;
    std::string opt = setup_in();
    if (opt != "yes" || opt != "y") {
        return;
    }
    std::cout << bold("What's your organitation?") << std::endl;
    std::cout << "i.e (com.maximsenterprise)" << std::endl;
    std::string org = setup_in();
    if (system(("mvn archetype:generate -DgroupId=" + org +
                "-DartifactId=" + proj->name +
                "-DarchetypeArtifactId=maven-archetype-quickstart "
                "-DinteractiveMode=false")
                   .c_str()) != 0) {
        error("Maven project initialitzation failed. Check that Maven is "
              "installed");
    }
    init_vcs(proj);
    boilerplate(proj);
}

void rust(Project *proj) {
    if (user_configuration.version == VCS::Git) {
        if (proj->lib) {
            if (system(("cargo new " + proj->name + " --lib").c_str()) != 0) {
                error("Initialization failed. Check that cargo is installed "
                      "and setted up");
            }
        } else if (system(("cargo init " + proj->name).c_str()) != 0) {
            error("Initialization failed. Check that cargo is installed and "
                  "setted up");
        }
        boilerplate(proj);
    } else if (user_configuration.version == VCS::Mercurial) {
        if (proj->lib) {
            if (system(
                    ("cargo new " + proj->name + " --lib --vcs hg").c_str()) !=
                0) {
                error("Initialization failed. Check that cargo is installed "
                      "and setted up");
            }
        } else if (system(("cargo init " + proj->name + " --vcs hg").c_str()) !=
                   0) {
            error("Initialization failed. Check that cargo is installed and "
                  "setted up");
        }
        boilerplate(proj);
    }
}

void ada(Project *proj) {
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

void boilerplate(Project *proj) {
    if (!proj->run) {
        return;
    }
    create_dir(proj->proj_path + "/.etern");
    if (proj->run) {
        std::ofstream run_file(proj->proj_path + "/.etern/run.hast");
        if (!run_file) {
            error("Cannot create file run.hast");
        }
        run_file << "hast-main\n" << "(run) " << proj->run_cmd;
        run_file.close();
    }
}

std::string get_arg(std::string raw_arg, std::vector<std::string> args,
                    int index) {
    if (raw_arg == "--lib") {
        return "lib";
    } else if (raw_arg == "-n") {
        return "current";
    } else if (raw_arg == "--no-vcs") {
        return "no_vcs";
    } else if (raw_arg == "--from") {
        if (index + 1 == args.size()) {
            error("From needs a repo to get started");
        } else {
            return "from " + args[index + 1];
        }
    } else if (raw_arg == "--commit") {
        return "commit";
    } else if (raw_arg == "--no-run") {
        return "no-run";
    } else {
        error("Invalid argument for 'new' command. (--lib, -n, --no-vcs, "
              "--from [repo], --no-run or --commit)");
    }
}
