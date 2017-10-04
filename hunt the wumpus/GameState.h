#pragma once

#include "Cave.h"

// GAMESTATE class
// stores relevant game information: cave, player, position of the wumpus
// and whether the game is over
struct GameState {
	// uses randomly generated cave
	explicit GameState(int caveSize);

	const Cave cave;
	Player player;
	int wumpusPosition;
	bool gameOver;
};

// outputs GameState to console
void printDebugInfo(const GameState& gameState);