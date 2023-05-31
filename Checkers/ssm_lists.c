#include "checkers.h"

/****************
* Function name: CreateNewCell
* Input: current checkers position + number of captures
* Output: a SingleSourceMovesListCell with tha same position and same amount of captures
* Function operation: returns a new allocated SingleSourceMovesListCell
****************/
SingleSourceMovesListCell* createNewCell(checkersPos* position, unsigned short cap) {
	SingleSourceMovesListCell* curr = (SingleSourceMovesListCell*)malloc(sizeof(SingleSourceMovesListCell));
	checkAlloc(curr);
	curr->position = position;
	curr->captures = cap;
	curr->next = NULL;
}
/****************
* Function name: isEmptyList
Input: Pointer to a SingleSourceMovesList.
Output: Returns true if the list is empty, false otherwise
Function operation: Checks if a given SingleSourceMovesList is empty.
/****************/
bool isEmptyList(SingleSourceMovesList* lst) {
	return (lst->head == NULL);
}
/****************
* Function name: isEmptyList
* Output: Returns a pointer to the newly allocated SingleSourceMovesList.
* Function operation: Creates an empty SingleSourceMovesList.
/****************/
SingleSourceMovesList* createEmptyList() {//creates empty list and returns it
	SingleSourceMovesList* curr = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
	checkAlloc(curr);
	curr->head = NULL;
	curr->tail = NULL;
	return curr;
}
/****************
* Input: Pointer to a SingleSourceMovesList, captures value, and checkers position.
* Output: Modifies the list by adding a new SingleSourceMovesListCell at the end.
* Function operation: Inserts data (position and captures) to the end of a given list.
/****************/
void insertDataToEndList(SingleSourceMovesList* lst, unsigned short cap, checkersPos* pos) {
	//inserts data to end list
	SingleSourceMovesListCell* cell = createNewCell(pos, cap);
	checkAlloc(cell);
	if (isEmptyList(lst)) {
		lst->head = lst->tail = cell;
	}
	else {
		lst->tail->next = cell;
		lst->tail = cell;
		cell->next = NULL;
	}
}
/****************
* Input: Pointer to a SingleSourceMovesTreeNode and the player.
* Output: Returns true if the player can perform the left-side capture, false otherwise.
* Function operation: Checks if a player can eat the opponent on their left.
/****************/
bool canEatLeft(SingleSourceMovesTreeNode* moves_root, Player player) {
	//checks if a player can eat the opponent on his left
	checkersPos currPos;
	INIT_POS(currPos, moves_root->pos->row, moves_root->pos->col);
	bool oppToLeft;

	goLeft(&currPos, player);
	//check if the position is to the side
	if (player == PLAYER_1)
		oppToLeft = exist(moves_root->board, &currPos, PLAYER_2);
	else
		oppToLeft = exist(moves_root->board, &currPos, PLAYER_1);
	//go to the position after the position with the opposition
	goLeft(&currPos, player);
	//if there is an opposition on the left and after the opposition there is place to move, thus the player can eat
	return oppToLeft && (IS_EMPTY_CELL(moves_root->board, currPos.row, currPos.col));
}
/****************
* Input: Pointer to a SingleSourceMovesList, pointer to a SingleSourceMovesTreeNode, and the player.
* Output: Modifies the list by adding cells representing the optimal moves.
* Function operation: Constructs a list of cells representing the optimal moves for a given player in a given move tree.
/****************/
void makeListOfCells(SingleSourceMovesList* lst, SingleSourceMovesTreeNode* root, Player player) {
	int capLeft, capRight;
	if (root == NULL)
		return;
	insertDataToEndList(lst, NO_CAPTURES, root->pos);
	if (root->nextMoves[LEFT] == NULL && root->nextMoves[RIGHT] == NULL)
		return;
	else if (root->nextMoves[LEFT] == NULL)
		makeListOfCells(lst, root->nextMoves[RIGHT], player);
	else if (root->nextMoves[RIGHT] == NULL)
		makeListOfCells(lst, root->nextMoves[LEFT], player);
	else {
		capLeft = root->nextMoves[LEFT]->total_captures_so_far;
		capRight = root->nextMoves[RIGHT]->total_captures_so_far;
		if (capLeft > capRight)
			makeListOfCells(lst, root->nextMoves[LEFT], player);
		else if (capRight > capLeft)
			makeListOfCells(lst, root->nextMoves[RIGHT], player);
		else if (root->total_captures_so_far != NO_CAPTURES) {
			if (canEatLeft(root, player))
			{
				makeListOfCells(lst, root->nextMoves[LEFT], player);
				free(root->nextMoves[RIGHT]->pos);
			}
			else
			{
				makeListOfCells(lst, root->nextMoves[RIGHT], player);
				free(root->nextMoves[LEFT]->pos);
			}
		}
		else {
			if (player == PLAYER_1)
			{
				makeListOfCells(lst, root->nextMoves[RIGHT], player);
				free(root->nextMoves[LEFT]->pos);
			}
			else
			{
				makeListOfCells(lst, root->nextMoves[LEFT], player);
				free(root->nextMoves[RIGHT]->pos);
			}
		}
	}
}
/****************
* Input: Pointer to a SingleSourceMovesTree.
* Output: Returns a SingleSourceMovesList containing the optimal moves for the checkers piece.
* Function operation: Finds the optimal move for a single checkers piece.
/****************/
SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree) {
	Player currPlayer;
	char row, col;
	int length;
	row = moves_tree->source->pos->row;
	col = moves_tree->source->pos->col;
	currPlayer = moves_tree->source->board[row][col];
	//SingleSourceMovesList* optList = createEmptyList();
	SingleSourceMovesList* curr = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
	checkAlloc(curr);
	curr->head = NULL;
	curr->tail = NULL;
	makeListOfCells(curr, moves_tree->source, currPlayer);
	if (moves_tree->source->total_captures_so_far != NO_CAPTURES) {
		SingleSourceMovesListCell* currCell = curr->head->next;
		while (currCell != NULL) {
			currCell->captures += 1;
			currCell = currCell->next;
		}
	}
	return curr;
}