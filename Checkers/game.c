#include "checkers.h"

//Q4
// 
// 
void Turn(Board board, Player player)
{
	int posMax, posCurr, capture;
	//gets all possible moves of the players
	multipleSourceMovesList* playerMovesList = FindAllPossiblePlayerMoves(board, player);
	multipleSourceMovesListCell* curr = playerMovesList->head;
	//indicates the first list as the longest one
	SingleSourceMovesList* maxList = curr->single_source_moves_list;
	//gets the length of the list to be able to compare between the lists
	int maxCaptures = maxList->tail->captures, capturesInList;
	//moves to the next list 
	curr = curr->next;
	while (curr != NULL) {//checks each time if we found a longer length list
		capturesInList = curr->single_source_moves_list->tail->captures;
		if (maxCaptures < capturesInList) {
			//if so, make her the max list, and update the length
			maxCaptures = capturesInList;
			maxList = curr->single_source_moves_list;
		}
		else if (maxCaptures == capturesInList) {
			//if we found two lists with the same length maxes the max one by the rules indicated in the project
			//gets each position
			posMax = getPosVal(maxList->head->position);
			posCurr = getPosVal(curr->single_source_moves_list->head->position);
			//by the type of the player, updates the max list
			if (player == PLAYER_1 && posCurr > posMax) 
					maxList = curr->single_source_moves_list;
			else if (player == PLAYER_2 && posCurr < posMax)
					maxList = curr->single_source_moves_list;
		}
		curr = curr->next;
	}
	//takes the max list and divide her by cells to indicate each move
	SingleSourceMovesListCell* firstMove, * secondMove;
	firstMove = maxList->head;
	secondMove = firstMove->next;
	bool leftCapture;
	bool move = false;
	checkersPos* deletePos = (checkersPos*)malloc(sizeof(checkersPos));
	checkAlloc(deletePos);
	while (secondMove != NULL) {//while there is more moves to do
		capture = abs(firstMove->position->row - secondMove->position->row) - 1;//if there was a capture
		deletePos->col = firstMove->position->col;
		deletePos->row = firstMove->position->row;
		if (capture == CAPTURE) {
			//checks weather the capture was left or right
			leftCapture = leftCap(firstMove->position, secondMove->position);
			if (leftCapture) {//if left updates the position to be deleted
				goLeft(deletePos, player);
				updateBoardByMove(board, firstMove->position, deletePos, secondMove->position, player);
				move = true;
			}
			else {
				goRight(deletePos, player);
				updateBoardByMove(board, firstMove->position, deletePos, secondMove->position, player);
				move = true;
			}
		}
		else {//no capture so there is no player to "delete"
			if (!move) {
				updateBoardByMove(board, firstMove->position, NULL, secondMove->position, player);
				move = true;
			}
		}
		firstMove = secondMove;
		secondMove = secondMove->next;
	}
	//free all the dynamic allocated items
	myFree(playerMovesList);

}


bool leftCap(checkersPos* startingPos, checkersPos* endingPos) {
	int startCol, endCol;
	startCol = startingPos->col;
	endCol = endingPos->col;
	return (startCol > endCol);
}

void myFree(multipleSourceMovesList* lst) {
	multipleSourceMovesListCell* curr = lst->head;
	while (curr != NULL) {
		freeSingleList(curr->single_source_moves_list);
		curr = curr->next;
	}
}

void freeSingleList(SingleSourceMovesList* lst) {
	SingleSourceMovesListCell* curr = lst->head;
	SingleSourceMovesListCell* saver = curr->next;
	while (saver != NULL) {
		free(curr);
		curr = saver;
		saver = curr->next;
	}
	free(curr);
}





void updateBoardByMove(Board board, checkersPos* deletedPos1, checkersPos* deletedPos2, checkersPos* add, Player pl) {
	//remove the requested player from the first position
	board[deletedPos1->row][deletedPos1->col] = EMPTY;
	//if there is a player to remove in the second position (aka there is a capture and thus two positions should be changed), then remove him
	if (deletedPos2 != NULL)
		board[deletedPos2->row][deletedPos2->col] = EMPTY;
	//update the new position of the player that made the move
	board[add->row][add->col] = pl;
}


int getPosVal(checkersPos* pos) {
	int sum = 0;
	sum += pos->col;
	sum += (pos->row) * 10;
	return sum;
}
int getSingleSourceListLength(SingleSourceMovesList* lst) {
	int counter = 0;
	SingleSourceMovesListCell* curr;
	curr = lst->head;
	while (curr != NULL) {
		counter++;
		curr = curr->next;
	}
	return counter;
}

//Q5
//
//
void PlayGame(Board board, Player starting_player)
{
	playerGameNode* curPlayer;
	game game;
	Board boardBeforeTurn;

	//initalize a new game with two players with 12 pieces
	initGame(&game, starting_player, board);
	//give the first turn to the first player
	curPlayer = game.startPlayer;
	printBoard(board);
	while (!game.gameOver)
	{
		//save the previous board to compare the boards after the turn and read the turn
		copyBoard(boardBeforeTurn, game.curBoard);
		//make the best turn for the current player
		Turn(game.curBoard, curPlayer->player);
		//update the number of pieces and captures for each player
		updateGameDeatils(boardBeforeTurn, &game, curPlayer);
		printBoard(game.curBoard);
		//check if there was a winner
		isGameOver(&game);
		PASS_TURN(curPlayer);
	}
	endGame(&game);
}
void isGameOver(game* game)
{
	//if one player ran out of pieces, the other player is the winner
	if (game->startPlayer->numOfPieces == 0)
	{
		game->winner = game->startPlayer->nextPl;
		game->gameOver = true;
	}
	else if (game->startPlayer->nextPl->numOfPieces == 0)
	{
		game->winner = game->startPlayer;
		game->gameOver = true;
	}
	//if T reached the end, he is the winner
	else if (isPlayerInRow(game->curBoard[BOARD_SIZE - 1], PLAYER_1))
	{
		game->winner = (game->startPlayer->player == PLAYER_1) ? game->startPlayer : game->startPlayer->nextPl;
		game->gameOver = true;
	}
	//if B reached the start, he is the winner
	else if (isPlayerInRow(game->curBoard[0], PLAYER_2))
	{
		game->winner = (game->startPlayer->player == PLAYER_2) ? game->startPlayer : game->startPlayer->nextPl;
		game->gameOver = true;
	}
}
void updateGameDeatils(Board boardBefore,game* game, playerGameNode* curPlayer)
{
	checkersPos posBefore, posAfter;
	unsigned short captures = 0, jump;

	//check where the current player piece was before and after the turn
	readTurn(boardBefore, game->curBoard, curPlayer->player, &posBefore, &posAfter);
	//print from what position to what position the plyaer moved
	PRINT_TURN(curPlayer->player, posBefore, posAfter);
	//if the player moved just one row or didn't move at all, there were no captures. else, if he jumped x rows then there were x/2 captures
	jump = abs(posBefore.row - posAfter.row);
	captures = jump / 2;
	curPlayer->moves += jump - captures;
	//update the number of captures
	curPlayer->capturesMade += captures;
	//check if the current capture was the biggest one so far
	curPlayer->biggestCaptureMade = max(curPlayer->biggestCaptureMade, captures);
	//if captures were made, remove the pieces that were "eaten" from the opposition player
	curPlayer->nextPl->numOfPieces -= captures;
}
void readTurn(Board boardBefore, Board boardAfter, Player curPl, checkersPos* posBefore, checkersPos* posAfter)
{
	unsigned short i, j;
	bool beforeFound = false, afterFound = false;

	for (i = 0; i < BOARD_SIZE && !(beforeFound && afterFound); i++)
	{
		for (j = 0; j < BOARD_SIZE && !(beforeFound && afterFound); j++)
		{
			//if at the current position the player exists after the turn but wasn't there beforehand, therefore that's the position after turn
			if (IS_PL_IN_CELL(boardAfter[i][j], curPl) && !IS_PL_IN_CELL(boardBefore[i][j], curPl))
			{
				posAfter->row = i;
				posAfter->col = j;
				afterFound = true;
			}
			//if at the current position the player exists before the turn but isn't there afterwards, therefore that's the position before turn
			else if (IS_PL_IN_CELL(boardBefore[i][j], curPl) && !IS_PL_IN_CELL(boardAfter[i][j], curPl))
			{
				posBefore->row = i;
				posBefore->col = j;
				beforeFound = true;
			}
		}
	}
}
void initGame(game* game, Player starting_player, Board board)
{
	playerGameNode* pl1, * pl2;

	copyBoard(game->curBoard, board);

	//create two player nodes
	pl1 = createNewPlayer(PLAYER_1, board);
	pl2 = createNewPlayer(PLAYER_2, board);
	//make a game "loop"
	pl1->nextPl = pl2;
	pl2->nextPl = pl1;
	
	game->startPlayer = (starting_player == PLAYER_1) ? pl1 : pl2;
	game->gameOver = false;
	game->winner = NULL;
}
playerGameNode* createNewPlayer(Player player, Board board)
{
	playerGameNode* newPlayer;

	//allocate space
	newPlayer = (playerGameNode*)malloc(sizeof(playerGameNode));
	checkAlloc(newPlayer);

	//fill deatils
	newPlayer->player = player;
	newPlayer->numOfPieces = START_NUM_PIECES;
	newPlayer->moves = 0;
	newPlayer->capturesMade = newPlayer->biggestCaptureMade = NO_CAPTURES;

	return newPlayer;
}
void endGame(game* game)
{
	playerGameNode* plWithBiggestCap;

	//present the winner
	printf("%c wins!\n%c performed %d moves.\n", game->winner->player, game->winner->player, game->winner->moves);
	//present the player with the biggest capture in a single move
	plWithBiggestCap = BIGGER_CAPTURE(game->winner, game->winner->nextPl);
	printf("%c performed the highest number of captures in a single move - %d\n", plWithBiggestCap->player, plWithBiggestCap->biggestCaptureMade);
	//free both players nodes
	free(game->winner->nextPl);
	free(game->winner);
}
bool isPlayerInRow(unsigned char row[], Player player)
{
	unsigned short int i;
	bool found = false;
	for (i = 0; i < BOARD_SIZE && !found; i++)
		if (row[i] == player)
			found = true;
	return found;
}