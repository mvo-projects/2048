#ifndef AI_H
# define AI_H
# include "game.h"
# include <stdio.h>

const unsigned int dep[15] = {8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6};
const unsigned int matrix[4][4] =	{
										{  2,     4,     8,    16},
										{  4,     6,    12,    32},
										{  8,    12,    24,    64},
										{  16,   32,    64,   128}
									};
const double probas[11] = { 0.00081, 0.00081, 0.00081, 0.0081, 0.0081, 0.009, 0.009, 0.01, 0.01, 0.1, 0.1};

	unsigned int chk_monotonic(unsigned int **board);
	long scoreMinusNeighbours8(unsigned int **board, size_t row, size_t col);
	long scoreMinusNeighbours(unsigned int **board, size_t row, size_t col);
	unsigned int searchBestValue(unsigned int **board);
	long long int getHeuristics(unsigned int **board);
	long long int searchBestScore(unsigned int **board, unsigned int depth, e_Player p, double prob, unsigned int currentDepth);
	e_Direction getBestDirection(unsigned int **board, unsigned int depth);
	bool playAI(void);

#endif
