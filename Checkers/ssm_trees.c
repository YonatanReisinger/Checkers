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
	SingleSourceMovesTreeNode* leftMove, * rightMove;
	checkersPos nextLeftPos, nextRightPos;
	unsigned short capturesLeft = 0, capturesRight = 0;
	Board boardLeft, boardRight;

	//get the position of the left diagonal
	nextLeftPos = getNextPos(player, src, LEFT);
	//get the position of the right diagonal
	nextRightPos = getNextPos(player, src, RIGHT);

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
		checkersPos nextNextLeftPos = getNextPos(player, &nextLeftPos, LEFT);
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
		checkersPos nextNextRightPos = getNextPos(player, &nextRightPos, RIGHT);
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
	return createNewSSMTreeNode(board, src, max(capturesLeft, capturesRight), leftMove, rightMove);
}

SingleSourceMovesTreeNode* findSideMove(Board board, Player player, checkersPos* src, checkersPos* nextPos, unsigned short* pcaptures)
{
	SingleSourceMovesTreeNode* sideMove;




	return sideMove;
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

	//allocate space in memory for the new position pointer
	SSMTreeNode->pos = (checkersPos*)malloc(sizeof(checkersPos));
	//make sure that the allocation succeeded
	checkAlloc(SSMTreeNode->pos);
	SSMTreeNode->pos->row = position->row;
	SSMTreeNode->pos->col = position->col;

	SSMTreeNode->total_captures_so_far = captures;
	SSMTreeNode->nextMoves[LEFT] = next1;
	SSMTreeNode->nextMoves[RIGHT] = next2;

	return SSMTreeNode;
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

SingleSourceMovesTree** getPiecesThatCanMove(Board board, Player player, unsigned short int* pSize)
{
	unsigned short int i, j, logSize = 0;
	SingleSourceMovesTree** piecesThatCanMove;
	checkersPos pos;

	piecesThatCanMove = (SingleSourceMovesTree**)malloc(START_NUM_PIECES * sizeof(SingleSourceMovesTree*));

	//go through all the board
	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++)
			//if there is a piece of the player in that place, check if it can move and add it or nor accordinly
			if (board[i][j] == player)
				addPiecesThatCanMove(piecesThatCanMove, &logSize, i, j, board);

	//shrink the array to it's logical size
	piecesThatCanMove = (SingleSourceMovesTree**)realloc(piecesThatCanMove, logSize * sizeof(SingleSourceMovesTree*));
	//update the return size
	*pSize = logSize;
	return piecesThatCanMove;
}

void addPiecesThatCanMove(SingleSourceMovesTree** piecesThatCanMove, unsigned short* plogSize, unsigned short pieceRow, unsigned short pieceCol, Board board)
{
	checkersPos pos;

	INIT_POS(pos, pieceRow, pieceCol)
	//find its moves options
	//maybe try gir rid of building tree !!!!!!!!!!!!!!!!!!!!
	piecesThatCanMove[(*plogSize)] = FindSingleSourceMoves(board, &pos);
	//if it has no where to advance, thus it should not be in the array of pieces that can move
	if (IS_NO_MOVES(piecesThatCanMove[(*plogSize)]->source))
		freeTree(piecesThatCanMove[(*plogSize)]);
	//if it has a place to advance, add it to the array
	else
		(*plogSize)++;

}