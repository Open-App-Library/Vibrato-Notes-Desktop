#!/bin/bash

mkdir -p qmake
cd qmake

if [ $1 == "help" ] || [ $1 == "h" ] || [ $1 == "-h" ] || [ $1 == "--help" ]; then
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
fi

qmake -project "QT += testlib" ../VibratoNotes-desktop.pro
bear -a -o ../compile_commands.json make
