#include "checkers.h"
void main()
{
	Board board;
	checkersPos pos = {4,3};
	initBoard(board);
	board[2][1] = EMPTY;
	board[3][2] = PLAYER_1;
	board[5][2] = EMPTY;
	board[4][3] = PLAYER_2;
	SingleSourceMovesTree* tr = FindSingleSourceMoves(board, &pos);
	
	printBoard(board);
	fflush(stdout);
	printBoard(tr->source->nextMoves[0]->board);
}