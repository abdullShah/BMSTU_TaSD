#!/bin/bash

gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c main.c -o main.o -g --coverage
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c ui.c -o ui.o -g --coverage
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c matrix.c -o matrix.o -g --coverage
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c measurement.c -o measurement.o -g --coverage
gcc -o app.exe main.o ui.o matrix.o measurement.o -fprofile-arcs -ftest-coverage -lm
