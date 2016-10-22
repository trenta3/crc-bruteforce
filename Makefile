SHELL:=/bin/bash
# Makefile to compile the programs and to clean directories

.PHONY: all compile test clean

all: compile test

compile:
	@echo -e "\nStarting compile...\n"
	mkdir -p bin
	g++ src/using-ntl.c -std=c++11 -o bin/using-ntl -g -I${HOME}/sw/include -L${HOME}/sw/lib -lntl -lgmp -lgf2x -lm

test: compile
	@echo -e "\nStarting Tests...\n"
	@cd test/using-ntl; ./test.sh

clean:
	rm -rf bin

