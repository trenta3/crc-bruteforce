#!/bin/bash

variant=../../bin/variant

for folder in $(ls -d */); do
    dir=${folder%%/};
    output=$(diff ${dir}/output.txt <(cat ${dir}/input.txt | ${variant}));

    if [ "${output}" != "" ]; then
	echo "variant: ERROR IN TEST ${dir}"
	echo "variant: DIFF FOR CASE FOLLOWS"
	echo "${output}"
	exit 1
    fi
done

echo "variant: ALL TESTS OK"

