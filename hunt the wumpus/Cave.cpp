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

// j is a possible neighbour of i if j > i + 1 and j doesn't have a third neighbour already
int numberOfPossibleNeighbours(int i, const std::vector<bool>& hasThirdNeighbour) {
    int mapSize = hasThirdNeighbour.size();
    int freeNodes = 0;
    for (int j = i+2; j < mapSize; ++j) {
        if (!hasThirdNeighbour[j]) {
            ++freeNodes;
        }
    }
    return freeNodes;
}

int findFirstIndexWithoutThirdNeighbourAfter(int i, const std::vector<bool>& hasThirdNeighbour) {
    int mapSize = hasThirdNeighbour.size();
    int result = i + 1;
    while (hasThirdNeighbour[result] && result < mapSize) {
        ++result;
    }
    return result;
}

// returns the map size if there are less than n possible neighbours
int getNthPossibleNeighbour(int i, int n, const std::vector<bool>& hasThirdNeighbour) {
    int mapSize = hasThirdNeighbour.size();
    int result = i + 1;
    while (n > 0 && result < mapSize) {
        result = findFirstIndexWithoutThirdNeighbourAfter(result, hasThirdNeighbour);
        --n;
    }
    return result;
}

int getRandomNeighbour(int i, const std::vector<bool>& hasThirdNeighbour) {
    return getNthPossibleNeighbour(i, getRandomNumber(1, numberOfPossibleNeighbours(i, hasThirdNeighbour)), hasThirdNeighbour);
}

void connectIndices(int i, int j, AdjacencyList& adjacencyList, std::vector<bool>& hasThirdNeighbour) {
    adjacencyList[i][2] = j;
    adjacencyList[j][2] = i;
    hasThirdNeighbour[i] = true;
    hasThirdNeighbour[j] = true;
}

void getAllButLastNeighbours(AdjacencyList& adjacencyList, std::vector<bool>& hasThirdNeighbour) {
    int mapSize = hasThirdNeighbour.size();
    int currentIndex = 0;
    for (int i = 0; i < mapSize / 2 - 1; ++i) {
        currentIndex = findFirstIndexWithoutThirdNeighbourAfter(currentIndex, hasThirdNeighbour);
        int neighbour = getRandomNeighbour(currentIndex, hasThirdNeighbour);
        connectIndices(currentIndex, neighbour, adjacencyList, hasThirdNeighbour);
    }
}

void getLastNeighbour(AdjacencyList& adjacencyList, std::vector<bool>& hasThirdNeighbour) {
    int mapSize = hasThirdNeighbour.size();
    int first = findFirstIndexWithoutThirdNeighbourAfter(0, hasThirdNeighbour);
    int second = findFirstIndexWithoutThirdNeighbourAfter(first, hasThirdNeighbour);
    // if the two remaining vertices are not adjacent, then all is well
    if (first + 1 != second) {
        connectIndices(first, second, adjacencyList, hasThirdNeighbour);
    }
    // otherwise we reconnect the previous vertex and its neighbour to second and first respectively
    else {
        int previousNeighbour = adjacencyList[first - 1][2];
        connectIndices(first, previousNeighbour, adjacencyList, hasThirdNeighbour);
        connectIndices(first - 1, second, adjacencyList, hasThirdNeighbour);
    }
}

void getThirdNeighbours(AdjacencyList& adjacencyList) {
    int mapSize = adjacencyList.size();
    std::vector<bool> hasThirdNeighbour(mapSize);

    getAllButLastNeighbours(adjacencyList, hasThirdNeighbour);
    getLastNeighbour(adjacencyList, hasThirdNeighbour);
}

AdjacencyList shuffleAdjacencyList(const AdjacencyList& adjacencyList) {
    size_t mapSize = adjacencyList.size();
    auto randomPermutation = getRandomPermutation(mapSize);
    AdjacencyList shuffledAdjacencyList(mapSize);

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
// first vertices 0 and 1, 1 and 2, etc. are connected to make a circuit
// then each vertex gets a random additional neighbour
// finally the indices are shuffled
AdjacencyList generateAdjacencyList(int size) {
    AdjacencyList adjacencyList(size);
	makeCircuit(adjacencyList);
	getThirdNeighbours(adjacencyList);
    return shuffleAdjacencyList(adjacencyList);
}

void randomizeRoomHazards(std::vector<Room>& rooms) {
    for (Room& room : rooms) {
        room.hasBats = getRandomBool(constant::BAT_PROBABILITY);
        room.hasPit = getRandomBool(constant::PIT_PROBABILITY);
    }
}

void clearStartingPositionFromHazards(std::vector<Room>& rooms) {
    rooms[constant::STARTING_POSITION].hasBats = false;
    rooms[constant::STARTING_POSITION].hasPit = false;
}

std::vector<Room> generateRooms(int size) {
    std::vector<Room> rooms(size);
    randomizeRoomHazards(rooms);
    clearStartingPositionFromHazards(rooms);
    return rooms;
}

// size has to be even (a 3-regular graph must have an even number of vertices)
const Cave generateCave(int size) {
	return Cave{ size, generateRooms(size), generateAdjacencyList(size)};
}