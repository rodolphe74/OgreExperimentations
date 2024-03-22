mkdir -Force Debug
mkdir -Force Release

cp -Force resources/msvcdebugcfg/plugins.cfg Debug
cp -Force resources/msvcdebugcfg/resources.cfg Debug
cp -Force resources/* Debug
cp -Force resources/Chessboard/ogre/*.mesh Debug

cp -Force resources/msvcreleasecfg/plugins.cfg Release
cp -Force resources/msvcreleasecfg/resources.cfg Release
cp -Force resources/* Release
cp -Force resources/Chessboard/ogre/*.mesh Release


# cp -Verbose -Force resources/msvcdebugcfg/plugins.cfg Debug
# cp -Verbose -Force resources/msvcdebugcfg/resources.cfg Debug
# cp -Verbose -Force resources/* Debug
# cp -Verbose -Force resources/Chessboard/ogre/*.mesh Debug

# cp -Verbose -Force resources/msvcreleasecfg/plugins.cfg Release
# cp -Verbose -Force resources/msvcreleasecfg/resources.cfg Release
# cp -Verbose -Force resources/* Release
# cp -Verbose -Force resources/Chessboard/ogre/*.mesh Release
