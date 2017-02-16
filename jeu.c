#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "2048.h"

/*!
 * Effectue les mouvements (à gauche ou à droite) des cases d’une ligne.
 * Renvoie 1 si l’on a déplacé au moins une case, 0 sinon.
 *
 * \param p : pointeur sur un jeu
 * \param line : indice de ligne
 * \param direction : 1 pour déplacement vers la gauche et -1 pour un déplacement vers la droite
 */
int move_line (game *p, int line, int direction){
    /*
	 * start vaudra (n-1) si on va vers la droite ou 0 si on va vers la gauche. C'est la variable correspondant au départ des boucles utilisees.
	 * limit vaudra 0 si on va vers la droite ou (n-1) si on va vers la gauche. C'est la variable correspondant à la fin des boucles utilisees.
	 * On parcourt donc la ligne de gauche à droite si on veut se deplacer vers la gauche. On la parcourt de droite à gauche sinon.
	 */
	int column, other_val, curr_val,
        size = (p->n-1),
        start = size*(direction==MOVE_RIGHT),
        limit = size*(direction==MOVE_LEFT),
        move = 0,
		pack = start;
    
	/*
	 * On tasse la ligne du cote du deplacement voulu.
	 * pack ne va augmenter que lorsqu'on rencontrera une case non vide, ce qui permet de deplacer les elements non nuls dans les cases vides.
	 * On ajoute a ca le vidage des cases deplacees pour reellement tasser la ligne et ne pas dupliquer de valeurs.
	 * S'il y a deplacement, on incremente move.
	 */
	for(column=start; column!=(limit+direction); column+=direction){
		if(get_value(p, line, column)!=0){
			set_value(p, line, pack, get_value(p, line, column));
			if(pack!=column){
				set_value(p, line, column, 0);
				move++;
			}
			pack += direction;
        }
    } 

	//Cette boucle effectue la fusion des cases identiques.
    for(column=start; column!=limit; column+=direction){
        other_val = get_value(p, line, column+direction);
        curr_val = get_value(p, line, column);

		//Si deux cases ont la meme valeur et que ca n'est pas 0, on stocke la somme dans la case courante, on incremente le nombre de cases vides et move (il y a deplacement).
        if(other_val==curr_val && curr_val!=0){    
			move++;
    	    (p->free_slots)++;
            set_value(p, line, column, curr_val+other_val);

			//On decale ensuite d'un cran dans la direction voulue toute les cases non modifiees et on mets les cases d'origine de celles decalees a 0.
            for(pack=(column+direction); pack!=limit; pack+=direction){
                set_value(p, line, pack, get_value(p, line, pack+direction));
				set_value(p, line, pack+direction, 0);
            }
			//Pour mettre la derniere case de la ligne a 0 (pas pris en compte par la boucle).
			set_value(p, line, pack, 0);
        }
    }
	//Si move est different de 0, il y a eu au moins un mouvement.
    return (move!=0);
}

/*!
 * Effectue les mouvements (à gauche ou à droite) des cases sur toutes les lignes.
 * Renvoie 1 si l’on a déplacé au moins une case, 0 sinon.
 *
 * \param p : pointeur sur un jeu
 * \param direction : 1 pour déplacement vers la gauche et -1 pour un déplacement vers la droite
 */
int move_lines (game *p, int direction){
    int line, 
		move = 0;

    for(line=0; line<(p->n); line++){
        move += move_line(p, line, direction);
    }
	
    return (move!=0);
}

/*!
 * Effectue les mouvements (vers le haut ou vers le bas) des cases d’une colonne.
 * Renvoie 1 si l’on a déplacé au moins une case, 0 sinon.
 *
 * \param p : pointeur sur un jeu
 * \param column : indice de colonne
 * \param direction : -1 pour déplacement vers la bas et 1 vers le haut
 */
int move_column (game *p, int column, int direction){
    int line, other_val, curr_val,
        size = (p->n-1),
        start = size*(direction==MOVE_RIGHT),
        limit = size*(direction==MOVE_LEFT),
        move = 0,
		pack = start;
    
	for(line=start; line!=(limit+direction); line+=direction){
		if(get_value(p, line, column)!=0){
			set_value(p, pack, column, get_value(p, line, column));
			if(pack!=line){
				set_value(p, line, column, 0);
				move++;
			}
			pack += direction;
        }
    } 

    for(line=start; line!=limit; line+=direction){
        other_val = get_value(p, line+direction, column);
        curr_val = get_value(p, line, column);

        if(other_val==curr_val && curr_val!=0){    
			move++;
    	    (p->free_slots)++;
            set_value(p, line, column, curr_val+other_val);

            for(pack=(line+direction); pack!=limit; pack+=direction){
                set_value(p, pack, column, get_value(p, pack+direction, column));
				set_value(p, pack+direction, column, 0);
            }
			set_value(p, pack, column, 0);
        }
    }
    return (move!=0);
}

/*!
 * Effectue les mouvements (vers le haut ou vers le bas) des cases de toutes les colonnes.
 * Renvoie 1 si l’on a déplacé au moins une case, 0 sinon.
 *
 * \param p : pointeur sur un jeu
 * \param direction : -1 pour déplacement vers la bas et 1 vers le haut
 */
int move_columns (game *p, int direction){
    int column, move = 0;

    for(column=0; column<(p->n); column++){
        move += move_column(p, column, direction);
    }
    return (move!=0);
}

/*!
 * Effectue le mouvement sur les lignes ou sur les colonnes suivant la valeur de direction.
 *
 * \param p : pointeur sur un jeu
 * \param direction : entier donnant la direction :
 * 0 : vers le bas
 * 1 : vers la droite
 * 2 : vers le haut
 * 3 : vers la gauche
 * Renvoie 1 si l’on a déplacé au moins une case, 0 sinon
 */
int move (game *p, int direction){
    
	switch(direction){
        case KEY_DOWN :
            return (move_columns(p, MOVE_DOWN));
            break;
        case KEY_RIGHT :
            return (move_lines(p, MOVE_RIGHT));
            break;
        case KEY_UP :
            return (move_columns(p, MOVE_UP));
            break;
        case KEY_LEFT :
            return (move_lines(p, MOVE_LEFT));
            break;
		default :
			return 0;
    }
}

/*!
 * Fonction permettant la saisie d’une direction ou de l’arrêt du jeu
 * (saisie répétée pour les autres touches)
 * Retourne :
 * -1 si l’utilisateur arrête le jeu
 * 0 si l’utilisateur souhaite déplacer vers le bas
 * 1 si l’utilisateur souhaite déplacer vers la droite
 * 2 si l’utilisateur souhaite déplacer vers le haut
 * 3 si l’utilisateur souhaite déplacer vers le gauche
 */
int input_read (){
	int arr_key;
    debutTerminalSansR();
    
	do{
        arr_key = (int)lectureFleche();
    }while(arr_key==NO_KEY);
	
    finTerminalSansR();                                             
    return arr_key;
}

/*!
 * Fonction permettant de jouer la partie en cours (on la suppose initialisée)
 * Retourne 1 si la partie est terminée (l’utilisateur a gagné ou perdu), et 0 sinon
 * (l’utilisateur a appuyé sur la touche Echap).
 */
int play (game *p){
	int direction;
	set_random_value(p);
    
	do{
	    show(p);
        direction = input_read();
		
		if(direction!=KEY_ESCAPE){
			//On ajoute une valeur aleatoire seulement s'il y a deplacement.
			if(move(p,direction)){
				set_random_value(p);
			}
		}

		if(game_over(p)){
			show(p);
		}                                
    }while(!game_over(p) && direction!=KEY_ESCAPE);

    return game_over(p);
}
