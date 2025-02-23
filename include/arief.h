#ifndef ARIEF_H
#define ARIEF_H

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

// Struktur Game:
// Menyimpan status permainan seperti skor, level, dan status permainan.
typedef struct {
    int hiscore;     // Skor tertinggi yang pernah dicapai
    int score;       // Skor permainan saat ini
    int level;       // Level permainan saat ini
    int play;        // Status permainan (misalnya, 1 untuk aktif, 0 untuk berhenti atau pause)
} Game;

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
int gameLevel;
GameState currentState, prevState;
Sound sfxMove, sfxSelect;
Font fontBody, fontHeader;
Texture2D blockTexture;

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

