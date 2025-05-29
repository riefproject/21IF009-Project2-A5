#ifndef ARIEF_H
#define ARIEF_H
#include "defines.h"

// =============================================================================
// ASSET AND RESOURCE MANAGEMENT
// Fungsi-fungsi untuk mengelola aset game, memuat dan membebaskan resource
// =============================================================================
#ifndef ASSET_INPUT_HELPERS
#define ASSET_INPUT_HELPERS

// Menyesuaikan ukuran window berdasarkan resolusi layar
// Menghitung ukuran optimal untuk mencegah window terlalu besar/kecil
void GetAdjustedWindowSize(int width, int height, int* outWidth, int* outHeight);

// Memuat aset berdasarkan tipe dan path yang diberikan
// Mengembalikan pointer ke InputAsset yang berisi data aset
InputAsset* inputAssets(TypeofAssets type, uint id, const char* path);

// Membuat struktur Assets kosong untuk menyimpan semua aset game
// Menginisialisasi linked list untuk berbagai tipe aset
Assets* createAssets(void);

// Mencari dan mengembalikan aset berdasarkan ID dari linked list
// Melakukan traversal pada linked list untuk menemukan aset yang sesuai
void* getAsset(SLLNode* head, uint id);

// Membebaskan memori aset berdasarkan tipe dan menghapus dari linked list
// Memanggil fungsi unload yang sesuai dengan tipe aset
void unloadAndFree(SLLNode* head, TypeofAssets type);

// Menghancurkan seluruh struktur Assets dan membebaskan semua memori
// Memanggil unloadAndFree untuk semua tipe aset lalu dealokasi struktur utama
void destroyAssets(Assets* assets);
#endif

// =============================================================================
// SETTINGS AND CONFIGURATION
// Fungsi-fungsi untuk mengelola pengaturan game dan konfigurasi
// =============================================================================

// Memuat pengaturan game dari file konfigurasi
// Membaca file settings dan mengisi struktur Settings dengan nilai default jika file tidak ada
void loadSettings(Settings* settings);

// Menyimpan pengaturan game ke file konfigurasi
// Menulis struktur Settings ke file untuk persistensi pengaturan
void saveSettings(Settings settings);

// =============================================================================
// GAME CONTEXT AND INITIALIZATION
// Fungsi-fungsi untuk inisialisasi dan pengelolaan konteks game utama
// =============================================================================

// Membuat dan menginisialisasi konteks game utama
// Mengalokasi memori untuk struktur Game dan menginisialisasi semua komponennya
Game* createGameContext(void);

// Menghancurkan konteks game dan membebaskan semua memori
// Dealokasi grid, bullets, dan semua struktur data yang digunakan
void destroyGameContext(Game* game);

// Menginisialisasi grid permainan sebagai doubly linked list
// Membuat grid kosong dengan ukuran yang ditentukan untuk permainan
DoublyLinkedList* initGameGrid(void);

// Fungsi utama untuk menjalankan window game
// Menginisialisasi Raylib, membuat window, dan menjalankan game loop utama
void mainWindow(void);

// =============================================================================
// SCREEN AND DISPLAY UTILITIES
// Fungsi-fungsi utilitas untuk tampilan layar dan scaling
// =============================================================================

// Menghitung faktor skala berdasarkan resolusi layar
// Mengembalikan ScaleFactor untuk menyesuaikan UI dengan berbagai resolusi
ScaleFactor GetScreenScaleFactor(void);

// =============================================================================
// TEXT AND UI RENDERING
// Fungsi-fungsi untuk rendering teks dan elemen antarmuka pengguna
// =============================================================================

// Memformat string dengan variadic arguments seperti printf
// Menggunakan vsnprintf untuk format string dengan parameter dinamis
void formatGameText(char* buffer, const char* format, ...);

// Menggambar teks terpusat horizontal pada posisi Y tertentu
// Menghitung posisi X agar teks berada di tengah layar
void drawCenteredText(Font font, const char* text, float y, int fontSize, float spacing, Color color);

// Menggambar opsi menu dengan highlight jika dipilih
// Mengubah warna dan efek visual berdasarkan status selected
void drawMenuOption(Font font, const char* text, float y, int fontSize, bool selected, Color color);

// Menggambar opsi toggle dengan indikator editing dan selection
// Menampilkan opsi yang bisa di-toggle dengan visual feedback
void drawToggleOption(Font font, const char* line, int startX, int y, int maxLabelWidth, int fontSize, float spacing, bool isSelected, bool isEditing, Color normalColor);

// Menggambar array label dan nilai dalam format kolom
// Menampilkan pasangan label-value dengan alignment yang rapi
void drawLabelsAndValues(const char* lines[], int lineCount, int startX, int startY, int maxLabelWidth, int fontSize, float spacing, Font font, Color color);

// Menggambar teks terpusat dalam area UI tertentu
// Memposisikan teks di tengah area UI yang ditentukan oleh parameter
void drawCenteredUIText(const char* text, float y, int fontSize, float uiAreaX, float uiAreaWidth, Color color);

// =============================================================================
// LAYOUT AND MEASUREMENT UTILITIES
// Fungsi-fungsi untuk mengukur dan menghitung tata letak UI
// =============================================================================

// Menghitung total tinggi yang dibutuhkan untuk array teks
// Menjumlahkan tinggi semua baris teks dengan spacing yang diberikan
float calculateTotalTextHeight(Font font, const char* lines[], int lineCount, int fontSize, float spacing);

// Menghitung lebar maksimum untuk label dan value secara terpisah
// Mengukur setiap string untuk menentukan layout terbaik
void calculateMaxWidths(const char* lines[], int lineCount, int fontSize, float spacing, int* maxLabelWidth, int* maxValueWidth, Font font);

// Menghitung total tinggi dalam pixel untuk sejumlah baris
// Menggunakan fontSize dan spacing untuk kalkulasi tinggi total
int calculateTotalHeight(int lineCount, int fontSize, float spacing);

// =============================================================================
// MENU SYSTEM AND NAVIGATION
// Fungsi-fungsi untuk sistem menu dan navigasi antar halaman
// =============================================================================

// Menangani navigasi menu dengan input keyboard
// Memproses input UP/DOWN dan mengembalikan selection baru
int handleMenuNavigation(int currentSelection, int maxOptions, GameResources* resources);

// Menggambar menu dengan highlight pada item yang dipilih
// Rendering semua opsi menu dengan visual feedback untuk selection
void drawMenu(GameResources* resources, const char* lines[], int lineCount, int selection, int fontSize, Color highlightColor);

// Menampilkan dan menangani menu utama game
// Loop menu utama dengan opsi Play, Settings, Credits, dll
void mainMenu(GameResources* resources);

// Menampilkan menu pause saat game sedang berjalan
// Menu overlay dengan opsi Resume, Settings, Exit to Menu
void pauseMenu(GameResources* resources);

// Menampilkan menu pemilihan mode permainan
// Interface untuk memilih difficulty atau mode game
void selectMode(GameResources* resources);

// =============================================================================
// DIALOG AND MESSAGE SYSTEM
// Fungsi-fungsi untuk menampilkan dialog dan pesan kepada pengguna
// =============================================================================

// Menampilkan dialog konfirmasi dengan multiple pilihan
// Modal dialog yang menunggu input user untuk konfirmasi aksi
bool showConfirmationDialog(GameResources* resources, const char* message, const char* options[], int optionCount, Color highlightColor);

// Menampilkan pesan informasi dengan subjudul
// Dialog informasi non-interaktif dengan pesan utama dan sub-pesan
void showMessageDialog(GameResources* resources, const char* message, const char* subMessage, Color messageColor, Color subMessageColor);

// Menampilkan konfirmasi keluar dari game
// Dialog "Are you sure you want to exit?" dengan opsi Yes/No
bool confirmExit(GameResources* resources);

// Menampilkan konfirmasi kembali ke menu sebelumnya
// Dialog konfirmasi untuk navigasi mundur yang destructive
bool confirmBack(GameResources* resources);

// Menampilkan konfirmasi reset high scores
// Dialog peringatan sebelum menghapus semua high score
bool confirmReset(GameResources* resources);

// Menampilkan pesan penolakan reset high scores
// Feedback visual ketika user membatalkan reset
void rejectReset(GameResources* resources);

// =============================================================================
// SCREEN TRANSITIONS AND EFFECTS
// Fungsi-fungsi untuk transisi antar layar dan efek visual
// =============================================================================

// Mengupdate nilai transisi untuk animasi smooth antar layar
// Menggunakan interpolasi untuk perpindahan yang halus
void updateTransition(float* transition, int* current, int target, int* direction, float deltaTime, float speed);

// Merender transisi antar mode permainan
// Animasi slide atau fade saat berganti mode game
void renderModeTransition(GameResources* resources, int current, int target, float transition, int direction);

// Merender transisi antar skin/tema
// Animasi perpindahan visual theme atau character skin
void renderSkinTransition(GameResources* resources, int current, int target, float transition, int direction);

// Menampilkan layar loading dengan progress bar
// Screen loading dengan animasi dan progress indicator
int loadingScreen(GameResources* resources, float* loadingTime);

// Menampilkan countdown sebelum game dimulai
// Countdown 3-2-1-GO sebelum gameplay aktif
void showCountdown(GameResources* resources);

// Menggambar timer countdown dengan efek visual
// Rendering countdown timer dengan animasi dan warna yang berubah
bool drawCountdownTimer(GameResources* resources, float* countdown, int y);

// =============================================================================
// GAME SCREENS AND VIEWS
// Fungsi-fungsi untuk menampilkan berbagai layar dalam game
// =============================================================================

// Menggambar background berdasarkan ID yang diberikan
// Rendering background image atau pattern untuk berbagai screen
void drawBG(GameResources* resources, uint id);

// Menampilkan layar credits dengan informasi pengembang
// Screen credits dengan scrolling text atau static information
void showCredits(GameResources* resources);

// Menampilkan layar kontrol dan panduan bermain
// Help screen dengan instruksi control dan gameplay
void showControls(GameResources* resources);

// Menampilkan layar pengaturan game
// Settings screen dengan berbagai opsi konfigurasi
void showSettings(GameResources* resources);

// Menampilkan layar high scores
// Leaderboard dengan best scores untuk setiap mode
void showHiScore(GameResources* resources);

// Menampilkan layar game over dengan skor final
// End screen dengan skor, ranking, dan opsi restart/menu
void gameOver(GameResources* resources, ll currentScore);

// Menggambar komponen skor pada layar game over
// Rendering skor current, high score, dan ranking
void drawGameOverScore(GameResources* resources, ll currentScore, ll currentHighScore, int* startY);

// =============================================================================
// GAME STATE MANAGEMENT
// Fungsi-fungsi untuk mengelola state dan alur permainan
// =============================================================================

// Keluar dari game dan cleanup semua resource
// Shutdown sequence yang aman dengan pembersihan memori
void exitGame(GameResources* resources);

// Reset semua high scores ke nilai default
// Menghapus semua record high score dan kembalikan ke 0
void resetHiScores(GameResources* resources);

// Menunggu hingga semua key dilepas
// Mencegah input spam dengan menunggu key release
void waitForKeyRelease(void);

// =============================================================================
// GAME MECHANICS AND LOGIC
// Fungsi-fungsi untuk logika inti permainan dan mekanika gameplay
// =============================================================================

// Menampilkan dan mengupdate gameplay utama
// Main game loop dengan rendering dan update logic
void displayGame(GameResources* resources);

// Mendapatkan kecepatan game berdasarkan mode yang dipilih
// Mengembalikan speed multiplier untuk berbagai difficulty
float getSpeedForMode(Game* game, int mode);

// Mendapatkan range jumlah blok untuk mode tertentu
// Menentukan min/max blok yang muncul berdasarkan difficulty
static void getBlockRangeForMode(int mode, int* minBlocks, int* maxBlocks);

// Mengecek apakah kondisi game over tercapai
// Validasi kondisi kalah seperti blok mencapai atas atau lives habis
bool isGameOverCheck(Game* game);

// =============================================================================
// BLOCK MANAGEMENT SYSTEM
// Fungsi-fungsi untuk mengelola blok dalam grid permainan
// =============================================================================

// Mendapatkan node row dari grid berdasarkan index
// Mengakses baris tertentu dalam doubly linked list grid
DLLNode* getRowNode(Game* game, int rowIndex);

// Mendapatkan node blok dari row berdasarkan index kolom
// Mengakses blok tertentu dalam linked list row
DLLNode* getBlockNode(DoublyLinkedList* row, int colIndex);

// Mendapatkan pointer ke blok pada posisi tertentu
// Akses langsung ke data blok pada koordinat grid
Block* getBlockAt(Game* game, int row, int col);

// Mengaktifkan blok pada posisi tertentu
// Set status blok menjadi aktif/visible di grid
bool activateBlockAt(Game* game, int row, int col);

// Menonaktifkan blok pada posisi tertentu
// Set status blok menjadi non-aktif/destroyed
bool deactivateBlockAt(Game* game, int row, int col);

// Menghapus semua blok dalam satu baris
// Reset semua blok dalam row menjadi non-aktif
void clearRow(DoublyLinkedList* row);

// Mengecek apakah ada blok aktif dalam baris
// Validasi apakah row masih memiliki blok yang visible
bool hasActiveBlocksInRow(DoublyLinkedList* row);

// Mengecek apakah ada blok aktif di bawah posisi tertentu
// Deteksi collision untuk blok yang jatuh
bool hasActiveBlockBelow(Game* game, int row);

// =============================================================================
// BLOCK MOVEMENT AND PHYSICS
// Fungsi-fungsi untuk pergerakan blok dan fisika dalam game
// =============================================================================

// Menangani pergerakan blok berdasarkan range yang diberikan
// Logic utama untuk movement dan spawning blok baru
void handleBlockMovement(Game* game, int minBlocks, int maxBlocks);

// Memindahkan semua blok ke bawah satu level
// Gravity effect yang menggerakkan blok turun
void moveBlocksDown(Game* game);

// Menyalin blok dari source row ke destination row
// Utility untuk transfer blok antar baris
void copyRowBlocks(DoublyLinkedList* sourceRow, DoublyLinkedList* destRow);

// Menggeser baris-baris ke atas mulai dari startRow
// Shift rows untuk menghapus baris yang completed
void shiftRowsUp(Game* game, int startRow);

// Mengecek apakah satu baris penuh terisi blok
// Deteksi line completion untuk scoring
bool isRowFull(Game* game, int row);

// Menangani baris yang terisi penuh
// Logic untuk menghapus line, scoring, dan shift rows
void handleFullRow(Game* game, int row);

// =============================================================================
// BLOCK GENERATION SYSTEM
// Fungsi-fungsi untuk menghasilkan dan menempatkan blok baru
// =============================================================================

// Menghasilkan blok baru berdasarkan parameter yang diberikan
// Spawn blok baru dengan algoritma distribusi yang seimbang
void generateNewBlocks(Game* game, int minBlocks, int maxBlocks, int* emptyColLength, int totalEmptyColumns);

// Mengisi gap kritis untuk mencegah pola tidak adil
// Prioritas spawn pada kolom yang kosong untuk balance
int fillCriticalGaps(Game* game, int remainingBlocks, int* emptyColLength);

// Mengisi blok yang tersisa secara random
// Distribusi blok sisa dengan probabilitas yang merata
void fillRemainingBlocks(Game* game, int remainingBlocks);

// =============================================================================
// COLLISION AND INTERACTION SYSTEM
// Fungsi-fungsi untuk deteksi tabrakan dan interaksi dalam game
// =============================================================================

// Memproses hit bullet pada grid position tertentu
// Damage application, explosion effect, dan bullet removal
void processBulletHit(Game* game, int gridX, int gridY, Bullets* bullets);

// Validasi apakah posisi grid masih dalam bounds
// Boundary checking untuk mencegah out-of-bounds access
bool isValidGridPosition(int x, int y);

// =============================================================================
// BLOCK RENDERING AND UPDATES
// Fungsi-fungsi untuk menggambar dan memperbarui tampilan blok
// =============================================================================

// Mengupdate state semua blok (animasi, effects, dll)
// Update logic untuk visual effects dan state transitions
void updateBlocks(Game* game, GameResources* resources);

// Inisialisasi visual dan data blok
// Setup awal untuk rendering dan collision data
void initBlocks(Game* game, GameResources* resources);

// Menggambar semua blok pada layar
// Rendering loop untuk semua blok dengan textures dan effects
void drawBlocks(Game* game, GameResources* resources);

// Debug function untuk print grid ke console
// Development utility untuk visualisasi grid state
void printGrid(Game* game);

// =============================================================================
// GAME UI AND HUD SYSTEM
// Fungsi-fungsi untuk antarmuka permainan dan heads-up display
// =============================================================================

// Menggambar ikon power-up dengan timer visual
// Rendering power-up icon dengan countdown indicator
void drawPowerUpIcon(Texture2D iconTexture, Vector2 position, float iconSize, float duration, Color timerColor);

// Mendapatkan visual data untuk power-up tertentu
// Mengambil texture, color, dan effect data untuk power-up
PowerUpVisuals getPowerUpVisuals(GameResources* resources, PowerUpType type);

// Menggambar indicator lives/health player
// Rendering heart icons atau health bar
void drawLives(GameResources* resources, int lives, Vector2 startPos, float spacing);

// Menggambar tombol laser dengan cooldown indicator
// UI button dengan visual feedback untuk special ability
void drawLaserButton(GameResources* resources, float cooldown, float uiAreaX, float uiAreaWidth, float y);

// Menggambar semua elemen UI dalam game
// Main UI rendering function untuk HUD complete
void drawGameUI(Game* game, GameResources* resources);

// =============================================================================
// SCORE AND ACHIEVEMENT SYSTEM
// Fungsi-fungsi untuk mengelola skor dan pencapaian pemain
// =============================================================================

// Mendapatkan high score untuk mode yang sedang dimainkan
// Mengambil best score dari save data untuk mode current
ll getCurrentModeHighScore(GameResources* resources);

// Mendapatkan skor maksimum yang akan ditampilkan
// Utility untuk display score dengan formatting yang tepat
ll getMaxScoreToShow(Game* game, GameResources* rsc);

#endif