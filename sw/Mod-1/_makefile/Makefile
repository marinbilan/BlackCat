# Makefile
CXXFLAGS = -g -no-pie -std=c++17

includeDirs = -I ../if \
              -I ../inc \
              -I ../networking/if \
              -I ../networking/inc \
              -I ../mock \
              -I ../../../libs/asio-1.22.1/include \
              -I ../../../libs/boost \

# 1 STEP: Add preReq
allPreReq = bcMain.o \
			Service0.o \
			Service1.o \
			ServiceBaseDerv.o \
			StockCalcSrv.o

OUTPUT = ../../../bins/blackCatBin

# SSL (HTTPS) libs
linkLibs = -L../../../libs/sslLibs
sslLibs = -lssl -lcrypto

# target: prerequisites
#	recepies
blackCatBin: $(allPreReq)
	g++ -pthread $(CXXFLAGS) $(allPreReq) -o $(OUTPUT) $(linkLibs) $(sslLibs)

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




# -------------------
# //// UNIT TEST ////
# -------------------
includeUTLibs = -I ../../../libs/unitTest/googlemock/include \
				-I ../../../libs/unitTest/googletest/include \

# *.a libs
linkUTLibs = -L ../../../libs/unitTest/googlemocklibs

gTestFlags = -lgtest -lgtest_main -lgmock -lgmock_main -pthread


# Files to compile
# TODO: Refactor this ******** ----
UNITTESTIN = ../unitTest/unitTest.cpp \
			 ../unitTest/UTStockCalcSrv.cpp \
			 ../src/Service0.cpp \
			 ../src/Service1.cpp \
			 ../src/ServiceBaseDerv.cpp \
			 ../src/StockCalcSrv.cpp \

UNITTESTIN_0 = ../unitTest/UTStockCalcSrv.cpp \
			   ../src/StockCalcSrv.cpp \
# ******** ----

UNITTESTOUT = ../../../bins/unitTestBin


unitTest:
	g++ $(UNITTESTIN) -o $(UNITTESTOUT) $(includeUTLibs) \
	$(includeDirs) $(linkUTLibs) $(gTestFlags) $(CXXFLAGS)



# //// UNIT TEST NETWORKING ////
# INPUT
UNITTEST_NETWORK_IN = ../networking/unitTest/unitTestNetworking.cpp \
				   ../src/HTTPSProxySrv.cpp \
				   ../networking/src/HTTPSClient.cpp \
# OUTPUT
UNITTEST_NETWORK_OUT = ../../../bins/unitTestBin

unitTestNet:
	g++ $(UNITTEST_NETWORK_IN) -o $(UNITTEST_NETWORK_OUT) $(includeUTLibs) \
	$(includeDirs) $(linkUTLibs) $(gTestFlags) $(CXXFLAGS) $(linkLibs) $(sslLibs)
# //// ////



# Clean
clean:
	rm *.o ../../../bins/blackCatBin ../../../bins/unitTestBin
