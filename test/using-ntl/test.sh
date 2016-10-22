#!/bin/bash

PROG=../../bin/using-ntl

for folder in $(ls -d */); do
    dir=${folder%%/}
    output=$(cat ${dir}/input.txt | ${PROG} 2>/dev/null)

    expect=$(cat ${dir}/output.txt | sed -r 's/\[/\\\[/g' | sed -r 's/\]/\\\]/g')
    
    echo "${output}" | grep -qx "${expect}"
    if [ "$?" != "0" ]; then
	echo "using-ntl: TEST ${dir} FAILED"
	echo "${output}"
	echo "using-ntl: EXPECTED OUTPUT WAS TO FIND"
	echo "${expect}"
	exit 1
    fi
done

echo "using-ntl: ALL TESTS OK"

