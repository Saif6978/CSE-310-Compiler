#!/usr/bin/bash
flex 2205119.l
g++ lex.yy.c -o lexer
./lexer input1.txt