#pragma once

#include<vector>
#include<array>

struct Room {
    bool hasPit;
    bool hasBats;
};

// Cave class
// stores cave size, adjacency list of rooms, and the position of hazards
struct Cave {
	int mapSize;
	std::vector<Room> rooms;
	std::vector<std::array<int,3>> adjacencyList;
};

// index ranges are not checked
bool isAdjacent(int firstIndex, int secondIndex, const Cave& cave);

// randomly generates a cave of the given size
// cave is guaranteed to be connected and the player's
// starting position won't contain any hazards
const Cave generateCave(int size);

// PLAYER class
// stores player position and number of available arrows
struct Player {
	int position;
	int arrows;
};


