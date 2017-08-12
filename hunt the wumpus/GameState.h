#pragma once

#include "Cave.h"

// GAMESTATE class
// stores every relevant game information: cave, player, position of the wumpus
// and whether the game is over
struct GameState {
	const Cave cave;
	Player player;
	int wumpusPosition;
	bool gameOver;
};