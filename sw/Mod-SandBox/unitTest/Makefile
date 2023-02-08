# +---------------+
# | [ UNIT TEST ] |
# +---------------+

# 
TARGET_NAME_UT = Mod-6-UT

#
PROJ_ROOR_DIR_UT = .
# ----

# INCLUDEs
INC_PATH = ../
PROJ_ROOR_DIR_UT_LIBS = ../../..
# STEP 3: Include dirs - if new dir for (.h) file is created
INC_DIRs = -I $(INC_PATH)if \
           -I $(INC_PATH)inc \
           -I $(PROJ_ROOR_DIR_UT_LIBS)/libs/unitTest/googlemock/include \
           -I $(PROJ_ROOR_DIR_UT_LIBS)/libs/unitTest/googletest/include

# INPUT
UT_COMPILE = ../src/Service0.cpp \
		     unitTest_Mod-6_0.cpp \

# OUTPUT
# Full binary file (with path)
TARGET_UT = $(PROJ_ROOR_DIR_UT)/bins/$(TARGET_NAME_UT)
# ----

CXXFLAGS_UT = -g -no-pie -std=c++17

# LIBS
# In googlemocklibs (libgmock.a libgmock_main.a libgtest.a libgtest_main.a)
# -L to include lib folders
LINK_LIBS_UT = -L$(PROJ_ROOR_DIR_UT_LIBS)/libs/sslLibs \
		       -L$(PROJ_ROOR_DIR_UT_LIBS)/libs/unitTest/googlemocklibs

# *.a libs - order is important
#         SSL            UNITTEST
LIBS_UT = -lssl -lcrypto -lgtest -lgtest_main -lgmock -lgmock_main -pthread


ut:
	g++ $(CXXFLAGS_UT) \
	$(UT_COMPILE) \
	-o $(TARGET_UT) \
	$(INC_DIRs) \
	$(LINK_LIBS_UT) \
	$(LIBS_UT)