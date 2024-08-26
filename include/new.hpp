#ifndef NEW_HPP
#define NEW_HPP

#include <string>
#include <vector>

void new_proj(std::vector<std::string> args, int arg_count);
std::string get_arg(std::string raw_arg, std::vector<std::string> args, int index);

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
};

void cpp(Project* proj);
void c_cpp(Project* proj);
void c(Project* proj);
void ada(Project* proj);
void erlang(Project* proj);
void javascript(Project* proj);
void java(Project* proj);
void csharp(Project* proj);
void rust(Project* proj);
void python(Project* proj);

void boilerplate(Project* proj);
void init_vcs(Project* proj);

#endif