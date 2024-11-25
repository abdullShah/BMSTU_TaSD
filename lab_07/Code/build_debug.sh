#!/bin/bash

gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c main.c -o main.o --coverage -g
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c ui.c -o ui.o --coverage -g
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c tree.c -o tree.o --coverage -g
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c avl_tree.c -o avl_tree.o --coverage -g
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c hash_table.c -o hash_table.o --coverage -g
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c measure.c -o measure.o --coverage -g
gcc -o app.exe main.o ui.o tree.o avl_tree.o hash_table.o measure.o -fprofile-arcs -ftest-coverage -lm