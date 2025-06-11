# ------------
# [ MAKEFILE ]
# ------------
# Author: Marin Bilan
# Date: 12.9.2024
# ------------


# Compiler flags
# CC = g++ -std=c++2a
CC = g++ -std=c++23

# Optimization
OPT = -O0
# Generate files that encode make rules for the .h dependencies
DEP_FLAGS = -MP -MD
# Cpp Flags
CPP_FLAGS = -Wall -Wextra -g $(OPT) $(DEP_FLAGS)

# ---- Unit Test flags
# -g with informations for Valgrind
CPP_FLAGS_UT = g++ -std=c++23 -Wall -Wextra -g 




# ---- Only things that need to be changed ----
# Binary to generate (Full binary file (with path))
# Bin target name is the same as Mod folder name (BIN_TARGET_NAME = Mod-SandBox0)
BIN_TARGET_NAME = $(shell basename $(CURDIR))
# BlackCat
PROJ_ROOR_DIR = ../..

# Folder (for Mod) does not need to be created
# Bins from all modules are in the same folder (../../bins)
BINARY = $(PROJ_ROOR_DIR)/bins/$(BIN_TARGET_NAME)

CURRENT_PROJ_DIR = .

# [1 STEP]
# Folders are added automatically - skip 1st step

# Auto detect of all dirs and subdirs
ALL_DIRS = $(shell find ./ -type d)

# Add prefix -I
ALL_INC_DIRS = $(foreach D, $(ALL_DIRS), -I$(D))


# ----
# [[2 STEP]]
# If new folder with source file (.cpp) is created, add folder here
CODE_DIRS = $(CURRENT_PROJ_DIR)/common \
		$(CURRENT_PROJ_DIR)/services/Tutorial_UDEMY_Cpp_Algos_0/src \

# Files to compile (.cpp) and link (.o)
# Get all .cpp files from all CODE_DIRS - to compile
CPP_FILES= $(foreach D, $(CODE_DIRS), $(wildcard $(D)/*.cpp))

OBJ_FILES = $(patsubst %.cpp, %.o, $(CPP_FILES))
DEP_FILES = $(patsubst %.cpp, %.d, $(CPP_FILES))
# ----




# ----
# [[3 STEP]]
# If new lib is added:
# Link (-L), add lib (-lLIBNAME) and include lib folder

# Common LIBs (Networking... )
# Link lib [LINKING STAGE]
# In sslLibs (libcrypto.a libcrypto.so, libssl.a libssl.so)
LINK_LIBS = -L$(PROJ_ROOR_DIR)/libs/sslLibs \
# ADD LIB NAME [LINKING STAGE]
LIBS = -lssl -lcrypto -pthread
# Include lib folder [COMPILE STAGE]
INC_DIRS_LIBS = -I$(PROJ_ROOR_DIR)/libs/boost \
# ----


# Do nothing - auto build
# ---- ==== ----
all: $(BINARY)
	@echo cpp files to compile ...
	@echo $(CPP_FILES)
	@echo include dirs ...
	@echo $(ALL_INC_DIRS)

# Link .o files
# Link libs
$(BINARY):$(OBJ_FILES)
	$(CC) -o $@ $^ $(LINK_LIBS) $(LIBS)

# Run multiple times - for each .o file, find .cpp file and compile
# .o files (OBJ_FILES) depend on .cpp files
# Only want the .cpp file dependency here, thus $< instead of $^
%.o:%.cpp
	$(CC) $(CPP_FLAGS) $(ALL_INC_DIRS) $(INC_DIRS_LIBS) -c -o $@ $<


clean:
	rm -rf $(BINARY) $(OBJ_FILES) $(DEP_FILES)
# ---- ==== ----







# [ UNIT TESTs ]


# BIN_FOLDER_NAME_UT = Mod-SandBox0_UT
BIN_FOLDER_NAME_UT = $(shell basename $(CURDIR))_UT

# Folder for bins
TARGET_UT_FOLDER = $(PROJ_ROOR_DIR)/bins/$(BIN_FOLDER_NAME_UT)

# Create folder if does not exist
unitTestModMakeDir:
	@echo Creating unit test bin directory if not exist
	mkdir -p $(TARGET_UT_FOLDER)
# ====


# UT common LIBs (Networking... )
# --------
# LINKING
# In sslLibs (libcrypto.a libcrypto.so, libssl.a libssl.so)
# In googlemocklibs (libgmock.a libgmock_main.a libgtest.a libgtest_main.a)
LINK_LIBS_UT = -L$(PROJ_ROOR_DIR)/libs/sslLibs \
		   -L$(PROJ_ROOR_DIR)/libs/unitTest/googlemocklibs
# *.a libs - order is important
#         SSL            UNITTEST
LIBS_UT = -lssl -lcrypto -lgtest -lgtest_main -lgmock -lgmock_main -pthread
# COMPILE
INC_LIBS_UT = -I $(PROJ_ROOR_DIR)/libs/boost \
	          -I $(PROJ_ROOR_DIR)/libs/unitTest/googlemock/include \
              -I $(PROJ_ROOR_DIR)/libs/unitTest/googletest/include
# --------


#  BIN_FOLDER_NAME_UT = Mod-SandBox0_UT
TARGET_NAME_UT_TUT = $(BIN_FOLDER_NAME_UT)_CppMemMng_PatriceRoyUTbin
# ../bins/Mod-SandBox0_UT/Mod-SandBox0_UT_CppMemMng_PatriceRoyUTbin 
BINARY_UT_SRV_TUT = $(PROJ_ROOR_DIR)/bins/$(BIN_FOLDER_NAME_UT)/$(TARGET_NAME_UT_TUT)

# IMOPRTANT: add -I for include folders
# inc Folders (can be added if folder also)
INC_TUT_DIRS = -I$(CURRENT_PROJ_DIR)/services/CppMemMng_PatriceRoy/inc \

# src Folders
CODE_DIRS_UT_SRV_TUT = $(CURRENT_PROJ_DIR)/services/CppMemMng_PatriceRoy/src \
					   $(CURRENT_PROJ_DIR)/services/CppMemMng_PatriceRoy/unitTest \
# .cpp
CPP_FILES_UT_SRV_TUT = services/CppMemMng_PatriceRoy/src/CppMemMng_PatriceRoy0.cpp \
					   services/CppMemMng_PatriceRoy/unitTest/CppMemMng_PatriceRoyUnitTest.cpp \
# .o
OBJ_FILES_UT_SRV_TUT = services/CppMemMng_PatriceRoy/src/CppMemMng_PatriceRoy0.o \
					   services/CppMemMng_PatriceRoy/unitTest/CppMemMng_PatriceRoyUnitTest.o


unitTestTut: $(BINARY_UT_SRV_TUT)
	@echo Compile ... unitTestTut

# Compile binary from all objects
# IMPORTANT $(LINK_LIBS_UT) $(LIBS_UT) needs to go to the end!
$(BINARY_UT_SRV_TUT):$(OBJ_FILES_UT_SRV_TUT)
	$(CC) -o $@ $^ $(LINK_LIBS_UT) $(LIBS_UT) 

# Run multiple times - for each .o file, find .cpp file and compile
# .o files (OBJFILES) depend on .cpp files
# Only want the .cpp file dependency here, thus $< instead of $^
%.o:%.cpp
	$(CPP_FLAGS_UT) $(INC_TUT_DIRS) $(INC_LIBS_UT) -c -o $@ $<


cleanUnitTestTut:
	@echo Clean Build
	-rm $(BINARY_UT_SRV_TUT) $(OBJ_FILES_UT_SRV_TUT)
# ---- ----


.PHONY: clean build_dir all
