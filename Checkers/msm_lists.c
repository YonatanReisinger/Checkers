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
		insertDataToEndMSMList(newMSMlist, FindSingleSourceOptimalMove(piecesThatCanMove[i]));

	free(piecesThatCanMove);
	return newMSMlist;
}
void insertDataToEndMSMList(multipleSourceMovesList* MSMList, SingleSourceMovesList* dataList)
{
	multipleSourceMovesListCell* MSMCell;
	//create new msm cell with ssm list as it's data
	MSMCell = createNewMSMCell(dataList, NULL);
	//insert the new cell to the msm list
	insertCellToEndMSMList(MSMList, MSMCell);
}
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

multipleSourceMovesList* makeEmptyMSMList()
{
	multipleSourceMovesList* newMSMlist;
	//allocate space for the new list
	newMSMlist = (multipleSourceMovesList*)malloc(sizeof(multipleSourceMovesList));
	newMSMlist->head = newMSMlist->tail = NULL;
	checkAlloc(newMSMlist);
	return newMSMlist;
}

/// לזכור למחוק בסוף !!!!!!!!!!!!!!!!!!!!!!!
void printMSMList(multipleSourceMovesListCell* head)
{
	multipleSourceMovesListCell* cur;
	SingleSourceMovesListCell* ssmCur;

	for (cur = head; cur != NULL; cur = cur->next)
	{
		printf("from %d %d position:\n", cur->single_source_moves_list->head->position->row, cur->single_source_moves_list->head->position->col);
		for (ssmCur = cur->single_source_moves_list->head; ssmCur != NULL; ssmCur = ssmCur->next)
		{
			printf("%d %d\n", ssmCur->position->row, ssmCur->position->col);
		}
	}
}