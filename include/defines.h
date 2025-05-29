#ifndef DEFINES_H
#define DEFINES_H

#include <stdbool.h>
#include "raylib.h"
#include "linkedlist.h"
#include "queue.h"
#include "scale.h"
// =======================================
//            Game Variables
// =======================================
typedef enum GameState GameState;
typedef enum PowerUpType PowerUpType;
typedef enum SoundAsset SoundAsset;
typedef enum FontAsset FontAsset;
typedef enum TextureAsset TextureAsset;
typedef enum BgTextures BgTextures;
typedef enum BgModeTextures BgModeTextures;
typedef enum TypeofAssets TypeofAssets;
typedef struct PowerUp PowerUp;
typedef struct HiScore HiScore;
typedef struct Settings Settings;
typedef struct Shooter Shooter;
typedef struct BlockList BlockList;
typedef struct BlockQueue BlockQueue;
typedef struct Assets Assets;
typedef struct GameResources GameResources;
typedef struct Game Game;
typedef struct openingTransition openingTransition;
// =======================================
//           Game Constants
// =======================================
// Core Constants
#define LOADING_TIME 2.0f
#define MAX_ROWS 17
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
#define auto_x(var) (int)(var * GetScreenScaleFactor().x)
#define auto_y(var) (int)(var * GetScreenScaleFactor().y)

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
#define len(var) sizeof(var) / sizeof(var[0])


// Debug Helper
#define DBG printf("[LOG] Haiiii");

typedef unsigned int uint;
typedef long long int ll;
typedef unsigned long long int ull;

// =====================================================================
//                          STRUKTUR DATA BENTUKAN
// =====================================================================

typedef struct Block {
    bool active; // Status aktif block
    int pos;     // Posisi block dalam grid
}Block;

// =====================================================================
//                          GAME VARIABLES
// =====================================================================

typedef enum SoundAsset {
    SOUND_MOVE,
    SOUND_SELECT,
    SOUND_SHOOT,
    SOUND_DEATH,
    SOUND_SPECIAL_BULLET,
    SOUND_HEAL,
    SOUND_POISON,
    SOUND_SLOWDOWN,
    SOUND_SPEEDUP,
    SOUND_COUNT
} SoundAsset;

typedef enum FontAsset {
    FONT_BODY,
    FONT_HEADER,
    FONT_INGAME,
    FONT_COUNT
} FontAsset;

typedef enum TextureAsset {
    TEXTURE_BLOCK,
    TEXTURE_BULLET,
    TEXTURE_HEART,
    TEXTURE_LASER_BUTTON,
    TEXTURE_RANDOM,
    TEXTURE_SPEEDUP,
    TEXTURE_SLOWDOWN,
    TEXTURE_MIN1_HP,
    TEXTURE_PLS1_HP,
    TEXTURE_SPECIAL_BULLET,
    TEXTURE_WHITE_ICON,
    TEXTURE_COUNT
} TextureAsset;

typedef enum Skin {
    TEXTURE_SKIN_1 = TEXTURE_COUNT,
    TEXTURE_SKIN_2,
    TEXTURE_SKIN_COUNT
} Skin;


typedef enum TextureSkin {
    SHOOTER_SKIN_1_L = TEXTURE_SKIN_COUNT,
    SHOOTER_SKIN_1_M,
    SHOOTER_SKIN_1_R,
    SHOOTER_SKIN_1_T,
    SHOOTER_SKIN_2_L,
    SHOOTER_SKIN_2_M,
    SHOOTER_SKIN_2_R,
    SHOOTER_SKIN_2_T,
    SHOOTER_SKIN_COUNT
} TextureSkin;

typedef enum BgTextures {
    BG_PLAY,
    BG_MAIN_MENU,
    BG_SETTINGS,
    BG_HIGHSCORES,
    BG_PAUSED,
    BG_CONTROLS,
    BG_CONFIRM,
    BG_PLAIN,
    CREDIT_SCENE,
    GAME_AREA,
    UI_AREA,
    BG_LOADING,
    ICON_LOADING,
    BG_COUNT
}BgTextures;

typedef enum BgModeTextures {
    BGMODE_SUPER_EZ,
    BGMODE_EZ,
    BGMODE_BEGINNER,
    BGMODE_MEDIUM,
    BGMODE_HARD,
    BGMODE_SUPER_HARD,
    BGMODE_EXPERT,
    BGMODE_MASTER,
    BGMODE_LEGEND,
    BGMODE_GOD,
    BGMODE_PROGRESSIVE,
    BGMODE_COUNT
}BgModeTextures;

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
    STATE_GAME_OVER,
    STATE_SCENE
} GameState;

typedef enum PowerUpType {
    POWERUP_NONE = 0,
    POWERUP_SPEED_UP,
    POWERUP_SLOW_DOWN,
    POWERUP_EXTRA_LIFE,
    POWERUP_BOMB,
    POWERUP_SPECIAL_BULLET,
    POWERUP_RANDOM,
    POWERUP_COUNT
} PowerUpType;

typedef enum TypeofAssets {
    TYPE_SOUND,
    TYPE_FONT,
    TYPE_TEXTURE
}TypeofAssets;
/* Struktur HiScore
 * Menyimpan data skor tertinggi yang pernah dicapai oleh pemain. */
typedef struct HiScore {
    char mode[50];              // Nama pemain
    ll score;        // Skor yang dicapai
} HiScore;

/* Struktur Settings
 * Menyimpan data settings yang digunakan dalam game. */
typedef struct Settings {
    int music;   // Status musik
    int sfx;     // Status efek suara
    int mode;    // Mode permainan
    uint skin;    // Skin yang digunakan
} Settings;

/* Struktur Shooter:
* Menyimpan posisi pemain (x, y) dan jumlah amunisi yang dimiliki. */
typedef struct Shooter {
    int x, y;    // Koordinat posisi pemain
    int ammo;    // Jumlah amunisi yang tersedia
} Shooter;


/* Struktur Bullets:
 * Menyimpan informasi tentang peluru yang ditembakkan dalam game. */
typedef struct {
    Vector2 position;  // Posisi peluru di dalam game
    bool active;       // Status apakah peluru sedang aktif atau tidak
} Bullets;

/* Struktur Assets:
* Menyimpan sumber daya game seperti suara, font, dan tekstur.
* Digunakan untuk mengelola semua aset yang dibutuhkan game. */
typedef struct Assets {
    SingleLinkedList sounds;    // Array untuk menyimpan efek suara
    SingleLinkedList fonts;     // Array untuk menyimpan font
    SingleLinkedList textures;  // Array untuk menyimpan tekstur
    SingleLinkedList bg;        // Array untuk menyimpan background
    SingleLinkedList bgMode;    // Array untuk menyimpan background for mode
    SingleLinkedList txMode;    // Array untuk menyimpan text for mode
    SingleLinkedList Shooter;    // Array untuk menyimpan text for mode
} Assets;
typedef struct InputAsset {
    uint id;
    void* data;
}InputAsset;
// Resources Function
#ifndef ASSET_INPUT_HELPERS
#define ASSET_INPUT_HELPERS
void GetAdjustedWindowSize(int width, int height, int* outWidth, int* outHeight);
InputAsset* inputAssets(TypeofAssets type, uint id, const char* path);
Assets* createAssets(void);
void* getAsset(SLLNode* head, uint id);
void unloadAndFree(SLLNode* head, TypeofAssets type);
void destroyAssets(Assets* assets);
#endif

// Resource Access
#define SOUND(rsc, id) (*(Sound*)(getAsset((rsc)->assets->sounds.head, (id))))
#define FONT(rsc, id) (*(Font*)(getAsset((rsc)->assets->fonts.head, (id))))
#define TEXTURE(rsc, id) (*(Texture2D*)(getAsset((rsc)->assets->textures.head, (id))))
#define BG(rsc, id) (*(Texture2D*)(getAsset((rsc)->assets->bg.head, (id))))
#define BGMODE(rsc, id) (*(Texture2D*)(getAsset((rsc)->assets->bgMode.head, (id))))
#define TXMODE(rsc, id) (*(Texture2D*)(getAsset((rsc)->assets->txMode.head, (id))))

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
    SingleLinkedList scores;        // Array skor tertinggi
    GameState currentState;            // Status game saat ini
    GameState prevState;              // Status game sebelumnya
    int gameLevel;                    // Level game saat ini
} GameResources;

/* Struktur Game:
* Struktur utama yang menyimpan seluruh status permainan.
* Mengelola peluru, grid permainan, skor, power-up, dan berbagai status game. */
typedef struct Game {
    SingleLinkedList* bullets;      // Array peluru yang aktif
    int bulletCount;                   // Jumlah peluru saat ini
    bool canShoot;                     // Status bisa menembak
    float reloadTimer;                 // Timer untuk reload
    int playerDirection;               // Arah hadap pemain
    // grid with nested linkedlist
    DoublyLinkedList* grid;
    int frameCounter;                  // Penghitung frame
    int rowAddDelay;                   // Delay penambahan baris
    ll score;                          // Skor pemain
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
    Queue activePowerups;              // Array power-up aktif (maksimal 3)
    int activeEffectsCount;           // Jumlah efek yang aktif
} Game;

typedef struct openingTransition { // Ditambahkan oleh Faliq
    float progress; // Ditambahkan oleh Faliq
}openingTransition;// Ditambahkan oleh Faliq
typedef struct PowerUpVisuals {
    Texture2D texture;
    Color timerColor;
} PowerUpVisuals;
#endif