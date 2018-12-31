#!/bin/bash

mkdir -p qmake
cd qmake

if [ $1 == "clean" ]; then
    make clean
    rm -f ../compile_commands.json
fi

bear -a -o ../compile_commands.json $BEAR_OPTIONS make
