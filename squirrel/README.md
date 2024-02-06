As described by the [Squirrel website](http://www.squirrel-lang.org/): 

> Squirrel is a high level imperative, object-oriented programming language, designed to be a light-weight scripting language that fits in the size, memory bandwidth, and real-time requirements of applications like video games.

The language is implemented as a stack based language that primarily uses Reference Counting (RC) for memory management, and have an auxiliary 
mark and sweep garbage collector on its virtual machine. The language have a similar API from [Lua](https://www.lua.org/) and even have a "table" data structure. 

It has been used in games like War Thunder, [Left 4 Dead 2](https://developer.valvesoftware.com/wiki/VScript) (plus many other Valve games),
and [OpenTTD](https://github.com/OpenTTD/OpenTTD).

## About the example
Here we are going to compile the Squirrel virtual machine and standard library. The standard library can help us setting up some initial dependencies, like the functions needed to compile Squirrel code.
Each function has comments on why we are using it, so that you can sequentially read the code and understand what it does.

This project has already all the dependencies available through git submodules. To compile for Windows, simply run the `compile.ps1` script in the current example folder using the Developer Powershell that can be download with [Visual Studio](https://visualstudio.microsoft.com/).

For more informations about the language, visit the [Squirrel reference](http://squirrel-lang.org/squirreldoc/reference/index.html).

## Tested and working on:
| Platform                                    | Architecture              |
|---------------------------------------------|---------------------------|
| Windows 11                                  | x86_64                    |