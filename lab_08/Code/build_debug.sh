#!/bin/bash

OUT_DIR="out"
SRC_FILES=(main.c ui.c graph.c)

OBJ_FILES=("${SRC_FILES[@]/%.c/.o}")
OBJ_FILES=("${OBJ_FILES[@]/#/"$OUT_DIR"/}")

mkdir -p "$OUT_DIR"
for i in "${!SRC_FILES[@]}"; do
  gcc -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -c "${SRC_FILES[i]}" -o "${OBJ_FILES[i]}" --coverage -g
done

gcc -o app.exe "${OBJ_FILES[@]}" -fprofile-arcs -ftest-coverage -lm
