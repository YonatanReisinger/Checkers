#ifndef __CHECKERS_H__
#define __CHECKERS_H__
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//defines
#define _CRT_SECURE_NO_WARNINGS
#define BOARD_SIZE 8

//typdefs
typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];
typedef unsigned char Player;

//structs
typedef struct _checkersPos
{
	char row;
	char col;
}checkersPos;

typedef struct _SingleSourceMovesTreeNode {
	Board board;
	checkersPos *pos;
	unsigned short total_captures_so_far;
	struct _SingleSourceMovesTreeNode* nextMoves[2];
}SingleSourceMovesTreeNode;

typedef struct _SingleSourceMovesTree {
	SingleSourceMovesTreeNode *source;
}SingleSourceMovesTree;

typedef struct _SingleSourceMovesListCell {
	checkersPos *position;
	unsigned short captures;
	struct _SingleSourceMovesListCell* next;
}SingleSourceMovesListCell;

typedef struct _SingleSourceMovesList {
	SingleSourceMovesListCell* head;
	SingleSourceMovesListCell *tail;
}SingleSourceMovesList;

typedef struct _multipleSourceMovesListCell {
	SingleSourceMovesList* single_source_moves_list;
	struct _multipleSourceMovesListCell* next;
}multipleSourceMovesListCell;

typedef struct _multipleSourceMovesList {
	multipleSourceMovesListCell* head;
	multipleSourceMovesListCell* tail;
}multipleSourceMovesList;

//functions
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src);
SingleSourceMovesList* FindSingleSourcOptimaleMove(SingleSourceMovesTree* moves_tree);
multipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player);
void Turn(Board board, Player player);

#endif // !__CHECKERS_H__
