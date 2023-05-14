#include "checkers.h"

SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src)
{

}
SingleSourceMovesList* FindSingleSourcOptimaleMove(SingleSourceMovesTree* moves_tree)
{

}
multipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player)
{

}
void Turn(Board board, Player player)
{

}
void PlayGame(Board board, Player starting_player)
{

}
void initBoard(Board board)
{
	unsigned short int i;
	#define END_PLAYER1_ROWS 3
	#define START_PLAYER2_ROWS 5

	//init T
	for (i = 0; i < END_PLAYER1_ROWS; i++)
		initBoardRow(board[i], i, PLAYER_1);
	//init empty middle
	for (i = END_PLAYER1_ROWS; i < START_PLAYER2_ROWS ; i++)
		initBoardRow(board[i], i, EMPTY);
	//init B
	for (i = START_PLAYER2_ROWS ; i < BOARD_SIZE; i++)
		initBoardRow(board[i], i, PLAYER_2);
}
void initBoardRow(unsigned char row[], unsigned short int rowInd, Player player)
{
	unsigned short int i;
	//get in which order the row should be filled
	bool fillEmpty;

	//get if the row should be filled with a player or with just empty cells
	if (player == EMPTY)
		fillEmpty = true;
	else
		fillEmpty = IS_EVEN(rowInd);

	//fill the row in empty cells / the player's letter in alternating manner
	for (i = 0; i < BOARD_SIZE; i++)
	{
		//if it's the turn for an empty cell, fill it
		if (fillEmpty)
		{
			row[i] = EMPTY;
			//alternate the flag just if its a row with players
			if(player != EMPTY)
				fillEmpty = false;
		}
		//if it's the turn for an the player's letter cell, fill it and alternate the flag
		else
		{
			row[i] = player;
			fillEmpty = true;
		}
	}
}
void printBoard(Board board)
{
	unsigned short int i;

	PRINT_PLUS_MINUS;
	PRINT_NUMS_ROW;
	PRINT_PLUS_MINUS;

	for (i = 0; i < BOARD_SIZE; i++)
	{
		//print the letter that represents the current row
		printf("|%c|", 'A' + i);
		//print the current status of the board
		printBoardRow(board[i]);
		PRINT_PLUS_MINUS;
	}
}
void printBoardRow(unsigned char row[])
{
	unsigned short int i;
	for (i = 0; i < BOARD_SIZE; i++)
		printf("%c|", row[i]);
	putchar(ENTER);
}