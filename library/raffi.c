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

void addScore(Game* game, int row) {
    int basePoints = 20;
    int rowMultiplier = row + 1; // Semakin bawah multiplier semakin besar

    game->score += basePoints * rowMultiplier;
}

long long int playerScore(Game* game) {
    return game->score;
}

char* gameMode(GameResources* resources) {
    return levelNames[resources->gameLevel];
}
