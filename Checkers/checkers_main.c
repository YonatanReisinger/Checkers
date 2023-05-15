#include "checkers.h"

void main()
{
	Board board;
	checkersPos pos = {2,7};
	initBoard(board);
	board[2][1] = EMPTY;
	board[3][2] = PLAYER_1;
	board[5][2] = EMPTY;
	board[4][3] = PLAYER_2;

	board[4][3] = PLAYER_1;
	board[3][2] = EMPTY;
	board[2][1] = PLAYER_1;
	board[1][0] = EMPTY;


	//SingleSourceMovesTree* tr = FindSingleSourceMoves(board, &pos);
	//SingleSourceMovesList* ssmList = FindSingleSourceOptimalMove(tr);
	
	multipleSourceMovesList* msmLst = FindAllPossiblePlayerMoves(board, PLAYER_1);

	printBoard(board);
	//printBoard(tr->source->nextMoves[0]->board);
}