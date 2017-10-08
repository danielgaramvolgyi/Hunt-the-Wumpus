#include "Cave.h"
#include "utils.h"
#include "constant.h"

#include <algorithm>
#include <random>

using AdjacencyList = std::vector<std::array<int, 3>>;

// forward declare every helper function to improve layout
std::vector<Room> generateRooms(int size);
void randomizeRoomHazards(std::vector<Room>& rooms);
void clearStartingPositionFromHazards(std::vector<Room>& rooms);
AdjacencyList generateAdjacencyList(int size);
void makeCircuit(AdjacencyList& adjacencyList);
void getThirdNeighbours(AdjacencyList& adjacencyList);
void drawAllEdgesExceptLast(AdjacencyList& adjacencyList, std::vector<bool>& hasThirdNeighbour);
void drawLastEdge(AdjacencyList& adjacencyList, std::vector<bool>& hasThirdNeighbour);
int getRandomNeighbour(int i, const std::vector<bool>& hasThirdNeighbour);
int numberOfPossibleNeighbours(int i, const std::vector<bool>& hasThirdNeighbour);
int getNthPossibleNeighbour(int i, int n, const std::vector<bool>& hasThirdNeighbour);
int findFirstIndexWithoutThirdNeighbourAfter(int i, const std::vector<bool>& hasThirdNeighbour);
void connectIndices(int i, int j, AdjacencyList& adjacencyList, std::vector<bool>& hasThirdNeighbour);
AdjacencyList permuteAdjacencyList(const AdjacencyList& adjacencyList);

bool isAdjacent(int firstIndex, int secondIndex, const Cave& cave) {
    const auto& firstAdjacent = cave.adjacencyList[firstIndex];
    return find(firstAdjacent.begin(), firstAdjacent.end(), secondIndex) != firstAdjacent.end();
}

// size has to be even (a 3-regular graph must have an even number of vertices)
const Cave generateCave(int size) {
    return Cave{ size, generateRooms(size), generateAdjacencyList(size) };
}

std::vector<Room> generateRooms(int size) {
    std::vector<Room> rooms(size);
    randomizeRoomHazards(rooms);
    clearStartingPositionFromHazards(rooms);
    return rooms;
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

// generates, in effect, the adjacency list of a connected 3-regular graph
AdjacencyList generateAdjacencyList(int size) {
    AdjacencyList adjacencyList(size);
    makeCircuit(adjacencyList); // first vertices 0 and 1, 1 and 2, etc. are connected to make a circuit
    getThirdNeighbours(adjacencyList); // then each vertex gets a random third neighbour
    return permuteAdjacencyList(adjacencyList); // finally the indices are permuted
}

void makeCircuit(AdjacencyList& adjacencyList) {
    for (std::size_t i = 0; i < adjacencyList.size(); ++i) {
        int next = (i + 1) % adjacencyList.size();
        adjacencyList[i][1] = next;
        adjacencyList[next][0] = i;
    }
}

void getThirdNeighbours(AdjacencyList& adjacencyList) {
    int mapSize = adjacencyList.size();
    std::vector<bool> hasThirdNeighbour(mapSize);

    drawAllEdgesExceptLast(adjacencyList, hasThirdNeighbour);
    // it's possible that the two remaining vertices are adjacent
    // we have to take care not to connect them again in that case
    drawLastEdge(adjacencyList, hasThirdNeighbour);
}

void drawAllEdgesExceptLast(AdjacencyList& adjacencyList, std::vector<bool>& hasThirdNeighbour) {
    int mapSize = hasThirdNeighbour.size();
    int currentIndex = 0;
    // find the first vertex that needs an additional neighbour and connect it to a random free vertex
    // do this mapSize/2 - 1 times
    for (int i = 0; i < mapSize / 2 - 1; ++i) {
        currentIndex = findFirstIndexWithoutThirdNeighbourAfter(currentIndex, hasThirdNeighbour);
        int neighbour = getRandomNeighbour(currentIndex, hasThirdNeighbour);
        connectIndices(currentIndex, neighbour, adjacencyList, hasThirdNeighbour);
    }
}

void drawLastEdge(AdjacencyList& adjacencyList, std::vector<bool>& hasThirdNeighbour) {
    int mapSize = hasThirdNeighbour.size();
    int firstRemaining = findFirstIndexWithoutThirdNeighbourAfter(0, hasThirdNeighbour);
    int secondRemaining = findFirstIndexWithoutThirdNeighbourAfter(firstRemaining, hasThirdNeighbour);
    // if the two remaining vertices are not adjacent, then all is well
    if (firstRemaining + 1 != secondRemaining) {
        connectIndices(firstRemaining, secondRemaining, adjacencyList, hasThirdNeighbour);
    }
    // otherwise we reconnect the previous vertex and its neighbour to secondRemaining and firstRemaining respectively
    else {
        int previousNeighbour = adjacencyList[firstRemaining - 1][2];
        connectIndices(firstRemaining, previousNeighbour, adjacencyList, hasThirdNeighbour);
        connectIndices(firstRemaining - 1, secondRemaining, adjacencyList, hasThirdNeighbour);
    }
}

int getRandomNeighbour(int i, const std::vector<bool>& hasThirdNeighbour) {
    int n = getRandomNumber(1, numberOfPossibleNeighbours(i, hasThirdNeighbour));
    return getNthPossibleNeighbour(i, n, hasThirdNeighbour);
}

// j is a possible neighbour of i if j > i + 1 and j doesn't have a third neighbour already
int numberOfPossibleNeighbours(int i, const std::vector<bool>& hasThirdNeighbour) {
    int mapSize = hasThirdNeighbour.size();
    int freeNodes = 0;
    for (int j = i + 2; j < mapSize; ++j) {
        if (!hasThirdNeighbour[j]) {
            ++freeNodes;
        }
    }
    return freeNodes;
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

int findFirstIndexWithoutThirdNeighbourAfter(int i, const std::vector<bool>& hasThirdNeighbour) {
    int mapSize = hasThirdNeighbour.size();
    int result = i + 1;
    while (hasThirdNeighbour[result] && result < mapSize) {
        ++result;
    }
    return result;
}

void connectIndices(int i, int j, AdjacencyList& adjacencyList, std::vector<bool>& hasThirdNeighbour) {
    adjacencyList[i][2] = j;
    adjacencyList[j][2] = i;
    hasThirdNeighbour[i] = true;
    hasThirdNeighbour[j] = true;
}

AdjacencyList permuteAdjacencyList(const AdjacencyList& adjacencyList) {
    size_t mapSize = adjacencyList.size();
    auto randomPermutation = getRandomPermutation(mapSize);
    AdjacencyList shuffledAdjacencyList(mapSize);

    // the new label of i will be randomPermutation[i]
    for (size_t i = 0; i < mapSize; ++i) {
        shuffledAdjacencyList[randomPermutation[i]] = adjacencyList[i];
    }
    // we have to change the labels in adjacencyList[i] for each i as well
    for (auto& list : shuffledAdjacencyList) {
        replaceNumbers(list.begin(), list.end(), randomPermutation);
        std::sort(list.begin(), list.end());
    }
    return shuffledAdjacencyList;
}