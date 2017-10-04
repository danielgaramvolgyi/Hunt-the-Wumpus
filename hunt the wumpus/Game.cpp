#include "Game.h"

void Game::playTurn() {
	ioHandler.prompt(gameState);
	ActionStatus turnResult = ioHandler.getAction()->execute(gameState);
	ioHandler.printResult(turnResult);
}

void Game::run() {
	ioHandler.startOfGame();
	while (!gameState.gameOver) {
		playTurn();
	}
	ioHandler.endOfGame();
}