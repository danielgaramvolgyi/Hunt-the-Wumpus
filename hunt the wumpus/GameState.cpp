#include "GameState.h"
#include "utils.h"
#include "constants.h"

// size has to be even (see generateCave)
GameState::GameState(int caveSize)
	:cave{generateCave(caveSize)}, 
	player{constant::STARTING_POSITION,constant::STARTING_ARROWS}, 
	gameOver{false} {
	//wumpus position is random and not equal to STARTING_POSITION
	int random = getRandomNumber(1, caveSize - 1);
	wumpusPosition = (random + constant::STARTING_POSITION) % caveSize;
}