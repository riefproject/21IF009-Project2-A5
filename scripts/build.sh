#!/bin/bash

# pake array karna akan diiterasi. Kalo mau nambah subfolder tambahin aja
SRC_DIRS=("src" 
          "library")

BUILD_DIR="build/output"
BIN_DIR="bin"
EXE_NAME="BlockShooter.exe"
EXE_PATH="$BIN_DIR/$EXE_NAME"
TEST_EXE_NAME="test.exe"
TEST_EXE_PATH="$BIN_DIR/$TEST_EXE_NAME"
RESOURCE_RC="assets/resource.rc"
RESOURCE_RES="$BUILD_DIR/resource.o"
TMP_PATH="temp"

# bukan array karna ga akan diiterasi (cuma flag). Tambahin aja kalo butuh subfolder tambahan
WNO="-Wno-unused-variable
     -Wno-switch
     -Wno-unused-parameter"

CFLAGS="$WNO
        -Iinclude
        -Ivendor/raylib-v5.5/include
        -Ivendor/reestruct-v0.1.0/include"
LDFLAGS="vendor/raylib-v5.5/lib/libraylib.a -lopengl32 -lgdi32 -lwinmm"
RSTFLAGS="vendor/reestruct-v0.1.0/lib/libreestruct.a"
OBJECT_FILES=()

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
    if [ ! -f "$EXE_PATH" ] || [ "$RESOURCE_RES" -nt "$EXE_PATH" ]; then
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
        gcc "${OBJECT_FILES[@]}" "$RESOURCE_RES" -o "$EXE_PATH" $LDFLAGS $RSTFLAGS
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
    compile_sources
    link_if_needed

    clear
    echo -e "✅ Build successful! Run './$EXE_PATH'"
}

run() {
    build
    if [ $? -ne 0 ]; then
        exit 1
    fi

    echo -e "\n🚀 Running ${BLUE}BlockShooter...${RESET}"
    "./$EXE_PATH"
    
    clear
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
    gcc "$BUILD_DIR/$TMP_PATH/main.o" "${OBJECT_FILES[@]}" -o "$TEST_EXE_PATH" $LDFLAGS
    
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