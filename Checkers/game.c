#include "checkers.h"

//Q5
//
//
void PlayGame(Board board, Player starting_player)
{
	playerGameNode* curPlayer;
	game game;

	initGame(&game, starting_player, board);
	curPlayer = game.startPlayer;

	printBoard(board);

	while (!game.gameOver)
	{
		printf("%c's turn:\n", curPlayer->player);
		Turn(game.curBoard, curPlayer->player);

		game.gameOver = IS_GAME_OVER(game.curBoard, curPlayer->numOfPieces, curPlayer->nextPl->numOfPieces);
	}

	endGame(&game);
}

void initGame(game* game, Player starting_player, Board board)
{
	playerGameNode* pl1, * pl2;

	copyBoard(game->curBoard, board);

	pl1 = createNewPlayer(PLAYER_1, board);
	pl2 = createNewPlayer(PLAYER_2, board);
	//make a game "loop"
	pl1->nextPl = pl2;
	pl2->nextPl = pl1;
	game->startPlayer = (starting_player == PLAYER_1) ? pl1 : pl2;
	game->gameOver = false;
	game->winner = EMPTY;
}
playerGameNode* createNewPlayer(Player player, Board board)
{
	playerGameNode* newPlayer;

	//allocate space
	newPlayer = (playerGameNode*)malloc(sizeof(playerGameNode));
	checkAlloc(newPlayer);

	//fill deatils
	newPlayer->player = player;
	newPlayer->bestMoves = FindAllPossiblePlayerMoves(board, player);
	newPlayer->numOfPieces = START_NUM_PIECES;
	newPlayer->capturesMade = newPlayer->biggestCaptureMade = 0;

	return newPlayer;
}
void endGame(game* game)
{
	free(game->startPlayer->nextPl);
	free(game->startPlayer);
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