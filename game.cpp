#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "game.h"

unsigned int random_2or4(void)
{
	unsigned int number;

	number = (rand()%100 + 1);
	if (number > 0 && number <= (PROB_RAND_4 * 100))
		return (4);
	return (2);
}

unsigned int random_bloc(unsigned int size)
{
	return (rand()%size);
}

t_CoordTab getBlanks(unsigned int **board)
{
	t_CoordTab res;
	int coord_index = 0;

	for (int j = 0; j < MAX_V; j++)
	{
		for (int i = 0; i < MAX_H; i++)
		{
			if (board[j][i] == 0)
			{
				res.tab[coord_index].i = i;
				res.tab[coord_index].j = j;
				coord_index++;
			}
		}
	}
	res.nbBlanks = coord_index;
	return (res);
}

void place_2or4(unsigned int **board)
{
	unsigned int newNumber;
	unsigned int rand_index;
	t_CoordTab coordTab;
	
	newNumber = random_2or4();
	coordTab = getBlanks(board);
	rand_index = random_bloc(coordTab.nbBlanks);
	board[coordTab.tab[rand_index].j][coordTab.tab[rand_index].i] = newNumber;
}

void freeBoard(unsigned int **board)
{
	size_t i;

	for (i = 0; i < MAX_H; i++)
		free(board[i]);
	free(board);
}

void fillZeros(unsigned int **board)
{
	size_t i;
	size_t j;

	for (i = 0; i < MAX_H; i++)
		for(j = 0; j < MAX_V; j++)
			board[j][i] = 0;
}

unsigned int ** init_Grid(void)
{
	unsigned int **board;

	if ((board = (unsigned int **)malloc(sizeof(unsigned int *) * MAX_V)) == NULL)
	{
		perror("malloc failed\n");
		exit(EXIT_FAILURE);
	}
	for (int j = 0; j < MAX_V; j++)
	{
		if ((board[j] = (unsigned int *)calloc(MAX_H, sizeof(unsigned int))) == NULL)
		{
			perror("malloc failed\n");
			exit(EXIT_FAILURE);
		}
	}	
	return (board);
}

void initPlays(unsigned int **board)
{
	place_2or4(board);
	place_2or4(board);
}

void slide_blocks(e_Direction dir, unsigned int **board, int index_j, int index_i)
{
	switch (dir)
	{
		case LEFT:
			for (int i = index_i; i < MAX_H - 1; i++)
			{
				board[index_j][i] = board[index_j][i + 1];
				board[index_j][i + 1] = 0;
	 		}
			break;
		case RIGHT:
			for (int i = index_i; i > 0; i--)
			{
				board[index_j][i] = board[index_j][i - 1];
				board[index_j][i - 1] = 0;
			}
			break;
		case UP:
			for (int j = index_j; j < MAX_V - 1; j++)
			{
				board[j][index_i] = board[j + 1][index_i];
				board[j + 1][index_i] = 0;
			}
			break;
		case DOWN:
			for (int j = index_j; j > 0; j--)
			{
				board[j][index_i] = board[j - 1][index_i];
				board[j - 1][index_i] = 0;
			}
			break;
		default:
			break;
	}
}

unsigned int slide_couple(e_Direction dir, unsigned int **board, unsigned int *add)
{
	unsigned int nbSlide;

	nbSlide = 0;
	*add = 0;
	switch(dir)
	{
		case LEFT:
			for (int j = 0; j < MAX_V; j++)
			{
				for (int i = 0; i < MAX_H - 1; i++)
				{
					if (board[j][i] == 0)
						break;
					if (board[j][i] == board[j][i + 1])
					{
						board[j][i] *= 2;
						*add += board[j][i];
						board[j][i + 1] = 0;
						slide_blocks(dir, board, j, i + 1);
						nbSlide++;
					}
				}
			}
			break;
		case RIGHT:
			for (int j = 0; j < MAX_V; j++)
			{
				for (int i = MAX_H - 1; i > 0; i--)
				{
					if (board[j][i] == 0)
						break;
					if (board[j][i] == board[j][i - 1])
					{
						board[j][i] *= 2;
						*add += board[j][i];
						board[j][i - 1] = 0;
						slide_blocks(dir, board, j, i - 1);
						nbSlide++;
					}
				}
			}
			break;
		case UP:
			for (int i = 0; i < MAX_H; i++)
			{
				for (int j = 0; j < MAX_V - 1; j++)
				{
					if (board[j][i] == 0)
						break;
					if (board[j][i] == board[j + 1][i])
					{
						board[j][i] *= 2;
						*add += board[j][i];
						board[j + 1][i] = 0;
						slide_blocks(dir, board, j+1, i);
						nbSlide++;
					}
				}
			}
			break;
			
		case DOWN:
			for (int i = 0; i < MAX_H; i++)
			{
				for (int j = MAX_V - 1; j > 0; j--)
				{
					if (board[j][i] == 0)
						break;
					if (board[j][i] == board[j - 1][i])
					{
						board[j][i] *= 2;
						*add += board[j][i];
						board[j - 1][i] = 0;
						slide_blocks(dir, board, j-1, i);
						nbSlide++;
					}
				}
			}
			break;
		default:
			break;
	}
	return (nbSlide);
}

bool slide_space(e_Direction dir, unsigned int **board)
{
	bool change;

	change = false;
	switch(dir)
	{
		case LEFT:
		for (int j = 0; j < MAX_V; j++)
		{
			for (int i = 0; i < MAX_H - 1; i++)
			{
				if (board[j][i] == 0)
				{
					for (int k = i + 1; k < MAX_H; k++)
					{
						if (board[j][k] != 0)
						{
							change = true;
							board[j][i] = board[j][k];
							board[j][k] = 0;
							break;
						}
					}
				}
			}
		}
		break;

		case RIGHT:
		for (int j = 0; j < MAX_V; j++)
		{
			for (int i = MAX_H - 1; i > 0; i--)
			{
				if (board[j][i] == 0)
				{
					for (int k = i - 1; k >= 0; k--)
					{
						if (board[j][k] != 0)
						{
							change = true;
							board[j][i] = board[j][k];
							board[j][k] = 0;
							break;
						}
					}
				}
			}
		}
		break;

		case UP:
		for (int i = 0; i < MAX_H; i++)
		{
			for (int j = 0; j < MAX_V - 1; j++)
			{
				if (board[j][i] == 0)
				{
					for (int k = j + 1; k < MAX_V; k++)
					{
						if (board[k][i] != 0)
						{
							change = true;
							board[j][i] = board[k][i];
							board[k][i] = 0;
							break;
						}
					}
				}
			}
		}
		break;

			
		case DOWN:
		for (int i = 0; i < MAX_H; i++)
		{
			for (int j = MAX_V - 1; j > 0; j--)
			{
				if (board[j][i] == 0)
				{
					for (int k = j - 1; k >= 0; k--)
					{
						if (board[k][i] != 0)
						{
							change = true;
							board[j][i] = board[k][i];
							board[k][i] = 0;
							break;
						}
					}
				}
			}
		}
		break;
		default:
			break;
	}
	return (change);
}	

void print_Grid(unsigned int **board, unsigned int score)
{
	printf("Votre score est de : %d\n", score); 
	for (int j = 0; j < MAX_V; j++)
	{
		for (int i = 0; i < MAX_H; i++)
			printf("%d ", board[j][i]);
		printf("\n");
	}
	printf("--------------\n");
}

void copyBoard(unsigned int **board, unsigned int **copy)
{
	for (int j = 0; j < MAX_V; j++)
	{
		for (int i = 0; i < MAX_H; i++)
			copy[j][i] = board[j][i];
	}
}

unsigned int **createCopy(unsigned int **board)
{
	unsigned int **copy;

	copy = init_Grid();
	copyBoard(board, copy);
	return (copy);
}

void insertValue(unsigned int **board, unsigned int row, unsigned int col, unsigned int value)
{
	board[row][col] = value;
}

bool isPlayable(unsigned int **copy)
{
	bool b1;
	unsigned int nbCouple;
	unsigned int add;

	add = 0;
	b1 = slide_space(LEFT, copy);
	nbCouple = slide_couple(LEFT, copy, &add);
	if (!b1 && (nbCouple == 0))
	{
		b1 = slide_space(RIGHT, copy);
		nbCouple = slide_couple(RIGHT, copy, &add);
		if (!b1 && (nbCouple == 0))
		{
			b1 = slide_space(UP, copy);
			nbCouple = slide_couple(UP, copy, &add);
			if (!b1 && (nbCouple == 0))
			{
				b1 = slide_space(DOWN, copy);
				nbCouple = slide_couple(DOWN, copy, &add);
				if (!b1 && (nbCouple == 0))
					return (false);
			}
		}
	}
	return (true);
}

unsigned int countBlanks(unsigned int **board)
{
	unsigned int res;
	size_t i, j;

	res = 0;
	for (j = 0; j < MAX_V; j++)
	{
		for (i = 0; i < MAX_H; i++)
		{
			if (board[j][i] == 0)
				res++;
		}
	}
	return (res);
}

