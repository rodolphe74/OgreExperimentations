# https://github.com/microsoft/vcpkg

cmake . -DCMAKE_TOOLCHAIN_FILE=C:\Users\rodoc\HOME\vcpkg\scripts\buildsystems\vcpkg.cmake
mkdir -Force Debug
mkdir -Force Release

cp -Verbose -Force resources/msvcdebugcfg/plugins.cfg Debug
cp -Verbose -Force resources/msvcdebugcfg/resources.cfg Debug
cp -Verbose -Force resources/* Debug
cp -Verbose -Force resources/Chessboard/ogre/*.mesh Debug

cp -Verbose -Force resources/msvcreleasecfg/plugins.cfg Release
cp -Verbose -Force resources/msvcreleasecfg/resources.cfg Release
cp -Verbose -Force resources/* Release
cp -Verbose -Force resources/Chessboard/ogre/*.mesh Release
