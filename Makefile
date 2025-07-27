
CXX := gcc
FLAGS := -O2 -Ivendor -Isrc
OUTPUT := build/kiro

SRC := $(shell python py/getcfiles.py src)

ifeq ($(LINUX), 1)
	$(error Linux not supported yet)
else
	LIBS := $(wildcard lib/windows/**)
	PASTE_DLLS := python py/pastedlls.py windows
endif

.PHONY: all clean paste_dlls

all: $(OUTPUT)
	$(PASTE_DLLS)
	./$(OUTPUT)

$(OUTPUT): $(SRC)
	$(CXX) $(SRC) -o $(OUTPUT) $(LIBS) $(FLAGS)

paste_dlls:
	$(PASTE_DLLS)

clean:
	rm -f $(OUTPUT)
