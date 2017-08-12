#pragma once

#include<vector>

// Cave class
// stores cave size, adjacency list of rooms, and the position of hazards
struct Cave {
	int mapSize;
	std::vector<std::vector<int>> adjacencyList;
	std::vector<bool> pits, bats;
};

// randomly generates a cave of the given size
// cave is guaranteed to be connected and the player's
// starting position (see below) won't contain any hazards
const Cave generateCave(int size);

constexpr int STARTING_POSITION = 5;
constexpr int STARTING_ARROWS = 0;

// PLAYER class
// stores player position and number of available arrows
struct Player {
	int position = STARTING_POSITION;
	int arrows = STARTING_ARROWS;
};


