This is a C++ example that embeds hashlink. It basically fetches hashlink dependencies and builds
the VM from source and feed code to it. This mimic [main.c](https://github.com/HaxeFoundation/hashlink/blob/master/src/main.c) from
hashlink itself and doesn't have any extra purposes. This uses a JIT that is only available for x86-64 platforms.

To successfully run the example you'll need to:
1. Install the [Haxe compiler](https://haxe.org/).
2. Go to this project `hx` folder.
3. Run on your terminal `haxe compile.hxml`.
4. Run the C++ project.

This will compile the Haxe code into hashlink bytecode so that the VM can properly run it. There's a .vscode folder to 
automate the process if you're using vscode to program in Haxe.

Because hashlink runs Haxe code directly from an entry point, the best way to interact with hashlink
is to use [native extensions](https://github.com/HaxeFoundation/hashlink/wiki/HashLink-native-extension-tutorial#why-creating-an-hashlink-extension)
so that you can interact with the VM directly. This works by exposing `hldll's` which are basically `dll's`
for hashlink created using C/C++ that can perform memory allocations, call methods and so forth directly into the VM (here is the [C API](https://github.com/HaxeFoundation/hashlink/wiki/C-API-Documentation)).
Haxe can afterward call code from a `hldll` (which requires creating a wrapper for the `hldll` in Haxe) and take advantage
of native C code.
