# MAKEFILE

NAME:=libsrender.a
CPPFLAGS:=-std=c++20 -Wall
DEBUGFLAGS:=-O1 -DDEBUG
RELEASEFLAGS:=-O3 -DNDEBUG

SRC:=SRender
OBJ:=temp
BIN:=build
DEBUG:=debug
RELEASE:=release

# For native comiling
CC:=g++
INCPATH:=-Ilinking/include/
LIBS:=/lib/libglfw.so.3.3 /lib/libglib-2.0.so /lib/libassimp.so

# For compiling windows builds on linux
#CW:=x86_64-w64-mingw32-gcc

# Dumb way to get variables specific to target
ifneq (,$(filter debug,$(MAKECMDGOALS)))
	CPPFLAGS:=$(CPPFLAGS) $(DEBUGFLAGS)
	OBJ:=$(OBJ)/$(DEBUG)
	BIN:=$(BIN)/$(DEBUG)
endif

ifneq (,$(filter release,$(MAKECMDGOALS)))
	CPPFLAGS:=$(CPPFLAGS) $(RELEASEFLAGS)
	OBJ:=$(OBJ)/$(RELEASE)
	BIN:=$(BIN)/$(RELEASE)
endif

HEADERS:=$(wildcard $(SRC)/*.hpp)
SOURCES:=$(wildcard $(SRC)/*.cpp)
OBJECTS:=$(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SOURCES))

.PHONY: makefile help clear clean build debug release example

# Default target simply tells you how to correctly use this makefile
.DEFAULT_GOAL:=help
help:
	@printf "clear: Remove bin files\nclean: Remove temp files\ndebug: Make a debug build\nrelease: Make a release build\n"

clear:
	rm -rf $(BIN)/

clean:
	rm -rf $(OBJ)/

# Archive the file into a proper library
$(BIN)/$(NAME): $(OBJECTS) $(OBJ)/glad.o
	ar rcs $(BIN)/$(NAME) $(OBJECTS) $(OBJ)/glad.o $(LIBS)

# Compile any object files that need to be updated
$(OBJ)/%.o:: $(SRC)/%.cpp $(HEADERS)
	$(CC) $(CPPFLAGS) -c $< -o $@ $(INCPATH)

# Compile glad
$(OBJ)/glad.o:: $(SRC)/glad.c $(HEADERS)
	$(CC) -std=c++20 $(RELEASEFLAGS) -c $< -o $@ $(INCPATH)

build: $(OBJ)/ $(BIN)/ $(BIN)/$(NAME)

debug: build
release: build

build/debug/example: build/debug/$(NAME)
	$(CC) -std=c++20 -Wall $(DEBUGFLAGS) Example/src/project.cpp -o $@ -ISRender/ $(INCPATH)

example: build/debug/example

# Making directories as needed
$(BIN)/: ; mkdir -p $@
$(OBJ)/: ; mkdir -p $@
