#!/bin/bash

mkdir -p bin/

cp ../../src/{f2p.c,debug.h} bin/

if [ "$1" != "" ]; then
    if [ -d "$1" ]; then
	cp $1/program.c bin/program-$1.c
	g++ -g bin/program-$1.c -o bin/prog$1 -Wall -Wextra -Wsign-conversion -Werror
    
	if [ "$?" != "0" ]; then
	    echo "f2p: FAILED TO COMPILE TEST $1"
	    exit 1
	fi
	
	echo "Running example: "
	cat <(cat $1/input.txt |  bin/prog$1 )
	
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
	g++ -g bin/program-${dir}.c -o bin/prog${dir} -Wall -Wextra -Wsign-conversion -Werror
	if [ "$?" != "0" ]; then
	    echo "f2p: FAILED TO COMPILE TEST ${dir}"
	    exit 1
	fi
	
	output=$(diff ${dir}/output.txt <(cat ${dir}/input.txt | bin/prog${dir} 2>&1));
	
	if [ "${output}" != "" ]; then
	    echo "f2p: ERROR IN TEST ${dir}"
	    echo "f2p: DIFF FOR CASE FOLLOWS"
	    echo "${output}"
	    exit 1
	fi
    done
    
    echo "f2p: ALL TESTS OK"
fi

