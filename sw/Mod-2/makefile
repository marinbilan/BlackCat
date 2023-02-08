MODULE_NAME = mod2
TEST_MODULE = test_mod2.cpp
TEST_BIN_DIR = ../../test_build

# Target (Final lib .so file)
TARGET_DIR = ../../libs
TARGET_NAME = lib$(MODULE_NAME).so
TARGET = $(TARGET_DIR)/$(TARGET_NAME)

# Prereq or dependencies
OBJ_DIR = ../../objs
OBJ1 = $(OBJ_DIR)/$(MODULE_NAME).o

CFLAGS = -fPIC -Wall -Werror
LDFLAGS =

# Building (2 steps) mod2.cpp to libmod2.so (TARGET_NAME)
$(TARGET): $(OBJ1)
	g++ -shared $< -o $@ $(LDFLAGS)	

$(OBJ1): $(MODULE_NAME).cpp
	g++ $(CFLAGS) -c $< -o $@

test:
	g++ $(TEST_MODULE) -o $(TEST_BIN_DIR)/$(MODULE_NAME) -L$(TARGET_DIR) -l$(MODULE_NAME) 
	
# -p flag: If dir is already created, do nothing
build_dir:
	mkdir -p $(TEST_BIN_DIR)

install:
	cp *.h ../../share_headers

clean:
	rm $(TARGET) $(OBJ1)
