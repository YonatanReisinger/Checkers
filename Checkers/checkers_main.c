#include "checkers.h"

void main()
{
	Board board;
	checkersPos pos = {2,7};
	initBoard(board);

	//PlayGame(board, PLAYER_1);
	/*board[2][1] = EMPTY;
	board[3][2] = PLAYER_1;
	board[5][2] = EMPTY;
	board[4][3] = PLAYER_2;*/

	/*board[4][3] = PLAYER_1;
	board[3][2] = EMPTY;
	board[2][1] = PLAYER_1;
	board[1][0] = EMPTY;*/

	/*board[5][0] = EMPTY;
	board[4][1] = PLAYER_2;
	board[2][7] = EMPTY;
	board[4][7] = PLAYER_1;*/

	/*board[5][0] = EMPTY;
	board[4][1] = PLAYER_2;
	board[5][2] = EMPTY;
	board[3][0] = PLAYER_2;

	board[2][7] = EMPTY;
	board[4][7] = PLAYER_1;
	board[2][5] = EMPTY;
	board[4][5] = PLAYER_1;*/

	/*board[2][1] = board[2][3] = board[2][5] = board[2][7] = EMPTY;
	board[1][2] = board[1][4] = EMPTY;
	board[3][0] = board[3][4] = board[3][6] = board[4][3] = board[4][7] = PLAYER_1;

	board[5][0] = board[5][2] = board[5][4] = board[5][6] = EMPTY;
	board[4][5] = PLAYER_2;

	board[1][6] = board[3][0] = EMPTY;
	board[2][7] = board[4][3] = PLAYER_1;
	board[4][5] = board[6][7] = EMPTY;
	board[5][6] =board[3][6] = PLAYER_2;*/
	
	//Turn(board, PLAYER_1);
	//SingleSourceMovesTree* tr = FindSingleSourceMoves(board, &pos);
	//SingleSourceMovesList* lst = FindSingleSourceOptimalMove(tr);
	//multipleSourceMovesList* msmLst = FindAllPossiblePlayerMoves(board, PLAYER_1);
	//printMSMList(msmLst->head);
	PlayGame(board, PLAYER_2);
	//printBoard(board);
	//printBoard(tr->source->nextMoves[0]->board);
}