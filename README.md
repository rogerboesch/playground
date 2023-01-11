# Playground
This project allows to execute and test games on Windows, macOS and Linux

# Setup

- **CMake** is used as meta build tool
- **SDL2** is used to provide a (graphical) environment on all 3 major platforms


## Setup assets folder

The game engine uses several asset files that must be in a folder *{HOME_FOLDER}\vexxon*.
Copy all files from the *assets* folder in the repo to this folder.
This must be done no matter if you are on Windows, macOS or Linux


## Windows

### Build Playground.exe

To **build** Playground.exe you have to install **SDL2** and **SDL2_image** on your Windows machine.

- Download **SDL2** from https://github.com/libsdl-org/SDL/releases/download/release-2.26.2/SDL2-devel-2.26.2-VC.zip
- Copy the folder *SDL2-2.26.2* to a folder (ex. *C:\Dev*)
- Set an environment variable **SDL2DIR** that points to that folder ```SET SDL2DIR={PATH_TO_FOLDER}```
- Download **SDL2_image** from https://github.com/libsdl-org/SDL_image/releases/download/release-2.6.2/SDL2_image-devel-2.6.2-VC.zip
- Copy the *SDL2_image-2.6.2* a folder (ex. *C:\Dev*)
- Set an environment variable **SDL2IMAGEDIRDIR** that points to that folder ```SET SDL2IMAGEDIR={PATH_TO_FOLDER}```
- Clone this repo to a folder (ex: *C:\Vexxon*)
- Open the Windows Command Prompt and go to that folder (ex. ```cd C:\Vectrex```)
- Create the Visual Studio project with ```cmake -B build -G"Visual Studio 17 2022"``` (*I use Visual Studio 2022 here*)

After that you have a Visual Studio solution file which you can use in the ```build``` folder.
To execute Playground.exe from within Visual Studio, **SDL2.dll** has to be in the same folder as the executable.


### Execute Playground.exe

To use **Playground.exe** download this repo and do the following:

- Copy **Playground.exe** to a folder
- Copy **SDL2.dll** to the same folder


## macOS

### Build Playground.app

To **build** Playground.app you have to install **SDL2** and **SDL2_image** on your macOS machine.
For this I use homebrew (package manager).

```
    $ brew install sdl2
    $ brew install sdl2_image
```

after that create the XCode project with

```
    cmake -B build -GXCode
```

Now you have a XCode solution file which you can use in the *build* folder.


### Execute Playground.app

To **use** Playground.app download this repo and copy **Playground.app** to a folder you want.
No further installation is needed.


## Linux

coming soon...


# Vexxon - The Game

Vexxon is a Zaxxon alike game I write for the Vectrex/PiTrex as part of my [Vectrex newsletter](https://vectrex.substack.com)