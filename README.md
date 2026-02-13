# overview

Source code for "Computer Graphics Programming in OpenGL Using C++, 3rd" on my understanding

## Book information

"Computer Graphics Programming in OpenGL with C++, 3rd Edition"
by V. Scott Gordon and John Clevenger
published by Mercury Learning ©2024.
![Book Cover](images/cover.png)

- [The book website from author](https://athena.ecs.csus.edu/~gordonvs/textC3E.html)
You can download the companion files there.

## Prepare the development environment

### Operation System

This repo is based on Windows 11 x64. And GPU is NVIDIA GeForce GTX 1080 with OpenGL 4.6 support. (Detected by [gpu-z](https://www.techpowerup.com/gpuz/))

### IDE

This repo uses Visual Studio 2026 Community Edition with Desktop C++ feature enabled.

### Powershell

Original version is 5.1. Now upgrade to 7.5.

#### check the current verson

```powershell
PS c:\> $PSVersionTable

Name                           Value
----                           -----
PSVersion                      5.1.26100.7705
PSEdition                      Desktop
PSCompatibleVersions           {1.0, 2.0, 3.0, 4.0...}
BuildVersion                   10.0.26100.7705
CLRVersion                     4.0.30319.42000
WSManStackVersion              3.0
PSRemotingProtocolVersion      2.3
SerializationVersion           1.1.0.1
```

#### upgrade to 7.5

Download the PowerShell-7.5.4-win-x64.msi package directly from Microsoft and install it.

```powershell
PS C:\> $PSVersionTable

Name                           Value
----                           -----
PSVersion                      7.5.4
PSEdition                      Core
GitCommitId                    7.5.4
OS                             Microsoft Windows 10.0.26100
Platform                       Win32NT
PSCompatibleVersions           {1.0, 2.0, 3.0, 4.0…}
PSRemotingProtocolVersion      2.3
SerializationVersion           1.1.0.1
WSManStackVersion              3.0
```

### Scoop

[Scoop website](https://scoop.sh/)

#### Installation

```powershell
PS C:\> Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
PS C:\> irm get.scoop.sh | iex
Initializing...
Downloading...
Extracting...
Creating shim...
Adding ~\scoop\shims to your path.
Scoop was installed successfully!
Type 'scoop help' for instructions.
PS C:\> scoop help
Usage: scoop <command> [<args>]

Available commands are listed below.

Type 'scoop help <command>' to get more help for a specific command.

Command    Summary
-------    -------
alias      Manage scoop aliases
bucket     Manage Scoop buckets
cache      Show or clear the download cache
cat        Show content of specified manifest.
checkup    Check for potential problems
cleanup    Cleanup apps by removing old versions
config     Get or set configuration values
create     Create a custom app manifest
depends    List dependencies for an app, in the order they'll be installed
download   Download apps in the cache folder and verify hashes
export     Exports installed apps, buckets (and optionally configs) in JSON format
help       Show help for a command
hold       Hold an app to disable updates
home       Opens the app homepage
import     Imports apps, buckets and configs from a Scoopfile in JSON format
info       Display information about an app
install    Install apps
list       List installed apps
prefix     Returns the path to the specified app
reset      Reset an app to resolve conflicts
search     Search available apps
shim       Manipulate Scoop shims
status     Show status and check for new app versions
unhold     Unhold an app to enable updates
uninstall  Uninstall an app
update     Update apps, or Scoop itself
virustotal Look for app's hash or url on virustotal.com
which      Locate a shim/executable (similar to 'which' on Linux)

```

### cmake

[cmake website](https://cmake.org)

#### Installation

```powershell
PS C:\> scoop install cmake
Installing 'cmake' (4.2.3) [64bit] from 'main' bucket
cmake-4.2.3-windows-x86_64.zip (48.1 MB) [====================================================================] 100%
Checking hash of cmake-4.2.3-windows-x86_64.zip ... ok.
Extracting cmake-4.2.3-windows-x86_64.zip ... done.
Linking ~\scoop\apps\cmake\current => ~\scoop\apps\cmake\4.2.3
Creating shim for 'cmake'.
Creating shim for 'cmcldeps'.
Creating shim for 'cpack'.
Creating shim for 'ctest'.
Creating shim for 'cmake-gui'.
Making C:\Users\boyal\scoop\shims\cmake-gui.exe a GUI binary.
Creating shortcut for cmake-gui (cmake-gui.exe)
'cmake' (4.2.3) was installed successfully!

PS C:\>  cmake --version
cmake version 4.2.3

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```

### premake

```powershell
PS C:\> scoop install premake
Installing 'premake' (5.0.0-beta8) [64bit] from 'main' bucket
premake-5.0.0-beta8-windows.zip (597.4 KB) [==================================================================] 100%
Checking hash of premake-5.0.0-beta8-windows.zip ... ok.
Extracting premake-5.0.0-beta8-windows.zip ... done.
Linking ~\scoop\apps\premake\current => ~\scoop\apps\premake\5.0.0-beta8
Creating shim for 'premake5'.
'premake' (5.0.0-beta8) was installed successfully!

PS C:\> premake5 --version
premake5 (Premake Build Script Generator) 5.0.0-beta8
```

### Window Management

- [GLFW - Graphics Library Framework](https://www.glfw.org/)
- [GLFW source code](https://github.com/glfw/glfw)

#### Compile and build

Open the 'x64 Native Tools Command Prompt for VS' in the Visual Studio 2026 group of the start menu. Cmake will detect the VS2026 automatically.

```powershell
E:\test\glfw-3.4>cmake -S E:\test\glfw-3.4 -B E:\test\glfw-3.4\build
-- Building for: Visual Studio 18 2026
-- The C compiler identification is MSVC 19.50.35724.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - not found
-- Found Threads: TRUE
-- Including Win32 support
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE)
-- Documentation generation requires Doxygen 1.9.8 or later
-- Configuring done (7.4s)
-- Generating done (0.3s)
-- Build files have been written to: E:/test/glfw-3.4/build

E:\test\glfw-3.4>cmake --build build --config Release
...
...
Done.

```

You will find glfw3.lib in the build\src\Release.

### Extension Library

![alt text](images/glew.png)

- [GLEW - OpenGL Extension Wrangler](https://glew.sourceforge.net/) Download the binary directly.
- [GLEW source code](https://github.com/nigels-com/glew.git)

Download the binary version glew-2.3.1-win32.zip, for example.

### Math Library

![glm](images/glm.png)

- [GLM - OpenGL Mathematics](https://glm.g-truc.net)
- [GLM source code](https://github.com/g-truc/glm)

Download the source code and it is a header-only libary.

### Texture Management

![SOIL](images/soil.png)

- [SOIL2 - Simple OpenGL Image Loader](https://github.com/SpartanJ/SOIL2)

Download the source code zip.
using premake to generate the visual studio solution.

```powershell
E:\test\SOIL2-1.31>premake5 vs2026
Building configurations...
Running action 'vs2026'...
Generated make/windows/SOIL2.slnx...
Generated make/windows/soil2-static-lib.vcxproj...
Generated make/windows/soil2-shared-lib.vcxproj...
Generated make/windows/soil2-test.vcxproj...
Generated make/windows/soil2-test.vcxproj.filters...
Generated make/windows/soil2-perf-test.vcxproj...
Generated make/windows/soil2-perf-test.vcxproj.filters...
Done (96ms).
```

The open the make\windows\SOIL2.slnx with Visual Studio 2026. Set the release and x64, then build the project soil2-static-lib. You will see the soil2.lib in lib\windows folder.

```powershell
E:\test\SOIL2-1.31>dir lib\windows

2026/02/13  22:29    <DIR>          .
2026/02/13  22:29    <DIR>          ..
2026/02/13  22:29         1,178,996 soil2.lib
2026/02/13  22:29           159,744 soil2.pdb

```
