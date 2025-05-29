#ifndef ALL_H
#define ALL_H

/**
 * MAIN INCLUDE HEADER - BLOCK SHOOTER GAME
 * ========================================
 *
 * Header utama yang mengumpulkan semua file header yang dibutuhkan untuk game.
 * File ini berfungsi sebagai central include point untuk seluruh proyek.
 *
 * Berisi:
 * - Library standar C yang diperlukan
 * - Library eksternal (Raylib untuk graphics)
 * - Header custom yang dibuat oleh masing-masing anggota kelompok
 */

 // =============================================================================
 // C STANDARD LIBRARY
 // Library standar C untuk operasi dasar seperti I/O, memory, string, math
 // =============================================================================
#include <stdio.h>      // Input/output operations (printf, scanf, file operations)
#include <stdbool.h>    // Boolean data type (true, false)
#include <string.h>     // String manipulation functions (strcpy, strlen, strcmp)
#include <stdlib.h>     // Memory allocation, random numbers, system calls
#include <math.h>       // Mathematical functions (sin, cos, sqrt, pow)
#include <time.h>       // Time and date functions untuk random seed dan timing

// =============================================================================
// EXTERNAL LIBRARY
// Library pihak ketiga untuk graphics, audio, dan input handling
// =============================================================================
#include "raylib.h"     // Graphics library untuk rendering, input, audio, window management

// =============================================================================
// CUSTOM LIBRARY
// Header file yang dibuat oleh anggota tim untuk berbagai modul game
// =============================================================================
#include "arief.h"      // Asset management, UI rendering, menu system, game screens, utilities
#include "raffi.h"      // Game mechanics, physics, collision detection, gameplay logic
#include "faliq.h"      // Player system, input handling, character management
#include "goklas.h"     // Audio system, sound effects, music management
#include "naira.h"      // Data structures, utilities, helper functions

#endif