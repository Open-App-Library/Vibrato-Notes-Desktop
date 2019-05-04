Vibrato Notes Desktop Client
----------------------------

## To compile & run

Requirements:

- Qt5
- [Chicken](https://www.call-cc.org/)

1.) Clone this repository.

`git clone https://gitlab.com/Open-App-Library/vibratonotes-desktop.git`

2.) Pull submodules

`git submodule update --init --recursive --remote`

3.) Compile MarkdownPanda

- `cd src/text-editor/markdownpanda`
- `sh build.sh`

**Note**: Make sure compiling MarkdownPanda finishes successfully.

3.) Open VibratoNotes-Desktop.pro in Qt Creator.

4.) Compile & run just like any other Qt Creator project.
