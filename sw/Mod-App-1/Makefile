# ------------
# [ MAKEFILE ]
# ------------
# Author: Marin Bilan
# Date: ####
# ------------

# Targets
# [ Module ] 
# make all
# make clean

# UNIT TESTs

# [ Networking ]
# make unitTestNet
# make cleanUnitTestNet

# [ ServiceX ]
# make unitTestSrvX
# make cleanUnitTestSrvX


# ---- Only things that need to be changed ----
# Binary to generate (Full binary file (with path))
# Bin target name is the same as Mod folder name (BIN_TARGET_NAME = Mod-App-X)
BIN_TARGET_NAME = $(shell basename $(CURDIR))
PROJ_ROOR_DIR = ../..

# Folder (for Mod) does not need to be created
# Bins from all modules are in the same folder (../../bins)
BINARY = $(PROJ_ROOR_DIR)/bins/$(BIN_TARGET_NAME)

CURRENT_PROJ_DIR = .

# [1 STEP]
# If new folder for (.h) is created add folder here
# Folders are added automatically - skip 1st step
#INC_DIRS = -I$(CURRENT_PROJ_DIR)/common \
           -I$(CURRENT_PROJ_DIR)/services/ServiceX/if \
           -I$(CURRENT_PROJ_DIR)/services/ServiceX/inc \
#           ...

# Auto detect of all dirs and subdirs
ALL_DIRS = $(shell find ./ -type d)

# Add prefix -I
ALL_INC_DIRS = $(foreach D, $(ALL_DIRS), -I$(D)) 


# ----
# [2 STEP]
# If new folder with source file (.cpp) is created, add folder here
CODE_DIRS = $(CURRENT_PROJ_DIR)/common \
		$(CURRENT_PROJ_DIR)/services/HTTPSProxy/src \
		$(CURRENT_PROJ_DIR)/services/Networking/src \
		$(CURRENT_PROJ_DIR)/services/ServiceX/src \
		$(CURRENT_PROJ_DIR)/services/Container/src \

# Files to compile (.cpp) and link (.o)
# Get all .cpp files from all CODE_DIRS - to compile
CPP_FILES= $(foreach D, $(CODE_DIRS), $(wildcard $(D)/*.cpp))

OBJ_FILES = $(patsubst %.cpp, %.o, $(CPP_FILES))
DEP_FILES = $(patsubst %.cpp, %.d, $(CPP_FILES))
# ----


# Compiler flags
CC = g++
# Optimization
OPT = -O0
# Generate files that encode make rules for the .h dependencies
DEP_FLAGS = -MP -MD
# Cpp Flags
CPP_FLAGS = -Wall -Wextra -g $(OPT) $(DEP_FLAGS)
# CPPFLAGS = -Wall -Wextra -g $(foreach D, $(INC_DIRS), -I$(D)) $(OPT) $(DEP_FLAGS)


# ----
# [3 STEP]
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



# --------------
# [ UNIT TESTs ]
# --------------

# COMMON STUFF FOR ALL UNIT TEST MODULES
# FLAGS
CPP_FLAGS_UT = -Wall -Wextra -g

# Inside obj folder will be MODULE FOLDER
# OBJ_DIR_UT = $(PROJ_ROOR_DIR)/objs/$(MODULE_NAME)

# BIN_FOLDER_NAME_UT = Mod-App-X_UT
BIN_FOLDER_NAME_UT = $(shell basename $(CURDIR))_UT

# Folder for bins
TARGET_UT_FOLDER = $(PROJ_ROOR_DIR)/bins/$(BIN_FOLDER_NAME_UT)

# Create folder if does not exist
unitTestModApp1MakeDir:
	@echo Creating unit test bin directory if not exist
	mkdir -p $(TARGET_UT_FOLDER)
# ====

# [1 STEP foreach target]
# If you are adding new Unit Test
# Binary file only name (*.out)
TARGET_NAME_UT_NET = $(BIN_FOLDER_NAME_UT)_Net
TARGET_NAME_UT_SRV_X = $(BIN_FOLDER_NAME_UT)_Srv_X
# ex: ../../bins/Mod-App-1/Mod-App-1_UT
BINARY_UT_NETWORKING = $(PROJ_ROOR_DIR)/bins/$(BIN_FOLDER_NAME_UT)/$(TARGET_NAME_UT_NET)
BINARY_UT_SRV_X = $(PROJ_ROOR_DIR)/bins/$(BIN_FOLDER_NAME_UT)/$(TARGET_NAME_UT_SRV_X)


# ----
# [2 STEP foreach target common folders to include]

# ALL_INC_DIRS - Already defined above
# ----


# [3 STEP foreach target define folders with .cpp files]
CODE_DIRS_UT_NET = $(CURRENT_PROJ_DIR)/services/HTTPSProxy/src \
				   $(CURRENT_PROJ_DIR)/services/Networking/src \
				   $(CURRENT_PROJ_DIR)/services/Networking/unitTest \
#			       $(CURRENT_PROJ_DIR)/services/ServiceX/src \
			       
CODE_DIRS_UT_SRV_X = $(CURRENT_PROJ_DIR)/services/ServiceX/src \
                     $(CURRENT_PROJ_DIR)/services/ServiceX/unitTest \


CPP_FILES_UT_NET = $(foreach D, $(CODE_DIRS_UT_NET), $(wildcard $(D)/*.cpp))
CPP_FILES_UT_SRV_X = $(foreach D, $(CODE_DIRS_UT_SRV_X), $(wildcard $(D)/*.cpp))

OBJ_FILES_UT = $(patsubst %.cpp, %.o, $(CPP_FILES_UT_NET))
OBJ_FILES_UT_SRV_X = $(patsubst %.cpp, %.o, $(CPP_FILES_UT_SRV_X))
# DEP_FILES = $(patsubst %.cpp, %.d, $(CPP_FILES))


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



# Create different targets for different unit tests inside module
# ---- NETWORKING UNIT TEST ----
unitTestNet: $(BINARY_UT_NETWORKING)
	@echo cpp files to compile ... $(CPP_FILES_NET_UT)

# Compile binary from all objects
# IMPORTANT $(LINK_LIBS_UT) $(LIBS_UT) needs to go to the end!
$(BINARY_UT_NETWORKING):$(OBJ_FILES_UT)
	$(CC) -o $@ $^ $(LINK_LIBS_UT) $(LIBS_UT) 

# Run multiple times - for each .o file, find .cpp file and compile
# .o files (OBJFILES) depend on .cpp files
# Only want the .cpp file dependency here, thus $< instead of $^
%.o:%.cpp
	$(CC) $(CPP_FLAGS_UT) $(ALL_INC_DIRS) $(INC_LIBS_UT) -c -o $@ $<


cleanUnitTestNet:
	@echo Clean Build
	-rm $(BINARY_UT_NETWORKING) $(OBJ_FILES_UT)
# ---- ----


# ---- SERVICE X UNIT TEST ----
unitTestSrvX: $(BINARY_UT_SRV_X)
	@echo Compile ... unitTestSrvX

# Compile binary from all objects
# IMPORTANT $(LINK_LIBS_UT) $(LIBS_UT) needs to go to the end!
$(BINARY_UT_SRV_X):$(OBJ_FILES_UT_SRV_X)
	$(CC) -o $@ $^ $(LINK_LIBS_UT) $(LIBS_UT) 

# Run multiple times - for each .o file, find .cpp file and compile
# .o files (OBJFILES) depend on .cpp files
# Only want the .cpp file dependency here, thus $< instead of $^
%.o:%.cpp
	$(CC) $(CPP_FLAGS_UT) $(ALL_INC_DIRS) $(INC_LIBS_UT) -c -o $@ $<


cleanUnitTestSrvX:
	@echo Clean Build
	-rm $(BINARY_UT_SRV_X) $(OBJ_FILES_UT_SRV_X)
# ---- ----


cleanAllUnitTest:
	@echo Clean all Unit Test targets
	-rm $(BINARY_NETWORKING_UT) $(OBJ_FILES_UT) $(BINARY_UT_SRV_X) $(OBJ_FILES_UT_SRV_X)

# TODO: Remove this
# Copy *.h files to the share_headers folder
#install:
#	rsync -avz *.h $(PROJ_ROOT_DIR)/share_headers

.PHONY: clean build_dir all