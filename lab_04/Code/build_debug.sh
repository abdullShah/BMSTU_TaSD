#!/bin/bash

gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c main.c -o main.o --coverage -g
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c ui.c -o ui.o --coverage -g
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c arr_stack.c -o arr_stack.o --coverage -g
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c list_stack.c -o list_stack.o --coverage -g
gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c measurement.c -o measurement.o --coverage -g
gcc -o app.exe main.o ui.o arr_stack.o list_stack.o measurement.o -fprofile-arcs -ftest-coverage -lm
