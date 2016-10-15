SHELL:=/bin/bash
# Makefile to compile the programs and to clean directories

.PHONY: all compile test clean

all: compile test

compile:
	mkdir -p bin
	gcc src/file2binary.c -O3 -o bin/file2binary -Wall -Wextra -Wsign-conversion
	g++ src/variant.cpp -O3 -o bin/variant -Wall -Wextra -Wsign-conversion

test:
	true

clean:
	rm -rf bin tmp

