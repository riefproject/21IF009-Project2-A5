# Compiler
CC = gcc

# Flags
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm
GRFLAGS = -static-libgcc -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32

# Paths
SRC_PATH = src library
OBJ_PATH = build/output
BIN_PATH = bin

# Source files
SRC = src/main.c library/arief.c library/naira.c library/raffi.c library/faliq.c library/goklas.c

# Object files (replace src/library with build/output)
OBJ = $(patsubst %.c,$(OBJ_PATH)/%.o,$(SRC))

# Output
TARGET = $(BIN_PATH)/game.exe

# Build
all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "Creating necessary directories..."
	@mkdir -p $(BIN_PATH)
	@echo "🔧 Linking..."
	$(CC) $^ -o $@ $(LDFLAGS) $(GRFLAGS)
    # tambahkan cls bang di line ini
	@echo "✅ Build successful! Run './$(TARGET)'"

# Compile each .c file into build/output/
$(OBJ_PATH)/%.o: %.c
	@echo "🔨 Compiling $<..."
	@mkdir -p $(dir $@)  # Ensure directory exists
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	@echo "🗑 Cleaning build directory..."
	@rm -rf $(OBJ_PATH) $(BIN_PATH)
	@echo "✅ Clean complete!"

# Run
run: all clear
	@echo "🚀 Running game..."
	@./$(TARGET)
