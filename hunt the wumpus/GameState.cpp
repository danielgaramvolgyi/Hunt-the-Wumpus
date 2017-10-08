#include "GameState.h"
#include "utils.h"
#include "constant.h"

#include <iostream>

// Wumpus position is random but not equal to the player position
int getWumpusPosition(int caveSize, int playerPosition) {
    int random = getRandomNumber(1, caveSize - 1);
    return (random + playerPosition) % caveSize;
}

// caveSize has to be even (see generateCave)
GameState::GameState(int caveSize)
    :cave{ generateCave(caveSize) },
    player{ constant::STARTING_POSITION,constant::STARTING_ARROWS },
    gameOver{ false },
    wumpusPosition{ getWumpusPosition(caveSize, constant::STARTING_POSITION) } {}

void printDebugInfo(const GameState& gameState) {
    std::cout << "Map size: " << gameState.cave.mapSize << '\n';
    for (int i = 0; i < gameState.cave.mapSize; ++i) {
        const Room& currentRoom = gameState.cave.rooms[i];
        const auto& currentNeighbours = gameState.cave.adjacencyList[i];

        std::cout << "Room " << i << ": ";
        if (i == gameState.player.position) {
            std::cout << "player ";
        }
        if (currentRoom.hasBats) {
            std::cout << "bats ";
        }
        if (currentRoom.hasPit) {
            std::cout << "pit ";
        }
        if (i == gameState.wumpusPosition) {
            std::cout << "Wumpus ";
        }
        std::cout << "neighbours: ";
        printNumbers(currentNeighbours.begin(), currentNeighbours.end());
        std::cout << '\n';
    }
    std::cout << "Number of arrows: " << gameState.player.arrows << '\n';
}