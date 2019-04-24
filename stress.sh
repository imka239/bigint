#!/bin/bash

mode="out"
tm="n"
help="nohelp"

while [ -n "$1" ]
do
    case "$1" in
        -sr) write="w";;
        -su) shutup="su";;
        -si) show_inp="si";;
        --rc) recompile="rc";;
        --nocheck) mode="raw";;
        -time) tm="t";;
        -help) help="HELP";;
        --help) help="HELP";;
        --) shift
        break ;;
    esac
    shift
done

if [ "$help" == "HELP" ]
then
    echo -e "\e[34musage : ./stress.sh [options...] -- \<operation> <test block size> <test block cnt> <1st arg (digits)> <2nd arg (digits)>\e[0m"
    echo -e "\e[1;34mpossible options : \n  -help, --help - show this message \n  --rc - recompile source\n  --nocheck - run unchecked (fast) tests\
        \n  -time - show timing info\n  -si - show input\n  -sr - show output\n  -su - shutup\e[0m"

exit
fi

if [ "$recompile" == "rc" ]
then
    echo -e "\e[1;34mrecompiling source...\e[0m"
    g++ -O2 -std=c++11 generator.cpp -o ./tmp/generator
    g++ -O2 -std=c++11 checker.cpp -o ./tmp/checker

    g++ -c -O2 -std=c++11 exec.cpp -o tmp/exec.o
    g++ -O2 tmp/exec.o build/CMakeFiles/big_integer.dir/big_integer.cpp.o \
                   build/CMakeFiles/big_integer.dir/_core_arithmetics.cpp.o \
                   build/CMakeFiles/big_integer.dir/engine/_asm_vector.asm.o -o tmp/exec
fi

ok=0
fl=0
echo -e "\e[1;35mtesting mode : operation = ($1), block_size = ($2), blocks_cnt = ($3), arg lengths = ($4, $5)\e[0m"

for (( i = 0; i < $3; i++ ))
do
    gen_test=$(echo "$2 $4 $5" | tmp/generator)

    if [ "$show_inp" == "si" ]
    then
        echo "INPUT:"
        echo "$gen_test"
    fi

    cpp_out=$(echo "$mode $2 $1 $gen_test $tm" | tmp/exec)
    if [ "$mode" == "out" ]
    then
        pyt_out=$(echo "$2 $1 $tm $gen_test" | tmp/calc_out.py)
    else
        pyt_out=$(echo "$2 $1 $tm $gen_test" | tmp/calc_raw.py)
    fi
    f=$(echo "$2 $cpp_out $pyt_out" | tmp/checker)

    if [ "$f" == "0" ]
    then
        ok=$[ok+$2]
        pass="\e[32mpassed\e[0m"
    else
        fl=$[fl+$f]
        pass="\e[31mfailed\e[0m"
    fi

    if [ "$shutup" != "su" ]
    then
        echo -e "block $[i+1] : $pass"
    fi

    if [ "$write" == "w" ]
    then
        echo "CPP:"
        echo "$cpp_out"
        echo ""
        echo "PYT:"
        echo "$pyt_out"
    fi
done

echo -e "testing finished : failed \033[31m$fl\033[0m tests, passed \033[32m$ok\033[0m tests"
