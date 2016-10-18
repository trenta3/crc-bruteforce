#!/bin/bash

mkdir -p bin/

cp ../../src/{crc-complete-bruteforce.c,debug.h} bin/

if [ "$1" != "" ]; then
    if [ -d "$1" ]; then
	cp $1/program.c bin/program-$1.c
	gcc -g bin/program-$1.c -o bin/prog$1 -Wall -Wextra -Wsign-conversion -Werror
    
	if [ "$?" != "0" ]; then
	    echo "crc-complete-bruteforce: FAILED TO COMPILE TEST $1"
	    exit 1
	fi
	
	echo "Running example: "
	diff $1/output.txt <(cat $1/input.txt |  bin/prog$1 )
	
	echo "Expected output: "
	cat $1/output.txt
    else
	echo "Directory non trovata: $1"
	exit 1
    fi
else
    for folder in $(ls -1 -d */ | grep -v bin); do
	dir=${folder%%/};
	
	cp ${dir}/program.c bin/program-${dir}.c
	gcc -g bin/program-${dir}.c -o bin/prog${dir} -Wall -Wextra -Wsign-conversion -Werror
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
fi

