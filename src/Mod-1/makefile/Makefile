# Makefile
CXXFLAGS = -g -no-pie -std=c++17

includeDirs = -I ../if \
              -I ../inc \
              -I ../mock \
              -I ../../../libs/asio-1.22.1/include \

# 1 STEP: Add preReq
allPreReq = bcMain.o \
			Service0.o \
			Service1.o \
			ServiceBaseDerv.o \
			StockCalcSrv.o

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

ServiceBaseDerv.o: ../src/ServiceBaseDerv.cpp
	g++ -c $(CXXFLAGS) $(includeDirs) ../src/ServiceBaseDerv.cpp

StockCalcSrv.o: ../src/StockCalcSrv.cpp
	g++ -c $(CXXFLAGS) $(includeDirs) ../src/StockCalcSrv.cpp



# UnitTest
includeUTDirs = -I ../../../libs/googlemock/include \
				-I ../../../libs/googletest/include \

linkUTLibs = -L ../../../libs/googlemocklibs

# 3 STEP: Add UnitTest if needed
UNITTESTIN = ../unitTest/unitTest.cpp \
			 ../unitTest/UTStockCalcSrv.cpp \
			 ../src/Service0.cpp \
			 ../src/Service1.cpp \
			 ../src/ServiceBaseDerv.cpp \
			 ../src/StockCalcSrv.cpp \

UNITTESTIN_0 = ../unitTest/UTStockCalcSrv.cpp \
			   ../src/StockCalcSrv.cpp \

UNITTESTOUT = ../../../bins/unitTestBin

gTestFlags = -lgtest -lgtest_main -lgmock -lgmock_main -pthread

unitTest:
	g++ $(UNITTESTIN_0) -o $(UNITTESTOUT) $(includeUTDirs) \
	$(includeDirs) $(linkUTLibs) $(gTestFlags) $(CXXFLAGS)

# Clean
clean:
	rm *.o ../../../bins/blackCatBin ../../../bins/unitTestBin
