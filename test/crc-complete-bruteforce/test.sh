#!/bin/bash

mkdir -p bin/

cp ../../src/{crc-complete-bruteforce.c,debug.h} bin/

for folder in $(ls -1 -d */ | grep -v bin); do
    dir=${folder%%/};

    cp ${dir}/program.c bin/program-${dir}.c
    gcc -O3 bin/program-${dir}.c -o bin/prog${dir} -Wall -Wextra -Wsign-conversion -Werror
    if [ "$?" != "0" ]; then
	echo "crc-complete-bruteforce: FAILED TO COMPILE TEST ${dir}"
	exit 1
    fi
    
    output=$(diff ${dir}/output.txt <(cat ${dir}/input.txt | bin/prog${dir}));

    if [ "${output}" != "" ]; then
	echo "crc-complete-bruteforce: ERROR IN TEST ${dir}"
	echo "crc-complete-bruteforce: DIFF FOR CASE FOLLOWS"
	echo "${output}"
	exit 1
    fi
done

echo "crc-complete-bruteforce: ALL TESTS OK"

