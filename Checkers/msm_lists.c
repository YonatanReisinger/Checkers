#include "checkers.h"

/****************
* Function name: FindAllPossiblePlayerMoves
* Input: checkers board + a letter that reprsents a certain player
* Output: list, that each node of the list represents a specific piece of that player and the list of the best move of that piece
* Function operation: Generates a list of possible moves for all the pieces of a specific player on the checkers board.
****************/
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
		insertDataToEndMSMList(newMSMlist, FindSingleSourceOptimalMove(piecesThatCanMove[i]));

	freeTreeArr(piecesThatCanMove, numOfPiecesThatCanMove);
	return newMSMlist;
}
/****************
* Function name: insertDataToEndMSMList
* Input: Multiple source moves list and a single source moves list.
* Function operation: Inserts a single source moves list as data to the end of a multiple source moves list.
****************/
void insertDataToEndMSMList(multipleSourceMovesList* MSMList, SingleSourceMovesList* dataList)
{
	multipleSourceMovesListCell* MSMCell;
	//create new msm cell with ssm list as it's data
	MSMCell = createNewMSMCell(dataList, NULL);
	//insert the new cell to the msm list
	insertCellToEndMSMList(MSMList, MSMCell);
}
/****************
* Function name: insertCellToEndMSMList
* Input: Multiple source moves list and a multiple source moves list cell.
* Function operation: Inserts a multiple source moves list cell to the end of a multiple source moves list.
****************/
void insertCellToEndMSMList(multipleSourceMovesList* MSMList, multipleSourceMovesListCell* MSMCell)
{
	//if the list is empty, thus the new node should be the head and tail
	if (IS_EMPTY_LIST(MSMList))
	{
		MSMCell->next = NULL;
		MSMList->head = MSMList->tail = MSMCell;
	}
	else
	{
		MSMCell->next = NULL;
		//the function inserts a node to the end, thus the new node is the new tail
		MSMList->tail->next = MSMCell;
		MSMList->tail = MSMCell;
	}
}
/****************
* Function name: createNewMSMCell
* Input: Single source moves list and a pointer to the next cell.
* Output: Newly created multiple source moves list cell.
* Function operation: Creates a new multiple source moves list cell with the provided single source moves list as data.
****************/
multipleSourceMovesListCell* createNewMSMCell(SingleSourceMovesList* dataList, multipleSourceMovesListCell* next)
{
	multipleSourceMovesListCell* MSMCell;
	//allocate space for the new node
	MSMCell = (multipleSourceMovesListCell*)malloc(sizeof(multipleSourceMovesListCell));
	checkAlloc(MSMCell);

	//fill deatils
	MSMCell->single_source_moves_list = dataList;
	MSMCell->next = next;

	return MSMCell;
}
/****************
* Function name: makeEmptyMSMList
* Output: Empty multiple source moves list.
* Function operation: Creates an empty multiple source moves list.
****************/
multipleSourceMovesList* makeEmptyMSMList()
{
	multipleSourceMovesList* newMSMlist;
	//allocate space for the new list
	newMSMlist = (multipleSourceMovesList*)malloc(sizeof(multipleSourceMovesList));
	newMSMlist->head = newMSMlist->tail = NULL;
	checkAlloc(newMSMlist);
	return newMSMlist;
}
/****************
* Function name: freeTreeArr
* Input: Array of single source moves trees and the size of the array.
* Function operation: Frees the memory allocated for an array of single source moves trees.
****************/
void freeTreeArr(SingleSourceMovesTree** treeArr, unsigned short size)
{
	unsigned short int i;
	for (i = 0; i < size; i++)
		freeTree(treeArr[i]);
	free(treeArr);
}