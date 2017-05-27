#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "game.h"
#include "ai.h"
#define MAX(a, b) ((a) > (b) ? a : b)
#define MIN(a, b) ((a) < (b) ? a : b)

unsigned int minNotOrdered(unsigned int **board)
{
	unsigned int nbNotOrderedRows;
	unsigned int nbNotOrderedCols;

	nbNotOrderedCols = 0;
	for (size_t i = 0; i < MAX_V; i++)
	{
		for (size_t j = 0; j < MAX_H - 1; j++)
		{
			if (board[j][i] > board[j + 1][i])
				(nbNotOrderedCols)++;
		}
	}
	nbNotOrderedRows = 0;
	for (size_t j = 0; j < MAX_H; j++)
	{
		for (size_t i = 0; i < MAX_V - 1; i++)
		{
			if (board[j][i] > board[j][i + 1])
				(nbNotOrderedRows)++;
		}
	}
	return (MIN(nbNotOrderedRows, nbNotOrderedCols));
}
	
long scoreMinusNeighbours8(unsigned int **board, size_t row, size_t col)
{
	unsigned int neighboursValue;
	int diff;

	if (row == 0 && col == 0)
		neighboursValue = board[row][col + 1] + board[row + 1][col] + board[row + 1][col + 1];
	else if (row == 0 && col == MAX_H - 1)
		neighboursValue = board[row][col - 1] + board[row + 1][col] + board[row + 1][col - 1];
	else if (row == 0)
		neighboursValue = board[row][col - 1] + board[row][col + 1] + board[row + 1][col] + board[row + 1][col - 1] + board[row + 1][col + 1];
	else if (col == 0 && row == MAX_V - 1)
		neighboursValue = board[row - 1][col] + board[row][col + 1] + board[row - 1][col + 1];
	else if (col == 0)
		neighboursValue = board[row - 1][col] + board[row + 1][col] + board[row][col + 1] + board[row - 1][col + 1] + board[row + 1][col + 1];
	else if (row == MAX_V - 1 && col == MAX_H - 1)
		neighboursValue = board[row - 1][col] + board[row][col - 1] + board[row - 1][col - 1];
	else if (col == MAX_H - 1)
		neighboursValue = board[row - 1][col] + board[row + 1][col] + board[row][col - 1] + board[row - 1][col - 1] + board[row + 1][col - 1];
	else if (row == MAX_V - 1)
		neighboursValue = board[row][col - 1] + board[row][col + 1] + board[row - 1][col] + board[row - 1][col - 1] + board[row - 1][col + 1];
	else
		neighboursValue = board[row][col - 1] + board[row - 1][col] + board[row][col + 1] + board[row + 1][col] + board[row - 1][col - 1] + board[row - 1][col + 1] + board[row + 1][col - 1] + board[row + 1][col + 1];
	diff = board[row][col] - neighboursValue;
	if (diff < 0)
		return (-diff);
	return (diff);
	
}

long scoreMinusNeighbours(unsigned int **board, size_t row, size_t col)
{
	unsigned int neighboursValue;
	int diff;

	if (row == 0 && col == 0)
		neighboursValue = board[row][col + 1] + board[row + 1][col];
	else if (row == 0 && col == MAX_H - 1)
		neighboursValue = board[row][col - 1] + board[row + 1][col];
	else if (row == 0)
		neighboursValue = board[row][col - 1] + board[row][col + 1] + board[row + 1][col];
	else if (col == 0 && row == MAX_V - 1)
		neighboursValue = board[row - 1][col] + board[row][col + 1];
	else if (col == 0)
		neighboursValue = board[row - 1][col] + board[row + 1][col] + board[row][col + 1];
	else if (row == MAX_V - 1 && col == MAX_H - 1)
		neighboursValue = board[row - 1][col] + board[row][col - 1];
	else if (col == MAX_H - 1)
		neighboursValue = board[row - 1][col] + board[row + 1][col] + board[row][col - 1];
	else if (row == MAX_V - 1)
		neighboursValue = board[row][col - 1] + board[row][col + 1] + board[row - 1][col];
	else
		neighboursValue = board[row][col - 1] + board[row - 1][col] + board[row][col + 1] + board[row + 1][col];
	diff = board[row][col] - neighboursValue;
	if (diff < 0)
		return (-diff);
	return (diff);
	
}


unsigned int searchBestValue(unsigned int **board)
{
	unsigned int i, j;
	unsigned int value = board[0][0];

	for (j = 0; j < MAX_V; j++)
	{
		for (i = 0; i < MAX_H; i++)
		{
			if (value < board[i][j])
				value = board[i][j];
		}
	}
	return (value);
}

long long int getHeuristics(unsigned int **board)
{
	long long int score;//, score_diff;
	size_t row, col;
	unsigned int nbBlanks;
	score = 0;
	for (row = 0; row < MAX_V; row++)
	{
		for (col = 0; col < MAX_H; col++)
			score += (board[row][col] * board[row][col] * matrix[row][col]);
	}
/*	score_diff = 0;
	for (row = 0; row < MAX_V; row++)
	{
		for (col = 0; col < MAX_H; col++)
			score_diff += scoreMinusNeighbours8(board, row, col);
	}
*/	nbBlanks = countBlanks(board);
	score *= (nbBlanks * nbBlanks);
//	score -= score_diff;
	score /= (chk_monotonic(board) + 1);
	return (score);
	//return (nbBlanks * nbBlanks * score * searchBestValue(board) / (chk_monotonic(board) + 1)); /*- score_diff);*/
}

long long int searchBestScore(unsigned int **board, unsigned int depth, e_Player p, double prob, unsigned int currentDepth)
{
	unsigned int i, index, add;
	long long int score;
	t_CoordTab indexBlank;
	unsigned int **copyB;

	score = 0;
	if (depth <= 0)
		return (getHeuristics(board));
/*	if (prob < probas[10 - currentDepth])
		return (0);
*/
	copyB = init_Grid();
	switch (p)
	{
		case PLAYER:
			bool hasSpace;
			unsigned int nb_couple;
			copyBoard(board, copyB);
			if (!isPlayable(copyB))
				return (score);
			for (index = 0; index < NB_DIRECTION; index++)
			{
				copyBoard(board, copyB);
				hasSpace = slide_space((e_Direction)index, copyB);
				nb_couple = slide_couple((e_Direction)index, copyB, &add);
				if (hasSpace || (nb_couple != 0))
					score = MAX(score, searchBestScore(copyB, depth - 1, RANDOM, prob, currentDepth + 1));
			}
			break;
		case RANDOM:
			indexBlank = getBlanks(board);
			for (i = 0; i < indexBlank.nbBlanks; i++)
			{
				copyBoard(board, copyB);
				insertValue(copyB, indexBlank.tab[i].j, indexBlank.tab[i].i, 2);
				score = score + ((double)PROB_RAND_2 / indexBlank.nbBlanks) * searchBestScore(copyB, depth - 1, PLAYER, prob * PROB_RAND_2, currentDepth + 1);
				copyBoard(board, copyB);
				insertValue(copyB, indexBlank.tab[i].j, indexBlank.tab[i].i, 4);
				score = score + ((double)PROB_RAND_4 / indexBlank.nbBlanks) * searchBestScore(copyB, depth - 1, PLAYER, prob * PROB_RAND_4, currentDepth + 1);
			}
			break;
		default:
			break;
	}
	freeBoard(copyB);
	return (score);
}

e_Direction getBestDirection(unsigned int **board, unsigned int depth)
{
	e_Direction bestDir;
	bestDir = NONE;
	unsigned int **test;
	long int max = MIN_INT;
	unsigned int maxT;

	maxT = 1;
	test = init_Grid();
	copyBoard(board, test);
	if (isPlayable(test))
	{
		maxT = omp_get_max_threads();
		if (maxT > 4)
			maxT = 4;
		#pragma omp parallel shared(bestDir) num_threads(maxT)
		{
			bool hasSpace;
			unsigned int nb_couple;
			unsigned int add;
			long long int vHeuristic;
			unsigned int **copy;

			copy = init_Grid();
			#pragma omp for
			for (unsigned int i = 0; i < NB_DIRECTION; i++)
			{
				copyBoard(board, copy);
				hasSpace = slide_space((e_Direction)i, copy);
				nb_couple = slide_couple((e_Direction)i, copy, &add);
				if (!hasSpace && (nb_couple == 0))
					vHeuristic = MIN_INT;
				else
					vHeuristic = searchBestScore(copy, depth - 1, RANDOM, 1.0, 0);
			//  printf("vHeuristic[%d] = %lld\n", i, vHeuristic);
				#pragma omp critical
				{
					if (max < vHeuristic)
					{
						max = vHeuristic;
						bestDir = (e_Direction)i;
					}
				}
			}
			freeBoard(copy);
		}
	}
	freeBoard(test);
	return (bestDir);
}

bool playAI(void)
{
	e_Direction dir;
	unsigned int add;

	copyBoard(playGrid, copyGrid);
	if (isPlayable(copyGrid))
	{
		dir = getBestDirection(playGrid, dep[countBlanks(playGrid)]);
		slide_space(dir, playGrid);
		slide_couple(dir, playGrid, &add);
		scoreTotal += add;
		place_2or4(playGrid);
		return (true);
	}
	return (false);
}
