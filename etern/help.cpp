#include "help.hpp"
#include "utils.hpp"
#include <iostream>

using namespace std;

void printHelp() {
    cout << "Usecases and usage instructions" << endl;

    // Project Management
    cout << bold("Project Management") << endl;

    // New command
    cout << endl << italic("new");
    cout << " -> Create a new project (etern new [lang] [name])" << endl;
    cout << "       Options:" << endl;
    cout << "       -n -> Create the project in the current folder" << endl;
    cout << "       --no-vcs -> No Version Control System" << endl;
    cout << "       --commit -> Begin with a commit already made" << endl;
    cout << "       --from [repo] -> Begin with an already done Repository"
         << endl;
    cout << "       --lib -> Use the library version of the template" << endl;
    cout << "       --no-run -> Do not create the run.hast file" << endl;

    // Remove command
    cout << italic("remove");
    cout << " -> Remove the project in the current directory" << endl;
    cout << "          Options:" << endl;
    cout << "          --all -> Remove all the files in the directory" << endl;
    cout << "          --vcs -> Remove only the Version Control System Files"
         << endl;

    cout << italic("run");
    cout << " -> Run the actual project. Check that ~/.etern/run.hast exists"
         << endl;
    // Program
    cout << endl << bold("Program") << endl;

    // Setup command
    cout << italic("setup") << " -> Reproduce the setup steps" << endl;
}
