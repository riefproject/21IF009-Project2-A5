#ifndef ARIEF_H
#define ARIEF_H
#include "defines.h"

<<<<<<< HEAD
#include <raylib.h>


// Define Values
#define LOADING_TIME 5.0f

// Screen Size
#define SCREEN_HEIGHT 640
#define SCREEN_WIDTH 480
#define GAME_SCREEN_HEIGHT 640
#define GAME_SCREEN_WIDTH 320
#define GAME_SCREEN 0, 0, 320, 640

// Key Mapping
#define OK_KEY IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Y)
#define MOVE_UP IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)
#define MOVE_DOWN IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)
#define MOVE_LEFT IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)
#define MOVE_RIGHT IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)
#define SHOOT IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)
#define BACK_KEY IsKeyPressed(KEY_B) || IsKeyPressed(KEY_BACKSPACE)
#define FORWARD_KEY IsKeyPressed(KEY_F)

// Game Constants
#define MAX_WIDTH_BLOCKS 11
#define MAX_LEVELS 11
#define INIT_STATE GameState currentState, prevState;

// Makro Function
#define new(var) (var *)malloc(sizeof(var)) 
#define delete(var) \
    free(var);      \
    var = NULL; 
#define INIT_GAME_VARIABLES int gameLevel; \
    GameState currentState, prevState;\
    Sound sfxMove, sfxSelect;\
    Font fontBody, fontHeader;\
    Texture2D blockTexture;

#define INIT_GAME_VARIABLES int gameLevel; \
    GameState currentState, prevState;\
    Sound sfxMove, sfxSelect;\
    Font fontBody, fontHeader;\
    Texture2D blockTexture;


#define INIT_GAME_VARIABLES int gameLevel; \
    GameState currentState, prevState;\
    Sound sfxMove, sfxSelect;\
    Font fontBody, fontHeader;\
    Texture2D blockTexture;


// Struktur HiScore
// Menyimpan data skor tertinggi yang pernah dicapai oleh pemain.
typedef struct {
    char mode[50];    // Nama pemain
    int score;        // Skor yang dicapai
} HiScore;

// Struktur Settings
// Menyimpan data settings yang digunakan dalam game.
typedef struct {
    int music;   // Status musik
    int sfx;     // Status efek suara
    int mode;    // Mode permainan
} Settings;

// Struktur Shooter:
// Menyimpan posisi pemain (x, y) dan jumlah amunisi yang dimiliki.
typedef struct {
    int x, y;    // Koordinat posisi pemain
    int ammo;    // Jumlah amunisi yang tersedia
} Shooter;

// Struktur Block:
// Mewakili sebuah blok di dalam game, bisa digunakan untuk rintangan atau elemen game lainnya.
// Disusun secara linked list untuk memudahkan manajemen blok secara dinamis.
typedef struct Block {
    int x, y;        // Koordinat posisi blok
    int jumlah;      // Jumlah atau nilai yang terkait dengan blok (misalnya, kekuatan atau tipe)
    struct Block* next;     // Pointer ke blok berikutnya dalam list
} Block;

// Struktur Data Bentukan: LinkedList
typedef struct BlockList {
    Block* head;
    Block* tail;
    int size;
} BlockList;

// Struktur BlockQueue:
// Struktur antrian untuk mengelola blok dengan metode FIFO (First In First Out).
typedef struct {
    Block* items[MAX_WIDTH_BLOCKS];
    int front;    // Pointer ke blok paling depan dalam antrian
    int rear;     // Pointer ke blok paling belakang dalam antrian
} BlockQueue;

// Struktur Bullet:
// Mewakili peluru yang digunakan dalam game. Struktur ini memungkinkan manajemen daftar peluru secara dinamis.
typedef struct Bullet {
    int x, y;        // Koordinat posisi peluru
    struct Bullet* next;    // Pointer ke peluru berikutnya dalam linked list
} Bullet;
=======
// =====================================================================
//                          GAME MANAGEMENT
// =====================================================================

/* Fungsi-fungsi untuk mengelola konteks game dan aset */
Game* createGameContext(void);              // Membuat dan menginisialisasi konteks game baru
void destroyGameContext(Game* game);        // Membersihkan dan menghapus konteks game

// Resources Function
#ifndef ASSET_INPUT_HELPERS
#define ASSET_INPUT_HELPERS
Assets* createAssets(void);
void* getAsset(SLLNode* node, uint id);
void unloadAndFree(SLLNode* head, void (*unloadFunc)(void*));
void destroyAssets(Assets* assets);
#endif

// =====================================================================
//                          BLOCK MANAGEMENT
// =====================================================================
// =====================================================================
//                          USER INTERFACE
// =====================================================================
>>>>>>> cbf3f8c5a6734aae2f3fe2c040e10d9982a3f4de

/* Fungsi-fungsi untuk menangani tampilan dan menu */
int loadingScreen(float* loadingTime);          // Menampilkan layar loading
void mainWindow(void);                          // Menampilkan jendela utama
void mainMenu(GameResources* resources);        // Menampilkan menu utama
void resetHiScores(GameResources* resources);   // Reset skor tertinggi
void rejectReset(GameResources* resources);     // Membatalkan reset skor
void gameOver(GameResources* resources, long long int currentScore);        // Menampilkan layar game over
void showCredits(GameResources* resources);
void selectMode(GameResources* resources);      // Memilih mode permainan
void showControls(GameResources* resources);    // Menampilkan kontrol game
void showHiScore(GameResources* resources);     // Menampilkan skor tertinggi
void showSettings(GameResources* resources);    // Menampilkan pengaturan
bool confirmExit(GameResources* resources);     // Konfirmasi keluar game
void exitGame(GameResources* resources);        // Keluar dari game
void pauseMenu(GameResources* resources);       // Menu pause
void countdownPause(GameResources* resources);  // Hitung mundur pause
bool confirmBack(GameResources* resources);     // Konfirmasi kembali

<<<<<<< HEAD
typedef enum {
    STATE_LOADING,
    STATE_MAIN_MENU,
    STATE_HIGH_SCORES,
    STATE_CONTROLS,
    STATE_SETTINGS,
    STATE_PLAY,
    STATE_QUIT,
    STATE_PAUSE,
    STATE_SELECT_LEVEL
} GameState;

// Implementasi Database
void loadSettings(Settings* settings);
void saveSettings(Settings settings);
void loadHiScores(HiScore scores[]);
void saveHiScores(HiScore scores[]);

// Implementasi BlockList
void addBlock(BlockList* list, int x, int y, int jumlah);
void updateBlock(BlockList* list, int pos, int x, int y, int jumlah);
void removeBlock(BlockList* list, int pos);
void printBlockList(BlockList* list);
int countBlock(BlockList* list);

// Implementasi BlockQueue
void initQueue(BlockQueue* q);
int isEmpty(BlockQueue* q);
int isFull(BlockQueue* q);
int countQueue(BlockQueue* q);
void enqueue(BlockQueue* q, Block* element);
Block* dequeue(BlockQueue* q);
void displayQueue(BlockQueue* q);
void clearQueue(BlockQueue* q);

// Tampilan
extern int gameLevel;
extern GameState currentState, prevState;
extern Sound sfxMove, sfxSelect;
extern Font fontBody, fontHeader;
extern Texture2D blockTexture;

void displayGame();
int loadingScreen(float* loadingTime);
void mainWindow();
void mainMenu();
void selectLevel();
void showControls();
void showHiScore(HiScore scores[]);
void showSettings(Settings* settings);
bool confirmExit();
void exitGame();
void pauseMenu();
void countdownPause();
bool confirmBack();

// debug
#define DBG printf("Haiiii");
#endif

=======
// =====================================================================
//                          GAME MECHANICS
// =====================================================================

/* Fungsi-fungsi untuk logika dan mekanika game */
float getSpeedForMode(Game* game, int mode);              // Mendapatkan kecepatan berdasarkan mode
void displayGame(GameResources* resources);               // Menampilkan game
bool isGameOverCheck(Game* game);                         // Cek kondisi game over
bool isRowFull(Game* game, int row);                      // Cek apakah baris penuh
bool hasActiveBlockBelow(Game* game, int row);            // Cek blok aktif di bawah
void shiftRowsUp(Game* game, int startRow);               // Geser baris ke atas
void handleFullRow(Game* game, int row);                  // Menangani baris penuh
void updateBlocks(Game* game, GameResources* resources);  // Update status blok
void initBlocks(Game* game, GameResources* resources);    // Inisialisasi blok
void drawBlocks(Game* game, GameResources* resources);    // Menggambar blok
void drawBlockUI(Game* game);                             // Menggambar UI blok
void printGrid(Game* game);                               // Menampilkan grid game

/* Fungsi-fungsi untuk pergerakan dan collision */
void handleBlockMovement(Game* game, int minBlocks, int maxBlocks);    // Menangani pergerakan blok
void moveBlocksDown(Game* game);                                       // Menggerakkan blok ke bawah
void generateNewBlocks(Game* game, int minBlocks, int maxBlocks, int* emptyColLength, int totalEmptyColumns);     // Membuat blok baru
int fillCriticalGaps(Game* game, int remainingBlocks, int* emptyColLength);                                       // Mengisi celah kritis
void fillRemainingBlocks(Game* game, int remainingBlocks);             // Mengisi sisa blok
void handleBulletCollisions(Game* game);                               // Menangani collision peluru
bool isValidGridPosition(int x, int y);                                // Validasi posisi grid
void processBulletHit(Game* game, int gridX, int gridY, Bullets* bullets);                                         // Memproses tembakan peluru
void drawGameUI(Game* game, GameResources* resources);         // Menggambar UI game
#endif
>>>>>>> cbf3f8c5a6734aae2f3fe2c040e10d9982a3f4de
