#include "checkers.h"

void main()
{
	Board board;
	checkersPos pos = {5,4};
	initBoard(board);
	/*board[2][1] = EMPTY;
	board[3][2] = PLAYER_1;
	board[5][2] = EMPTY;
	board[4][3] = PLAYER_2;

	board[4][3] = PLAYER_1;
	board[3][2] = EMPTY;
	board[2][1] = PLAYER_1;
	board[1][0] = EMPTY;*/


	multipleSourceMovesList* msmLst = FindAllPossiblePlayerMoves(board, PLAYER_1);

	printBoard(board);
	//printBoard(tr->source->nextMoves[0]->board);
}