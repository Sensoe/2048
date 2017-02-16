#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "2048.h"

/*!                                                            *
* Fonction créant un repertoire pour stocker les sauvegardes. */
void makeSave()
{
	printf("Creation du repertoire de sauvegardes :\n");

	if (system("mkdir saves") == 0)
		printf("Le dossier a bien ete cree dans votre repertoire courant.\n\n\n");
	else
		printf("Impossible donc de creer le dossier dans votre repertoire courant.\n\n\n");
}

/*!                                                         *
* Fonction sauvegardant la partie en cours                 *
*                                                          *
* \param p : pointeur sur la partie courante à sauvegarder *
*                                                          *
* Retourne 0 en cas de problème, 1 sinon                   */
int save_game(struct partie *p)
{
	FILE *save = NULL, *saveD = NULL;
	char nomSav[TAILLE_CH] = "saves/";
	char nomChoix[TAILLE_CH] = { '\0' };

	printf("Veuillez donner un nom a la sauvegarde : ");

	do{
		scanf("%s", nomChoix);

		if (verificationSave(nomChoix) == 0)
			printf("Ce nom n'est pas valide, veuillez entrer des caracteres alphanumeriques.\n");
	} while (verificationSave(nomChoix) == 0);

	strcat(nomChoix, ".ygreg");
	strcat(nomSav, nomChoix);

	save = fopen(nomSav, "wb");
	saveD = fopen("saves/index.rolex", "ab");

	if (save != NULL && saveD != NULL && p != NULL)
	{
		fwrite(p, sizeof(struct partie), 1, save);
		fclose(save);
		fwrite(nomSav, sizeof(char), TAILLE_CH, saveD);
		fclose(saveD);

		return 1;
	}
	return 0;
}

/*!                                                       *
* Fonction chargeant la partie sauvegardée choisie.      *
*                                                        *
* \param p : adresse du pointeur sur la partie à charger *
*                                                        *
* Retourne 0 en case de problème, 1 sinon                */
int load_game(struct partie *p)
{
	FILE *save = NULL, *saveD = NULL;
	int i = 1, j;
	char nomSav[TAILLE_CH] = { '\0' };

	saveD = fopen("saves/index.rolex", "rb");

	if (saveD != NULL)
	{
		printf("\nListe des sauvegardes disponibles :");

		while (fread(nomSav, sizeof(char), TAILLE_CH, saveD) != 0)
		{
			printf("\n%d - %s", i, nomSav + 6);
			i++;
		}

		printf("\n\nQuelle sauvegarde souhaitez vous charger ? Entrez son numero : ");

		do{
			scanf("%d", &j);
		} while (j>i);

		rewind(saveD);

		for (i = 1; i <= j; i++)
			fread(nomSav, sizeof(char), TAILLE_CH, saveD);

		printf("\n%s chargee!\n", nomSav + 6);
		fclose(saveD);
	}

	save = fopen(nomSav, "rb");

	if (save != NULL && p != NULL)
	{
		fread(p, sizeof(struct partie), 1, save);
		fclose(save);

		return 1;
	}
	return 0;
}

/*!                                                                                                       
* Fonction affichant le menu (si p!=NULL, on permet de reprendre une partie et de sauvegarder la partie) 
*                                                                                                        
* \param p : pointeur sur la partie courante.                                                            
* Valeur de retour :                                                                                     
* 0 si les joueurs veulent quitter le jeu,                                                               
* 1 sils veulent commencer une nouvelle partie,                                                         
* 2 sils veulent charger la dernière partie sauvegardée,                                                
* 3 sils veulent sauvegarder la partie courante,                                                        
* 4 sils veulent reprendre la partie courante.     
*/
int menu(struct partie *p)
{
	int choix;

	printf(" ------------------------------------------------------------------------------\n");
	printf("                            ...             ...                                \n");
	printf("                           / \\\\\\           /// \\                                \n");
	printf("                          (   )))   2048    (((   )                               \n");
	printf("                           \\ ///   (4x4)   \\\\\\ /                               \n");
	printf("\n ------------------------------------------------------------------------------\n");

	do{
		printf("\n\nQue souhaitez-vous faire ?\n0 - Quitter\n1 - Nouvelle Partie\n2 - Charger une sauvegarde\n");

		if (p != NULL)
			printf("3 - Sauvegarder la partie en cours\n4 - Retour au jeu en cours\n");

		scanf("%d", &choix);

		if (choix<QUIT_GAME || (choix>SAVE_GAME && p == NULL) || choix>RESUME_GAME)
			printf("\nVotre saisie est invalide, veuillez entrer une des options proposees.\n");
	} while (choix<0 || (choix>2 && p == NULL) || choix>4);

	return choix;
}
