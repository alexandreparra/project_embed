mkdir build
cd build
cmake ..
msbuild .\ALL_BUILD.vcxproj
cd ..

cd hx
haxe compile.hxml
cd ..

cd bin
.\hl_embed.exe