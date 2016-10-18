SHELL:=/bin/bash
# Makefile to compile the programs and to clean directories

.PHONY: all compile test clean

all: compile test

compile:
	@echo -e "\nStarting compile...\n"
	mkdir -p bin
	cp src/crc-complete-bruteforce.c test/crc-complete-bruteforce/bin/crc-complete-bruteforce.c
	cp src/debug.h test/crc-complete-bruteforce/bin/debug.h

test: compile
	@echo -e "\nStarting Tests...\n"
	@cd test/crc-complete-bruteforce; ./test.sh

clean:
	rm -rf bin

