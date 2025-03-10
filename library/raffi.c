#include "../include/all.h"
#include <stdio.h>

void initializeDb(HiScore scores[]){ //Inisialisasi database dengan nilai 0.
	FILE* file = fopen("../db/hiscores.dat", "w");
    if (!file) {
        printf("Database belum dibuat!\n");
        return;
    }
    for (int i = 0; i < MAX_LEVELS; i++) {
            sprintf(scores[i].mode, "Level %d", i + 1);
            if (i == 10){
            	strcpy(scores[i].mode, "Endless");
			}
            scores[i].score = 0;
            fprintf(file, "%s,%d\n", scores[i].mode , scores[i].score);
    }  
    fclose(file);
}

void playerInitialization(Game *game){ //Kode untuk test score player
	printf("Set level (1-10 untuk level, 11 untuk Endless: \n");
	scanf("%d", &game->level);
	while(game->level < 1 || game->level > 11){
		printf("Masukkan Level Valid (1-11): \n");
		scanf("%d", &game->level);
	}
	printf("Level : %d\n", game->level);
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
