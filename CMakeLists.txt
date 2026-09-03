cmake_minimum_required(VERSION 3.16)
project(BlockShooter VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler Warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wno-unused-parameter)
endif()

# Output Directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Sources and Headers
set(SOURCES
    src/main.cpp
    src/Scale.cpp
    src/AssetManager.cpp
    src/SettingsManager.cpp
    src/ScoreManager.cpp
    src/Player.cpp
    src/BulletManager.cpp
    src/PowerUpManager.cpp
    src/Grid.cpp
    src/Game.cpp
    src/UIManager.cpp
    src/GameEngine.cpp
)

set(HEADERS
    include/BlockShooter.hpp
    include/Defines.hpp
    include/Scale.hpp
    include/AssetManager.hpp
    include/SettingsManager.hpp
    include/ScoreManager.hpp
    include/Player.hpp
    include/BulletManager.hpp
    include/PowerUpManager.hpp
    include/Grid.hpp
    include/Game.hpp
    include/UIManager.hpp
    include/GameEngine.hpp
)

add_executable(BlockShooter ${SOURCES} ${HEADERS})

target_include_directories(BlockShooter PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/vendor/raylib-v5.5/include
)

# Raylib Linking & Platform Configuration
if(WIN32)
    target_link_directories(BlockShooter PRIVATE ${CMAKE_SOURCE_DIR}/vendor/raylib-v5.5/lib)
    target_link_libraries(BlockShooter PRIVATE raylib opengl32 gdi32 winmm)
elseif(APPLE)
    find_package(raylib QUIET)
    if(raylib_FOUND)
        target_link_libraries(BlockShooter PRIVATE raylib)
    else()
        find_library(COCOA_LIB Cocoa REQUIRED)
        find_library(IOKIT_LIB IOKit REQUIRED)
        find_library(COREVIDEO_LIB CoreVideo REQUIRED)
        find_library(OPENGL_LIB OpenGL REQUIRED)
        find_library(AUDIOTOOLBOX_LIB AudioToolbox REQUIRED)
        find_library(COREAUDIO_LIB CoreAudio REQUIRED)

        target_link_directories(BlockShooter PRIVATE ${CMAKE_SOURCE_DIR}/vendor/raylib-v5.5/lib)
        target_link_libraries(BlockShooter PRIVATE
            raylib
            ${COCOA_LIB}
            ${IOKIT_LIB}
            ${COREVIDEO_LIB}
            ${OPENGL_LIB}
            ${AUDIOTOOLBOX_LIB}
            ${COREAUDIO_LIB}
        )
        set_target_properties(BlockShooter PROPERTIES
            BUILD_RPATH "@executable_path/../vendor/raylib-v5.5/lib"
            INSTALL_RPATH "@executable_path/../vendor/raylib-v5.5/lib"
        )
    endif()
elseif(UNIX)
    find_package(raylib QUIET)
    if(raylib_FOUND)
        target_link_libraries(BlockShooter PRIVATE raylib)
    else()
        target_link_directories(BlockShooter PRIVATE ${CMAKE_SOURCE_DIR}/vendor/raylib-v5.5/lib)
        target_link_libraries(BlockShooter PRIVATE
            raylib
            m
            pthread
            dl
            rt
            GL
            X11
        )
        set_target_properties(BlockShooter PROPERTIES
            BUILD_RPATH "$ORIGIN/../vendor/raylib-v5.5/lib"
            INSTALL_RPATH "$ORIGIN/../vendor/raylib-v5.5/lib"
        )
    endif()
endif()
