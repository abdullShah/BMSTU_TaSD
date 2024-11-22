#!/bin/bash

gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c main.c -o main.o --coverage -g
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c ui.c -o ui.o --coverage -g
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c tree.c -o tree.o --coverage -g
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c measure.c -o measure.o --coverage -g
gcc -o app.exe main.o ui.o tree.o measure.o -fprofile-arcs -ftest-coverage -lm
