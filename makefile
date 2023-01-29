# Current issues
# Compiling source is SRender/src/*.cpp or *.hpp
# Compiling imgui is SRender/imgui/*.cpp or *.h
# Resulting file is libsrender.a

NAME:=libsrender.a
CFLAGS:=-std=c++20 -Wall

SRC:=SRender/src

OBJ:=temp
BIN:=build
DEBUG:=debug
RELEASE:=release
WINDOWS:=windows

# For native comiling
C:=gcc
INCPATH:=-Ilinking/include/
LIBPATH:=-Llinking/lib
LIBS:=-lassimp.lib -lglade.lib -lglfw3.lib

# For compiling windows builds on linux
CW:=x86_64-w64-mingw32-gcc
INCPATHW:=-Ilinking/include/
LIBPATHW:=-Llinking/lib
LIBSW:=-lassimp.lib -lglade.lib -lglfw3.lib

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

ifneq (,$(filter debugw,$(MAKECMDGOALS)))
	CFLAGS:=$(CFLAGS) -O1 -DDEBUG
	OBJ:=$(WINDOWS)/$(OBJ)/$(DEBUG)
	BIN:=$(WINDOWS)/$(BIN)/$(DEBUG)

	C:=$(CW)
	INCPATH:=$(INCPATHW)
	LIBPATH:=$(LIBPATHW)
	LIBS:=$(LIBSW)
endif

ifneq (,$(filter releasew,$(MAKECMDGOALS)))
	CFLAGS:=$(CFLAGS) -O3 -DNDEBUG
	OBJ:=$(WINDOWS)/$(OBJ)/$(RELEASE)
	BIN:=$(WINDOWS)/$(BIN)/$(RELEASE)

	C:=$(CW)
	INCPATH:=$(INCPATHW)
	LIBPATH:=$(LIBPATHW)
	LIBS:=$(LIBSW)
endif

HEADERS:=$(wildcard $(SRC)/*.hpp) $(wildcard $(IMGUI)/*.h)
SOURCES:=$(wildcard $(SRC)/*.cpp)
OBJECTS:=$(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SOURCES))

.PHONY: makefile all clean build debug release debugw releasew

# Default target simply tells you how to correctly use this makefile
.DEFAULT_GOAL:=all
all:
	@printf "debug, release, debugw, releasew, clean\n"

clean:
	rm -rf $(OBJ)/
	rm -rf $(BIN)/

# Make any needed directories (bad)
%/: ; mkdir -p $@

# Archive the file into a proper library
$(BIN)/$(NAME): $(OBJECTS)
	ar rcs $(BIN)/$(NAME) $(OBJECTS)

# Compile any object files that need to be updated
$(OBJ)/%.o:: $(SRC)/%.cpp $(HEADERS)
	$(C) $(CFLAGS) -c $< -o $@ $(INCPATH) $(LIBPATH) $(LIBS)

build: $(OBJ)/ $(BIN)/ $(BIN)/$(NAME)

debug: build
release: build
debugw: build
releasew: build
