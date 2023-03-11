# Current issues
# libraries need to also be compiled with gcc
# Libraries need to be compiles for the platform

# gcc -std=c++20 -Wall Example/src/project.cpp -o build/debug/srender -ISRender/src -Ilinking/include -Lbuild/debug -lsrender -Llinking/lib -lassimp -lglad -lglfw3

NAME:=libsrender.a
CFLAGS:=-std=c++20 -Wall

SRC:=SRender

OBJ:=temp
BIN:=build
DEBUG:=debug
RELEASE:=release

# For native comiling
C:=g++
INCPATH:=-Ilinking/include/
LIBS:=/lib/libglfw.so.3.3 /lib/libglib-2.0.so

# For compiling windows builds on linux
#CW:=x86_64-w64-mingw32-gcc

# Dumb way to get variables specific to target
ifneq (,$(filter debug,$(MAKECMDGOALS)))
	CFLAGS:=$(CFLAGS) -O1 -DDEBUG
	OBJ:=$(OBJ)/$(DEBUG)
	BIN:=$(BIN)/$(DEBUG)
endif

ifneq (,$(filter release,$(MAKECMDGOALS)))
	CFLAGS:=$(CFLAGS) -O3 -DNDEBUG
	OBJ:=$(OBJ)/$(RELEASE)
	BIN:=$(BIN)/$(RELEASE)
endif

HEADERS:=$(wildcard $(SRC)/*.hpp)
SOURCES:=$(wildcard $(SRC)/*.cpp)
OBJECTS:=$(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SOURCES))

.PHONY: makefile all clean build debug release

# Default target simply tells you how to correctly use this makefile
.DEFAULT_GOAL:=all
all:
	@printf "debug, release, clean\n"

clean:
	rm -rf $(OBJ)/
	rm -rf $(BIN)/

# Make any needed directories (bad)
%/: ; mkdir -p $@

# Archive the file into a proper library
$(BIN)/$(NAME): $(OBJECTS) $(OBJ)/glad.o
	ar rcs $(BIN)/$(NAME) $(OBJECTS) $(OBJ)/glad.o $(LIBS)

# Compile any object files that need to be updated
$(OBJ)/%.o:: $(SRC)/%.cpp $(HEADERS)
	$(C) $(CFLAGS) -c $< -o $@ $(INCPATH)

# Compile glad
$(OBJ)/glad.o:: $(SRC)/glad.c $(HEADERS)
	$(C) -std=c17 -Wall -O3 -DNDEBUG -c $< -o $@ $(INCPATH)

build: $(OBJ)/ $(BIN)/ $(BIN)/$(NAME)

debug: build
release: build
