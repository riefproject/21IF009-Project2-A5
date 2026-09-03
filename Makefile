# Compiler
CXX ?= g++
WINDRES ?= windres

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

# Flags & Includes
INCLUDES = -Iinclude -Ivendor/raylib-v5.5/include
WNOFLAGS = -Wall -Wextra -Wno-unused-parameter

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
	CXX = clang++
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

CXXFLAGS = -std=c++17 $(WNOFLAGS) $(INCLUDES) $(RAYLIB_CFLAGS)
LDFLAGS = $(RAYLIB_LDFLAGS) $(RAYLIB_RPATH)

# Paths
SRC_PATH = src
OBJ_PATH = build/output
BIN_PATH = bin
TMP_PATH = temp

# Source files
SRC = src/main.cpp \
      src/Scale.cpp \
      src/AssetManager.cpp \
      src/SettingsManager.cpp \
      src/ScoreManager.cpp \
      src/Player.cpp \
      src/BulletManager.cpp \
      src/PowerUpManager.cpp \
      src/Grid.cpp \
      src/Game.cpp \
      src/UIManager.cpp \
      src/GameEngine.cpp

# Object files
OBJ = $(patsubst src/%.cpp,$(OBJ_PATH)/%.o,$(SRC))

# Output
TARGET_NAME = BlockShooter$(EXE_EXT)
TARGET = $(BIN_PATH)/$(TARGET_NAME)

# Build
all: $(TARGET)

$(TARGET): $(OBJ) $(RC_OBJ)
	@echo "Creating necessary directories..."
	@mkdir -p $(BIN_PATH)
	@echo "🔧 Linking C++ executable..."
	$(CXX) $^ -o $@ $(LDFLAGS)
	@echo "✅ Build successful! Run './$(TARGET)'"

ifeq ($(PLATFORM),WINDOWS)
$(RC_OBJ): $(RC_FILE)
	@echo "🔨 Compiling resources $<..."
	@mkdir -p $(dir $@)
	$(WINDRES) $< -o $@
endif

# Compile each .cpp file into build/output/
$(OBJ_PATH)/%.o: src/%.cpp
	@echo "🔨 Compiling $<..."
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	@echo "🗑 Cleaning build directory..."
	@rm -rf $(OBJ_PATH) $(BIN_PATH)
	@echo "✅ Clean complete!"

# Run
run: all
	@echo "🚀 Running game..."
	@./$(TARGET)

rebuild: clean all

.PHONY: all clean run rebuild
