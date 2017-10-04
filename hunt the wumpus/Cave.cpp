#include "Cave.h"
#include "utils.h"
#include "constants.h"

#include<algorithm>

bool isAdjacent(int firstIndex, int secondIndex, const Cave& cave) {
	const auto& firstAdjacent = cave.adjacencyList[firstIndex];
	return find(firstAdjacent.begin(), firstAdjacent.end(), secondIndex) != firstAdjacent.end();
}

void makeCircuit(std::vector<std::array<int, 3>>& adjacencyList) {
	for (std::size_t i = 0; i < adjacencyList.size(); ++i) {
		int next = (i + 1) % adjacencyList.size();
		adjacencyList[i][1] = next;
		adjacencyList[next][0] = i;
	}
}

// TODO
void getThirdNeighbours(std::vector<std::array<int, 3>>& adjacencyList) {
	for (std::size_t i = 0; i < adjacencyList.size() / 2; ++i) {
		int neighbour = i + adjacencyList.size() / 2;
		adjacencyList[i][2] = neighbour;
		adjacencyList[neighbour][2] = i;
	}
}

// TODO
std::vector<std::array<int, 3>> shuffleIndices(std::vector<std::array<int, 3>>& adjacencyList) {
	return adjacencyList;
}

// generates, in effect, a connected 3-regular graph
// first vertices 0 and 1, 1 and 2, etc. are connected to ensure connectedness
// then each vertex gets a random additional neighbour
// finally the indices are shuffled
std::vector<std::array<int, 3>> generateAdjacencyList(int size) {
	std::vector<std::array<int, 3>> adjacencyList(size);
	makeCircuit(adjacencyList);
	getThirdNeighbours(adjacencyList);
	return shuffleIndices(adjacencyList);
}

bool generateRandomBool(double probability) {
    bool randomBool;
    if (getRandomNumber(0, 99) < 100 * probability) randomBool = true;
	return randomBool;
}

void randomizeRooms(std::vector<Room>& rooms) {
    for (Room& room : rooms) {
        room.hasBats = generateRandomBool(constant::BAT_PROBABILITY);
        room.hasPit = generateRandomBool(constant::PIT_PROBABILITY);
    }
}

void clearStartingPosition(std::vector<Room>& rooms) {
    rooms[constant::STARTING_POSITION].hasBats = false;
    rooms[constant::STARTING_POSITION].hasPit = false;
}

std::vector<Room> generateRooms(int size) {
    std::vector<Room> rooms(size);
    randomizeRooms(rooms);
    clearStartingPosition(rooms);
    return rooms;
}

// size has to be even (a 3-regular graph must have an even number of vertices)
const Cave generateCave(int size) {
	return Cave{ size, generateRooms(size), generateAdjacencyList(size)};
}
