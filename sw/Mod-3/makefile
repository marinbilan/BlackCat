# -----------------------
# ---- MAKEFILE MOD-3----

# Name of obj file
MODULE_NAME = mod3

TARGET_DIR = ../../bins
TARGET_NAME = mod3_bin
# Name of bin file
TARGET = $(TARGET_DIR)/$(TARGET_NAME)

OBJ_DIR = ../../objs
OBJ1 = $(OBJ_DIR)/$(MODULE_NAME).o

# Compilation stuff
# Include folders during compilation
INC_PATH = ../../share_headers
WARN_FLAGS = -Wall -Werror
CFLAGS = $(WARN_FLAGS) -I$(INC_PATH)


# Library path
LIBS_PATH = ../../libs
LDFLAGS = -L$(LIBS_PATH) -lmod2


# Incremental build
$(TARGET): $(OBJ1)
	g++ $< -o $@ $(LDFLAGS)

$(OBJ1): $(MODULE_NAME).cpp
	g++ $(CFLAGS) -c $< -o $@

clean:
	rm $(TARGET) $(OBJ1)


# ---------------------------------
# ---------------------------------