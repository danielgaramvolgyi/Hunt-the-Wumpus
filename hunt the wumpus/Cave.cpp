#include "Cave.h"
#include "utils.h"
#include "constant.h"

#include <algorithm>
#include <random>

using AdjacencyList = std::vector<std::array<int, 3>>;

bool isAdjacent(int firstIndex, int secondIndex, const Cave& cave) {
	const auto& firstAdjacent = cave.adjacencyList[firstIndex];
	return find(firstAdjacent.begin(), firstAdjacent.end(), secondIndex) != firstAdjacent.end();
}


void makeCircuit(AdjacencyList& adjacencyList) {
	for (std::size_t i = 0; i < adjacencyList.size(); ++i) {
		int next = (i + 1) % adjacencyList.size();
		adjacencyList[i][1] = next;
		adjacencyList[next][0] = i;
	}
}

int numberOfPossibleNeighbours(int i, const std::vector<bool>& hasNeighbours) {
    int mapSize = hasNeighbours.size();
    int freeNodes = 0;
    for (int j = i+2; j < mapSize; ++j) {
        if (!hasNeighbours[j]) {
            ++freeNodes;
        }
    }
    return freeNodes;
}

// returns the map size if there are less than n possible neighbours
int getNthPossibleNeighbour(int i, int n, const std::vector<bool>& hasNeighbour) {
    int mapSize = hasNeighbour.size();
    int possibleNeighbour = i + 1;
    while (n > 0 && possibleNeighbour < mapSize) {
        do {
            ++possibleNeighbour;
        } while (possibleNeighbour < mapSize && hasNeighbour[possibleNeighbour]);
        --n;
    }
    return possibleNeighbour;
}

int getRandomNeighbour(int i, const std::vector<bool>& hasNeighbour) {
    return getNthPossibleNeighbour(i, getRandomNumber(1, numberOfPossibleNeighbours(i, hasNeighbour)), hasNeighbour);
}

int findFirstNeighbourlessIndexAfter(int i, const std::vector<bool>& hasNeighbour) {
    int result = i;
    while (hasNeighbour[result]) {
        ++result;
    }
    return result;
}

void connectIndices(int i, int j, AdjacencyList& adjacencyList, std::vector<bool>& hasNeighbour) {
    adjacencyList[i][2] = j;
    adjacencyList[j][2] = i;
    hasNeighbour[i] = true;
    hasNeighbour[j] = true;
}

// TODO
void getThirdNeighbours(AdjacencyList& adjacencyList) {
    int mapSize = adjacencyList.size();
    std::vector<bool> hasNeighbour(mapSize);
    int currentIndex = 0;
    for (int i = 0; i < mapSize / 2 - 1; ++i) {
        while (hasNeighbour[currentIndex]) {
            ++currentIndex;
        }
        int neighbour = getRandomNeighbour(currentIndex, hasNeighbour);
        connectIndices(currentIndex, neighbour, adjacencyList, hasNeighbour);
    }

    int firstNeighbourless = findFirstNeighbourlessIndexAfter(0, hasNeighbour);
    int secondNeighbourless = findFirstNeighbourlessIndexAfter(firstNeighbourless + 1, hasNeighbour);
    if (firstNeighbourless + 1 != secondNeighbourless) {
        connectIndices(firstNeighbourless, secondNeighbourless, adjacencyList, hasNeighbour);
    }
    else {
        connectIndices(firstNeighbourless - 1, secondNeighbourless, adjacencyList, hasNeighbour);
        connectIndices(firstNeighbourless, (secondNeighbourless + 1) % mapSize, adjacencyList, hasNeighbour);
    }
}

AdjacencyList shuffleAdjacencyList(const AdjacencyList& adjacencyList) {
    size_t mapSize = adjacencyList.size();
    auto randomPermutation = getRandomPermutation(mapSize);
    std::vector<std::array<int, 3>> shuffledAdjacencyList(mapSize);
    for (size_t i = 0; i < mapSize; ++i) {
        shuffledAdjacencyList[randomPermutation[i]] = adjacencyList[i];
    }
    for (auto& list : shuffledAdjacencyList) {
        replaceNumbers(list.begin(), list.end(), randomPermutation);
        std::sort(list.begin(), list.end());
    }
    return shuffledAdjacencyList;
}

// generates, in effect, a connected 3-regular graph
// first vertices 0 and 1, 1 and 2, etc. are connected to ensure connectedness
// then each vertex gets a random additional neighbour
// finally the indices are shuffled
AdjacencyList generateAdjacencyList(int size) {
    AdjacencyList adjacencyList(size);
	makeCircuit(adjacencyList);
	getThirdNeighbours(adjacencyList);
    return shuffleAdjacencyList(adjacencyList);
}

void randomizeRooms(std::vector<Room>& rooms) {
    for (Room& room : rooms) {
        room.hasBats = getRandomBool(constant::BAT_PROBABILITY);
        room.hasPit = getRandomBool(constant::PIT_PROBABILITY);
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
