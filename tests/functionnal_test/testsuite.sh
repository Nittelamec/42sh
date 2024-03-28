#!/bin/sh

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
TURQUOISE="\e[36m"
WHITE="\e[0m"

TOTAL_RUN=0
TOTAL_FAIL=0

ref_file_out=/tmp/.ref_file_out
ref_file_err=/tmp/.ref_file_err
my_file_out=/tmp/.my_file_out
my_file_err=/tmp/.my_file_err

BIN="$1"

if [ "$#" -eq 0 ]; then
    echo "Usage: ./testsuite.sh 42sh [CATEGORIES]"
    exit 1
fi
REALBIN=$(realpath $BIN)

my_categories=
shift
FIRST=true
if [ "$#" -gt 0 ]; then
    for var in "$@";
    do
        if $FIRST; then

            my_categories+=" -name"
            my_categories+=" $var"
            FIRST=false
        else
            my_categories+=" -o -name"
            my_categories+=" $var"
        fi

    done
fi


run_test() {
    [ -e "$1" ] || echo "Missing test file $1" 1>&2
    success=true
    TOTAL_RUN=$((TOTAL_RUN+1))

    echo -ne "$BLUE-->> ${WHITE}$1...$WHITE"
    bash --posix "$1" > $ref_file_out 2> $ref_file_err
    REF_CODE=$?
    $REALBIN "$1" > $my_file_out 2> $my_file_err
    MY_CODE=$?
    
    diff --color=always -u $ref_file_out $my_file_out > $1.diff
    DIFF_CODE=$?

    if [ $REF_CODE != $MY_CODE ]; then
        echo -ne "$RED RETURN$WHITE"
        success=false
    fi
    if [ $DIFF_CODE != 0 ]; then
        echo -ne "$RED STDOUT$WHITE"
        success=false
    fi

    if { [ -s $ref_file_err ] && [ ! -s $my_file_err ]; } ||
        { [ ! -s $ref_file_err ] && [ -s $my_file_err ]; }; then
        echo -ne "$RED STDERR$WHITE"
        success=false
    fi

    if $success; then
        echo -e "$GREEN OK$WHITE"
        rm -f $1.diff
    else
        [ -s "$(realpath $1.diff)" ] && echo -ne "$RED (cat $(realpath $1.diff))$WHITE"
        echo
        TOTAL_FAIL=$((TOTAL_FAIL + 1))
    fi
}

run_test_as_input() {
    [ -e "$1" ] || echo "Missing test file $1" 1>&2
    success=true
    TOTAL_RUN=$((TOTAL_RUN+1))

    echo -ne "$BLUE-->> ${WHITE}$1...$WHITE"
    bash --posix < "$1" > $ref_file_out 2> $ref_file_err
    REF_CODE=$?
    $REALBIN < "$1" > $my_file_out 2> $my_file_err
    MY_CODE=$?
    
    diff --color=always -u $ref_file_out $my_file_out > $1.diff
    DIFF_CODE=$?

    if [ $REF_CODE != $MY_CODE ]; then
        echo -ne "$RED RETURN$WHITE"
        success=false
    fi
    if [ $DIFF_CODE != 0 ]; then
        echo -ne "$RED STDOUT$WHITE"
        success=false
    fi

    if { [ -s $ref_file_err ] && [ ! -s $my_file_err ]; } ||
        { [ ! -s $ref_file_err ] && [ -s $my_file_err ]; }; then
        echo -ne "$RED STDERR$WHITE"
        success=false
    fi

    if $success; then
        echo -e "$GREEN OK$WHITE"
        rm -f $1.diff
    else
        [ -s "$(realpath $1.diff)" ] && echo -ne "$RED (cat $(realpath $1.diff))$WHITE"
        echo
        TOTAL_FAIL=$((TOTAL_FAIL + 1))
    fi
}



run_category() {
    cd $1
    source ./testsuite.sh
    cd - >/dev/null
}

run_testsuite() {
    for category in $@; do
        [ ${category} = "." ] && continue

        echo -e "$TURQUOISE==================================================================="
        printf "$YELLOW%-36s $TURQUOISE%s\n" "CATEGORIE:            ${category#*/}"
        echo -e "$TURQUOISE==================================================================="

        run_category $category
    done
}

run_testsuite $(find . -type d$my_categories)

PERCENT_SUCCESS=$(((TOTAL_RUN - TOTAL_FAIL) * 100 / TOTAL_RUN))

echo -e "$BLUE==================================================================="
echo -e "$WHITE RECAP: $([ $PERCENT_SUCCESS = 100 ] && echo $GREEN || echo $RED) $PERCENT_SUCCESS%"
echo -e "$BLUE==================================================================="

rm -f $ref_file_out $my_file_out $ref_file_err $my_file_err
