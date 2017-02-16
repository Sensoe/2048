all: 2048

2048: 2048.o jeu.o organisation.o save.o menu.o terminalCouleur.o saisieM.o
	gcc 2048.o jeu.o organisation.o save.o menu.o terminalCouleur.o -o 2048 -lm
 
2048.o: 2048.c 2048.h
	gcc -c 2048.c -o 2048.o

saisieM.o: saisieM.c 2048.h saisieM.h
	gcc -c saisieM.c -o saisieM.o
 
jeu.o: jeu.c 2048.h saisieM.h
	gcc -c jeu.c -o jeu.o

terminalCouleur.o: terminalCouleur.c 2048.h terminalCouleur.h
	gcc -c terminalCouleur.c -o terminalCouleur.o

organisation.o: organisation.c 2048.h terminalCouleur.h
	gcc -c organisation.c -o organisation.o

sauvegarde.o: save.c 2048.h
	gcc -c save.c -o save.o

menu.o: menu.c 2048.h
	gcc -c menu.c -o menu.o

clean:
	rm -rf *o 2048
