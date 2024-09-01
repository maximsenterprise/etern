# Etern, A Project Management Tool

Projects can be tedious to setup and manage, especially when you use a lot of languages, frameworks and tools. Etern is meant to solve that.

## Contents

-   [Installation](#installation)
-   [Project Management](#project-management)
-   [Template Scripts](#template-scripts)
-   [Build Scripts](#build-scripts)

All of this points are going to be explained in the following sections.

## Installation

### Installation from Release

Download the binary and the `path.sh` file to your computer. In the same directory input this command:

```bash
./path.sh # Pass here the path to the binary
```

This will add the binary to your path.

### Installation from Source

Clone the repository and run the following commands:

```bash
git clone https://github.com/maximsenterprise/etern.git
cd etern
make # Check that all commands are installed and work well
./path.sh ./bin/etern
```

### Setup

To begin setup simply type `etern` in your terminal.

The setup is simple, it will ask for three things:

-   The Version Control System you are using, it can be `git` or `hg`.
-   If you want bolds and italics in the output, it can be `yes` or `no`.
-   The templates directory, the default is `~/.etern/templates`.

## Project Management

Projects are managed by the `etern` command. You can create a new project by typing `etern new` and following the instructions.

-   Indicate the language you are going to use.
    The defaults are:

    -   `cpp` or `c++` for C++
    -   `c_c++` or `c_cpp` for C mixed with C++
    -   `c` for C
    -   `ada` for Ada
    -   `erlang` for Erlang
    -   `js` or `javascript` for JavaScript
    -   `java` for Java
    -   `c#` or `csharp` for C#
    -   `rust` for Rust
    -   `python` for Python
    -   `crystal` for Crystal
    -   `html` for HTML
    -   `electron` for Electron
    -   `flutter` for Flutter
    -   `elixir` for Elixir
    -   `fsharp` or `f#` for F#
    -   `go` for Go
    -   `haskell` for Haskell
    -   `lua` for Lua
    -   `nim` for Nim
    -   `ocaml` for OCaml
    -   `ruby` for Ruby
    -   `rails` for the Rails Framework
    -   `scala` for Scala
    -   `spark` for [Spark](https://github.com/maximsenterprise/spark)
    -   `swift` for Swift
    -   `zig` for Zig

-   Indicate the name of the project.
-   You can also put some flags:
    -   `--no-vcs` to not initialize a git or mercurial repository.
    -   `--lib` to create a library project.
    -   `--no-run` to not create a run script.
        and more...

## Template Scripts

Templates are a core part of Etern. They are used to generate the project structure for projects you create.

Normally templates remain in the `~/.etern/templates` directory, but you can change this in the setup.

Templates are written in ruby and are just like any other ruby script. Their name is the name of the template followed by `.rb`, i.e: `basic.rb`.

## Build Scripts

Build scripts are used to build your project, it is important to note that they are `Hastfiles` so `Hast` must be installed

[Hast](https://github.com/maximsenterprise/hast) is a build system that is used to build projects. It is written in Typescript and is very simple to use.

To run simply execute `etern run` in the project directory.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author

Created by Maxims Enterprise in 2024. All rights reserved
