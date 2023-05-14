#include "checkers.h"

SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src)
{
	SingleSourceMovesTree* tree;
	//if there is no player in this position, return NULL
	if (IS_EMPTY_CELL(board, src->row, src->col))
		return NULL;
	//else, there should be a tree, fill the tree using a helper function
	tree = makeEmptyTree();
	tree->source = FindSingleSourceMovesHelper(board, src, board[src->row][src->col]);

	return tree;
}
SingleSourceMovesTreeNode* FindSingleSourceMovesHelper(Board board, checkersPos* src, Player player)
{
	SingleSourceMovesTreeNode* leftMove, *rightMove;
	checkersPos nextLeftPos, nextRightPos;
	unsigned short capturesLeft = 0, capturesRight = 0;
	
	//get the position of the left diagonal
	nextLeftPos = getNextPos(board[src->row][src->col], src, LEFT);
	//get the position of the right diagonal
	nextRightPos = getNextPos(board[src->row][src->col], src, RIGHT);

	//if the next left position is outside the board or it is the same player as the current one, thus there is no where to advance
	if (!IS_CELL_VALID(nextLeftPos.row, nextLeftPos.col) || board[nextLeftPos.row][nextLeftPos.col] == player)
		leftMove = NULL;
	//if the left is empty, the player can move there and then has to stop
	else if (IS_EMPTY_CELL(board, nextLeftPos.row, nextLeftPos.col))
		leftMove = createNewSSMTreeNode(board, &nextLeftPos, NO_CAPTURES, NULL, NULL);
	//else, the opponnent is in the left postion
	else
	{
		checkersPos nextNextLeftPos = getNextPos(board[src->row][src->col], &nextLeftPos, LEFT);
		//if the next position is an opponnent and the position after him is valid and empty, thus a capture can be made
		if (IS_CELL_VALID(nextNextLeftPos.row, nextNextLeftPos.col) && IS_EMPTY_CELL(board, nextNextLeftPos.row, nextNextLeftPos.col))
		{
			capturesLeft++;
			leftMove = FindSingleSourceMovesHelper(board, &nextNextLeftPos, player);
		}
		//if the position after the opponnent is occupied, then a move can't be made
		else
			leftMove = NULL;
	}

	//if the next right position is outside the board or it is the same player as the current one, thus there is no where to advance
	if (!IS_CELL_VALID(nextRightPos.row, nextRightPos.col) || board[nextRightPos.row][nextRightPos.col] == player)
		rightMove = NULL;
	//if the right is empty, the player can move there and then has to stop
	else if (IS_EMPTY_CELL(board, nextRightPos.row, nextRightPos.col))
		rightMove = createNewSSMTreeNode(board, &nextRightPos, NO_CAPTURES, NULL, NULL);
	//else, the opponnent is in the left postion
	else
	{
		checkersPos nextNextRightPos = getNextPos(board[src->row][src->col], &nextRightPos, RIGHT);
		//if the next position is an opponnent and the position after him is valid and empty, thus a capture can be made
		if (IS_CELL_VALID(nextNextRightPos.row, nextNextRightPos.col) && IS_EMPTY_CELL(board, nextNextRightPos.row, nextNextRightPos.col))
		{
			capturesRight++;
			rightMove = FindSingleSourceMovesHelper(board, &nextNextRightPos, player);
		}
		//if the position after the opponnent is occupied, then a move can't be made
		else
			rightMove = NULL;
	}

	//retrun a newTnode that start at the src, contains the maximum number of captures that can be made from this position and has the two options that the current player has
	return createNewSSMTreeNode(board, src, max(capturesLeft,capturesRight), leftMove, rightMove);
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
SingleSourceMovesTreeNode* createNewSSMTreeNode(Board board, checkersPos* position, unsigned short captures, SingleSourceMovesTreeNode* next1, SingleSourceMovesTreeNode* next2)
{
	SingleSourceMovesTreeNode* SSMTreeNode;
	//allocate space in memory for the new single source moves tree node
	SSMTreeNode = (SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode));
	//make sure that the allocation succeeded
	checkAlloc(SSMTreeNode);

	//fill deatils
	copyBoard(SSMTreeNode->board, board);
	SSMTreeNode->pos = position;
	SSMTreeNode->total_captures_so_far = captures;
	SSMTreeNode->nextMoves[LEFT] = next1;
	SSMTreeNode->nextMoves[RIGHT] = next2;

	return SSMTreeNode;
}
void copyBoard(Board destBoard, Board srcBoard)
{
	unsigned short int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++)
			destBoard[i][j] = srcBoard[i][j];
}
SingleSourceMovesTree* makeEmptyTree()
{
	SingleSourceMovesTree* newTree;
	//allocate space in memory for the new single source moves tree
	newTree = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));
	//make sure that the allocation succeeded
	checkAlloc(newTree);
	return newTree;
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
void checkAlloc(void* ptr)
{
	if (ptr == NULL)
	{
		puts("allocation failure!");
		exit(1);
	}
}