# Compiler
CC = gcc
WINDRES = windres

# Flags
WNOFLAGS = -Wno-unused-variable -Wno-switch -Wno-unused-parameter
CFLAGS = $(WNOFLAGS) -Iinclude -Ivendor/raylib-v5.5/include -Ivendor/reestruct-v0.1.0/include
LDFLAGS = vendor/raylib-v5.5/lib/libraylib.a -lopengl32 -lgdi32 -lwinmm
RSTFLAGS =  vendor/reestruct-v0.1.0/lib/libreestruct.a 

# Graphics library flags (cuma kompatibel di compiler 32-bit)
GRFLAGS = -static-libgcc -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32

# Paths
SRC_PATH = src library
OBJ_PATH = build/output
BIN_PATH = bin
TMP_PATH = temp

# Source files
SRC = src/main.c library/arief.c library/naira.c library/raffi.c library/faliq.c library/goklas.c 

# Resource file
RC_FILE = assets/resource.rc
RC_OBJ = $(OBJ_PATH)/resource.o

# Object files (replace src/library with build/output)
OBJ = $(patsubst %.c,$(OBJ_PATH)/%.o,$(SRC))

# Output
TARGET_NAME = "BlockShooter.exe"
TARGET = $(BIN_PATH)/$(TARGET_NAME)

# Build
all: $(TARGET)

$(TARGET): $(OBJ) $(RC_OBJ)
	@echo "Creating necessary directories..."
	@mkdir -p $(BIN_PATH)
	@echo "🔧 Linking..."
	$(CC) $^ -o $@ $(LDFLAGS) $(RSTFLAGS) 
	@clear
	@echo "✅ Build successful! Run './$(TARGET)'"

# Compile resource file
$(RC_OBJ): $(RC_FILE)
	@echo "🔨 Compiling resources $<..."
	@mkdir -p $(dir $@)
	$(WINDRES) $< -o $@

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
run: all
	@echo "🚀 Running game..."
	@"./$(BIN_PATH)/$(TARGET_NAME)"

rebuild: clean run
# Test files
TEST_SRC = $(TMP_PATH)/main.c library/arief.c library/naira.c library/raffi.c library/faliq.c library/goklas.c
TEST_OBJ = $(patsubst %.c,$(OBJ_PATH)/%.o,$(TEST_SRC))
TEST_TARGET = $(BIN_PATH)/test.exe

# Test target
test: clean-test $(TEST_TARGET)
	@echo "🧪 Running tests..."
	@./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ)
	@echo "Creating necessary directories..."
	@mkdir -p $(BIN_PATH)
	@echo "🔧 Linking test executable..."
	$(CC) $^ -o $@ $(LDFLAGS)
	@echo "✅ Test build successful!"

# Compile test files
$(OBJ_PATH)/$(TMP_PATH)/%.o: $(TMP_PATH)/%.c
	@echo "🔨 Compiling test file $<..."
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean tests
clean-test:
	@echo "🗑 Cleaning test files..."
	@rm -f $(TEST_TARGET)
	@rm -rf $(OBJ_PATH)/$(TMP_PATH)
	@echo "✅ Test clean complete!"