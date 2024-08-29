#ifndef NEW_HPP
#define NEW_HPP

#include <string>
#include <vector>

void new_proj(std::vector<std::string> args, int arg_count);
std::string get_arg(std::string raw_arg, std::vector<std::string> args,
                    int index);

class Project {
  public:
    std::string name;
    std::string lang;
    bool create_in_new;
    std::string from_repo;
    bool use_vcs;
    bool lib;
    bool begin_commit;
    std::string new_repo;
    std::string proj_path;
    std::string repo_name = "";
    bool run;
    std::string run_cmd = "";
};

class Shared {
  public:
    static Project *proj;
};

void cpp(Project *proj);
void c_cpp(Project *proj);
void c(Project *proj);
void ada(Project *proj);
void erlang(Project *proj);
void javascript(Project *proj);
void java(Project *proj);
void csharp(Project *proj);
void rust(Project *proj);
void python(Project *proj);
void crystal(Project *proj);
void html(Project *proj);
void electron(Project *proj);
void flutter(Project *proj);
void elixir(Project *proj);
void fsharp(Project *proj);
void go(Project *proj);
void haskell(Project *proj);
void lua(Project *proj);
void nim(Project *proj);
void ocaml(Project *proj);
void ruby(Project *proj);
void rails(Project *proj);
void scala(Project *proj);
void spark(Project *proj);
void swift(Project *proj);
void zig(Project *proj);

void boilerplate(Project *proj);
void init_vcs(Project *proj);

#endif
