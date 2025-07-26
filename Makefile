
CXX := gcc
FLAGS := -O2 -Ivendor -Isrc
OUTPUT := build/kiro

SRC := $(shell python py/getcfiles.py src)

all:
	$(CXX) $(SRC) -o $(OUTPUT) $(FLAGS)
	./build/kiro

clean:
	rm -f $(OUTPUT)
