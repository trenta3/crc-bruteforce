#!/bin/bash

f2b=../../bin/file2binary

for folder in $(ls -d */); do
    dir=${folder%%/};
    output=$(diff ${dir}/output.txt <(cat ${dir}/input.txt | ${f2b}));

    if [ "${output}" != "" ]; then
	echo "file2binary: ERROR IN TEST ${dir}"
	echo "file2binary: DIFF FOR CASE FOLLOWS"
	echo "${output}"
	exit 1
    fi
done

echo "file2binary: ALL TESTS OK"

