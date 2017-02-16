#ifndef lala_H
#define lala_H

#define MOVE_RIGHT (-1)
#define MOVE_LEFT 1
#define MOVE_DOWN (-1)
#define MOVE_UP 1

typedef enum{
	QUIT_GAME,
	NEW_GAME,
	LOAD_GAME,
	SAVE_GAME,
	RESUME_GAME
}Select;

typedef struct{
	int n;
	int max_value;
	int free_slots;
	int *grid;
}game;

game* init_game(int n, int max_value);
void free_memory(game *p);
int valid_slot(game *p, int line, int column);
int get_value(game *p, int line, int column);
void set_value(game*p, int line, int column, int val);
void show(game *p);
int empty_slot(game *p, int line, int column);
void set_random_value(game *p);
int victory(game *p);
int defeat(game*p);
int game_over(game*p);
int move_line(game *p, int line, int direction);
int move_lines(game *p, int direction);
int move_column(game *p, int column, int direction);
int move_columns(game *p, int direction);
int move(game *p, int direction);
int input_read();
int play(game *p);
void makeSave();
int save_game(game *p);
int load_game(game *p);
int menu(game *p);

#endif
