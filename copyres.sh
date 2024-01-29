#!/bin/sh
echo CMake path:$1
mkdir -p $1/build
cp -v $1/resources/peppermintcfg/plugins.cfg $1/build
cp -v $1/resources/peppermintcfg/resources.cfg $1/build
cp -v $1/resources/* $1/build
cp -v $1/resources/Chessboard/ogre/*.mesh $1/build
