#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "2048.h"

/*!
 * Alloue la grille de la variable jeu passée par adresse.
 * Initialise les cases de la grille avec des cases vides (valeurs nulles)
 * Initialise les champs n et max_value avec les valeurs passées en paramètre
 *
 * \param p : pointeur sur une partie de 2048
 * \param n : taille de la grille
 * \param max_value : valeur à atteindre pour gagner
 */
void init_game (game *p, int n, int max_value){
	if(p!=NULL){
		p->grid = calloc(n*n, sizeof(int));
		p->n = n;
		p->max_value = max_value;
		p->free_slots = n*n;
	}
	else
		printf("Une erreur de segmentation est survenue lors de l'initialisation de la partie. Veuillez réessayer plus tard.");
}

/*!
 * Libère la mémoire allouée pour la grille du jeu passé par adresse.
 *
 * \param p : pointeur sur une partie de 2048
 */
void free_memory (game *p){
	free(p->grid);
}

/*!
 * Fonction retournant 1 si la case (i, j) existe, 0 sinon.
 *
 */
int valid_slot (game *p, int line, int column){
    return (line>=0 && line<(p->n) && column>=0 && column<(p->n));
}

/*!
 * Fonction retournant la valeur de la case (ligne, colonne) de la partie p,
 * ou -1 si la case n’existe pas.
 *
 * \param p : pointeur sur la partie en cours
 * \param line : entier correspondant au numéro de ligne
 * \param column : entier correspondant au numéro de colonne
 */
int get_value (game *p, int line, int column){
    return (p->grid[((p->n)*line)+column]*(valid_slot(p, line, column))-!(valid_slot(p, line, column)));
}

/*!
 * Fonction modifiant la valeur de la case (ligne, colonne) de la partie p, avec la valeur val
 *
 * \param p : pointeur sur la partie en cours
 * \param line : entier orrespondant au numéro de ligne
 * \param column : entier orrespondant au numéro de colonne
 * \param val : entier à mettre dans la case (i, j) (si elle existe)
 */
void set_value (game*p, int line, int column, int val){
    if(valid_slot(p, line, column)){
		p->grid[((p->n)*line)+column] = val;
	}
}

/*!
 * Fonction affichant une grille en couleur à l’écran.
 *
 * Le typedef enum COULEUR_TERMINAL contenu dans terminalCouleur.h nous permet de traiter chaque couleur
 * du panel comme étant un entier compris entre 0 et 7.
 * On attribue donc grâce à un log de base 2 une valeur comprise entre 0 et 7 à la couleur de la case courante,
 * couleur variant ainsi selon la puissance de 2 comprise dans la case en question.
 *
 *
 * \param p : pointeur sur la partie que l’on souhaite afficher
 */
void show (game *p){
    int line, column, curr_val;
    COULEUR_TERMINAL bg_color, fg_color, absolute_color;
	
	clear_terminal();

    for(line=0; line<(p->n); line++){
		/*
		 * Chaque ligne a (p->n) colonnes de hauteur egale a 3 pour donner de l'epaisseur. Donc chaque ligne est composee de 3 sous-lignes.
		 * curr_val : valeur de la case actuelle
		 * absolute_color : puissance de 2 correspondant à curr_val (si curr_val=8, absolute_color=3 car 8=2^3)
		 * fg_color : couleur d'ecriture (On ecrira toujours en blanc, plus simple que de changer a chaque couleur et plus lisible)
		 * bg_color : couleur de fond (On module par 7 pour ne pas avoir de case de couleur blanche)
		 */
		for(column=0; column<(p->n)*3; column++){
            fg_color = WHITE;
			curr_val = get_value(p, line, column%(p->n));
            absolute_color = (COULEUR_TERMINAL)(log2((double)(curr_val)));
            bg_color = (absolute_color%7)+(absolute_color>=7);

			// Si on est au debut d'une sous-ligne, on fait un saut de ligne et on decale de 3 tabulations pour centrer l'affichage
            if(column%(p->n)==0){
                printf("\n\t\t\t");
			}

			/*
			 * Si on est pas sur la sous-ligne du milieu on affiche seulement une chaine vide de 7 caracteres.
			 * Sinon si la valeur de la case est 0, on affiche un point sur 7 caracteres.
			 * Sinon on affiche la valeur de la case sur 7 caracteres en tenant compte de la longueur du chiffre a afficher.
			 */
            if(column<(p->n) || column>=2*(p->n)){
				color_printf(fg_color, bg_color, "       ");
			}
            else if(curr_val==0){
				color_printf(fg_color, bg_color, "   .   ");
            }
			else{
				if(curr_val<10){
                    color_printf(fg_color, bg_color, "   %d   ", curr_val);
                }
				else if(curr_val<100){
                    color_printf(fg_color, bg_color, "   %d  ", curr_val);
                }
				else if(curr_val<1000){
                    color_printf(fg_color, bg_color, "  %d  ", curr_val);
				}
            }
		}
	}
	printf("\n");
}