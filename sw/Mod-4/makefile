MODULE_NAME = mod4
# TEST_MODULE = test_mod2.cpp
# TEST_BIN_DIR = ../../test_build

# Target (Final lib .so file)
TARGET_DIR = ../../libs
TARGET_NAME = lib$(MODULE_NAME).a
TARGET = $(TARGET_DIR)/$(TARGET_NAME)

# Prereq or dependencies
OBJ_DIR = ../../objs
OBJ1 = $(OBJ_DIR)/$(MODULE_NAME).o

CFLAGS = -fPIC -Wall -Werror
LDFLAGS =

# Building (2 steps) mod4.cpp to ... (TARGET_NAME)
# $@ - Target
# $^ - Objects (OBJ1) all objects to TARGET ($@)
$(TARGET): $(OBJ1)
	ar rcs $@ $^

$(OBJ1): $(MODULE_NAME).cpp
	g++ $(CFLAGS) -c $< -o $@

# TODO: Add test target

clean:
	rm $(TARGET) $(OBJ1)