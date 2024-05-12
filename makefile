# MAKEFILE

NAME:=libsrender.a
SRC:=SRender
OBJ:=temp/gcc
BIN:=build/gcc
DEBUG:=debug
RELEASE:=release

CXX:=g++
CXXFLAGS:=-std=c++20 -Wall
DFLAGS:=-Og -DDEBUG -D_DEBUG
RFLAGS:=-O3 -DNDEBUG -D_RELEASE
INCPATH:=-Ilinking/include/
LDFLAGS:=-lglfw -lglib-2.0 -lassimp

# Dumb way to get variables specific to target
ifneq (,$(filter verbose,$(MAKECMDGOALS)))
	DFLAGS:=$(DFLAGS) -D_VERBOSE
	RFLAGS:=$(RFLAGS) -D_VERBOSE
endif
ifneq (,$(filter debug,$(MAKECMDGOALS)))
	CXXFLAGS:=$(CXXFLAGS) $(DFLAGS)
	ifeq (,$(filter clean,$(MAKECMDGOALS)))
		OBJ:=$(OBJ)/$(DEBUG)
	endif
	ifeq (,$(filter clear,$(MAKECMDGOALS)))
		BIN:=$(BIN)/$(DEBUG)
	endif
else
	CXXFLAGS:=$(CXXFLAGS) $(RFLAGS)
	ifeq (,$(filter clean,$(MAKECMDGOALS)))
		OBJ:=$(OBJ)/$(RELEASE)
	endif
	ifeq (,$(filter clear,$(MAKECMDGOALS)))
		BIN:=$(BIN)/$(RELEASE)
	endif
endif

HEADERS:=$(wildcard $(SRC)/*.hpp)
SOURCES:=$(wildcard $(SRC)/*.cpp)
OBJECTS:=$(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SOURCES))

.PHONY: makefile help clear clean debug release build example run verbose

# Default target simply tells you how to correctly use this makefile
.DEFAULT_GOAL:=help
help:
	@printf "clear: Remove bin files\n\
	clean: Remove temp files\n\
	debug: Make a debug build\n\
	release: Make a release build\n"

clear:
	rm -rf $(BIN)/

clean:
	rm -rf $(OBJ)/

# Copy assets to build
$(BIN)/assets/:
	cp -r Example/assets $(BIN)/assets

# Compile any object files that need to be updated
$(OBJ)/%.o:: $(SRC)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCPATH)

# Compile glad
$(OBJ)/glad.o:: linking/include/glad/glad.c linking/include/glad/glad.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCPATH)

# Archive the file into a proper library
$(BIN)/$(NAME): $(OBJ)/ $(BIN)/ $(OBJECTS) $(OBJ)/glad.o
	ar rcs $(BIN)/$(NAME) $(OBJECTS) $(OBJ)/glad.o

build: $(BIN)/$(NAME)
debug: build

$(OBJ)/project.o:: Example/project.cpp Example/project.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCPATH) -ISRender/

# Compile the example project using the SRender library
$(BIN)/example: $(OBJ)/ $(BIN)/ $(BIN)/$(NAME) $(OBJ)/project.o $(BIN)/assets/
	$(CXX) $(CXXFLAGS) -o $(BIN)/example $(OBJ)/project.o $(LDFLAGS) $(BIN)/$(NAME)

example: $(BIN)/example

# Making directories as needed
$(BIN)/:
	mkdir -p $@
$(OBJ)/:
	mkdir -p $@

run:
	$(BIN)/example

# Dummy command
verbose:
	@printf ""
