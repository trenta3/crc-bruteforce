SHELL:=/bin/bash
# Makefile to compile the programs and to clean directories

.PHONY: all compile test clean

all: compile test

compile:
	@echo -e "\nStarting compile...\n"
	mkdir -p bin
	gcc src/file2binary.c -O3 -o bin/file2binary -Wall -Wextra -Wsign-conversion
	g++ src/variant.cpp -O3 -o bin/variant -Wall -Wextra -Wsign-conversion
	g++ src/variant_testgen.cpp -O3 -o bin/variant_testgen -Wall -Wextra -Wsign-conversion
	cp src/calculateCRCpolynomial.g bin/calculateCRCpolynomial.g

test: compile
	@echo -e "\nStarting Tests...\n"
	@cd test/file2binary; ./test.sh
	@cd test/variant; ./test.sh
	@cd test/calculateCRCpolynomial; ./test.sh
	@cd test/complete; ./test.sh

clean:
	rm -rf bin

