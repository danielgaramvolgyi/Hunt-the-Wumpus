#pragma once

#include<vector>
#include<array>

struct Room {
    bool hasPit;
    bool hasBats;
};

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

struct Player {
	int position;
	int arrows;
};


