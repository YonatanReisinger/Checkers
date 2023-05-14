#ifndef __CHECKERS_H__
#define __CHECKERS_H__
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//defines
#define _CRT_SECURE_NO_WARNINGS
#define BOARD_SIZE 8
#define EMPTY ' '
#define ENTER '\n'
#define PLAYER_1 'T'
#define PLAYER_2 'B'
#define LEFT 0
#define RIGHT 1
#define NO_CAPTURES 0
#define START_NUM_PIECES 12
//defines for 3

//macros
#define PRINT_PLUS_MINUS puts("+-+-+-+-+-+-+-+-+-+")
#define PRINT_NUMS_ROW puts("+ |1|2|3|4|5|6|7|8|")
#define IS_EVEN(num) (((num) % 2) == 0)
#define IS_ODD(num) (((num) % 2) == 1)
#define IS_EMPTY_CELL(board,row,col) ((board[row][col]) == (EMPTY))
#define IS_CORD_VALID(cord) ((0 <= cord) && (cord < BOARD_SIZE))
#define IS_CELL_VALID(row, col) (IS_CORD_VALID(row) && IS_CORD_VALID(col))

//macros for 3
#define IS_EMPTY_LIST(lst) ((lst->head) == NULL)
#define INIT_POS(pos,Row, Col) pos.row = Row; pos.col = Col;
#define IS_NO_MOVES(tNode) (((tNode->nextMoves[LEFT]) == NULL) && ((tNode->nextMoves[RIGHT]) == NULL))
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
SingleSourceMovesTreeNode* FindSingleSourceMovesHelper(Board board, checkersPos* src, Player player);
checkersPos getNextPos(Player player, checkersPos* currentPos, bool direction);
SingleSourceMovesTreeNode* createNewSSMTreeNode(Board board, checkersPos* position, unsigned short captures, SingleSourceMovesTreeNode* next1, SingleSourceMovesTreeNode* next2);
void copyBoard(Board destBoard, Board srcBoard);
void updateBoard(Board oldBoard, Board newBoard, checkersPos* deletedPos1, checkersPos* deletedPos2, checkersPos* add, Player pl);
SingleSourceMovesTree* makeEmptyTree();
SingleSourceMovesList* FindSingleSourcOptimaleMove(SingleSourceMovesTree* moves_tree);

multipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player);
multipleSourceMovesList* makeEmptyMSMList();
SingleSourceMovesTree** getPiecesThatCanMove(Board board, Player player, unsigned short int* pSize);
void insertDataToEndMSMList(multipleSourceMovesList* MSMList, SingleSourceMovesList* data);
void insertCellToEndMSMList(multipleSourceMovesList* MSMList, multipleSourceMovesListCell* MSMCell);
multipleSourceMovesListCell* createNewMSMCell(SingleSourceMovesList* data, multipleSourceMovesListCell* next);

void Turn(Board board, Player player);
void PlayGame(Board board, Player starting_player);
void initBoard(Board board);
void initBoardRow(unsigned char row[], unsigned short int rowInd, Player player);
void printBoard(Board board);
void printBoardRow(unsigned char row[]);
void checkAlloc(void* ptr);
void freeTree(SingleSourceMovesTree* tr);
void freeTreeHelper(SingleSourceMovesTreeNode* root);

#endif // !__CHECKERS_H__
