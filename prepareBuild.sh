#!/bin/sh
mkdir -p ./build
cp -v resources/peppermintcfg/plugins.cfg build
cp -v resources/peppermintcfg/resources.cfg build
cp -v resources/* build
cp -v resources/Chessboard/ogre/*.mesh build
