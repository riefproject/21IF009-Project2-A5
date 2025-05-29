#ifndef DEFINES_H
#define DEFINES_H

#include <stdbool.h>
#include "raylib.h"
#include "linkedlist.h"
#include "queue.h"
#include "scale.h"

// =============================================================================
// TYPE DEFINITIONS AND FORWARD DECLARATIONS
// Deklarasi forward untuk semua struktur data yang akan digunakan
// =============================================================================
typedef enum GameState GameState;
typedef enum SoundAsset SoundAsset;
typedef enum FontAsset FontAsset;
typedef enum TextureAsset TextureAsset;
typedef enum Skin Skin;
typedef enum TextureSkin TextureSkin;
typedef enum BgTextures BgTextures;
typedef enum BgModeTextures BgModeTextures;
typedef enum PowerUpType PowerUpType;
typedef enum TypeofAssets TypeofAssets;
typedef struct Block Block;
typedef struct Bullets Bullets;
typedef struct HiScore HiScore;
typedef struct Settings Settings;
typedef struct Shooter Shooter;
typedef struct PowerUp PowerUp;
typedef struct InputAsset InputAsset;
typedef struct Assets Assets;
typedef struct GameResources GameResources;
typedef struct Game Game;
typedef struct openingTransition openingTransition;
typedef struct PowerUpVisuals PowerUpVisuals;

// =============================================================================
// BASIC TYPE ALIASES
// Alias untuk tipe data yang sering digunakan
// =============================================================================
typedef unsigned int uint;
typedef long long int ll;
typedef unsigned long long int ull;

// =============================================================================
// CORE GAME CONSTANTS
// Konstanta dasar untuk pengaturan game dan gameplay
// =============================================================================

// Game Timing and Loading
#define LOADING_TIME 2.0f

// Grid Dimensions
#define MAX_ROWS 17
#define MAX_COLUMNS 10
#define MAX_WIDTH_BLOCKS 11
#define MAX_LEVELS 11
#define MAX_BULLETS 100

// =============================================================================
// DIFFICULTY SYSTEM CONFIGURATION
// Pengaturan tingkat kesulitan dengan range blok dan kecepatan
// =============================================================================

// Block Range Macros - Menentukan jumlah blok yang muncul per baris
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

// Block Fall Speed Configuration - Kecepatan jatuh blok per level
#define DFT_SPEED 0.3f
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
// MD11: Progressive - menggunakan sistem yang sudah ada

// =============================================================================
// SCREEN AND DISPLAY CONFIGURATION
// Pengaturan resolusi layar dan area permainan
// =============================================================================

// Base Screen Resolution
#define MIN_SCREEN_WIDTH 480
#define MIN_SCREEN_HEIGHT 640
#define ASPECT_RATIO_WIDTH 3
#define ASPECT_RATIO_HEIGHT 4

// Game Area Dimensions
#define BASE_WIDTH MIN_SCREEN_WIDTH
#define BASE_HEIGHT MIN_SCREEN_HEIGHT
#define GAME_SCREEN_HEIGHT 640
#define GAME_SCREEN_WIDTH 320
#define GAME_SCREEN 0, 0, 320, 640

// Screen Scaling Helpers - Menyesuaikan koordinat dengan resolusi layar
#define auto_x(var) (int)(var * GetScreenScaleFactor().x)
#define auto_y(var) (int)(var * GetScreenScaleFactor().y)

// =============================================================================
// INPUT CONTROL DEFINITIONS
// Definisi input keyboard dan mouse untuk kontrol game
// =============================================================================

// Action Keys - Tombol untuk aksi utama
#define OK_KEY IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Y)
#define SHOOT_KEY IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
#define BACK_KEY IsKeyPressed(KEY_B) || IsKeyPressed(KEY_BACKSPACE)
#define FORWARD_KEY IsKeyPressed(KEY_F)

// Movement Keys - Tombol untuk pergerakan
#define MOVE_UP IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)
#define MOVE_DOWN IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)
#define MOVE_LEFT IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)
#define MOVE_RIGHT IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)

// Continuous Input - Input yang ditekan terus menerus
#define SHOOT_RELEASE IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_ENTER) || IsMouseButtonDown(MOUSE_LEFT_BUTTON)

// =============================================================================
// UTILITY MACROS
// Makro utilitas untuk kemudahan development
// =============================================================================

// Array Length Calculator
#define len(var) sizeof(var) / sizeof(var[0])

// Debug Helper
#define DBG printf("[LOG] Haiiii");

// =============================================================================
// GAME STATE ENUMERATION
// Enumerasi untuk berbagai state dalam game
// =============================================================================
typedef enum GameState {
    STATE_LOADING,        // Layar loading awal
    STATE_MAIN_MENU,      // Menu utama
    STATE_HIGH_SCORES,    // Layar high scores
    STATE_CONTROLS,       // Layar panduan kontrol
    STATE_SETTINGS,       // Layar pengaturan
    STATE_PLAY,           // Gameplay aktif
    STATE_QUIT,           // Keluar dari game
    STATE_PAUSE,          // Game di-pause
    STATE_SELECT_LEVEL,   // Pemilihan level/mode
    STATE_GAME_OVER,      // Layar game over
    STATE_SCENE           // Scene khusus
} GameState;

// =============================================================================
// ASSET ENUMERATION SYSTEM
// Enumerasi untuk mengelola berbagai aset game
// =============================================================================

// Sound Assets - Identifikasi file audio
typedef enum SoundAsset {
    SOUND_MOVE,           // Suara pergerakan
    SOUND_SELECT,         // Suara pemilihan menu
    SOUND_SHOOT,          // Suara tembakan
    SOUND_DEATH,          // Suara kematian
    SOUND_SPECIAL_BULLET, // Suara peluru khusus
    SOUND_HEAL,           // Suara healing
    SOUND_POISON,         // Suara poison effect
    SOUND_SLOWDOWN,       // Suara slowdown effect
    SOUND_SPEEDUP,        // Suara speedup effect
    SOUND_COUNT
} SoundAsset;

// Font Assets - Identifikasi file font
typedef enum FontAsset {
    FONT_BODY,    // Font untuk teks body/content
    FONT_HEADER,  // Font untuk header/judul
    FONT_INGAME,  // Font untuk UI dalam game
    FONT_COUNT
} FontAsset;

// Texture Assets - Identifikasi texture utama
typedef enum TextureAsset {
    TEXTURE_BLOCK,         // Texture blok
    TEXTURE_BULLET,        // Texture peluru
    TEXTURE_HEART,         // Texture ikon hati/nyawa
    TEXTURE_LASER_BUTTON,  // Texture tombol laser
    TEXTURE_RANDOM,        // Texture random power-up
    TEXTURE_SPEEDUP,       // Texture speedup power-up
    TEXTURE_SLOWDOWN,      // Texture slowdown power-up
    TEXTURE_MIN1_HP,       // Texture minus 1 HP
    TEXTURE_PLS1_HP,       // Texture plus 1 HP
    TEXTURE_SPECIAL_BULLET,// Texture peluru khusus
    TEXTURE_WHITE_ICON,    // Texture ikon putih
    TEXTURE_COUNT
} TextureAsset;

// Skin System - Texture untuk skin pemain
typedef enum Skin {
    TEXTURE_SKIN_1 = TEXTURE_COUNT,
    TEXTURE_SKIN_2,
    TEXTURE_SKIN_COUNT
} Skin;

// Shooter Skin Textures - Texture untuk berbagai pose shooter
typedef enum TextureSkin {
    SHOOTER_SKIN_1_L = TEXTURE_SKIN_COUNT, // Skin 1 menghadap kiri
    SHOOTER_SKIN_1_M,                      // Skin 1 menghadap tengah
    SHOOTER_SKIN_1_R,                      // Skin 1 menghadap kanan
    SHOOTER_SKIN_1_T,                      // Skin 1 menembak
    SHOOTER_SKIN_2_L,                      // Skin 2 menghadap kiri
    SHOOTER_SKIN_2_M,                      // Skin 2 menghadap tengah
    SHOOTER_SKIN_2_R,                      // Skin 2 menghadap kanan
    SHOOTER_SKIN_2_T,                      // Skin 2 menembak
    SHOOTER_SKIN_COUNT
} TextureSkin;

// Background Textures - Texture untuk berbagai layar
typedef enum BgTextures {
    BG_PLAY,        // Background gameplay
    BG_MAIN_MENU,   // Background menu utama
    BG_SETTINGS,    // Background pengaturan
    BG_HIGHSCORES,  // Background high scores
    BG_PAUSED,      // Background pause
    BG_CONTROLS,    // Background kontrol
    BG_CONFIRM,     // Background konfirmasi
    BG_PLAIN,       // Background polos
    CREDIT_SCENE,   // Background credits
    GAME_AREA,      // Area permainan
    UI_AREA,        // Area UI
    BG_LOADING,     // Background loading
    ICON_LOADING,   // Ikon loading
    BG_COUNT
} BgTextures;

// Mode-Specific Backgrounds - Background khusus untuk setiap mode
typedef enum BgModeTextures {
    BGMODE_SUPER_EZ,    // Background mode Super Easy
    BGMODE_EZ,          // Background mode Easy
    BGMODE_BEGINNER,    // Background mode Beginner
    BGMODE_MEDIUM,      // Background mode Medium
    BGMODE_HARD,        // Background mode Hard
    BGMODE_SUPER_HARD,  // Background mode Super Hard
    BGMODE_EXPERT,      // Background mode Expert
    BGMODE_MASTER,      // Background mode Master
    BGMODE_LEGEND,      // Background mode Legend
    BGMODE_GOD,         // Background mode God
    BGMODE_PROGRESSIVE, // Background mode Progressive
    BGMODE_COUNT
} BgModeTextures;

// Power-Up System - Jenis-jenis power-up yang tersedia
typedef enum PowerUpType {
    POWERUP_NONE = 0,      // Tidak ada power-up
    POWERUP_SPEED_UP,      // Mempercepat pergerakan
    POWERUP_SLOW_DOWN,     // Memperlambat blok
    POWERUP_EXTRA_LIFE,    // Tambahan nyawa
    POWERUP_BOMB,          // Bom perusak blok
    POWERUP_SPECIAL_BULLET,// Peluru khusus
    POWERUP_RANDOM,        // Power-up random
    POWERUP_COUNT
} PowerUpType;

// Asset Type Classification - Klasifikasi tipe aset
typedef enum TypeofAssets {
    TYPE_SOUND,    // Aset audio
    TYPE_FONT,     // Aset font
    TYPE_TEXTURE   // Aset texture
} TypeofAssets;

// =============================================================================
// CORE DATA STRUCTURES
// Struktur data utama untuk gameplay
// =============================================================================

// Struktur Block - Representasi blok individual dalam grid
typedef struct Block {
    bool active; // Status aktif blok (true = terlihat, false = hancur)
    int pos;     // Posisi block dalam grid (untuk indexing)
} Block;

// Struktur Bullets - Representasi peluru yang ditembakkan
typedef struct Bullets {
    Vector2 position;  // Posisi peluru di koordinat layar
    bool active;       // Status aktif peluru (true = bergerak, false = hancur)
} Bullets;

// =============================================================================
// GAME MANAGEMENT STRUCTURES
// Struktur untuk mengelola berbagai aspek permainan
// =============================================================================

// Struktur HiScore - Menyimpan record skor tertinggi
typedef struct HiScore {
    char mode[50];    // Nama mode permainan
    ll score;         // Skor tertinggi yang dicapai
} HiScore;

// Struktur Settings - Menyimpan pengaturan permainan
typedef struct Settings {
    int music;   // Status musik (0 = off, 1 = on)
    int sfx;     // Status efek suara (0 = off, 1 = on)
    int mode;    // Mode permainan yang dipilih
    uint skin;   // Skin pemain yang digunakan
} Settings;

// Struktur Shooter - Representasi pemain
typedef struct Shooter {
    int x, y;    // Koordinat posisi pemain di grid
    int ammo;    // Jumlah amunisi yang tersedia
} Shooter;

// Struktur PowerUp - Manajemen power-up aktif
typedef struct PowerUp {
    PowerUpType type;   // Jenis power-up yang aktif
    float duration;     // Durasi total efek power-up
    bool active;        // Status aktif power-up
    float timer;        // Timer countdown untuk power-up
} PowerUp;

// =============================================================================
// ASSET MANAGEMENT SYSTEM
// Sistem untuk mengelola dan mengakses aset game
// =============================================================================

// Struktur InputAsset - Wrapper untuk aset yang dimuat
typedef struct InputAsset {
    uint id;      // ID unik untuk aset
    void* data;   // Pointer ke data aset (Sound, Font, Texture, dll)
} InputAsset;

// Struktur Assets - Container untuk semua aset game
typedef struct Assets {
    SingleLinkedList sounds;    // Linked list untuk aset audio
    SingleLinkedList fonts;     // Linked list untuk aset font
    SingleLinkedList textures;  // Linked list untuk aset texture
    SingleLinkedList bg;        // Linked list untuk background
    SingleLinkedList bgMode;    // Linked list untuk background mode
    SingleLinkedList txMode;    // Linked list untuk text mode
    SingleLinkedList Shooter;   // Linked list untuk aset shooter
} Assets;

// =============================================================================
// ASSET FUNCTION DECLARATIONS
// Deklarasi fungsi untuk manajemen aset (diimpor dari arief.h)
// =============================================================================
#ifndef ASSET_INPUT_HELPERS
#define ASSET_INPUT_HELPERS
void GetAdjustedWindowSize(int width, int height, int* outWidth, int* outHeight);
InputAsset* inputAssets(TypeofAssets type, uint id, const char* path);
Assets* createAssets(void);
void* getAsset(SLLNode* head, uint id);
void unloadAndFree(SLLNode* head, TypeofAssets type);
void destroyAssets(Assets* assets);
#endif

// =============================================================================
// ASSET ACCESS MACROS
// Makro untuk mengakses aset dengan mudah dan type-safe
// =============================================================================
#define SOUND(rsc, id) (*(Sound*)(getAsset((rsc)->assets->sounds.head, (id))))
#define FONT(rsc, id) (*(Font*)(getAsset((rsc)->assets->fonts.head, (id))))
#define TEXTURE(rsc, id) (*(Texture2D*)(getAsset((rsc)->assets->textures.head, (id))))
#define BG(rsc, id) (*(Texture2D*)(getAsset((rsc)->assets->bg.head, (id))))
#define BGMODE(rsc, id) (*(Texture2D*)(getAsset((rsc)->assets->bgMode.head, (id))))
#define TXMODE(rsc, id) (*(Texture2D*)(getAsset((rsc)->assets->txMode.head, (id))))

// =============================================================================
// MAIN GAME STRUCTURES
// Struktur utama untuk mengelola seluruh state permainan
// =============================================================================

// Struktur GameResources - Manajemen resource dan state global
typedef struct GameResources {
    Assets* assets;              // Pointer ke semua aset game
    Settings settings;           // Pengaturan permainan
    SingleLinkedList scores;     // Linked list skor tertinggi
    GameState currentState;      // State game saat ini
    GameState prevState;         // State game sebelumnya
    int gameLevel;              // Level permainan saat ini
} GameResources;

// Struktur Game - State lengkap permainan yang sedang berlangsung
typedef struct Game {
    // Bullet Management
    SingleLinkedList* bullets;    // Linked list peluru aktif
    int bulletCount;             // Jumlah peluru yang ada
    bool canShoot;               // Flag bisa menembak atau tidak
    float reloadTimer;           // Timer untuk reload peluru
    int playerDirection;         // Arah hadap pemain (kiri/kanan/tengah)

    // Grid System
    DoublyLinkedList* grid;      // Grid permainan sebagai doubly linked list

    // Game Timing and Mechanics
    int frameCounter;            // Counter frame untuk timing
    int rowAddDelay;            // Delay penambahan baris baru
    ll score;                   // Skor pemain saat ini
    float blockFallTimer;       // Timer untuk jatuhnya blok
    bool gameOver;              // Flag game over
    int lives;                  // Jumlah nyawa pemain

    // Special Abilities
    float laserCooldown;        // Cooldown ability laser
    float laserDuration;        // Durasi efek laser
    bool laserActive;           // Status laser sedang aktif

    // Power-Up System
    float powerupTimer;         // Timer untuk power-up
    float powerupDuration;      // Durasi power-up
    PowerUp currentPowerup;     // Power-up yang sedang aktif
    Vector2 powerupPosition;    // Posisi power-up di layar
    bool powerupActive;         // Status power-up aktif
    Queue activePowerups;       // Queue power-up yang aktif (maksimal 3)
    int activeEffectsCount;     // Jumlah efek yang sedang berjalan
} Game;

// =============================================================================
// VISUAL AND TRANSITION STRUCTURES
// Struktur untuk efek visual dan transisi
// =============================================================================

// Struktur openingTransition - Manajemen transisi pembuka
typedef struct openingTransition {
    float progress; // Progress transisi (0.0 - 1.0)
} openingTransition;

// Struktur PowerUpVisuals - Data visual untuk power-up
typedef struct PowerUpVisuals {
    Texture2D texture;    // Texture untuk power-up
    Color timerColor;     // Warna timer power-up
} PowerUpVisuals;

#endif