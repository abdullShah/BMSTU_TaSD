#!/bin/bash 
 
FILES="*.out *.o *.temp *.exe *.gcov *.gcno *.gcda"
for file in $FILES 
do
    rm "$file" -f
done

rm ./func_tests/measure/* -f
