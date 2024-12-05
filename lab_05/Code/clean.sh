#!/bin/bash 
 
FILES="*.out *.o *.temp *.exe *.gcov *.gcno *.gcda"
for file in $FILES 
do
    rm ./out/$file -f
done

rm app.exe -f
