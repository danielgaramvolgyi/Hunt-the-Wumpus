#include "Game.h"

void Game::playTurn() {
    ioHandler.turnPrompt(gameState);
    if (turnCount == 0) {
        ioHandler.helpPrompt();
    }
    ActionStatus turnResult = ioHandler.getAction()->execute(gameState);
    ioHandler.printResultOfAction(turnResult);
    ++turnCount;
}

void Game::run() {
    ioHandler.startOfGamePrompt();
    while (!gameState.gameOver) {
        playTurn();
    }
    ioHandler.endOfGamePrompt();
}