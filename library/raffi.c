#include "../include/all.h"
#include <stdio.h>

typedef enum{
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

void initializeDb(HiScore scores[]){ //Inisialisasi database dengan nilai 0.
	FILE* file = fopen("../db/hiscores.dat", "w");
    if (!file) {
        printf("Database belum dibuat!\n");
        return;
    }
    for (int i = 0; i < MAX_LEVELS; i++) {
            sprintf(scores[i].mode, "%s", levelNames[i]);
            scores[i].score = 0;
            fprintf(file, "%s,%d\n", scores[i].mode , scores[i].score);
    }  
    fclose(file);
}

void playerInitialization(Game *game){ //Kode untuk test score player
	game->score = 0;
}

void playerScore(Game *game, HiScore scores[], int posY){
	int  level = game->level-1;
	game->score+=100*posY; //Semakin dekat blok dengan pesawat, skor yang didapat akan lebih besar 
	scores[level].score = getMax(scores, game);
}

int getMax(HiScore scores[], Game *game){ //Mencari nilai terbesar dari score player dengan highscore
	int level = game->level-1;
	if (scores[level].score < game->score){
		return game->score;
	} else {
		return scores[level].score;
	}
}
