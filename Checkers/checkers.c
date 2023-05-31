#include "checkers.h"

// etc
/****************
* Function name: updateBoard
Input: Board oldBoard, Board newBoard, checkersPos* deletedPos1, checkersPos* deletedPos2, checkersPos* add, Player pl
Output: None
Function operation: This function updates the game board by making the necessary changes after a player's move.
It takes the old board state, the new board state, the positions of deleted pieces (if any), the position of the added piece, and the player making the move.
It updates the new board by copying the old board, removing the player from the first position, removing the second position (if provided), and adding the player to the new position.
****************/
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
/****************
* Function name: getNextPos
* Input: Player player, checkersPos* currentPos, bool direction
* Output: checkersPos
* Function operation: This function calculates the next position for a player's move based on the current position, player type, and movement direction. It takes the player (either PLAYER_1 or PLAYER_2), the current position, and the direction of movement (LEFT or RIGHT). It determines the next position by incrementing or decrementing the row based on the player type and adjusting the column based on the direction.
The calculated next position is returned.
****************/
checkersPos getNextPos(Player player, checkersPos* currentPos, bool direction)
{
	checkersPos nextPos;
	//if its the 'T' player, move downwards, if it is the 'B' player, move upwards
	nextPos.row = (player == PLAYER_1) ? (currentPos->row) + 1 : (currentPos->row) - 1;
	//moving left means going to the previous column. right means going to the next column
	nextPos.col = (direction == LEFT) ? (currentPos->col) - 1 : (currentPos->col) + 1;
	return nextPos;
}
/****************
* Function name: copyBoard
Input: Board destBoard, Board srcBoard
Output: None
Function operation: This function copies the contents of the source board to the destination board.
It iterates over each cell of the board and assigns the corresponding value from the source board to the destination board.
****************/
void copyBoard(Board destBoard, Board srcBoard)
{
	unsigned short int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++)
			destBoard[i][j] = srcBoard[i][j];
}
/****************
*Function name: exist
Input: Board board, checkersPos* position, Player player
Output: bool
Function operation: This function checks if a given player exists in a specific position on the board. It takes the game board, the position to check, and the player to search for.
It returns true if the player exists at the specified position on the board; otherwise, it returns false.
****************/
bool exist(Board board, checkersPos* position, Player player) {//if a player exist in a given row/column
	return (board[position->row][position->col] == player);
}
/****************
*Function name: goRight
Input: checkersPos* pos, Player player
Output: None
Function operation: This function updates the player's position by moving it one step to the right. The new position is calculated based on the player type. If the player is PLAYER_1, both the column and row of the position are incremented by 1.
If the player is PLAYER_2, the column is incremented by 1, and the row is decremented by 1.
****************/
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
/****************
* Function name: goLeft
Input: checkersPos* pos, Player player
Output: None
Function operation: This function updates the player's position by moving it one step to the left. The new position is calculated based on the player type. If the player is PLAYER_1, the column is decremented by 1, and the row is incremented by 1.
If the player is PLAYER_2, both the column and row of the position are decremented by 1.
****************/
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
/****************
* Function name: initBoard
Input: Board board
Output: None
Function operation: This function initializes the game board with the initial setup for a game of checkers. It takes the board as input and fills it according to the rules of the game.
The 'T' player's pieces are placed in the top rows, the middle rows are left empty, and the 'B' player's pieces are placed in the bottom rows.
****************/
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
/****************
*
****************/
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
/****************
* Function name: printBoard
* Input: game board
* Function operation: prints all the players in the board
****************/
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
/****************
* Function name: printBoardRow
* Input: row from the board
* Function operation: prints all the players in the row
****************/
void printBoardRow(unsigned char row[])
{
	unsigned short int i;
	for (i = 0; i < BOARD_SIZE; i++)
		printf("%c|", row[i]);
	putchar(ENTER);
}
/****************
* Function name: checkAlloc
* Input: a pointer
* Function operation:checked if a dynamic allocation was successful 
****************/
void checkAlloc(void* ptr)
{
	if (ptr == NULL)
	{
		puts("allocation failure!");
		exit(1);
	}
}
/****************
* Function name: freeTree
* Input: tree
* Function operation: goes through all the tree and free each node using a helper function
****************/
void freeTree(SingleSourceMovesTree* tr)
{
	freeTreeHelper(tr->source);
}
/****************
* Function name: freeTreeHelper
* Input: root of a binary tree
* Function operation: goes through all the tree and free each node
****************/
void freeTreeHelper(SingleSourceMovesTreeNode* root)
{
	if (root == NULL)
		return;
	freeTreeHelper(root->nextMoves[LEFT]);
	freeTreeHelper(root->nextMoves[RIGHT]);
	free(root);
}