#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

filename=$1


bison -d -o ${filename}.tab.c ${filename}.y
gcc -c -I. ${filename}.tab.c
flex -o lex.yy.c ${filename}.l
gcc -c -I. lex.yy.c
gcc -o ${filename} lex.yy.o ${filename}.tab.o
 