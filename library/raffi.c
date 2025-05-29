/**
 * RAFFI MODULE - GAME MECHANICS & PHYSICS
 * =======================================
 *
 * Modul yang menangani game mechanics, physics, collision detection, dan gameplay logic.
 * Berisi implementasi untuk sistem scoring, database management, dan level progression.
 *
 * Komponen utama:
 * - Database initialization dan high score management
 * - Score calculation dan progression system
 * - Level difficulty configuration
 * - Game state utilities dan data persistence
 */

#include "defines.h"
#include "all.h"

 // Struktur 
typedef enum {
    Super_EZ,
    Easy,
    Beginner,
    Medium,
    Hard,
    Super_Hard,
    Expert,
    Master,
    Legend,
    God,
    Progressive,
}Level;

const char* levelNames[] = {
    "Super EZ",
    "Easy",
    "Beginner",
    "Medium",
    "Hard",
    "Super Hard",
    "Expert",
    "Master",
    "Legend",
    "God",
    "Progressive",
};

// =============================================================================
// DATABASE INITIALIZATION AND MANAGEMENT
// Inisialisasi dan manajemen database untuk high scores
// =============================================================================

// Menginisialisasi struktur database untuk high scores
// Membuat linked list kosong sebagai container untuk menyimpan data scores
SingleLinkedList* initializeDb() {
    FILE* file = fopen("db/hiscores.dat", "w");
    if (!file) {
        printf("[LOG] Database belum dibuat!\n");
        return NULL;
    }
    SingleLinkedList* list = createSingleLinkedList();

    for (int i = 0; i < MAX_LEVELS; i++) {
        HiScore* score = (HiScore*)malloc(sizeof(HiScore));
        strcpy(score->mode, levelNames[i]);
        score->score = 0;
        SLL_insertBack(list, score);
        fprintf(file, "%s,%lld\n", score->mode, score->score);
    }
    fclose(file);
    return list;
}

// Memuat data high scores dari file database
// Membaca file save dan mengisi linked list dengan data high scores yang tersimpan
SingleLinkedList* loadHiScores() {
    FILE* file = fopen("db/hiscores.dat", "r");
    if (!file) {
        printf("[LOG] File tidak ditemukan! Menggunakan default skor 0.\n");
        return initializeDb();
    }

    SingleLinkedList* list = createSingleLinkedList();
    char mode[20];
    long long int score;

    while (fscanf(file, "%[^,],%lld\n", mode, &score) != EOF) {
        HiScore* hscore = (HiScore*)malloc(sizeof(HiScore));
        strcpy(hscore->mode, mode);  // Pake hscore, bukan score
        hscore->score = score;       // Assign score value ke hscore->score
        SLL_insertBack(list, hscore);// Insert hscore, bukan score
    }
    fclose(file);
    return list;
}

// Menyimpan data high scores ke file database
// Menulis linked list scores ke file untuk persistensi data
void saveHiScores(SingleLinkedList* list) {
    if (!list || !list->head) {
        printf("[LOG] Invalid list - tidak menyimpan skor!\n");
        return;
    }

    FILE* file = fopen("db/hiscores.dat", "w");
    if (!file) {
        printf("[LOG] Gagal menyimpan skor!\n");
        return;
    }

    for (int i = 0; i < MAX_LEVELS; ++i) {
        char* curMode = levelNames[i];
        ll curScore = 0;
        bool found = false;

        SLLNode* temp = list->head;
        while (temp != NULL) {
            HiScore* score = (HiScore*)SLL_getNodeData(temp);
            if (score && score->mode && strcmp(score->mode, curMode) == 0) {
                curScore = score->score;
                found = true;
                break;
            }
            temp = SLL_getNextNode(temp);
        }

        if (!found) {
            printf("[LOG] Warning: Mode '%s' not found in scores list\n", curMode);
        }

        if (fprintf(file, "%s,%lld\n", curMode, curScore) < 0) {
            printf("[LOG] Error writing to file for mode: %s\n", curMode);
            fclose(file);
            return;
        }
    }

    // fflush(file);
    fclose(file);
    printf("[LOG] High scores saved successfully\n");
}

// =============================================================================
// SCORE MANAGEMENT SYSTEM
// Sistem manajemen skor dan pencapaian pemain
// =============================================================================

// Memperbarui high score jika skor current lebih tinggi
// Membandingkan skor current dengan high score dan update jika perlu
void updateHighScore(Game* game, GameResources* resources) {
    if (!game) return;

    SLLNode* temp = resources->scores.head;
    const char* mode = levelNames[resources->gameLevel];

    while (temp) {
        HiScore* score = (HiScore*)SLL_getNodeData(temp);
        if (strcmp(score->mode, mode) == 0) {
            if (score->score < game->score) {
                score->score = game->score;
            }
            break;
        }
        temp = SLL_getNextNode(temp);
    }

    saveHiScores(&resources->scores);
}

// Menambahkan skor berdasarkan baris yang dihancurkan
// Menghitung dan menambahkan poin berdasarkan jumlah baris yang cleared
void addScore(Game* game, int row) {
    int basePoints = 20;
    int rowMultiplier = row + 1; // Semakin bawah multiplier semakin besar

    game->score += basePoints * rowMultiplier;
}

// Mendapatkan skor current pemain
// Mengambil nilai skor yang sedang berjalan dalam game session
long long int playerScore(Game* game) {
    return game->score;
}

// =============================================================================
// GAME STATE UTILITIES
// Utilitas untuk mengambil informasi state game
// =============================================================================

// Mendapatkan nama mode permainan yang sedang dimainkan
// Mengembalikan string identifier untuk mode game current
char* gameMode(GameResources* resources) {
    return levelNames[resources->gameLevel];
}
