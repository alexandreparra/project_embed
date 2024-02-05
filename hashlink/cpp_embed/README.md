This is a C++ example that embeds Hashlink. It basically fetches Hashlink dependencies and builds
the VM from source and feed code to it. This mimic [main.c](https://github.com/HaxeFoundation/hashlink/blob/master/src/main.c) from
Hashlink itself and doesn't really have any extra purposes unless you are fiddling with how the VM works. This uses a JIT that is only available for x86-64 platforms.

To successfully run the example you'll need to:
1. Install the [Haxe compiler](https://haxe.org/).
2. If using Windows, simply run the `compile.ps1` file.

This will compile the Haxe code into Hashlink bytecode so that the VM can properly run it. If you want to change the Haxe code and compile it yourself,
and you are using VSCode, theres already a configuration inside `hx/.vscode` to compile it.

Because hashlink runs Haxe code directly from an entry point, the best way to interact with hashlink
is to use [native extensions](https://github.com/HaxeFoundation/hashlink/wiki/HashLink-native-extension-tutorial#why-creating-an-hashlink-extension)
so that you can interact with the VM directly. This works by exposing `hldll's` which are basically `dll's`
for hashlink created using C/C++ that can perform memory allocations, call methods and so forth directly into the VM (here is the [C API](https://github.com/HaxeFoundation/hashlink/wiki/C-API-Documentation)).
Haxe can afterward call code from a `hldll` (which requires creating a wrapper for the `hldll` in Haxe) and take advantage
of native C code.

## Tested and working on:
| Platform                                    | Architecture              |
|---------------------------------------------|---------------------------|
| Windows 11                                  | x86_64                    |