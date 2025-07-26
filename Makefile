
CXX := gcc
FLAGS := -O2 -Ivendor -Isrc
OUTPUT := build/kiro

SRC := $(shell python python/getcfiles.py src)

all:
	$(CXX) $(SRC) -o $(OUTPUT) $(FLAGS)
	./kiro

clean:
	rm -f $(OUTPUT)
