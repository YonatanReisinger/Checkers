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


//macros for 5
#define IS_PL_IN_CELL(pos, pl) ((pos) == (pl))
#define PASS_TURN(curPlayer) curPlayer = curPlayer->nextPl
#define PRINT_TURN(pl, posBefore, posAfter) printf("%c's turn:\n%c%d->%c%d\n", pl, posBefore.row + 'A', posBefore.col, posAfter.row + 'A', posAfter.col)
#define BIGGER_CAPTURE(pl1, pl2) (((pl1->biggestCaptureMade) >= (pl2->biggestCaptureMade)) ? (pl1) : (pl2))

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

typedef struct PlayerGameNode {
	Player player; //player letter
	unsigned short int numOfPieces; //current pieces of him on the board
	unsigned short int capturesMade; //captures made so far in the game by the player
	unsigned short int biggestCaptureMade; //biggest jump made so far by the player in the game
	struct PlayerGameNode* nextPl;
}playerGameNode;

typedef struct Game {
	Board curBoard;
	playerGameNode* startPlayer;
	bool gameOver;
	playerGameNode* winner;
}game;
//functions

//Q1
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src);
SingleSourceMovesTreeNode* FindSingleSourceMovesHelper(Board board, checkersPos* src, Player player);
checkersPos getNextPos(Player player, checkersPos* currentPos, bool direction);
SingleSourceMovesTreeNode* createNewSSMTreeNode(Board board, checkersPos* position, unsigned short captures, SingleSourceMovesTreeNode* next1, SingleSourceMovesTreeNode* next2);
void copyBoard(Board destBoard, Board srcBoard);
void updateBoard(Board oldBoard, Board newBoard, checkersPos* deletedPos1, checkersPos* deletedPos2, checkersPos* add, Player pl);
SingleSourceMovesTree* makeEmptyTree();
SingleSourceMovesTreeNode* findSideMove(bool direction, Board board, Player player, checkersPos* src, unsigned short* pcaptures);
SingleSourceMovesTreeNode* addCaptureToTree(bool direction, Board board, Board sideBoard, Player player, checkersPos* myPos, checkersPos* oppPos, unsigned short* pcaptures);

//Q2
SingleSourceMovesListCell* createNewCell(checkersPos* position, unsigned short cap);
bool isEmptyList(SingleSourceMovesList* lst);
SingleSourceMovesList* createEmptyList();
void insertDataToEndList(SingleSourceMovesList* lst, unsigned short cap, checkersPos* pos);
bool exist(Board board, checkersPos* position, Player player);
void goRight(checkersPos* pos, Player player);
void goLeft(checkersPos* pos, Player player);
bool canEatLeft(SingleSourceMovesTreeNode* moves_root, Player player);
bool canEatRight(SingleSourceMovesTreeNode* moves_root, Player player);
short int howManyCaptured(SingleSourceMovesTreeNode* moves_root, Board board, int capLeft, int capRight, Player player);
void makeListOfCells(SingleSourceMovesList* lst, SingleSourceMovesTreeNode* root, Player player);
SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree);


//Q3
multipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player);
multipleSourceMovesList* makeEmptyMSMList();
SingleSourceMovesTree** getPiecesThatCanMove(Board board, Player player, unsigned short int* pSize);
void freeTreeArr(SingleSourceMovesTree** treeArr, unsigned short size);
void addPiecesThatCanMove(SingleSourceMovesTree** piecesThatCanMove, unsigned short* plogSize, unsigned short pieceRow, unsigned short pieceCol, Board board);
void insertDataToEndMSMList(multipleSourceMovesList* MSMList, SingleSourceMovesList* dataList);
void insertCellToEndMSMList(multipleSourceMovesList* MSMList, multipleSourceMovesListCell* MSMCell);
multipleSourceMovesListCell* createNewMSMCell(SingleSourceMovesList* dataList, multipleSourceMovesListCell* next);
void printMSMList(multipleSourceMovesListCell* head);

//Q5
//
//

void PlayGame(Board board, Player starting_player);
bool isPlayerInRow(unsigned char row[], Player player);
void initGame(game* game, Player starting_player, Board board);
playerGameNode* createNewPlayer(Player player, Board board);
void endGame(game* game);
void updateGameDeatils(Board boardBefore, game* game, playerGameNode* curPlayer);
void readTurn(Board boardBefore, Board boardAfter, Player curPl, checkersPos* posBefore, checkersPos* posAfter);
void isGameOver(game* game);

// etc
//
//
void Turn(Board board, Player player);
void initBoard(Board board);
void initBoardRow(unsigned char row[], unsigned short int rowInd, Player player);
void printBoard(Board board);
void printBoardRow(unsigned char row[]);
void checkAlloc(void* ptr);
void freeTree(SingleSourceMovesTree* tr);
void freeTreeHelper(SingleSourceMovesTreeNode* root);

#endif // !__CHECKERS_H__
