#include "checkers.h"

//Q1
//
//

void updateBoard(Board oldBoard, Board newBoard ,checkersPos* deletedPos1, checkersPos* deletedPos2, checkersPos* add, Player pl)
{
	copyBoard(newBoard, oldBoard);
	//remove the requested player from the first position
	newBoard[deletedPos1->row][deletedPos1->col] = EMPTY;
	//if there is a player to remove in the second position (aka there is a capture and thus two positions should be changed), then remove him
	if (deletedPos2 != NULL)
		newBoard[deletedPos2->row][deletedPos2->col] = EMPTY;
	//update the new position of the player that made the move
	newBoard[add->row][add->col] = pl;
}
checkersPos getNextPos(Player player, checkersPos* currentPos, bool direction)
{
	checkersPos nextPos;
	//if its the 'T' player, move downwards, if it is the 'B' player, move upwards
	nextPos.row = (player == PLAYER_1) ? (currentPos->row) + 1 : (currentPos->row) - 1;
	//moving left means going to the previous column. right means going to the next column
	nextPos.col = (direction == LEFT) ? (currentPos->col) - 1 : (currentPos->col) + 1;
	return nextPos;
}

void copyBoard(Board destBoard, Board srcBoard)
{
	unsigned short int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++)
			destBoard[i][j] = srcBoard[i][j];
}



//Q2
///
//

bool exist(Board board, checkersPos* position, Player player) {//if a player exist in a given row/column
	return (board[position->row][position->col] == player);
}


void goRight(checkersPos* pos, Player player) {//Takes the player positon right, by the player type
	if (player == PLAYER_1) {
		pos->col += 1;
		pos->row += 1;
	}
	else {
		pos->col += 1;
		pos->row -= 1;
	}
}


void goLeft(checkersPos* pos, Player player) {//Takes the player positon left, by the player type
	if (player == PLAYER_1) {
		pos->col -= 1;
		pos->row += 1;
	}
	else {
		pos->col -= 1;
		pos->row -= 1;
	}
}


//etc
//
//

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
void checkAlloc(void* ptr)
{
	if (ptr == NULL)
	{
		puts("allocation failure!");
		exit(1);
	}
}
void freeTree(SingleSourceMovesTree* tr)
{
	freeTreeHelper(tr->source);
}
void freeTreeHelper(SingleSourceMovesTreeNode* root)
{
	if (root == NULL)
		return;
	freeTreeHelper(root->nextMoves[LEFT]);
	freeTreeHelper(root->nextMoves[RIGHT]);
	free(root);
}