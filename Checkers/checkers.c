#include "checkers.h"

multipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player)
{
	multipleSourceMovesList* newMSMlist;
	SingleSourceMovesTree** piecesThatCanMove;
	unsigned short int numOfPiecesThatCanMove, i;

	newMSMlist = makeEmptyMSMList();
	//find all the pieces of that player that can make moves
	piecesThatCanMove = getPiecesThatCanMove(board, player, &numOfPiecesThatCanMove);

	//initalize a list with all the pieces that cam move of a certain player and their best moves
	for (i = 0; i < numOfPiecesThatCanMove; i++)
		insertDataToEndMSMList(newMSMlist, FindSingleSourcOptimaleMove(piecesThatCanMove[i]));

	free(piecesThatCanMove);
	return newMSMlist;
}
void insertDataToEndMSMList(multipleSourceMovesList* MSMList, SingleSourceMovesList* data)
{
	multipleSourceMovesListCell* MSMCell;
	MSMCell = createNewMSMCell(data, NULL);
	insertCellToEndMSMList(MSMList, MSMCell);
}
void insertCellToEndMSMList(multipleSourceMovesList* MSMList, multipleSourceMovesListCell* MSMCell)
{

}
multipleSourceMovesListCell* createNewMSMCell(SingleSourceMovesList* data, multipleSourceMovesListCell* next)
{
	multipleSourceMovesListCell* MSMCell;
	MSMCell = (multipleSourceMovesListCell*)malloc(sizeof(multipleSourceMovesListCell));
	checkAlloc(MSMCell);

	//fill deatils
	MSMCell->single_source_moves_list = data;
	MSMCell->next = next;

	return MSMCell;
}
SingleSourceMovesTree** getPiecesThatCanMove(Board board, Player player, unsigned short int *pSize)
{
	unsigned short int i, j, logSize = 0;
	SingleSourceMovesTree** piecesThatCanMove;
	checkersPos pos;

	piecesThatCanMove = (SingleSourceMovesTree**)malloc(START_NUM_PIECES * sizeof(SingleSourceMovesTree*));

	//go through all the board
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			//if there is a piece of the player in that place
			if (board[i][j] == player)
			{
				INIT_POS(pos, i, j)
				//find its moves options
				piecesThatCanMove[logSize] = FindSingleSourceMoves(board, &pos);
				//if it has no where to advance, thus it should not be in the array of pieces that can move
				if (IS_NO_MOVES(piecesThatCanMove[logSize]->source))
					freeTree(piecesThatCanMove[logSize]);
				//if it has a place to advance, add it to the array
				else
					logSize++;
			}
		}
	}

	piecesThatCanMove = (SingleSourceMovesTree**)realloc(piecesThatCanMove, logSize * sizeof(SingleSourceMovesTree*));
	*pSize = logSize;
	return piecesThatCanMove;
}
multipleSourceMovesList* makeEmptyMSMList()
{
	multipleSourceMovesList* newMSMlist;
	newMSMlist = (multipleSourceMovesList*)malloc(sizeof(multipleSourceMovesList));
	newMSMlist->head = newMSMlist->tail = NULL;
	checkAlloc(newMSMlist);
	return newMSMlist;
}



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
	Board boardLeft, boardRight;

	//get the position of the left diagonal
	nextLeftPos = getNextPos(board[src->row][src->col], src, LEFT);
	//get the position of the right diagonal
	nextRightPos = getNextPos(board[src->row][src->col], src, RIGHT);

	//if the next left position is outside the board or it is the same player as the current one, thus there is no where to advance
	if (!IS_CELL_VALID(nextLeftPos.row, nextLeftPos.col) || board[nextLeftPos.row][nextLeftPos.col] == player)
		leftMove = NULL;
	//if the left is empty, the player can move there and then has to stop
	else if (IS_EMPTY_CELL(board, nextLeftPos.row, nextLeftPos.col))
	{
		updateBoard(board, boardLeft, src, NULL, &nextLeftPos, player);
		leftMove = createNewSSMTreeNode(boardLeft, &nextLeftPos, NO_CAPTURES, NULL, NULL);
	}
	//else, the opponnent is in the left postion
	else
	{
		checkersPos nextNextLeftPos = getNextPos(board[src->row][src->col], &nextLeftPos, LEFT);
		//if the next position is an opponnent and the position after him is valid and empty, thus a capture can be made
		if (IS_CELL_VALID(nextNextLeftPos.row, nextNextLeftPos.col) && IS_EMPTY_CELL(board, nextNextLeftPos.row, nextNextLeftPos.col))
		{
			capturesLeft++;
			updateBoard(board, boardLeft, src, &nextLeftPos, &nextNextLeftPos, player);
			leftMove = FindSingleSourceMovesHelper(boardLeft, &nextNextLeftPos, player);
			capturesLeft += leftMove->total_captures_so_far;
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
	{
		updateBoard(board, boardRight, src, NULL, &nextRightPos, player);
		rightMove = createNewSSMTreeNode(board, &nextRightPos, NO_CAPTURES, NULL, NULL);
	}
	//else, the opponnent is in the left postion
	else
	{
		checkersPos nextNextRightPos = getNextPos(board[src->row][src->col], &nextRightPos, RIGHT);
		//if the next position is an opponnent and the position after him is valid and empty, thus a capture can be made
		if (IS_CELL_VALID(nextNextRightPos.row, nextNextRightPos.col) && IS_EMPTY_CELL(board, nextNextRightPos.row, nextNextRightPos.col))
		{
			capturesRight++;
			updateBoard(board, boardRight, src, &nextRightPos, &nextNextRightPos, player);
			rightMove = FindSingleSourceMovesHelper(board, &nextNextRightPos, player);
			capturesRight += rightMove->total_captures_so_far;
		}
		//if the position after the opponnent is occupied, then a move can't be made
		else
			rightMove = NULL;
	}

	//retrun a newTnode that start at the src, contains the maximum number of captures that can be made from this position and has the two options that the current player has
	return createNewSSMTreeNode(board, src, max(capturesLeft,capturesRight), leftMove, rightMove);
}
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