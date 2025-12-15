#!/bin/bash

# pake array karna akan diiterasi. Kalo mau nambah subfolder tambahin aja
SRC_DIRS=("src"
          "library")

BUILD_DIR="build/output"
BIN_DIR="bin"
TMP_PATH="temp"

PLATFORM="unknown"
case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*) PLATFORM="windows" ;;
    Linux*) PLATFORM="linux" ;;
    Darwin*) PLATFORM="mac" ;;
    *) PLATFORM="unknown" ;;
esac

EXE_EXT=""
RESOURCE_ENABLED=0
if [ "$PLATFORM" = "windows" ]; then
    EXE_EXT=".exe"
    RESOURCE_ENABLED=1
fi

EXE_NAME="BlockShooter${EXE_EXT}"
EXE_PATH="$BIN_DIR/$EXE_NAME"
TEST_EXE_NAME="test${EXE_EXT}"
TEST_EXE_PATH="$BIN_DIR/$TEST_EXE_NAME"
RESOURCE_RC="assets/resource.rc"
RESOURCE_RES="$BUILD_DIR/resource.o"

# bukan array karna ga akan diiterasi (cuma flag). Tambahin aja kalo butuh subfolder tambahan
WNO="-Wno-unused-variable
     -Wno-switch
     -Wno-unused-parameter"

detect_raylib_pkgconfig() {
    if command -v pkg-config >/dev/null 2>&1 && pkg-config --exists raylib; then
        RAYLIB_CFLAGS="$(pkg-config --cflags raylib)"
        RAYLIB_LDFLAGS="$(pkg-config --libs raylib)"
        return 0
    fi
    return 1
}

RAYLIB_CFLAGS=""
RAYLIB_LDFLAGS=""
RAYLIB_RPATH=""
if [ "$PLATFORM" = "windows" ]; then
    RAYLIB_LDFLAGS="-Lvendor/raylib-v5.5/lib -lraylib -lopengl32 -lgdi32 -lwinmm"
elif [ "$PLATFORM" = "linux" ]; then
    if ! detect_raylib_pkgconfig; then
        if [ -f "vendor/raylib-v5.5/lib/libraylib.so" ]; then
            RAYLIB_LDFLAGS="-Lvendor/raylib-v5.5/lib -lraylib -lm -lpthread -ldl -lrt -lGL -lX11"
            RAYLIB_RPATH="-Wl,-rpath,'\$ORIGIN/../vendor/raylib-v5.5/lib'"
        else
            RAYLIB_LDFLAGS="-lraylib -lm -lpthread -ldl -lrt -lGL -lX11"
        fi
    fi
elif [ "$PLATFORM" = "mac" ]; then
    if ! detect_raylib_pkgconfig; then
        if [ -f "vendor/raylib-v5.5/lib/libraylib.dylib" ]; then
            RAYLIB_LDFLAGS="-Lvendor/raylib-v5.5/lib -lraylib -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL -framework AudioToolbox -framework CoreAudio"
            RAYLIB_RPATH="-Wl,-rpath,@executable_path/../vendor/raylib-v5.5/lib"
        elif command -v brew >/dev/null 2>&1 && brew --prefix raylib >/dev/null 2>&1; then
            RAYLIB_PREFIX="$(brew --prefix raylib 2>/dev/null)"
            RAYLIB_CFLAGS="-I${RAYLIB_PREFIX}/include"
            RAYLIB_LDFLAGS="-L${RAYLIB_PREFIX}/lib -lraylib -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL -framework AudioToolbox -framework CoreAudio"
            RAYLIB_RPATH="-Wl,-rpath,${RAYLIB_PREFIX}/lib"
        else
            RAYLIB_LDFLAGS="-lraylib -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL -framework AudioToolbox -framework CoreAudio"
            RAYLIB_NEEDS_HINT=1
        fi
    fi
else
    RAYLIB_LDFLAGS="-lraylib"
fi

CFLAGS="$WNO
        -Iinclude
        -Ivendor/raylib-v5.5/include
        -Ivendor/reestruct-v1.0.0/include
        $RAYLIB_CFLAGS"
LDFLAGS="$RAYLIB_LDFLAGS $RAYLIB_RPATH"
RSTFLAGS="vendor/reestruct-v1.0.0/lib/libreestruct.a"
OBJECT_FILES=()

verify_raylib() {
    if [ "$PLATFORM" = "unknown" ]; then
        echo -e "${YELLOW}⚠️  Unknown platform; attempting to link raylib generically.${RESET}"
    fi
    mkdir -p "$BUILD_DIR"
    local test_bin="$BUILD_DIR/.raylib_check${EXE_EXT}"
    echo "int main(void){return 0;}" | gcc $CFLAGS -x c - -o "$test_bin" $LDFLAGS >/dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ Raylib not found for this platform.${RESET}"
        if [ "$PLATFORM" = "mac" ]; then
            echo "Install via 'brew install raylib' or export RAYLIB_CFLAGS/RAYLIB_LDFLAGS."
        elif [ "$PLATFORM" = "linux" ]; then
            echo "Install raylib from your package manager (and ensure pkg-config is available), or export RAYLIB_CFLAGS/RAYLIB_LDFLAGS."
        else
            echo "Ensure raylib libraries are available and update RAYLIB_CFLAGS/RAYLIB_LDFLAGS accordingly."
        fi
        rm -f "$test_bin"
        exit 1
    fi
    rm -f "$test_bin"
}

# Biar CLI nya cakep
RED='\e[31m'
GREEN='\e[32m'
YELLOW='\e[33m'
BLUE='\e[34m'
RESET='\e[0m' # No Color
BOLD='\e[1m'
ITALIC='\e[3m'
UNDERLINE='\e[4m'

# =====================================================================================
# . . . FUNCTIONS . . .
# =====================================================================================

maybe_clear() {
    if command -v clear >/dev/null 2>&1; then
        clear
    fi
}

clean() {
    echo "🧹 Cleaning build directories..."
    rm -rf "$BUILD_DIR" "$BIN_DIR" "$RESOURCE_RES"
    echo "✅ Clean complete!"
}

clean_test() {
    echo "🗑 Cleaning test files..."
    rm -f "$TEST_EXE_PATH"
    rm -rf "$BUILD_DIR/$TMP_PATH"
    echo "✅ Test clean complete!"
}

compile_if_needed() {
    local src_file=$1
    local out_file=$2

    mkdir -p "$(dirname "$out_file")"

    if [ ! -f "$out_file" ] || [ "$src_file" -nt "$out_file" ]; then
        echo "🔨 Compiling $src_file..."
        gcc $CFLAGS -c "$src_file" -o "$out_file"
        if [ $? -ne 0 ]; then
            echo -e "${RED}❌ Compilation failed: $src_file ${RESET}"
            exit 1
        fi
    else
        echo -e "${GREEN}✅ Skipping $src_file (up to date) ${RESET}"
    fi

    OBJECT_FILES+=("$out_file")
}

compile_sources() {
    for dir in "${SRC_DIRS[@]}"; do
        for src_file in "$dir"/*.c; do
            [ -f "$src_file" ] || continue
            local filename=$(basename "$src_file" .c)
            local out_file="$BUILD_DIR/$dir/${filename}.o"
            compile_if_needed "$src_file" "$out_file"
        done
    done
}

compile_resource_if_needed() {
    if [ "$RESOURCE_ENABLED" -ne 1 ]; then
        return
    fi

    if [ ! -f "$RESOURCE_RES" ] || [ "$RESOURCE_RC" -nt "$RESOURCE_RES" ]; then
        echo "🎨 Compiling resource file..."
        mkdir -p "$(dirname "$RESOURCE_RES")"
        windres "$RESOURCE_RC" -o "$RESOURCE_RES"
        if [ $? -ne 0 ]; then
            echo -e "${RED}❌ Resource compilation failed! ${RESET}"
            exit 1
        fi
    else
        echo -e "${GREEN}✅ Skipping resource compilation (up to date) ${RESET}"
    fi
}

link_if_needed() {
    compile_resource_if_needed

    local need_link=false
    if [ ! -f "$EXE_PATH" ]; then
        need_link=true
    elif [ "$RESOURCE_ENABLED" -eq 1 ] && [ "$RESOURCE_RES" -nt "$EXE_PATH" ]; then
        need_link=true
    else
        for obj in "${OBJECT_FILES[@]}"; do
            if [ "$obj" -nt "$EXE_PATH" ]; then
                need_link=true
                break
            fi
        done
    fi

    if $need_link; then
        echo "🔧 Linking..."
        local link_inputs=("${OBJECT_FILES[@]}")
        if [ "$RESOURCE_ENABLED" -eq 1 ]; then
            link_inputs+=("$RESOURCE_RES")
        fi
        gcc "${link_inputs[@]}" -o "$EXE_PATH" $LDFLAGS $RSTFLAGS
        if [ $? -ne 0 ]; then
            echo -e "${RED}❌ Linking failed! ${RESET}"
            exit 1
        fi
    else
        echo -e "${GREEN}✅ Skipping linking (up to date) ${RESET}"
    fi
}

build() {
    echo "🚧 Starting build process..."
    mkdir -p "$BIN_DIR"
    verify_raylib
    compile_sources
    link_if_needed

    maybe_clear
    echo -e "✅ Build successful! Run './$EXE_PATH'"
}

run() {
    build
    if [ $? -ne 0 ]; then
        exit 1
    fi

    echo -e "\n🚀 Running ${BLUE}BlockShooter...${RESET}"
    "./$EXE_PATH"
    
    maybe_clear
    if [ $? -eq 0 ]; then
        echo -e "\n\nThanks for playing ${GREEN}Block Shooter${RESET}"
    fi
}

test() {
    clean_test
    if [ $? -ne 0 ]; then
        exit 1
    fi
    
    echo "Creating necessary directories..."
    mkdir -p "$BUILD_DIR/$TMP_PATH" "$BIN_DIR"
    
    echo "🔨 Compiling test file $TMP_PATH/main.c..."
    gcc $CFLAGS -c "$TMP_PATH/main.c" -o "$BUILD_DIR/$TMP_PATH/main.o"
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ Compilation of $TMP_PATH/main.c failed! ${RESET}"
        exit 1
    fi
    
    # Reset OBJECT_FILES for test compilation
    OBJECT_FILES=()
    
    # Compile library files for tests
    for src_file in library/*.c; do
        [ -f "$src_file" ] || continue
        local filename=$(basename "$src_file" .c)
        local out_file="$BUILD_DIR/library/${filename}.o"
        compile_if_needed "$src_file" "$out_file"
    done
    
    echo "🔧 Linking test executable..."
    gcc "$BUILD_DIR/$TMP_PATH/main.o" "${OBJECT_FILES[@]}" -o "$TEST_EXE_PATH" $LDFLAGS $RSTFLAGS
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ Test linking failed! ${RESET}"
        exit 1
    fi
    
    echo "✅ Test build successful!"
    echo "🧪 Running tests..."
    "./$TEST_EXE_PATH"
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ Tests failed with error code $?! ${RESET}"
        exit 1
    fi
}

rebuild() {
    echo "♻️ Rebuilding from scratch..."
    clean
    if [ $? -ne 0 ]; then
        exit 1
    fi
    run
}

parse_args() {
    DEBUG=0
    HELP=0
    POSITIONAL=()

    for arg in "$@"; do
        if [ "$arg" == "--debug" ]; then
            DEBUG=1
        elif [ "$arg" == "--help" ]; then
            HELP=1
        else
            POSITIONAL+=("$arg")
        fi
    done

    if [ $HELP -eq 1 ]; then
        if [ ${#POSITIONAL[@]} -ne 0 ] || [ $DEBUG -eq 1 ]; then
            echo "❌ Error: --help harus dipanggil tanpa parameter lain."
            show_help
            exit 1
        fi
        show_help
        exit 0
    fi

    # Set global variabel untuk dipakai di main
    DEBUG_FLAG=$DEBUG
    set -- "${POSITIONAL[@]}"
    POSITIONAL_ARGS=("$@")
}

show_help() {
    echo -e "\n${BOLD}${BLUE}BlockShooter${RESET}${BOLD} Build Script${RESET}"
    echo -e "============================================================\n"
    
    echo -e "Usage: $0 [--debug] [clean|build|run|rebuild|test|clean-test] [--help]\n"
    echo -e "${UNDERLINE}Commands:${RESET}"
    echo -e "  ${BOLD}build      ${RESET}  : Compile dan link (incremental)"
    echo -e "  ${BOLD}run        ${RESET}  : Build dan jalankan game"
    echo -e "  ${BOLD}clean      ${RESET}  : Bersihkan hasil build"
    echo -e "  ${BOLD}rebuild    ${RESET}  : Bersihkan lalu build dan run"
    echo -e "  ${BOLD}test       ${RESET}  : Jalankan tests"
    echo -e "  ${BOLD}clean-test ${RESET}  : Bersihkan file test"
    echo -e "  ${BOLD}--debug    ${RESET}  : Aktifkan mode debug (set -x)"
    echo -e "  ${BOLD}--help     ${RESET}  : Tampilkan pesan ini\n"
    echo -e "${UNDERLINE}Notes:${RESET}"
    echo -e "  --help harus dipanggil sendiri, tanpa param lain."
}

# =====================================================================================
# . . . MAIN SCRIPT . . .
# =====================================================================================

parse_args "$@"

if [ "$DEBUG_FLAG" -eq 1 ]; then
    set -x
fi

# Lanjut dengan main case pakai "${POSITIONAL_ARGS[0]}"
case "${POSITIONAL_ARGS[0]}" in
    "clean") clean ;;
    "rebuild") rebuild ;;
    "run") run ;;
    "test") test ;;
    "clean-test") clean_test ;;
    ""|"build") build; run;;
    *)
        echo -e "${RED}❌ Unknown parameter: ${POSITIONAL_ARGS[0]} ${RESET}"
        show_help
        exit 1
        ;;
esac

exit 0
