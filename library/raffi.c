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
    Endless,
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
    "Endless",
};

void initializeDb(HiScore scores[]) {
    FILE* file = fopen("db/hiscores.dat", "w");
    if (!file) {
        printf("Database belum dibuat!\n");
        return;
    }
    for (int i = 0; i < MAX_LEVELS; i++) {
        sprintf(scores[i].mode, "%s", levelNames[i]);
        scores[i].score = 0;
        fprintf(file, "%s,%lld\n", scores[i].mode, scores[i].score);
    }
    fclose(file);
}

void loadHiScores(HiScore scores[]) {
    FILE* file = fopen("db/hiscores.dat", "r");
    if (!file) {
        printf("File tidak ditemukan! Menggunakan default skor 0.\n");
        initializeDb(scores);
        return;
    }

    char mode[20];
    long long int score;
    int index = 0;

    while (fscanf(file, "%[^,],%lld\n", mode, &score) != EOF && index < MAX_LEVELS) {
        strcpy(scores[index].mode, mode);
        scores[index].score = score;
        index++;
    }

    fclose(file);
}

void saveHiScores(HiScore scores[]) {
    FILE* file = fopen("db/hiscores.dat", "w");
    if (!file) {
        printf("Gagal menyimpan skor!\n");
        return;
    }

    for (int i = 0; i < MAX_LEVELS; i++) {
        fprintf(file, "%s,%lld\n", scores[i].mode, scores[i].score);
    }

    fclose(file);
}

void updateHighScore(Game* game, GameResources* resources) {
    if (!game) return;

    HiScore scores[MAX_LEVELS];
    loadHiScores(scores);
    scores[resources->gameLevel].score = getMax(scores, game, resources);
    saveHiScores(scores);
}

void addScore(Game* game, int row) {
    int basePoints = 50;
    int rowMultiplier = row + 1; // Semakin bawah multiplier semakin besar

    game->score += basePoints * rowMultiplier;
}

long long int playerScore(Game* game) {
    return game->score;
}

int getMax(HiScore scores[], Game* game, GameResources* resources) {
    int level = resources->gameLevel - 1;
    if (scores[level].score < game->score) {
        return game->score;
    }
    else {
        return scores[level].score;
    }
}
