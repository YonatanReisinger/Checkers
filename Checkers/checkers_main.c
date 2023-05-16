#include "checkers.h"

void main()
{
	Board board;
	checkersPos pos = {5,4};
	initBoard(board);

	board[2][1] = EMPTY;
	board[3][2] = PLAYER_1;
	board[5][2] = EMPTY;
	board[4][3] = PLAYER_2;

	board[4][3] = PLAYER_1;
	board[3][2] = EMPTY;
	board[2][1] = PLAYER_1;
	board[1][0] = EMPTY;

	//PlayGame(board, PLAYER_1);
	SingleSourceMovesTree* tr = FindSingleSourceMoves(board, &pos);
	//SingleSourceMovesList* ssmList = FindSingleSourceOptimalMove(tr);
	
	//multipleSourceMovesList* msmLst = FindAllPossiblePlayerMoves(board, PLAYER_2);
	//printMSMList(msmLst->head);

	printBoard(board);
	//printBoard(tr->source->nextMoves[0]->board);
}