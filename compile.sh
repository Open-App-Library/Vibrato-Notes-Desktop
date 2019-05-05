#!/usr/bin/env bash

# This script will compile your project and generate a compile_commands.json.
# How friggin awesome is that??

# Let's start off our journey by creaing a build directory.
if ! [[ -n $BUILD_DIR ]]; then
    BUILD_DIR="build/"
fi
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Handy-dandy ASCII color variables
normal=\\e[0m
red=\\e[31m
yellow=\\e[33m
cyan=\\e[36m
green=\\e[32m


################################
# Process command-line options #
################################
if [[ $# < 1 ]]; then
    echo ""
elif [[ "$1" == "help" ]] || [[ "$1" == "h" ]] || [[ "$1" == "--help" ]] || [[ "$1" == "-h" ]]; then
    echo "compile.sh"
    printf "$yellow"
    echo "A tool for compiling your project and generating a compile_commands.json."
    echo ""
    printf "$cyan"
    echo "Usage:"
    printf "$normal"
    echo "  * View the help screen:"
    echo "    ./compile.sh --help"
    echo ""
    echo "  * General Usage:"
    echo "    ./compile.sh"
    echo ""
    echo "  * Clean your build:"
    echo "    ./compile.sh clean"
    echo ""
    printf "$cyan"
    echo "Environmental Variables:"
    printf "$normal"
    echo "  * \$BUILD_DIR"
    echo "    Directory for project to be created"
    echo "    --Defaults to \"build/\""
    echo ""
    echo "    Keep in mind that build/ is recommended because it"
    echo "    is also gitignored. Also note that if your specified"
    echo "    path does not exist, it will be recursively created."
    echo ""
    echo "  * \$QMAKE_CMD"
    echo "    Custom qmake command. (ex: \"qmake-qt5\")"
    echo "    --Defaults to \"qmake\""
    echo ""
    exit
elif [[ $1 == "clean" ]]; then
    make clean
    rm -f ../compile_commands.json
    exit
elif [[ $1 == "only_hope" ]]; then
    printf "                    .==.\n                   ()''()-.\n        .---.       ;--; /\n      .'_:___\". _..'.  __'.\n      |__ --==|'-''' \'...;\n      [  ]  :[|       |---\\n      |__| I=[|     .'    '.\n      / / ____|     :       '._\n     |-/.____.'      | :       :\n    /___\ /___\      '-'._----'\n        art by Shanaka Dias\n\n"
    exit
else
    echo "Unknown option \"$1\" :-("
    exit
fi


####################################
# Do a very basic dependency check #
####################################

# We are going to test if bear, qmake, and make are availible.
export HAS_BEAR=0 HAS_QMAKE=0 HAS_MAKE=0

# If user did not specify the QMAKE_CMD variable, default to "qmake"
if ! [[ -n $QMAKE_CMD ]]; then
    QMAKE_CMD="qmake"
fi

# Compile tool dependency checking function

function dep_check {
    program_name=$1
    missing_message=$2
    var_name=$3
    if ! which $program_name>/dev/null 2>&1; then
        printf "\n$missing_message"
    else
        eval $var_name=true
    fi
}

# Check for bear, qmake, and make
dep_check "bear" \
          "$yellow[WARNING]$normal 'bear' not found. compile_commands.json will not be generated.\n\tNeed to install bear? Get it here: https://github.com/rizsotto/Bear\n" \
          "HAS_BEAR"
dep_check $QMAKE_CMD \
          "$red[FATAL]$normal 'qmake' not found.\n  If you would like to specify a custom qmake command, you may set the QMAKE_CMD environmental variable\n   $cyan ex. QMAKE_CMD=\"qmake-qt5\" ./compile.sh$normal\n" \
          "HAS_QMAKE"
dep_check "make" \
          "$red[FATAL]$normal 'make' not found. Please ensure GNU make is installed\n" \
          "HAS_MAKE"

# Display error/warning messages if needed
if [[ $HAS_QMAKE != true || $HAS_MAKE != true ]]; then
    printf "$red\n========== Failed dependency check :( ==========$normal\n\n"
    exit
elif [[ $HAS_BEAR != true ]]; then
    printf "$yellow\n========== You've got 1 warning ¯\_(ツ)_/¯ ==========$normal\n\n"
fi

# Inform user that we are now compiling their project
printf "$yellow"
printf "Compiling your project...\n\n"
printf "$normal"


####################################
# Compiling the project            #
####################################

echo "QT += testlib" > VibratoNotes.pro
echo "include(../VibratoNotes-Desktop.pro)" >> VibratoNotes.pro

qmake VibratoNotes.pro

if [[ $HAS_BEAR == true ]]; then
    bear -a -o ../compile_commands.json make -j$(nproc)
else
    make -j$(nproc)
fi
