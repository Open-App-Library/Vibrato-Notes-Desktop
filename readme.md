Vibrato Notes Desktop Client
----------------------------

## To compile & run

Requirements:

- Qt5
- [Chicken](https://www.call-cc.org/)

### 1.) Clone this repository.

`git clone https://gitlab.com/Open-App-Library/vibratonotes-desktop.git`

### 2.) Pull submodules

`git submodule update --init --recursive --remote`

### 3.) Compile MarkdownPanda

- `cd src/text-editor/markdownpanda`
- `sh build.sh`

### 4.) Compile the project

#### Option A: Use Qt Creator

There are two ways to compile this project. The easiest is to open VibratoNotes-Desktop.pro in Qt Creator.

#### Option B: Use `compile.sh`

My favorite way to compile this project is to run the bash script `compile.sh`. This badass little script will additionally generate a compile_commands.json if you have [bear](https://github.com/rizsotto/Bear) installed.

```
compile.sh
A tool for compiling your project and generating a compile_commands.json.

Usage:
  * View the help screen:
    ./compile.sh --help

  * General Usage:
    ./compile.sh

  * Clean your build:
    ./compile.sh clean

Environmental Variables:
  * $BUILD_DIR
    Directory for project to be created
    --Defaults to "build/"

    Keep in mind that build/ is recommended because it
    is also gitignored. Also note that if your specified
    path does not exist, it will be recursively created.

  * $QMAKE_CMD
    Custom qmake command. (ex: "qmake-qt5")
    --Defaults to "qmake"
```
