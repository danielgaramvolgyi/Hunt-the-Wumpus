#include "Game.h"

void Game::playTurn() {
    ioHandler.turnPrompt(gameState);
    ActionStatus turnResult = ioHandler.getAction()->execute(gameState);
    ioHandler.printResultOfAction(turnResult);
}

void Game::run() {
    ioHandler.startOfGamePrompt();
    while (!gameState.gameOver) {
        playTurn();
    }
    ioHandler.endOfGamePrompt();
}