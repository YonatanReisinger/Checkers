#include "checkers.h"

SingleSourceMovesListCell * createNewCell(checkersPos * position, unsigned short cap) {
	SingleSourceMovesListCell* curr = (SingleSourceMovesListCell*)malloc(sizeof(SingleSourceMovesListCell));
	checkAlloc(curr);
	curr->position = position;
	curr->captures = cap;
	curr->next = NULL;
}

bool isEmptyList(SingleSourceMovesList* lst) {
	return (lst->head == NULL);
}

SingleSourceMovesList* createEmptyList() {//creates empty list and returns it
	SingleSourceMovesList* curr = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
	checkAlloc(curr);
	curr->head = NULL;
	curr->tail = NULL;
	return curr;
}

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

bool canEatLeft(SingleSourceMovesTreeNode* moves_root, Player player) {
	//checks if a player can eat the opponent on his left
	checkersPos currPos;
	currPos.col = moves_root->pos->col;
	currPos.row = moves_root->pos->row;
	if (player == PLAYER_1) {
		goLeft(&currPos, player);
		if (exist(moves_root->board, &currPos, PLAYER_2)) {
			goLeft(&currPos, player);
			if (IS_EMPTY_CELL(moves_root->board, currPos.row, currPos.col)) {
				return true;
			}
		}
		return false;
	}
	else {
		goLeft(&currPos, player);
		if (exist(moves_root->board, &currPos, PLAYER_1)) {
			goLeft(&currPos, player);
			if (IS_EMPTY_CELL(moves_root->board, currPos.row, currPos.col)) {
				return true;
			}
		}
		return false;
	}
}

bool canEatRight(SingleSourceMovesTreeNode* moves_root, Player player) {
	//checks if a player can eat the opponent on his left
	checkersPos currPos;
	currPos.col = moves_root->pos->col;
	currPos.row = moves_root->pos->row;
	if (player == PLAYER_1) {
		goRight(&currPos, player);
		if (exist(moves_root->board, &currPos, PLAYER_2)) {
			goRight(&currPos, player);
			if (IS_EMPTY_CELL(moves_root->board, currPos.row, currPos.col)) {
				return true;
			}
		}
		return false;
	}
	else {
		goRight(&currPos, player);
		if (exist(moves_root->board, &currPos, PLAYER_1)) {
			goRight(&currPos, player);
			if (IS_EMPTY_CELL(moves_root->board, currPos.row, currPos.col)) {
				return true;
			}
		}
		return false;
	}
}

short int howManyCaptured(SingleSourceMovesTreeNode* moves_root, Board board, int capLeft, int capRight, Player player) {
	int captureLeft, captureRight;
	if (moves_root == NULL) {
		return 0;
	}
	else {
		if (canEatLeft(moves_root, player)) {
			captureLeft = howManyCaptured(moves_root->nextMoves[LEFT], moves_root->board, capLeft + 1, capRight, player);
		}
		else {
			captureLeft = howManyCaptured(moves_root->nextMoves[LEFT], moves_root->board, capLeft, capRight, player);
		}
		if (canEatRight(moves_root, player)) {
			captureRight = howManyCaptured(moves_root->nextMoves[RIGHT], moves_root->board, capLeft, capRight + 1, player);
		}
		else {
			howManyCaptured(moves_root->nextMoves[RIGHT], moves_root->board, capLeft, capRight, player);
		}
	}
	return max(captureLeft, capRight);
}


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
				makeListOfCells(lst, root->nextMoves[LEFT], player);
			else
				makeListOfCells(lst, root->nextMoves[RIGHT], player);
		}
		else {
			if (player == PLAYER_1)
				makeListOfCells(lst, root->nextMoves[RIGHT], player);
			else
				makeListOfCells(lst, root->nextMoves[LEFT], player);
		}
	}
}

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