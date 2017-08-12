#pragma once

#include "GameState.h"
#include "Action.h"
#include "IOHandler.h"

// GAME class
// handles game events
class Game {
public:
	Game(int mapSize);

	// keeps running turns until the game ends (the player wins, loses or quits)
	void run();
private:

	// prompts user for action and then executes action
	void playTurn();

	GameState gameState;
	IOHandler ioHandler;
};