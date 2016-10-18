SHELL:=/bin/bash
# Makefile to compile the programs and to clean directories

.PHONY: all compile test clean

all: compile test

compile:
	@echo -e "\nStarting compile...\n"
	mkdir -p bin
	gcc -O3 src/crc-complete-bruteforce.c -o bin/crc-complete-bruteforce

test: compile
	@echo -e "\nStarting Tests...\n"
	@cd test/crc-complete-bruteforce; ./test.sh

clean:
	rm -rf bin

