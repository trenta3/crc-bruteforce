#!/bin/bash

cCRCp="gap -q ../../bin/calculateCRCpolynomial.g"

for folder in $(ls -d */); do
    dir=${folder%%/}
    output=$(cat ${dir}/input.txt | ${cCRCp})

    expect=$(cat ${dir}/output.txt)
    echo -n "${output}" | tr -d $'\r' | grep -q -x "${expect}"

    if [ "$?" != "0" ]; then
	echo "calculateCRCpolynomial: TEST ${dir} FAILED"
	echo "${output}"
	echo "calculateCRCpolynomial: EXPECTED OUTPUT WAS TO FIND"
	echo "${expect}"
	exit 1
    fi
done

echo "calculateCRCpolynomial: ALL TESTS OK"

