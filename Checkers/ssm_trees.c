#include "checkers.h"

/****************
* Function name: FindSingleSourceMoves
* Input: board of checkers game + position on the board
* Output: a binary tree that represents all the possible moves of a specific piece in that position
* Function operation:using a helper function,
the function generates a tree that each of it's nodes represent a position on the board that the player can move to
****************/
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
/****************
* Function name: FindSingleSourceMovesHelper
* Input: board of checkers game + position on the board + the type of player at this position
* Output: a binary tree that represents all the possible moves of a specific piece in that position
* Function operation: the function generates a tree node that represent a position on the board of the player and the next positions he can move to
****************/
SingleSourceMovesTreeNode* FindSingleSourceMovesHelper(Board board, checkersPos* src, Player player)
{
	SingleSourceMovesTreeNode* leftMove, * rightMove;
	unsigned short capturesLeft = 0, capturesRight = 0;

	leftMove = findSideMove(LEFT ,board, player, src, &capturesLeft);
	rightMove = findSideMove(RIGHT ,board, player, src, &capturesRight);

	//retrun a newTnode that start at the src, contains the maximum number of captures that can be made from this position and has the two options that the current player has
	return createNewSSMTreeNode(board, src, max(capturesLeft, capturesRight), leftMove, rightMove);
}
/****************
* Function name: findSideMove
* Input: Direction (left or right), checkers board, player type, current position, and a pointer to store the number of captures.
* Output:Tree node representing the possible move in the given direction.
* Function operation: Determines the possible move in a specific direction (left or right) for a player on the checkers board.
****************/
SingleSourceMovesTreeNode* findSideMove(bool direction, Board board, Player player, checkersPos* src, unsigned short* pcaptures)
{
	SingleSourceMovesTreeNode* sideMove;
	Board sideBoard;
	checkersPos nextPos;

	//get the position of the diagonal
	nextPos = getNextPos(player, src, direction);

	//if the next left position is outside the board or it is the same player as the current one, thus there is no where to advance
	if (!IS_CELL_VALID(nextPos.row, nextPos.col) || board[nextPos.row][nextPos.col] == player)
		sideMove = NULL;

	//if the left is empty, the player can move there and then has to stop
	else if (IS_EMPTY_CELL(board, nextPos.row, nextPos.col))
	{
		updateBoard(board, sideBoard, src, NULL, &nextPos, player);
		sideMove = createNewSSMTreeNode(sideBoard, &nextPos, NO_CAPTURES, NULL, NULL);
	}
	//else, the opponnent is in the left postion, attempt a capture
	else
		sideMove = addCaptureToTree(direction, board, sideBoard, player, src, &nextPos, pcaptures);

	return sideMove;
}
/****************
* Function name: addCaptureToTree
* Input: Direction (left or right), checkers board, temporary board for capturing, player type, current position, opponent position, and a pointer to store the number of captures.
* Output: Tree node representing the captured position if a capture is possible, otherwise NULL.
* Function operation: Adds a capture move to the tree if it is possible for the player in a specific direction.
****************/
SingleSourceMovesTreeNode* addCaptureToTree(bool direction, Board board, Board sideBoard ,Player player, checkersPos* myPos, checkersPos* oppPos, unsigned short* pcaptures)
{
	SingleSourceMovesTreeNode* sideMove;
	checkersPos nextNextPos = getNextPos(player, oppPos, direction);

	//if the next position is an opponnent and the position after him is valid and empty, thus a capture can be made
	if (IS_CELL_VALID(nextNextPos.row, nextNextPos.col) && IS_EMPTY_CELL(board, nextNextPos.row, nextNextPos.col))
	{
		(*pcaptures)++;
		//delte my old position and the opponent position from the board, update my piece to be in the position after the capture is made
		updateBoard(board, sideBoard, myPos, oppPos, &nextNextPos, player);
		//build another tree for the postion adter the capture
		sideMove = FindSingleSourceMovesHelper(sideBoard, &nextNextPos, player);
		//update the number of captures
		(*pcaptures) += sideMove->total_captures_so_far;
	}
	//if the position after the opponnent is occupied, then a capture can't be made
	else
		sideMove = NULL;

	return sideMove;
}
/****************
* Function name: createNewSSMTreeNode
* Input: Checkers board, position, number of captures, and two child tree nodes.
* Output: Newly created tree node.
* Function operation: Creates a new tree node for the single source moves tree.
****************/
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
/****************
* Function name: makeEmptyTree
* Output: Empty single source moves tree.
* Function operation: Creates an empty single source moves tree.
****************/
SingleSourceMovesTree* makeEmptyTree()
{
	SingleSourceMovesTree* newTree;
	//allocate space in memory for the new single source moves tree
	newTree = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));
	//make sure that the allocation succeeded
	checkAlloc(newTree);
	return newTree;
}
/****************
* Function name: getPiecesThatCanMove
* Input: Checkers board, player type, and a pointer to store the size of the resulting array.
* Output: Array of single source moves trees representing the pieces that can move.
* Function operation: Retrieves all the pieces of a player on the board that have possible moves.
****************/
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
/****************
* Function name: addPiecesThatCanMove
* Input: Array of single source moves trees, pointer to the logical size of the array, piece row, piece column, and the checkers board.
* Function operation: Adds a piece to the array of pieces that can move if it has possible moves.
****************/
void addPiecesThatCanMove(SingleSourceMovesTree** piecesThatCanMove, unsigned short* plogSize, unsigned short pieceRow, unsigned short pieceCol, Board board)
{
	checkersPos pos;

	INIT_POS(pos, pieceRow, pieceCol)
	//find its moves options
	piecesThatCanMove[(*plogSize)] = FindSingleSourceMoves(board, &pos);
	//if it has no where to advance, thus it should not be in the array of pieces that can move
	if (IS_NO_MOVES(piecesThatCanMove[(*plogSize)]->source))
		freeTree(piecesThatCanMove[(*plogSize)]);
	//if it has a place to advance, add it to the array
	else
		(*plogSize)++;
}