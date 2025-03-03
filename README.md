# Rogue Forever
A dungeon crawler game.

Game currently consists of drawing a static image from a map representation file
and a spritesheet. On top of this image, a player character placeholder is drawn
to the coordinate 0, 0 in left upper corner. The map representation file and
spritesheet were created using Tiled map editor.

Tiled home page:
https://www.mapeditor.org/

## How to install for playing the game

In order to run the game a main executable is required in addition to some
runtime dependencies, such as graphics assets and maps.

Operating system specific installation instructions can be found below.

* [Instaling on Debian Linux](#installing-on-debian-linux)
* [Installing on Windows](#installing-on-windows)

### Installing on Linux variants
Linux installation method is compiling the main executable from source and
proceeding to retrieve runtime dependencies from the web, possibly
utilizing a provided shell script. To install, you may walk through the
following more detailed instructions.

#### Installing on Debian Linux
This instruction is written for a 0.0.0 test release. Please substitute 0.0.0
with the version number of the release which you wish to install.

Open a terminal program for running commands, such as `wget` command.

First, update system packages.
```
sudo apt update && sudo apt upgrade && sudo apt autoremove
```

Change current directory to the directory where you want to store the files
of the game.
```
cd ${HOME}/games # For example
```

Using a web browser, navigate to page:
https://github.com/pottumuusi/rogue-forever/releases

Right click and copy link of 'Source code (tar.gz)' of wanted release. Paste
the link to `wget`.
```
wget -O rogue-forever-0.0.0.tar.gz https://github.com/pottumuusi/rogue-forever/archive/refs/tags/0.0.0.tar.gz
```

Extract the compressed file and change directory to the newly created directory.
```
tar xvf ./rogue-forever-0.0.0.tar.gz
cd rogue-forever-0.0.0/
```

Install build dependencies, which make it possible to compile source code.
This script takes a while. An estimate duration is under 10 minutes.
```
./scripts/install_build_dependencies/install_build_dependencies.sh linux | tee output_install_build_dependencies.txt
```

Retrieve runtime dependencies from the web.
```
./scripts/install_runtime_dependencies_release.sh | tee output_install_runtime_dependencies_release.txt
```

Optionally run the unit tests of project.
```
make test
```

Compile the main executable.
```
make linux
```

The game can then be started.
```
make linux_run
```

### Installing on Windows
For Windows, an executable is provided along with a script for installing
runtime dependencies from the web. To install, you may walk through the
following instructions.

Using a web browser, navigate to page:
https://github.com/pottumuusi/rogue-forever/releases

Download a Windows release zip to a directory where you want to store the game
and extract the zip package.

Open Powershell as administrator for running upcoming commands.

Change current directory to the directory extracted from the zip.
```
cd "C:\Users\johndoe\games\rogue-forever-windows-0.0.0" # For example
```

Query for the current execution policy. The result is used for restoring the
original execution policy when install is done.
```
Get-ExecutionPolicy
```

Allow running unsigned scripts.
```
Set-ExecutionPolicy --ExecutionPolicy Unrestricted
```

Retrieve runtime dependencies from the web by running script from release zip.
```
.\install_runtime_dependencies_release.ps1
```
The command should produce a warning. To download the dependencies, please
consult the warning message for allowing running the script.

To restore execution policy, please run
```
Set-ExecutionPolicy --ExecutionPolicy <original_policy>
```
replacing '<original_policy>' with the policy which was queried earlier in the
instructions. You can alternatively use
```
Set-ExecutionPolicy --ExecutionPolicy Default
```
in case you have been using default execution policy.

The game can now be started by running executable `rogue_forever.exe`.

## How to install for developing the game
For developing the game, you can fork this repository and develop your copy
further. The following instructions are written against the original
repository.

### Developing for Linux
Clone the repository.
```
sudo apt update && sudo apt upgrade && sudo apt autoremove
```
```
ssh-keygen -t ed25519 # In case you have yet to generate SSH keypair
```
```
sudo apt install git
```
```
git clone git@github.com:pottumuusi/rogue-forever.git
```

In the project base directory, install build dependencies.
```
./scripts/install_build_dependencies/install_build_dependencies.sh linux | tee output_install_build_dependencies.txt
```

Install runtime dependencies.
```
./scripts/install_runtime_dependencies_development.sh | tee output_install_runtime_dependencies_development.txt
```

Run the unit tests of project.
```
make test
```

Compile the main executable.
```
make linux
```

The game can then be started for testing your changes.
```
make linux_run
```

### Developing for Windows
Clone the repository.
```
sudo apt update && sudo apt upgrade && sudo apt autoremove
```
```
ssh-keygen -t ed25519 # In case you have yet to generate SSH keypair
```
```
sudo apt install git
```
```
git clone git@github.com:pottumuusi/rogue-forever.git
```

In the project base directory, install build dependencies.
```
./scripts/install_build_dependencies/install_build_dependencies.sh full | tee output_install_build_dependencies.txt
```

Install runtime dependencies.
```
./scripts/install_runtime_dependencies_development.sh | tee output_install_runtime_dependencies_development.txt
```

Run the unit tests of project.
```
make test
```

Compile the main executable.
```
make windows
```

For testing your changes, the game executable `rogue_forever.exe` can then be
started on a Windows host, where runtime dependencies have been installed. There
is a script `scripts/install_runtime_dependencies_release.ps1` for installing
the runtime dependencies for Windows. This script is packaged to the release zip
of Windows.  See Windows install instructions for an example on how to utilize
the script.

A straightforward way for Windows development is to utilize Windows Subsystem
for Linux (WSL). In WSL, perform the above development environment setup. Then
to test the changes made to game, you can use `make` target `windows_deploy`.
Before running `make` with `windows_deploy` target, please modify the deploy
destination path in Makefile to suit your needs.
