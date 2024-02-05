mkdir build
cd build
cmake ..
msbuild .\ALL_BUILD.vcxproj
cd ..
cd bin
cp ..\example.nut .
.\squirrel_embed.exe