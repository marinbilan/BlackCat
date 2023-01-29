# ------------
# [ MAKEFILE ]
# ------------
# Author: Marin Bilan
# Date: ####
# ------------


# ---- Only things that needs to be changed ----
# Binary to generate (Full binary file (with path))
BIN_TARGET_NAME = Mod-App-1
BINARY = ../../bins/$(BIN_TARGET_NAME)

CODEDIRS = ./common

INC_PATH = .
COMMON = common
NET = networking
INCDIRS = -I $(INC_PATH)/$(COMMON) \
          -I $(INC_PATH)/$(NET)/if \
          -I $(INC_PATH)/$(NET)/inc
# ---- ----


# Compiler
CC = g++
# Optimization
OPT = -O0
# Generate files that encode make rules for the .h dependencies
DEPFLAGS = -MP -MD
# Flags
CPPFLAGS = -Wall -Wextra -g $(INCDIRS) $(OPT) $(DEPFLAGS)
# Also it is possible to add -I automatically
# CPPFLAGS = -Wall -Wextra -g $(foreach D, $(INCDIRS), -I$(D)) $(OPT) $(DEPFLAGS)

# Files to compile (.cpp) and link (.o)
# Get all .cpp files from all CODEDIRS - to compile
CPPFILES= $(foreach D, $(CODEDIRS), $(wildcard $(D)/*.cpp))

OBJFILES = $(patsubst %.cpp, %.o, $(CPPFILES))
DEPFILES = $(patsubst %.cpp, %.d, $(CPPFILES))


all: $(BINARY)
	@echo cpp files to compile ...
	@echo $(CPPFILES)

# Compile binary from all objects
$(BINARY):$(OBJFILES)
	$(CC) -o $@ $^

# Run multiple times - for each .o file, find .cpp file and compile
# .o files (OBJFILES) depend on .cpp files
# Only want the .cpp file dependency here, thus $< instead of $^
%.o:%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<


clean:
	rm -rf $(BINARY) $(OBJFILES) $(DEPFILES)




# --------------
# [ UNIT TESTs ]
# --------------

# //// UNIT TEST NETWORKING ////
PROJ_ROOR_DIR_UT = ../..

# Folder name
BIN_FOLDER_NAME_UT = Mod-App-1_UT
# Binary file only name (*.out)
TARGET_NAME_UT = Mod-App-1_UT

# OUTPUT
# Full binary file (with path)
# ex: ../../bins/Mod-App-1/Mod-App-1_UT
TARGET_UT = $(PROJ_ROOR_DIR_UT)/bins/$(BIN_FOLDER_NAME_UT)/$(TARGET_NAME_UT)
# ----

# ----
# Inside obj folder will be module folder
# OBJ_DIR_UT = $(PROJ_ROOR_DIR)/objs/$(MODULE_NAME)
TARGET_UT_DIR = $(PROJ_ROOR_DIR_UT)/bins/$(BIN_FOLDER_NAME_UT)

# //// FULL SUBMODULE DIRs CREATION ////
build_utBinDir:
	@echo Creating unit test bin directory if not exist
	mkdir -p $(TARGET_UT_DIR)
# ----



# INPUT BUILD FILEs
UNITTEST_NETWORK_IN = networking/unitTest/unitTestNetworking.cpp \
		              src/HTTPSProxySrv.cpp \
	                  networking/src/HTTPSClient.cpp


INC_PATH = ./
# STEP 3: Include dirs - if new dir for (.h) file is created
INC = -I $(INC_PATH) \
      -I $(PROJ_ROOT_DIR)/share_headers \
      -I $(INC_PATH)if \
      -I $(INC_PATH)inc \
      -I $(INC_PATH)common \
      -I $(INC_PATH)services/if \
      -I $(INC_PATH)services/inc \
      -I $(INC_PATH)services/src \
      -I $(INC_PATH)networking/if \
      -I $(INC_PATH)networking/inc \
      -I $(PROJ_ROOR_DIR_UT)/libs/boost \
      -I $(PROJ_ROOR_DIR_UT)/libs/unitTest/googlemock/include \
      -I $(PROJ_ROOR_DIR_UT)/libs/unitTest/googletest/include



# ----

CXXFLAGS_UT = -g -no-pie -std=c++17

# LIBS
# In googlemocklibs (libgmock.a libgmock_main.a libgtest.a libgtest_main.a)
LINK_LIBS_UT = -L$(PROJ_ROOR_DIR_UT)/libs/sslLibs \
		       -L$(PROJ_ROOR_DIR_UT)/libs/unitTest/googlemocklibs

# *.a libs - order is important
#         SSL            UNITTEST
LIBS_UT = -lssl -lcrypto -lgtest -lgtest_main -lgmock -lgmock_main -pthread


unitTestNet:
	g++ $(CXXFLAGS_UT) \
	$(UNITTEST_NETWORK_IN) \
	-o $(TARGET_UT) \
	$(INC) \
	$(LINK_LIBS_UT) \
	$(LIBS_UT)



# Copy *.h files to the share_headers folder
inslall:
	rsync -avz *.h $(PROJ_ROOT_DIR)/share_headers

cleanUT:
	@echo Clean Build
	-rm $(TARGET_UT)


.PHONY: clean build_dir all