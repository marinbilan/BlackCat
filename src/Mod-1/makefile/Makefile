# Makefile
CXXFLAGS = -g -no-pie -std=c++17


includeDirs = -I ../if \
              -I ../inc \
              -I ../mock \

# 1 STEP: Add preReq
allPreReq = bcMain.o \
			Service0.o \
			Service1.o \

OUTPUT = ../../../bins/blackCatBin

# target: prerequisites
#	recepies
blackCatBin: $(allPreReq)
	g++ -pthread $(CXXFLAGS) $(allPreReq) -o $(OUTPUT)

# 2 STEP: Add recepie
bcMain.o: ../src/bcMain.cpp
	g++ -c $(CXXFLAGS) $(includeDirs) ../src/bcMain.cpp

Service0.o: ../src/Service0.cpp
	g++ -c $(CXXFLAGS) $(includeDirs) ../src/Service0.cpp

Service1.o: ../src/Service1.cpp
	g++ -c $(CXXFLAGS) $(includeDirs) ../src/Service1.cpp


# UnitTest
includeUTDirs = -I ../../../libs/googlemock/include \
				-I ../../../libs/googletest/include \

linkUTLibs = -L ../../../libs/googlemocklibs

# 3 STEP: Add UnitTest if needed
UNITTESTIN = ../unitTest/unitTest.cpp \
			 ../src/Service0.cpp \
			 ../src/Service1.cpp \

UNITTESTOUT = ../../../bins/unitTestBin

gTestFlags = -lgtest -lgtest_main -lgmock -lgmock_main -pthread

unitTest:
	g++ $(UNITTESTIN) -o $(UNITTESTOUT) $(includeUTDirs) \
	$(includeDirs) $(linkUTLibs) $(gTestFlags)


# Clean
clean:
	rm *.o ../../../bins/blackCatBin ../../../bins/unitTestBin
