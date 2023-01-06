# SDL Playground
This project allows to execute and test games on Windows, macOS and Linux

# Setup

- CMake is used as meta build tool
- SDL2 is used to provide a (graphical) environment on all 3 major platforms

## Windows

### Build Playground.exe

To use Playground.exe you have to install **SDL2** and **SDL2_image** on your Windows machine (only if you want to compile and link the project).
If you just want to use the final binary, **SDL2.dll** (which is in that repo too) is enough.

- Download **SDL2** from https://github.com/libsdl-org/SDL/releases/download/release-2.26.2/SDL2-devel-2.26.2-VC.zip
- Copy the folder ```SDL2-2.26.2``` to a folder (ex. *C:\Dev*)
- Set an environment variable ```SDL2DIR``` that points to that folder ```SET SDL2DIR={PATH_TO_FOLDER}```
- Download **SDL2_image** from https://github.com/libsdl-org/SDL_image/releases/download/release-2.6.2/SDL2_image-devel-2.6.2-VC.zip
- Copy the ```SDL2_image-2.6.2``` a folder (ex. *C:\Dev*)
- Set an environment variable ```SDL2IMAGEDIRDIR``` that points to that folder ```SET SDL2IMAGEDIR={PATH_TO_FOLDER}```
- Download this repo and copy to a folder (ex: *C:\Vexxon*)
- Open the Windows Command Prompt and go to that folder (ex. ```cd C:\Vectrex```)
- Create the Visual Studio project with ```cmake -B build -G"Visual Studio 17 2022"``` (*I use Visual Studio 2022 here*)

After that you have a Visual Studio solution file which you can use in the ```build``` folder.
To execute Playground.exe from within Visual Studio, **SDL2.dll* has to be in the same folder as the executable.
Also level1.txt (which is in the assets folder) must for now be in %HOMEFOLDER%\vexxon\level1.txt


### Execute Playground.exe

To use Playground.exe download this repo and do the following:

- Copy **Playground.exe** to a folder
- Copy **SDL2.dll** to the same folder
- Create a folder ```%HOMEFOLDER%\vexxon\```
- Copy **level1.txt** (which is in the assets folder) to ```%HOMEFOLDER%\vexxon```


## macOS

coming soon...

## Linux

coming soon...

# Vexxon - The Game

Vexxon is a Zaxxon alike game I write for the Vectrex/PiTrex as part of my [Vectrex newsletter](https://vectrex.substack.com)