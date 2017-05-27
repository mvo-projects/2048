#ifndef GAME_H
# define GAME_H

#define MAX_H 4
#define MAX_V 4
#define MAX_BLANKS 16
#define PROB_RAND_4 0.1
#define PROB_RAND_2 0.9
#define NB_DIRECTION 4
#define MIN_INT -2147483647

	typedef struct s_Coord
	{
		unsigned int i;
		unsigned int j;
	} t_Coord;
	
	typedef struct s_CoordTab
	{
		t_Coord tab[MAX_BLANKS];
		unsigned int nbBlanks;
	} t_CoordTab;

	typedef enum
	{
		PLAYER, RANDOM
	} e_Player;

	typedef enum
	{
		DOWN, RIGHT, LEFT, UP, NONE
	} e_Direction;

/*extern unsigned int **playGrid;

extern unsigned int **copyGrid;
extern unsigned int scoreTotal;
extern unsigned int scoreTotal; //A mettre dans un .c
*/	extern unsigned int **playGrid;
	extern unsigned int **copyGrid;
 	extern unsigned int scoreTotal;

	unsigned int random_2or4(void);
	unsigned int random_bloc(unsigned int size);
	t_CoordTab getBlanks(unsigned int **board);
	void place_2or4(unsigned int **board);
	void freeBoard(unsigned int **board);
	void fillZeros(unsigned int **board);
	unsigned int ** init_Grid(void);
	void initPlays(unsigned int **board);
	void slide_blocks(e_Direction dir, unsigned int **board, int index_j, int index_i);
	unsigned int slide_couple(e_Direction dir, unsigned int **board, unsigned int *add);
	bool slide_space(e_Direction dir, unsigned int **board);
	void print_Grid(unsigned int **board, unsigned int score);
	void copyBoard(unsigned int **board, unsigned int **copy);
	unsigned int **createCopy(unsigned int **board);
	void insertValue(unsigned int **board, unsigned int row, unsigned int col, unsigned int value);
	bool isPlayable(unsigned int **copy);
	unsigned int countBlanks(unsigned int **board);

#endif
