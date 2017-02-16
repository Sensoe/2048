#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "2048.h"

/*!
 * Retourne 1 si la case est vide, 0 sinon
 */
int empty_slot (game *p, int line, int column){
	return (get_value(p, line, column)==0);
}

/*!
 * Ajoute une valeur (2 ou 4 choisi aléatoirement) sur une case vide
 * (elle aussi choisie aléatoirement).
 */
void set_random_value (game *p){
	int *free_slots[p->free_slots];
	int slot, val, 
		dim = (p->n), 
		empty = 0;

	srand(time(NULL));
	
	/*
	 * On parcourt tout le plateau de jeu pour identifier les cases vides.
	 * Des qu'une case vaut 0 on stocke son adresse dans le tableau de pointeurs sur entier free_slots.
	 * Ce tableau contient donc l'adresse de chacune des cases vides a la sortie de la boucle.
	 */
	for(slot=0; slot<(dim*dim); slot++){
		if(get_value(p, slot/dim, slot%dim)==0){
			free_slots[empty++] = &(p->grid[(dim*(slot/dim))+slot%dim]);
		}
	}

	/*
	 * Si l'on a au moins une case de libre, on ajoute une valeur aleatoirement.
	 * La case a remplir est tiree au hasard parmis les indices possibles de free_slots[] et on y accede directement grace a son adresse stockee dans ce tableau.
	 * Enfin on diminue de 1 le nombre de cases vides.
	 */
	if(p->free_slots>0){
		val = ((rand()%2)+1)*2;
		slot = rand()%(p->free_slots);
		*free_slots[slot] = val;
		(p->free_slots)--;
	}
}

/*!
 * Retourne 1 si la partie est gagnée, 0 sinon.
 */
int victory (game *p){
	int slot = 0, 
		dim = p->n;
	
	do{
		if(get_value(p, slot/dim, slot%dim)==(p->max_value)){
			return 1;
		}
	}while (++slot<(dim*dim));
	
	return 0;
}

/*!
 * Retourne 1 si la partie est perdue, 0 sinon.
 */
int defeat (game*p){
    int line, column, val,
		slot = 0,
		dim = p->n, 
		limit = (dim*dim)-1;

	if(p->free_slots == 0){
		do{
			line = slot/dim, column = slot%dim, val = get_value(p, line, column);
			if(((val == get_value(p, line+1, column) && line != (dim-1)) 
				|| (val == get_value(p, line, column+1) && column != (dim-1)))
				&& val != 0){
				return 0;
			}
	    }while(++slot<limit);
	}
	return (p->free_slots==0);
}

/*!
 * Retourne 1 si la partie est terminée, 0 sinon.
 */
int game_over (game*p){
	return (victory(p) || defeat(p));
}