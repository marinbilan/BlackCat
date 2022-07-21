# ---------------------------------
# ---- UDEMY MAKEFILE TUTORIAL ----

# Name of obj file
MODULE_NAME = mod1

TARGET_DIR = ../../bins
TARGET_NAME = mod1_bin
# Name of bin file
TARGET = $(TARGET_DIR)/$(TARGET_NAME)

OBJ_DIR = ../../objs
OBJ1 = $(OBJ_DIR)/$(MODULE_NAME).o

CFLAGS = -Wall -Werror
LDFLAGS = -lm


# Incremental build
$(TARGET): $(OBJ1)
	g++ $< -o $@ $(LDFLAGS)

$(OBJ1): $(MODULE_NAME).cpp
	g++ $(CFLAGS) -c $< -o $@

clean:
	rm $(TARGET) $(OBJ1)


# ---------------------------------
# ---------------------------------
