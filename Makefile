# Compiler
CC = gcc
WINDRES = windres

# Flags
WNOFLAGS = -Wno-unused-variable -Wno-switch -Wno-unused-parameter
INCLUDES = -Iinclude -Ivendor/raylib-v5.5/include -Ivendor/reestruct-v1.0.0/include
RSTFLAGS = vendor/reestruct-v1.0.0/lib/libreestruct.a

# Paths
SRC_PATH = src library
OBJ_PATH = build/output
BIN_PATH = bin
TMP_PATH = temp

# Platform detection
UNAME_S := $(shell uname -s)
ifeq ($(OS),Windows_NT)
	PLATFORM := WINDOWS
endif
ifeq ($(PLATFORM),)
	ifneq (,$(findstring MINGW,$(UNAME_S)))
		PLATFORM := WINDOWS
	else ifneq (,$(findstring MSYS,$(UNAME_S)))
		PLATFORM := WINDOWS
	else ifneq (,$(findstring CYGWIN,$(UNAME_S)))
		PLATFORM := WINDOWS
	else ifeq ($(UNAME_S),Linux)
		PLATFORM := LINUX
	else ifeq ($(UNAME_S),Darwin)
		PLATFORM := MAC
	else
		PLATFORM := UNKNOWN
	endif
endif

EXE_EXT :=
RC_OBJ :=
RC_FILE :=
RAYLIB_CFLAGS :=
RAYLIB_LDFLAGS :=
RAYLIB_RPATH :=

ifeq ($(PLATFORM),WINDOWS)
	EXE_EXT := .exe
	RC_FILE := assets/resource.rc
	RC_OBJ := $(OBJ_PATH)/resource.o
	RAYLIB_LDFLAGS := -Lvendor/raylib-v5.5/lib -lraylib -lopengl32 -lgdi32 -lwinmm
else ifeq ($(PLATFORM),LINUX)
	ifeq ($(shell pkg-config --exists raylib && echo yes),yes)
		RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib)
		RAYLIB_LDFLAGS := $(shell pkg-config --libs raylib)
	else ifneq ($(wildcard vendor/raylib-v5.5/lib/libraylib.so),)
		RAYLIB_LDFLAGS := -Lvendor/raylib-v5.5/lib -lraylib -lm -lpthread -ldl -lrt -lGL -lX11
		RAYLIB_RPATH := -Wl,-rpath,'$$ORIGIN/../vendor/raylib-v5.5/lib'
	else
		RAYLIB_LDFLAGS := -lraylib -lm -lpthread -ldl -lrt -lGL -lX11
	endif
else ifeq ($(PLATFORM),MAC)
	ifeq ($(shell pkg-config --exists raylib && echo yes),yes)
		RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib)
		RAYLIB_LDFLAGS := $(shell pkg-config --libs raylib)
	else ifneq ($(wildcard vendor/raylib-v5.5/lib/libraylib.dylib),)
		RAYLIB_LDFLAGS := -Lvendor/raylib-v5.5/lib -lraylib -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL -framework AudioToolbox -framework CoreAudio
		RAYLIB_RPATH := -Wl,-rpath,@executable_path/../vendor/raylib-v5.5/lib
	else ifneq ($(shell brew --prefix raylib 2>/dev/null),)
		RAYLIB_PREFIX := $(shell brew --prefix raylib)
		RAYLIB_CFLAGS := -I$(RAYLIB_PREFIX)/include
		RAYLIB_LDFLAGS := -L$(RAYLIB_PREFIX)/lib -lraylib -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL -framework AudioToolbox -framework CoreAudio
		RAYLIB_RPATH := -Wl,-rpath,$(RAYLIB_PREFIX)/lib
	else
		RAYLIB_LDFLAGS := -lraylib -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL -framework AudioToolbox -framework CoreAudio
	endif
else
	$(warning Unknown platform '$(UNAME_S)'; falling back to generic raylib flags)
	RAYLIB_LDFLAGS := -lraylib
endif

CFLAGS = $(WNOFLAGS) $(INCLUDES) $(RAYLIB_CFLAGS)
LDFLAGS = $(RAYLIB_LDFLAGS) $(RAYLIB_RPATH)

# Source files
SRC = src/main.c library/arief.c library/naira.c library/raffi.c library/faliq.c library/goklas.c

# Object files (replace src/library with build/output)
OBJ = $(patsubst %.c,$(OBJ_PATH)/%.o,$(SRC))

# Output
TARGET_NAME = BlockShooter$(EXE_EXT)
TARGET = $(BIN_PATH)/$(TARGET_NAME)

# Build
all: $(TARGET)

$(TARGET): $(OBJ) $(RC_OBJ)
	@echo "Creating necessary directories..."
	@mkdir -p $(BIN_PATH)
	@echo "🔧 Linking..."
	$(CC) $^ -o $@ $(LDFLAGS) $(RSTFLAGS)
	@echo "✅ Build successful! Run './$(TARGET)'"

ifeq ($(PLATFORM),WINDOWS)
$(RC_OBJ): $(RC_FILE)
	@echo "🔨 Compiling resources $<..."
	@mkdir -p $(dir $@)
	$(WINDRES) $< -o $@
endif

# Compile each .c file into build/output/
$(OBJ_PATH)/%.o: %.c
	@echo "🔨 Compiling $<..."
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	@echo "🗑 Cleaning build directory..."
	@rm -rf $(OBJ_PATH) $(BIN_PATH)
	@echo "✅ Clean complete!"

# Run
run: all
	@echo "🚀 Running game..."
	@./$(TARGET)

rebuild: clean run

# Test files
TEST_SRC = $(TMP_PATH)/main.c library/arief.c library/naira.c library/raffi.c library/faliq.c library/goklas.c
TEST_OBJ = $(patsubst %.c,$(OBJ_PATH)/%.o,$(TEST_SRC))
TEST_TARGET = $(BIN_PATH)/test$(EXE_EXT)

# Test target
test: clean-test $(TEST_TARGET)
	@echo "🧪 Running tests..."
	@./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ)
	@echo "Creating necessary directories..."
	@mkdir -p $(BIN_PATH)
	@echo "🔧 Linking test executable..."
	$(CC) $^ -o $@ $(LDFLAGS) $(RSTFLAGS)
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
