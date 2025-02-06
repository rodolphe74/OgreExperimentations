# OgreExperimentations
3D &amp; physic engine association.

<img src="video/Ogre-Rook.gif" alt="Dancing" width=640 />

<img src="video/Ogre-Bullet.gif" alt="Gravity with Bullet" width=640 />

# Compilation
Using [Visual Studio](https://visualstudio.microsoft.com/fr/) and [vcpkg](https://vcpkg.io/en/).

After Visual Studio installation, open a Developper PowerShell terminal.

```
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
vcpkg install ogre
vcpkg install bullet3
```
```
git clone https://github.com/rodolphe74/OgreExperimentations.git
cd OgreExperimentations
git submodule update
git submodule init
sln.ps1
cmake . -DCMAKE_TOOLCHAIN_FILE=...\vcpkg\scripts\buildsystems\vcpkg.cmake
```

In Debug folder, in plugins.cfg file, adapt line begining with PluginFolder to ```...\vcpkg\packages\ogre_x64-windows\debug\plugins\ogre``` folder.
The same for Release folder

In Debug folder, in resources.cfg, adapt each path to vcpkg ogre folder.
The same for Release folder.

open OgreB.sln
