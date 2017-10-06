#pragma once

#include "Cave.h"

// GAMESTATE class
// stores relevant game information: cave, player, position of the wumpus
// and whether the game is over
struct GameState {
	const Cave cave;
	Player player;
	int wumpusPosition;
	bool gameOver;

	// generates a random cave of the given size
	explicit GameState(int caveSize);
};

// outputs GameState to cout
void printDebugInfo(const GameState& gameState);