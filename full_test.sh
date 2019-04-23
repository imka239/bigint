#!/bin/bash

mode="out"
tm="n"

while [ -n "$1" ]
do
    case "$1" in
        -sr) write="w";;
        -su) shutup="-su";;
        -si) show_inp="-si";;
        -rc) recompile="-rc";;
        -nocheck) mode="-nocheck";;
        -time) tm="-time";;
        --) shift
        break ;;
    esac
    shift
done

./stress.sh $write $shutup $show_inp $recompile $mode $tm \
          -- \+ $1 $2 $3 $4
./stress.sh $write $shutup $show_inp $mode $tm \
          -- \- $1 $2 $3 $4
./stress.sh $write $shutup $show_inp $mode $tm \
          -- \* $1 $2 $3 $4
./stress.sh $write $shutup $show_inp $mode $tm \
          -- \/ $1 $2 $3 $4
./stress.sh $write $shutup $show_inp $mode $tm \
          -- \% $1 $2 $3 $4
