# ------------
# [ MAKEFILE ]
# ------------
# Author: Marin Bilan
# Date: ####
# ------------


# ---- Only things that needs to be changed ----
# Binary to generate (Full binary file (with path))
BIN_TARGET_NAME = Mod-App-2
PROJ_ROOR_DIR = ../..

# Folder does not need to be created
# Bins from all modules are in the same folder (../../bins)
BINARY = $(PROJ_ROOR_DIR)/bins/$(BIN_TARGET_NAME)

CURRENT_PROJ_DIR = .


# [1 STEP]
# If new folder for (.h) is created or new lib is included in project, add folder here
INC_DIRS = -I$(CURRENT_PROJ_DIR)/common \
           -I$(CURRENT_PROJ_DIR)/services/ServiceX/if \
           -I$(CURRENT_PROJ_DIR)/services/ServiceX/inc \
           -I$(PROJ_ROOR_DIR)/libs/boost \
#           -I$(CURRENT_PROJ_DIR)/networking/if \
#           -I$(CURRENT_PROJ_DIR)/networking/inc \

# [2 STEP]
# If new folder for source file (.cpp) is created, add folder here
CODE_DIRS = $(CURRENT_PROJ_DIR)/common \
		$(CURRENT_PROJ_DIR)/services/ServiceX/src \

# Auto detect of all dirs and subdirs
# Not used for now - This can auto include all folders from . folder
# TODO: Try this (note that libs folders needs to be included separately)
ALL_DIRS = $(shell find ./ -type d)
# ---- ----


# Compiler flags
CC = g++
# Optimization
OPT = -O0
# Generate files that encode make rules for the .h dependencies
DEP_FLAGS = -MP -MD
# Cpp Flags
CPP_FLAGS = -Wall -Wextra -g $(OPT) $(DEP_FLAGS)
# Also it is possible to add -I automatically
# TODO: Try this
# CPPFLAGS = -Wall -Wextra -g $(foreach D, $(INC_DIRS), -I$(D)) $(OPT) $(DEP_FLAGS)

# Files to compile (.cpp) and link (.o)
# Get all .cpp files from all CODE_DIRS - to compile
CPP_FILES= $(foreach D, $(CODE_DIRS), $(wildcard $(D)/*.cpp))

OBJ_FILES = $(patsubst %.cpp, %.o, $(CPP_FILES))
DEP_FILES = $(patsubst %.cpp, %.d, $(CPP_FILES))


# LIBs
# [LINKING STAGE]
# In sslLibs (libcrypto.a libcrypto.so, libssl.a libssl.so)
LINK_LIBS = -L$(PROJ_ROOR_DIR)/libs/sslLibs \
#
LIBS = -lssl -lcrypto -pthread
# [COMPILE STAGE]
INC_DIRS_LIBS = -I$(PROJ_ROOR_DIR)/libs/boost \



# ---- ==== ----
all: $(BINARY)
	@echo cpp files to compile ...
	@echo $(CPP_FILES)

# Link .o files
# Link libs
$(BINARY):$(OBJ_FILES)
	$(CC) -o $@ $^ $(LINK_LIBS) $(LIBS)

# Run multiple times - for each .o file, find .cpp file and compile
# .o files (OBJ_FILES) depend on .cpp files
# Only want the .cpp file dependency here, thus $< instead of $^
%.o:%.cpp
	$(CC) $(CPP_FLAGS) $(INC_DIRS) $(INC_DIRS_LIBS) -c -o $@ $<


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
BIN_FOLDER_NAME_UT = Mod-App-2_UT
CUR_DIR = $(shell basename $(CURDIR))_UT

# Folder for bins
TARGET_UT_FOLDER = $(PROJ_ROOR_DIR)/bins/$(BIN_FOLDER_NAME_UT)

# Create folder if does not exist
unitTestModApp1MakeDir:
	@echo Creating unit test bin directory if not exist
	mkdir -p $(TARGET_UT_FOLDER)
# ====

# [1 STEP foreach target]
# Binary file only name (*.out)
TARGET_NAME_NET_UT = Mod-App-2_Net_UT
TARGET_NAME_SRV_X_UT = Mod-App-2_Srv_X_UT
# ex: ../../bins/Mod-App-1/Mod-App-1_UT
BINARY_NETWORKING_UT = $(PROJ_ROOR_DIR)/bins/$(BIN_FOLDER_NAME_UT)/$(TARGET_NAME_NET_UT)
BINARY_SRV_X_UT = $(PROJ_ROOR_DIR)/bins/$(BIN_FOLDER_NAME_UT)/$(TARGET_NAME_SRV_X_UT)


# ----
# [2 STEP foreach target common folders to include]
# Include dirs
# Usually all dirs in module (TODO: Get all folders using shell)
INC_UT = -I $(CURRENT_PROJ_DIR)/common \
	  -I $(CURRENT_PROJ_DIR)/if \
	  -I $(CURRENT_PROJ_DIR)/inc \
      -I $(CURRENT_PROJ_DIR)/services/Networking/if \
      -I $(CURRENT_PROJ_DIR)/services/Networking/inc \
      -I $(CURRENT_PROJ_DIR)/services/ServiceX/if \
      -I $(CURRENT_PROJ_DIR)/services/ServiceX/inc \
# ----

# [3 STEP foreach target define folders with .cpp files]
CODE_DIRS_NET_UT = $(CURRENT_PROJ_DIR)/src \
				   $(CURRENT_PROJ_DIR)/services/Networking/src \
			       $(CURRENT_PROJ_DIR)/services/ServiceX/src \
			       $(CURRENT_PROJ_DIR)/services/Networking/unitTest \

CODE_DIRS_SRV_X_UT = $(CURRENT_PROJ_DIR)/services/ServiceX/src \
                     $(CURRENT_PROJ_DIR)/services/ServiceX/unitTest \


CPP_FILES_NET_UT = $(foreach D, $(CODE_DIRS_NET_UT), $(wildcard $(D)/*.cpp))
CPP_FILES_SRV_X_UT = $(foreach D, $(CODE_DIRS_SRV_X_UT), $(wildcard $(D)/*.cpp))

OBJ_FILES_UT = $(patsubst %.cpp, %.o, $(CPP_FILES_NET_UT))
OBJ_FILES_SRV_X_UT = $(patsubst %.cpp, %.o, $(CPP_FILES_SRV_X_UT))
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
unitTestNet: $(BINARY_NETWORKING_UT)
	@echo cpp files to compile ... $(CPP_FILES_NET_UT)
	@echo current dir ... $(CUR_DIR)

# Compile binary from all objects
# IMPORTANT $(LINK_LIBS_UT) $(LIBS_UT) needs to go to the end!
$(BINARY_NETWORKING_UT):$(OBJ_FILES_UT)
	$(CC) -o $@ $^ $(LINK_LIBS_UT) $(LIBS_UT) 

# Run multiple times - for each .o file, find .cpp file and compile
# .o files (OBJFILES) depend on .cpp files
# Only want the .cpp file dependency here, thus $< instead of $^
%.o:%.cpp
	$(CC) $(CPP_FLAGS_UT) $(INC_UT) $(INC_LIBS_UT) -c -o $@ $<


cleanUnitTestNet:
	@echo Clean Build
	-rm $(BINARY_NETWORKING_UT) $(OBJ_FILES_UT)
# ---- ----


# ---- SERVICE X UNIT TEST ----
unitTestSrvX: $(BINARY_SRV_X_UT)
	@echo Compile ... unitTestSrvX

# Compile binary from all objects
# IMPORTANT $(LINK_LIBS_UT) $(LIBS_UT) needs to go to the end!
$(BINARY_SRV_X_UT):$(OBJ_FILES_SRV_X_UT)
	$(CC) -o $@ $^ $(LINK_LIBS_UT) $(LIBS_UT) 

# Run multiple times - for each .o file, find .cpp file and compile
# .o files (OBJFILES) depend on .cpp files
# Only want the .cpp file dependency here, thus $< instead of $^
%.o:%.cpp
	$(CC) $(CPP_FLAGS_UT) $(INC_UT) $(INC_LIBS_UT) -c -o $@ $<


cleanUnitTestSrvX:
	@echo Clean Build
	-rm $(BINARY_SRV_X_UT) $(OBJ_FILES_SRV_X_UT)
# ---- ----


cleanAllUnitTest:
	@echo Clean all Unit Test targets
	-rm $(BINARY_NETWORKING_UT) $(OBJ_FILES_UT) $(BINARY_SRV_X_UT) $(OBJ_FILES_SRV_X_UT)

# TODO: Remove this
# Copy *.h files to the share_headers folder
#install:
#	rsync -avz *.h $(PROJ_ROOT_DIR)/share_headers

.PHONY: clean build_dir all