SHELL:=/bin/bash
# Makefile to compile the programs and to clean directories

.PHONY: all compile test clean

all: compile test

compile:
	@echo -e "\nStarting compile...\n"
	mkdir -p bin
	cp src/f2p.c test/f2p/bin/f2p.c
	cp src/debug.h test/f2p/bin/debug.h

test: compile
	@echo -e "\nStarting Tests...\n"
	@cd test/f2p; ./test.sh

clean:
	rm -rf bin

