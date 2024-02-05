## Project Embed
This project shows real examples on how to embed scripting languages and runtimes on your native C/C++ code, from building the projects to interacting with their VM's. All examples relies on [CMake](https://cmake.org/) for building C/C++ code. Most dependencies needed to run the examples are managed as `git submodules`, to clone the project with all dependencies run this command:

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