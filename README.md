## Project Embed
This project shows real examples on how to embed scripting languages and runtimes on your native C/C++ code, from building the projects to interacting with their VM's. This is an instructional/educational repository, read each `README.md` file to understand what is happening. I try to bring most interoperability functionalities into the examples, if something is not clear, or a better example may be provided, please fill an issue or send a pull request.

## Dependencies 
All examples relies on [CMake](https://cmake.org/), and an up to date C/C++ compiler:
- Windows: Install [Visual Studio and download the C++ toolchain](https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170).
- Linux: Use your package manager to install GCC or Clang.
- MacOS: Download and install the latest Xcode and Xcode Command Line tools.

The external dependencies are managed as `git submodules` for most examples. To clone the project with all dependencies run this command:
```sh
git clone --recursive https://github.com/alexandreparra/project_embed.git
```

If you've already pulled the repository but didn't init the submodules, simply run this command at the project root:
```sh
git submodule update --init --recursive
```

If any other dependencies are needed, the respective `README.md` files inside each language folders or examples will list them accordingly. 

## Available Languages
| Language Name                               | Info                      |
|---------------------------------------------|---------------------------|
| [Haxe/Hashlink](https://hashlink.haxe.org/) | Statically typed, GC, JIT |
| [Squirrel](http://www.squirrel-lang.org/)   | Dynamically typed, RC, GC |