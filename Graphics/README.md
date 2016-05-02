# lhcvmm
Large Hadron Collider Visual Music Machine - Graphics Engine

# Directory structure

* addons: openFrameworks addons (www.ofxaddons.com) used in this project. Some are addons developed by Opensemble specifically for the LHCVMM, and some were forked from their original repositories.

* common_classes: Classes for general purpose common tasks.

* examples: Simple projects for testing and exemplify the use of the tools developed for the LHCVMM’s Graphics Engine.

* apps: Complex apps that are part of the LHCVMM’s Graphics Engine.

# Compilation

The Graphics Engine of the LHCVMM is developed using openFrameworks v0.9.0 (www.openFrameworks.com). To compile the examples in this repository you need to clone lhcvmm main directory into OF apps dir (OF_ROOT_DIR/apps/lhcvmm).

### OSX
In OS X projects can be compiled using the Xcode projects or using the Makefiles by Terminal.

### Linux
Projects can be compiled using the Makefiles by Terminal.

# Creating a New Project
### Path
The project must be placed either into examples/ or apps/ directories.

### Makefile
The OF root directory must be: OF_ROOT=$(realpath ../../../../..)

### config.make
You need to add these lines to link the local addons and common_classes folders:
PROJECT_EXTERNAL_SOURCE_PATHS = ../../common_classes
PROJECT_EXTERNAL_SOURCE_PATHS += ../../addons

### Xcode project
- Project.xconfig: OF_PATH = ../../../../..
- Add files to project: OF_ROOT/libs/openFrameworksCompiled/project/osx/CoreOF.xconfig && openFrameworksLib.xcodeproj
- Build Phases -> Target Dependencies: openFrameworksLib
- Link Binary With Libraries: openFrameworksDebug.a
- Run Script: Change to "../../../../.."

### emptyExample
The simplest way to create a new project is to copy the examples/emptyExample project. This project has a ready to compile Xcode project and the Makefile and config.make already configured.

### addons
All addons used in the LHCVMM must be placed into lhcvmm/Graphics/addons. Apps and examples projects must link the addons to this path.
addons.make file must always be blank to avoid references to OF_ROOT/addons.

### Documentation
All projects must include a README.md file with the following info:
* TITLE
* Description
* Dependencies (all dependencies must be included in the lhcvmm/Graphics repository)
* Compilation instructions.
