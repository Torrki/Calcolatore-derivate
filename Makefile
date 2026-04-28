###############################################################################
# Project Configuration
###############################################################################

# Name of the final executable
TARGET      := Derivate

# Directories
SRC_DIR     := src
BUILD_DIR   := bin
OBJ_DIR     := $(BUILD_DIR)/obj

###############################################################################
# Compiler and Flags
###############################################################################

CC      := gcc

# External libraries:
LIBS     	 := 
LIB_DIRS 	 := 
INCLUDE_DIRS := include

# Construct linker flags
LDFLAGS := $(foreach d,$(LIB_DIRS),-L$(d)) \
           $(foreach l,$(LIBS),-l$(l))

# Construct include flags
IFLAGS := $(foreach i,$(INCLUDE_DIRS),-I$(i))

CFLAGS  := -Wall -Wextra -O2 -std=c11 $(IFLAGS) -D _POSIX_C_SOURCE=200809L

###############################################################################
# Source and Object Files
###############################################################################

# All .c files under src/
SRCS := $(wildcard $(SRC_DIR)/*.c)

# Object files located under build/obj/
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

###############################################################################
# Build Rules
###############################################################################

# Default target
all: $(BUILD_DIR)/$(TARGET)

# Link the final program
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile .c → .o, placing objects in build/obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure object directory exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

###############################################################################
# Utility Targets
###############################################################################

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
