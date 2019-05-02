#!/bin/bash

mkdir -p qmake
cd qmake

if [ $# -lt 1 ]; then
    echo "<==================================>"
    echo "| Generating compile_commands.json |"
    echo "<==================================>"
    echo ""
elif [ "$1" == "help" ] || [ "$1" == "h" ] || [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
    echo "gen_compile_commands.sh"
    echo "======================="
    echo "Generate a compile_commands.json file."
    echo ""
    echo "Usage:"
    echo "  * General Usage:"
    echo "    ./gen_compile_commands.sh"
    echo ""
    echo "  * Clean your project:"
    echo "    ./gen_compile_commands.sh clean"
    echo ""
    exit
elif [ $1 == "clean" ]; then
    make clean
    rm -f ../compile_commands.json
    exit
else
    echo "Unknown option \"$1\" :("
    exit
fi

echo "QT += testlib" > VibratoNotes.pro
echo "include(../VibratoNotes-Desktop.pro)" >> VibratoNotes.pro
qmake VibratoNotes.pro
bear -a -o ../compile_commands.json make -j(nproc)
