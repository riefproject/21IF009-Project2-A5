#include "defines.h"
#include "all.h"

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

SingleLinkedList *initializeDb() {
    FILE* file = fopen("db/hiscores.dat", "w");
    if (!file) {
        printf("Database belum dibuat!\n");
        return;
    }
    SingleLinkedList *list = createSingleLinkedList();

    for (int i = 0; i < MAX_LEVELS; i++) {
        HiScore *score = (HiScore*)malloc(sizeof(HiScore));
        strcpy(score->mode, levelNames[i]);
        score->score = 0;
        SLL_insertBack(list, score);
        fprintf(file, "%s,%lld\n", score->mode, score->score);
    }
    fclose(file);
    return list;
}

SingleLinkedList *loadHiScores() {
    FILE* file = fopen("db/hiscores.dat", "r");
    if (!file) {
        printf("File tidak ditemukan! Menggunakan default skor 0.\n");
        return initializeDb();
    }

    SingleLinkedList *list = createSingleLinkedList();
    char mode[20];
    long long int score;

    while (fscanf(file, "%[^,],%lld\n", mode, &score) != EOF) {
        HiScore *score = (HiScore*)malloc(sizeof(HiScore));
        strcpy(score->mode, mode);
        score->score = score;
        SLL_insertBack(list, score);
    }
    fclose(file);
    return list;
}

void saveHiScores(SingleLinkedList *list) {
    FILE* file = fopen("db/hiscores.dat", "w");
    if (!file) {
        printf("Gagal menyimpan skor!\n");
        return;
    }

    SLLNode* temp = SLL_getFront(list);
    while(temp){
        HiScore* score = (HiScore*)SLL_getNodeData(temp);
        fprintf(file, "%s,%lld\n", score->mode, score->score);
        temp = SLL_getNextNode(temp);
    }

    fclose(file);
}

void updateHighScore(Game* game, GameResources* resources) {
    if (!game) return;

    SingleLinkedList *list = loadHiScores();
    SLLNode *temp = SLL_getFront(list);
    const char *mode = levelNames[resources->gameLevel];

    while(temp){
        HiScore* score = (HiScore*)SLL_getNodeData(temp);
        if(strcmp(score->mode,mode) == 0){
            if(score->score < game->score){
                score->score = game->score;
            }
            break;
        }
        temp = SLL_getNextNode(temp);
    }
    saveHiScores(list);
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
