

# Compiler & Flags
CXX := gcc
CFLAGS := -O2 -Ivendor -Isrc

# Platform-specific
ifeq ($(LINUX),1)
    LIBS := $(wildcard lib/linux/*)
    PASTE_DLLS := python3 py/pastedlls.py linux
    EXE_EXT :=
else
    LIBS := $(wildcard lib/windows/*)
    PASTE_DLLS := python py/pastedlls.py windows
    EXE_EXT := .exe
endif

# Output binary
OUTPUT := build/kiro$(EXE_EXT)

# Source/Objects
SRC := $(shell python py/getcfiles.py src)
OBJ := $(patsubst src/%.c, obj/%.o, $(SRC))

all: prepare $(OUTPUT) paste_dlls
	./$(OUTPUT) build

# Link all object files into final binary
$(OUTPUT): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LIBS) $(CFLAGS)

# Compile individual .c files into .o
obj/%.o: src/%.c
	$(CXX) -c $< -o $@ $(CFLAGS)

paste_dlls:
	$(PASTE_DLLS)

clean:
	python py/clean.py

prepare:
	python py/mkobj.py src

