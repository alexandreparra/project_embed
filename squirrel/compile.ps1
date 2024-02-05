mkdir build
cd build
cmake ..
msbuild .\ALL_BUILD.vcxproj
cp ..\example.nut .
cp squirrel\bin\Debug\squirrel.dll .
cp squirrel\bin\Debug\sqstdlib.dll .
.\Debug\squirrel_embed.exe