#ifndef DEFINES_H
#define DEFINES_H

#include <raylib.h>
#include <stdbool.h>

// =======================================
//            Game Variables
// =======================================
enum GameState;
enum PowerUpType;
enum SoundAsset;
enum FontAsset;
enum TextureAsset;
struct PowerUp;
struct ScaleFactor;
struct HiScore;
struct Settings;
struct Shooter;
struct Block;
struct BlockList;
struct BlockQueue;
struct Assets;
struct GameResources;
struct Game;
struct openingTransition;
// =======================================
//           Game Constants
// =======================================
// Core Constants
#define LOADING_TIME 2.0f
#define MAX_ROWS 16
#define MAX_COLUMNS 10
#define MAX_WIDTH_BLOCKS 11
#define MAX_LEVELS 11
#define MAX_BULLETS 100

// Block Range Per Level
// (a,b) -> a to b blocks will appear per row
#define GET_RANGE_LEVEL(min, max) *minBlocks = (min); *maxBlocks = (max); break
#define MD1_RANGE GET_RANGE_LEVEL(8, 9)    // Super EZ
#define MD2_RANGE GET_RANGE_LEVEL(7, 9)    // Easy
#define MD3_RANGE GET_RANGE_LEVEL(7, 8)    // Beginner
#define MD4_RANGE GET_RANGE_LEVEL(7, 8)    // Medium
#define MD5_RANGE GET_RANGE_LEVEL(6, 7)    // Hard
#define MD6_RANGE GET_RANGE_LEVEL(6, 7)    // Super Hard
#define MD7_RANGE GET_RANGE_LEVEL(6, 7)    // Expert
#define MD8_RANGE GET_RANGE_LEVEL(5, 6)    // Master
#define MD9_RANGE GET_RANGE_LEVEL(5, 6)    // Legend
#define MD10_RANGE GET_RANGE_LEVEL(5, 5)   // God
#define MD11_RANGE GET_RANGE_LEVEL(5, 9)   // Progressive

// Block Fall Speed Per Level
#define DFT_SPEED 0.6f
#define MD1_SPEED DFT_SPEED                  // 0.60 - Super EZ
#define MD2_SPEED (4.0f/3.0f) * DFT_SPEED    // 0.80 - Easy
#define MD3_SPEED (5.0f/3.0f) * DFT_SPEED    // 1.00 - Beginner
#define MD4_SPEED 2.0f * DFT_SPEED           // 1.20 - Medium
#define MD5_SPEED (7.0f/3.0f) * DFT_SPEED    // 1.40 - Hard
#define MD6_SPEED (8.0f/3.0f) * DFT_SPEED    // 1.60 - Super Hard
#define MD7_SPEED 3.0f * DFT_SPEED           // 1.80 - Expert
#define MD8_SPEED (10.0f/3.0f) * DFT_SPEED   // 2.00 - Master
#define MD9_SPEED (11.0f/3.0f) * DFT_SPEED   // 2.20 - Legend
#define MD10_SPEED 4.0f * DFT_SPEED          // 2.40 - God
// MD11: Progressive - uses existing system

// =======================================
//           Screen Settings
// =======================================
// Base Screen
#define MIN_SCREEN_WIDTH 480
#define MIN_SCREEN_HEIGHT 640
#define ASPECT_RATIO_WIDTH 3
#define ASPECT_RATIO_HEIGHT 4

// Game Screen
#define BASE_WIDTH MIN_SCREEN_WIDTH
#define BASE_HEIGHT MIN_SCREEN_HEIGHT
#define GAME_SCREEN_HEIGHT 640
#define GAME_SCREEN_WIDTH 320
#define GAME_SCREEN 0, 0, 320, 640

// Screen Helpers
#define auto_x(var) (int)(var * scale.x)
#define auto_y(var) (int)(var * scale.y)

// =======================================
//           Input Controls
// =======================================
// Action Keys
#define OK_KEY IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Y)
#define SHOOT_KEY IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
#define BACK_KEY IsKeyPressed(KEY_B) || IsKeyPressed(KEY_BACKSPACE)
#define FORWARD_KEY IsKeyPressed(KEY_F)

// Movement Keys
#define MOVE_UP IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)
#define MOVE_DOWN IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)
#define MOVE_LEFT IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)
#define MOVE_RIGHT IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)

// Release Keys
#define SHOOT_RELEASE IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_ENTER) || IsMouseButtonDown(MOUSE_LEFT_BUTTON)
// =======================================
//           Helper Macros
// =======================================
// Memory Management
#define new(var) (var *)malloc(sizeof(var)) 
#define delete(var) \
      free(var);      \
      var = NULL; 
#define len(var) sizeof(var) / sizeof(var[0])

 // Resource Access
#define SOUND(rsc, id) ((rsc)->assets->sounds[id])
#define FONT(rsc, id) ((rsc)->assets->fonts[id])
#define TEXTURE(rsc, id) ((rsc)->assets->textures[id])

// Debug Helper
#define DBG printf("Haiiii");


// =====================================================================
//                          STRUKTUR DATA BENTUKAN
// =====================================================================

/* Struktur Block:
 * Mewakili sebuah blok di dalam game, bisa digunakan untuk rintangan atau elemen game lainnya.
 * Disusun secara linked list untuk memudahkan manajemen blok secara dinamis.*/
typedef struct Block {
    int x, y;               // Koordinat posisi blok
    int jumlah;             // Jumlah atau nilai yang terkait dengan blok (misalnya, kekuatan atau tipe)
    struct Block* next;     // Pointer ke blok berikutnya dalam list
} Block;

/* Struktur Data Bentukan: LinkedList
 * Struktur linear dinamis untuk menampung blok */
typedef struct BlockList {
    Block* head;
    Block* tail;
    int size;
} BlockList;

/* Struktur BlockQueue:
 * Struktur antrian untuk mengelola blok dengan metode FIFO (First In First Out). */
typedef struct BlockQueue {
    Block* items[MAX_WIDTH_BLOCKS];
    int front;    // Pointer ke blok paling depan dalam antrian
    int rear;     // Pointer ke blok paling belakang dalam antrian
} BlockQueue;


// =====================================================================
//                          GAME VARIABLES
// =====================================================================

typedef enum SoundAsset {
    SOUND_MOVE,
    SOUND_SELECT,
    SOUND_COUNT
} SoundAsset;

typedef enum FontAsset {
    FONT_BODY,
    FONT_HEADER,
    FONT_COUNT
} FontAsset;

typedef enum TextureAsset {
    TEXTURE_BLOCK,
    TEXTURE_BULLET,
    TEXTURE_SHOOTER_L,
    TEXTURE_SHOOTER_M,
    TEXTURE_SHOOTER_R,
    TEXTURE_SHOOTER_T,
    TEXTURE_COUNT
} TextureAsset;

typedef enum GameState {
    STATE_LOADING,
    STATE_MAIN_MENU,
    STATE_HIGH_SCORES,
    STATE_CONTROLS,
    STATE_SETTINGS,
    STATE_PLAY,
    STATE_QUIT,
    STATE_PAUSE,
    STATE_SELECT_LEVEL,
    STATE_GAME_OVER
} GameState;

typedef enum PowerUpType {
    POWERUP_NONE = 0,
    POWERUP_SPEED_UP,
    POWERUP_SLOW_DOWN,
    POWERUP_UNLIMITED_AMMO,
    POWERUP_EXTRA_LIFE,
    POWERUP_BOMB,
    POWERUP_SPECIAL_BULLET,
    POWERUP_RANDOM,
    POWERUP_COUNT
} PowerUpType;

/* Strukture ScaleFactor
 * Menyimpan faktor skala untuk mengubah ukuran objek dalam game.
 * Digunakan untuk mengubah ukuran objek dalam game agar sesuai dengan resolusi layar.
 */
typedef struct ScaleFactor {
    float x;
    float y;
} ScaleFactor;

/* Struktur HiScore
 * Menyimpan data skor tertinggi yang pernah dicapai oleh pemain. */
typedef struct HiScore {
    char mode[50];              // Nama pemain
    long long int score;        // Skor yang dicapai
} HiScore;

/* Struktur Settings
 * Menyimpan data settings yang digunakan dalam game. */
typedef struct Settings {
    int music;   // Status musik
    int sfx;     // Status efek suara
    int mode;    // Mode permainan
} Settings;

/* Struktur Shooter:
* Menyimpan posisi pemain (x, y) dan jumlah amunisi yang dimiliki. */
typedef struct Shooter {
    int x, y;    // Koordinat posisi pemain
    int ammo;    // Jumlah amunisi yang tersedia
} Shooter;


typedef struct {
    Vector2 position;
    int direction;
    bool active;
} Bullets;

/* Struktur Assets:
* Menyimpan sumber daya game seperti suara, font, dan tekstur.
* Digunakan untuk mengelola semua aset yang dibutuhkan game. */
typedef struct Assets {
    Sound sounds[SOUND_COUNT];           // Array untuk menyimpan efek suara
    Font fonts[FONT_COUNT];              // Array untuk menyimpan font
    Texture2D textures[TEXTURE_COUNT];   // Array untuk menyimpan tekstur
} Assets;

/* Struktur PowerUp:
* Mengelola power-up yang dapat diambil pemain dalam permainan.
* Mengatur tipe, durasi, dan status aktif power-up. */
typedef struct PowerUp {
    PowerUpType type;      // Jenis power-up
    float duration;        // Durasi efek power-up
    bool active;          // Status aktif power-up
    float timer;          // Timer untuk durasi power-up
} PowerUp;

/* Struktur GameResources:
* Menyimpan sumber daya dan status umum permainan.
* Mengelola aset, pengaturan, skor tertinggi, dan status permainan. */
typedef struct GameResources {
    Assets* assets;                    // Pointer ke aset game
    Settings settings;                 // Pengaturan game
    HiScore scores[MAX_LEVELS];        // Array skor tertinggi
    GameState currentState;            // Status game saat ini
    GameState prevState;              // Status game sebelumnya
    int gameLevel;                    // Level game saat ini
} GameResources;

/* Struktur Game:
* Struktur utama yang menyimpan seluruh status permainan.
* Mengelola peluru, grid permainan, skor, power-up, dan berbagai status game. */
typedef struct Game {
    Bullets bullets[MAX_BULLETS];      // Array peluru yang aktif
    int bulletCount;                   // Jumlah peluru saat ini
    bool canShoot;                     // Status bisa menembak
    float reloadTimer;                 // Timer untuk reload
    int playerDirection;               // Arah hadap pemain
    bool grid[MAX_ROWS][MAX_COLUMNS];  // Grid permainan
    int frameCounter;                  // Penghitung frame
    int rowAddDelay;                   // Delay penambahan baris
    long long int score;               // Skor pemain
    float blockFallTimer;              // Timer jatuhnya block
    bool gameOver;                     // Status game over
    int lives;                         // Jumlah nyawa pemain
    float laserCooldown;               // Cooldown laser
    float laserDuration;               // Durasi efek laser
    bool laserActive;                  // Status laser aktif
    float powerupTimer;                // Timer power-up
    float powerupDuration;             // Durasi power-up
    PowerUp currentPowerup;            // Power-up yang sedang aktif
    Vector2 powerupPosition;           // Posisi power-up
    bool powerupActive;                // Status power-up aktif
    struct {
        PowerUpType type;              // Tipe power-up
        float duration;                // Durasi power-up
        bool active;                   // Status aktif
    } activePowerups[3];              // Array power-up aktif (maksimal 3)
    int activeEffectsCount;           // Jumlah efek yang aktif
} Game;

typedef struct openingTransition { // Ditambahkan oleh Faliq
    float progress; // Ditambahkan oleh Faliq
}openingTransition;// Ditambahkan oleh Faliq

#endif