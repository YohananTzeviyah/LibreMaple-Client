# [LibreMaple](https://libremaple.org/) Client: a fork/continuation of JourneyClient

[![License](https://img.shields.io/badge/license-AGPL%20v3.0%2B-brightgreen.svg)](https://www.gnu.org/licenses/agpl-3.0.html)
[![C++ standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![clang++ version](https://img.shields.io/badge/clang%2B%2B-6.0.0+-dd66aa.svg)](http://clang.llvm.org/)
[![Maplestory version](https://img.shields.io/badge/Maplestory-GMS_v83-orange.svg)](https://en.wikipedia.org/wiki/MapleStory)

## JourneyClient

JourneyClient is a custom, made-from-scratch game client for
[Maplestory](https://en.wikipedia.org/wiki/MapleStory).
JourneyClient is written in [C++](https://en.wikipedia.org/wiki/C%2B%2B) and
was created by Daniel Allendorf in 2015.

The [LibreMaple](https://libremaple.org/) project aims to make a version of
Maplestory that is
[100% free and open](https://www.fsf.org/about/what-is-free-software) on both
the server and client sides, to preserve the freedom of its users and enable
creativity and collaboration around the game. As such, the LibreMaple project
takes the JourneyClient as a starting point.

LibreMaple aims to make the client
[cross-platform](https://en.wikipedia.org/wiki/Cross-platform)
([GNU+Linux](https://en.wikipedia.org/wiki/Linux)/[macOS](https://en.wikipedia.org/wiki/MacOS)/[Windows NT](https://en.wikipedia.org/wiki/Windows_NT)/[FreeBSD](https://en.wikipedia.org/wiki/FreeBSD)/etc.)
as well, so decisions are made with a cross-platform mindset as well as a free
software one.

## Supported versions

The client is currently compatible with GMS version 83 servers. For the UI file
(UI.nx), one from GMS version 154+ should be used.

## How to compile/install

### [Unix-like systems](https://en.wikipedia.org/wiki/Unix-like) excluding macOS (GNU+Linux, FreeBSD, etc.) (may also work for Unix-like Windows subsystems like [Cygwin](https://en.wikipedia.org/wiki/Cygwin), [MinGW](https://en.wikipedia.org/wiki/MinGW), or [WSL](https://en.wikipedia.org/wiki/Windows_Subsystem_for_Linux))

#### Dependencies

* [clang](http://clang.llvm.org/) (version 6+)
* [lld](https://lld.llvm.org/) (version 6+)
* [llvm](https://llvm.org/) (version 6+, including any "tools" or "runtime")
* [cmake](https://cmake.org/) (version 3.10+)
* [make](https://www.gnu.org/software/make/) (GNU Make version 4+)
* [git](https://git-scm.com/)
* [wget](https://www.gnu.org/software/wget/)
* [tar](https://www.gnu.org/software/tar/)
* unzip
* [autoconf](https://www.gnu.org/software/autoconf/autoconf.html)
* [libtool](https://www.gnu.org/software/libtool/)
* [sh or bash](https://en.wikipedia.org/wiki/Bourne_shell)
* [glut](http://freeglut.sourceforge.net/) / freeglut / freeglut3,
  freeglut3-dev (an implementation of GLUT, including development files,
  version 3)
* libXmu / libxmu (including the "development" version, if that's separate)
* libXi / libxi (including the "development" version)
* libgl-dev / libgl1-mesa-dev / libgl / mesa / libGL-devel (implementation of
  OpenGL with development files)
* libosmesa-dev / libosmesa / osmesa (skip this if your package manager doesn't
  have it)
* xorg / xorg-dev / xorg-server-devel (Xorg development libraries)
* build-essential (for users of Debian-based distibutions only)

(**Note for [Solus](https://solus-project.com/) users:**
`sudo eopkg it -c system.devel`)

Most of these dependencies should be available from your package manager
(`pacman`, `apt`, `dnf`, `zypper`, `pkg`, `xbps-install`, `eopkg`, `yum`,
etc.), but for all dependencies listed with a version number, **make sure
to check that the version your package manager gives you is high enough**.
If your package manager doesn't have something or doesn't have a high enough
version, you may have to install it using instructions from the relevant
website and/or use auxilliary package management (`yaourt`, PPAs, `copr`,
etc.).

#### Instructions

```bash
$ git clone https://github.com/Libre-Maple/LibreMaple-Client.git
$ git clone https://github.com/NoLifeDev/NoLifeNx.git nlnx

# The next command is probably best replaced by navigating to
# https://sourceforge.net/projects/asio/files/latest/download
# and downloading from there to get the latest stable version of ASIO.
$ wget https://downloads.sourceforge.net/project/asio/asio/1.12.1%20%28Stable%29/asio-1.12.1.tar.bz2
$ mkdir asio
$ tar xf asio-* --strip-components=1 -C asio/
$ rm asio-*

$ git clone https://github.com/glfw/glfw.git

# Again, the following command can be replaced by just going to
# https://sourceforge.net/projects/glew/files/glew
# and downloading the latest version of GLEW.
$ wget https://downloads.sourceforge.net/project/glew/glew/2.1.0/glew-2.1.0.tgz
$ mkdir glew
$ tar xf glew-* --strip-components=1 -C glew/
$ rm glew-*

$ git clone git://git.sv.nongnu.org/freetype/freetype2.git freetype

# www.un4seen.com
$ wget http://www.un4seen.com/files/bass24-linux.zip
$ mkdir bass
$ unzip bass2* -d bass/
$ rm bass2*

$ git clone https://github.com/lz4/lz4.git

$ export CC='/usr/bin/clang'
$ export CXX='/usr/bin/clang++'

$ cd freetype
$ sh autogen.sh
$ ./configure
$ make
$ cd ..

$ cd glew
$ make
$ cd ..

$ cd glfw
$ mkdir build
$ cd build
$ cmake ..
$ make
$ cd ../..

$ cd lz4
$ make
$ cd ..

$ cd LibreMaple-Client
$ mkdir build
$ cd build
# Build type may also be `Release`, `RelWithDebInfo`, or `MinSizeRel`.
# Additionally,pass in the argument `-D NATIVE_BUILD=true` if you desire a
# native-only build (`-march=native`).
$ cmake -D CMAKE_BUILD_TYPE=Debug ..
# Or `make -jN` with N being the number of CPU cores you wish to utilize.
$ make
# For `Release` and `MinSizeRel` builds only:
$ strip JourneyClient
```

If all this is successful, you should have the executable in your current
directory (`LibreMaple-Client/build`).

### macOS

#### Dependencies

* macOS Command Line Tools (can be installed using `xcode-select --install`,
  includes clang, git, make, and some things needed for basic clang
  functionality)
* [Homebrew](https://brew.sh/)
* [llvm](https://llvm.org/) (`brew install llvm`)
* [cmake](https://cmake.org/) (`brew install cmake`)
* [wget](https://www.gnu.org/software/wget/) (`brew install wget`)
* [autoconf](https://www.gnu.org/software/autoconf/autoconf.html)
  (`brew install autoconf`)
* [libtool](https://www.gnu.org/software/libtool/) (`brew install libtool`; the
  macOS CL tools come with a libtool, but this installs the GNU version with
  'g' prefix e.g. `glibtoolize`)
* [freeglut](http://freeglut.sourceforge.net/) (`brew install freeglut`; may
  require you to separately install XQuartz, one way to do this is
  `brew cask install xquartz`)
* [freetype](https://www.freetype.org/) (`brew install freetype`)

#### Instructions

```bash
$ git clone https://github.com/Libre-Maple/LibreMaple-Client.git
$ git clone https://github.com/NoLifeDev/NoLifeNx.git nlnx

# The next command is probably best replaced by navigating to
# https://sourceforge.net/projects/asio/files/latest/download
# and downloading from there to get the latest stable version of ASIO.
$ wget https://downloads.sourceforge.net/project/asio/asio/1.12.1%20%28Stable%29/asio-1.12.1.tar.bz2
$ mkdir asio
$ tar xf asio-* --strip-components=1 -C asio/
$ rm asio-*

$ git clone https://github.com/glfw/glfw.git

# Again, the following command can be replaced by just going to
# https://sourceforge.net/projects/glew/files/glew
# and downloading the latest version of GLEW.
$ wget https://downloads.sourceforge.net/project/glew/glew/2.1.0/glew-2.1.0.tgz
$ mkdir glew
$ tar xf glew-* --strip-components=1 -C glew/
$ rm glew-*

# www.un4seen.com
$ wget http://www.un4seen.com/files/bass24-osx.zip
$ mkdir bass
$ unzip bass2* -d bass/
$ rm bass2*

$ git clone https://github.com/lz4/lz4.git

$ export CC='clang'
$ export CXX='clang++'

$ cd glew
$ make
$ cd ..

$ cd glfw
$ mkdir build
$ cd build
$ cmake ..
$ make
$ cd ../..

$ cd lz4
$ make
$ cd ..

$ cd LibreMaple-Client
$ mkdir build
$ cd build
# Build type may also be `Release`, `RelWithDebInfo`, or `MinSizeRel`.
# Additionally,pass in the argument `-D NATIVE_BUILD=true` if you desire a
# native-only build (`-march=native`).
$ cmake -D CMAKE_BUILD_TYPE=Debug ..
# Or `make -jN` with N being the number of CPU cores you wish to utilize.
$ make
# For `Release` and `MinSizeRel` builds only:
$ strip JourneyClient
```

If all this is successful, you should have the executable in your current
directory (`LibreMaple-Client/build`). However, it probably will not run right
away, since the paths that the executable looks for dylibs on will be wrong.

In order to fix this, gather up the following files, copying them to wherever
you find most appropriate if desired (you may want to put them all into the
same directory as the executable for ease of use). All paths shown here are
relative to the base directory where you cloned all of the repos and extracted
the tarballs:

* `libbass.dylib` (found as `bass/libbass.dylib`)
* `libGLEW.dylib` (found as `glew/lib/libGLEW.dylib`, this should be a symlink
  to `libGLEW.2.1.0.dylib` or something like that, so be aware)
* `liblz4.dylib` (found as `lz4/lib/liblz4.dylib`, this should be a symlink to
  `liblz4.1.8.2.dylib` or something like that, so be aware)

Now that you have the locations of these dylibs, you can change the executable
to point to them. Like this, for example:

```bash
$ install_name_tool -change @loader_path/libbass.dylib ./libbass.dylib JourneyClient
$ install_name_tool -change /usr/local/lib/libGLEW.2.1.0.dylib ./libGLEW.2.1.0.dylib JourneyClient
$ install_name_tool -change /usr/local/lib/liblz4.1.dylib ./liblz4.1.8.2.dylib JourneyClient
```

### Windows NT (Windows 7, 8, 10+)

Coming soon...

#### Dependencies

Coming soon...

#### Instructions

Coming soon...

## Compiler

LibreMaple is built against the latest version of
[Clang](http://clang.llvm.org/) (6.0.X as of this writing), again for
cross-platform considerations and efficiency. [The GCC](https://gcc.gnu.org/)
(version 7.3.X as of this writing) is another excellent choice, especially for
those on GNU+Linux operating systems (or other OSs which default to GCC), but
GCC is **not** officially supported by LibreMaple.

That being said, don't be shy to file issues involving GCC or to make PRs to
improve ease of use/compatibility with it.

## C++ details

JourneyClient was originally developed under
[the C++14 standard](https://en.wikipedia.org/wiki/C%2B%2B14). However,
**LibreMaple strictly uses**
**[the C++17 standard](https://en.wikipedia.org/wiki/C%2B%2B17) and makes use**
**of C++17 features**.

In addition, **this project follows**
**[the ISO C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines)**,
so any code that does not comply with said guidelines can be considered a bug,
unless of course the transgression has been justified and thoroughly commented.
[A complete copy of the guidelines is available here](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md).

## Configuration

The build can be configured by editing the Journey.h file.
The following options are available:

* `JOURNEY_USE_CRYPTO` : Use cryptography when communicating for the server.
* `JOURNEY_USE_XXHASH` : Use xxhash for file check (additional dependency)
* `JOURNEY_USE_ASIO`   : Use Asio for networking (additional dependency)

LibreMaple uses crypto (`JOURNEY_USE_CRYPTO`), and also uses ASIO
(`JOURNEY_USE_ASIO`) to maintain cross-platform compatibility.

## Dependencies

* Nx library:
    [NoLifeNX](https://github.com/NoLifeDev/NoLifeNx) (depends on
    [LZ4](https://github.com/lz4/lz4))
* Graphics:
    [GLFW3](http://www.glfw.org/download.html),
    [GLEW](http://glew.sourceforge.net/), [FreeType](http://www.freetype.org/)
* Audio:
    [Bass](http://www.un4seen.com/)
* Utility:
    [xxHash](https://github.com/Cyan4973/xxHash) (optional)
* Networking:
    [Asio](http://think-async.com/)
