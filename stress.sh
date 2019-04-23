#!/bin/bash

mode="out"
tm="n"

while [ -n "$1" ]
do
    case "$1" in
        -sr) write="w";;
        -su) shutup="su";;
        -si) show_inp="si";;
        -rc) recompile="rc";;
        -nocheck) mode="raw";;
        -time) tm="t";;
        --) shift
        break ;;
    esac
    shift
done

if [ "$recompile" == "rc" ]
then
    echo -e "\033[1;34mrecompiling source...\033[0m"
    g++ -O2 -std=c++11 generator.cpp -o ./tmp/generator
    g++ -O2 -std=c++11 checker.cpp -o ./tmp/checker

    g++ -c -O2 -std=c++11 exec.cpp -o tmp/exec.o
    g++ -O2 tmp/exec.o build/CMakeFiles/big_integer.dir/big_integer.cpp.o \
                   build/CMakeFiles/big_integer.dir/_core_arithmetics.cpp.o \
                   build/CMakeFiles/big_integer.dir/engine/_asm_vector.asm.o -o tmp/exec
fi

ok=0
fl=0
echo -e "\033[1;35mtesting mode : operation = ($1), block_size = ($2), blocks_cnt = ($3), arg lengths = ($4, $5)\033[0m"

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
        pass="\033[32mpassed\033[0m"
    else
        fl=$[fl+$f]
        pass="\033[31mfailed\033[0m"
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
