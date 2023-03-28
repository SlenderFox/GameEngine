# MAKEFILE

NAME:=libsrender.a
CXXFLAGS:=-std=c++20 -Wall
DFLAGS:=-O1 -DDEBUG -D_DEBUG
RFLAGS:=-O3 -DNDEBUG -D_RELEASE

SRC:=SRender
OBJ:=temp
BIN:=build
DEBUG:=debug
RELEASE:=release

# For native comiling
CXX:=g++
INCPATH:=-Ilinking/include/
LIBS:=/lib/libglfw.so.3.3 /lib/libglib-2.0.so /lib/libassimp.so

# For compiling windows builds on linux
#CW:=x86_64-w64-mingw32-gcc

# Dumb way to get variables specific to target
ifneq (,$(filter debug,$(MAKECMDGOALS)))
	CXXFLAGS:=$(CXXFLAGS) $(DFLAGS)
	OBJ:=$(OBJ)/$(DEBUG)
	BIN:=$(BIN)/$(DEBUG)
else
	CXXFLAGS:=$(CXXFLAGS) $(RFLAGS)
	OBJ:=$(OBJ)/$(RELEASE)
	BIN:=$(BIN)/$(RELEASE)
endif

HEADERS:=$(wildcard $(SRC)/*.hpp)
SOURCES:=$(wildcard $(SRC)/*.cpp)
OBJECTS:=$(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SOURCES))

.PHONY: makefile help clear clean debug release build example

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
	ar rcs $(BIN)/$(NAME) $(OBJECTS) $(OBJ)/glad.o $(LIBS)

#build: $(OBJ)/ $(BIN)/ $(OBJECTS) $(OBJ)/glad.o $(BIN)/assets/
#	$(CXX) $(CXXFLAGS) -o $(BIN)/$(NAME) $(OBJECTS) $(OBJ)/glad.o $(LIBS)

example: $(OBJ)/ $(BIN)/ $(BIN)/$(NAME) $(BIN)/assets/
	$(CXX) $(CXXFLAGS) -o $(BIN)/example $(OBJ)/glad.o $(LIBS) $(BIN)/$(NAME)

build: $(BIN)/$(NAME)
debug: build

# Making directories as needed
$(BIN)/: ; mkdir -p $@
$(OBJ)/: ; mkdir -p $@
