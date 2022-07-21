# ***********************************
# SAMPLE MAKEFILE FOR TARGET - BINARY
# # NOTES:
# We may have to modify attributes
# ***********************************

# This is lazy initialisation, because until
# we use it, it will not be validated.
BUILD_TYPE = $(RELEASE)
# BUILD_TYPE = $(DEBUG)

# Folder name
MODULE_NAME = Mod-Template-1
# Binary file only name (a.out)
TARGET_NAME = Mod-Template-1

PROJ_ROOR_DIR = ../..
# Inside obj folder will be module folder
OBJ_DIR = $(PROJ_ROOR_DIR)/objs/$(MODULE_NAME)



# ******** DO NOT MODIFY BELOW ATTRIBUTES ******** #
BUILD_TIMESTAMP = $(shell date + '%d-%m-%Y.%H-%M-%S.%N')


INSTALLATION_PATH = $(shell echo $$INSTALLATION_PATH)
# If nothing is set in terminal, (in if - after command is nothing)
# assume instalation path is out proj root dir
ifeq ($(INSTALLATION_PATH),)
	INSTALLATION_PATH = $(PROJ_ROOR_DIR)
#	INSTALLATION_PATH = /usr/local/mycustombuild
endif

TARGET_DIR = $(INSTALLATION_PATH)/bins
# Full binary file (with path)
# ex: ../../bins/Mod-Template-1
TARGET = $(TARGET_DIR)/$(TARGET_NAME)
LIBRARY_DIR = $(INSTALLATION_PATH)/libs


# C++ compiler
CXX = $(shell echo $$CXX)
ifeq ($(CXX),)
	CXX = g++
endif

# C++ linker
LDXX = $(shell echo $$LDXX)
ifeq ($(LDXX),)
	LDXX = g++
endif

STDFLAG = -std=c++17
# ******** DO NOT MODIFY ABOVE ATTRIBUTES ******** #


# STEP 1: If new file is created - add it here (.o) and (.cpp)
MAIN_OBJ = $(OBJ_DIR)/main.o

# SOURCE_1 = $(MODULE_NAME)
# OBJ_1 = $(OBJ_DIR)/$(SOURCE_1).o

# WE MAY NEED TO ADD MORE DEPENDENCIES AS PER REQUIREMENT.\
1. DO NOT ADD EXTENSION C/CPP AS IT MAKES EASIER TO CREATE .o FILE \
2. ADD *.o IN ALL_OBJS. \
3. ADD INSTRUCTION TO BUILD TARGET (.o)

# SOURCE_2 =
# OBJ_2 = $(OBJ_DIR)/$(SOURCE_2).o
# SOURCE_3 = 
# ...


# STEP 2: Add obj var here
# Add OBJ_* in the begining if required
ALL_OBJS = $(MAIN_OBJ) $(OBJ_1)


# STEP 3: If new folder is added, include here
# Include dirs
INC = -I./ -I$(PROJ_ROOT_DIR)/share_headers


# Choose release/debug
DEBUG = -pipe -g -Wall -W -fPIC
#RELEASE = -DNDEBUG -W -fPIC
RELEASE = -pipe -O3 -Wall -W -fPIC

# -D stands for DEFINE. If want to define any\
macro which is used in code for timestamp or git revision...
DEFINES = -DBUILD_TIMESTAMP_STR=\"$(BUILD_TIMESTAMP)\" \
          -DEINSTALLATION_PATH_STR=\"$(INSTALLATION_PATH)\"
#LD_OPT = -W1, -O1

#UNCOMENT IF LIKE TO SEE FOLLOWING WARNINGS. AT LEAST ONCE NEEDS TO BE \
RUN FOR EACH MODULE
WARN = -Wall -Wextra -Werror -Wwrite-strings -Wno-parentheses -pedantic \
       -Warray-bounds -Wno-unused-variable -Wno-unused-function \
       -Wno-unused-parameter -Wno-unused-result

CCFLAGS = $(STDFLAG) $(BUILD_TYPE) $(DEFINES) $(WARN) $(INC)

# Add dependency library here by separating them with space. Below example is \
to add libzmq, libm and libcustomlib

# Note: Add the "<path-to-library> -llibraryname" by keeping space between in as \
shown below. If library is present in the current directory, we can use path al L.

# DEP_LIBS = -lzmq -lm -L$(LIBRARY_DIR) -lcustomlib -L. -lnewlibincurrentdir

# RPATH IS USED FOR LINKING LIBS IN SPECIFIC PATH
# RPATH="-W1,-rpath,$(LIBRARY_DIR)/$(THIRD_PARTY_LIB_DIR)"
# In below path - linker will search for our libs
RPATH="-Wl,-rpath,$(LIBRARY_DIR)"

#LDFLAGS = $(DEBUG) $(PROF) -L$(LIBRARY_DIR) -L$(THIRD_PARTY_LIB_DIR) -fPIC \
-lpthread $(DEP_LIBS) $(RPATH)
LDFLAGS = $(DEP_LIBS) $(RPATH)


# Default make target ($ make)
# TARGET is full name (with path)
all: $(TARGET)

# Always the same
# ALL_OBJS - $^ (Multiple objs files)
# Gather and link all obj (.o) files
$(TARGET): $(ALL_OBJS)
	$(LDXX) $(LD_OPT) -o $@ $^ $(LDFLAGS)

# This below part is changing depend. what are we build
# $< will compile (to .o file) only one (main.cpp) file
$(MAIN_OBJ): main.cpp
	$(CXX) $(CCFLAGS) -o $@ -c $<

#$(OBJ_1): $(SOURCE_1).cpp
#	$(CXX) $(CCFLAGS) -o $@ -c $<

#$(OBJ_2): $(SOURCE_2).cpp
#	$(CXX) $(CCFLAGS) -o $@ -c $<


build_dir:
	@echo Creating object directory if not exist
	mkdir -p $(OBJ_DIR)

# Copy *.h files to the share_headers folder
inslall:
	rsync -avz *.h $(PROJ_ROOT_DIR)/share_headers

clean:
	@echo Clean Build
	-rm $(ALL_OBJS)
	-rm $(TARGET)

.PHONY: clean build_dir all