[Hashlink](https://hashlink.haxe.org/) is a virtual machine for the [Haxe programming language](https://haxe.org/). Its extensively used for game development and general purpose programming. Because of Haxe's compiler capabilities of
transpiling Haxe to many languages, it's really suited for creating maintainable projects across different stacks and has gained popularity among game devs.

Hashlink has a focus on performance, it comes with a JIT for fast development, and it can be totally transpiled to C to achieve better performance. It can have [native extensions](https://github.com/HaxeFoundation/hashlink/wiki/HashLink-native-extension-tutorial#why-creating-an-hashlink-extension) which are shared libraries created with C/C++ that can interoperate with the Hashlink VM to provide better performance.

To correctly compile the example projects here, you'll need to install:
- [Haxe](https://haxe.org/)
- [Hashlink](https://hashlink.haxe.org/)

*Apple Silicon disclaimer*: Apple computers that use the M1 or other M* processors which are arm64 (aarch64), may give some issues while trying to run Hashlink examples. First you won't be able to use JIT because it's only available for x86_64, but you could try to transpile the Haxe code to C and compile it natively. I've had many problems doing so and didn't find a good solution so I won't say that it works corectly under Apple Silicon (Intel based Macs should work with no problems).
