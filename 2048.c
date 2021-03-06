﻿#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "2048.h"

int main(){
	int exit, check, end;
	game *p=NULL;
	
	makeSave();

	do{
		exit = menu(p);

		if (exit == NEW_GAME)
		{
			p=init_game(4, 2048);
			set_random_value(p);
			set_random_value(p);
			check=1;
		}
		else if (exit == LOAD_GAME)
		{
			p = calloc(sizeof(game), 1);
			check = load_game(p);
		}
		else if (exit == SAVE_GAME)
			check = save_game(p);
		if (exit != QUIT_GAME && check != 0 && p != NULL)
		{
			end = play(p);

			if (end == 1)
			{
				if (defeat(p) == 1)
				{
					printf("Vous avez perdu.\n");
				}
				else
				{
					printf("Vous avez gagné!\n");
				}
				free_memory(p);
				p = NULL;
			}
		}
		else if (check == 0 && p != NULL)
		{
			if (exit == LOAD_GAME)
				printf("\nImpossible de charger la partie comme demande.\nFin du programme.\n");
			else if (exit == SAVE_GAME)
				printf("\nImpossible de sauvegarder la partie comme demande.\nFin du programme.\n");
		}
	} while (exit != QUIT_GAME && check != 0);

    return 0;
}
