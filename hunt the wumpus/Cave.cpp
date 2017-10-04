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

// each element has the given probability of being true
std::vector<bool> generateRandomBools(int size, double probability) {
	std::vector<bool> randomBools(size);
	for (auto& p : randomBools) {
		if (getRandomNumber(0, 99) < 100 * probability) p = true;
	}
	return randomBools;
}

// size has to be even (a 3-regular graph must have an even number of vertices)
const Cave generateCave(int size)
{
	auto pits = generateRandomBools(size, constant::PIT_PROBABILITY);
	auto bats = generateRandomBools(size, constant::BAT_PROBABILITY);
	pits[constant::STARTING_POSITION] = false;
	bats[constant::STARTING_POSITION] = false;

	return Cave{ size, generateAdjacencyList(size),pits,bats };
}
