# ********************************
# MAKEFILE FOR TARGET - BINARY
# NOTES:
# We may have to modify attributes
# ********************************

$(info --==Start Makefile==--)


# ----==== BUILD SYSTEM MOD-7====----
TARGET_DIR=../../bins
TARGET_NAME=Mod-7

# Only lines to change
# BINARY=bin
BINARY=$(TARGET_DIR)/$(TARGET_NAME)
# src lib ...
CODEDIRS=. ./lib \
		   ./src

INCDIRS=. ./include/ \
          ./if \
          ./inc

# COMPILER
CC=g++
OPT=-O0
# Generate files that encode make rules for .h depend.
DEPFLAGS=-MP -MD

# LINKER
# Auto add the -I onto each include directory
# -I. -I./include/
CFLAGS=-Wall -Wextra -g $(foreach D,$(INCDIRS), -I$(D)) $(OPT) $(DEPFLAGS)

# Files to Compile
# CFILES=main.cpp comp0.cpp
# Get all files from CODEDIRS with extension .cpp and assign to CFILES
# ex: ./x.cpp ./lib/main.cpp - Names with full path
CFILES=$(foreach D,$(CODEDIRS),$(wildcard $(D)/*.cpp))

# regex replacement
#OBJECTS=main.o comp0.o
# For each .cpp file convert that file in .o file
OBJECTS=$(patsubst %.cpp,%.o,$(CFILES))
# .d files will make sure that if we change .h file - recompile
# where it is included
DEPFILES=$(patsubst %.cpp,%.d,$(CFILES))

# In this step we prepared naked .cpp and .o files
# and we are ready for compiling

# -- Output --
# In order to execute command - bins are needed
ALL: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

# Apply on any single .cpp file and create .o file
# $@ - whatever on left side (%.o)
# $^ - whatever on right side (%.cpp)
# Because of issue related to multiple .cpp files to .o file
# Make sure you are doing $< instead of $^ !! (Check this)
%.o:%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
# -- == --


clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

# Combine shell with Makefile
# @ silences the printing of the command
# $(info ...) prints output
diff:
	$(info The status of repo, and the volume of per-file changes:)
	@git status
	@git diff --stat

# Include dependencies (Check more on this)
-include $(DEPFILES)