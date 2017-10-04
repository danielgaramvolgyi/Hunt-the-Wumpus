// TODO: IOHandler implementation, Cave generation, gameStatus debug output
// TODO LATER: add exceptions (gamestate validity)

#include "Game.h"

constexpr int CAVESIZE = 20;

int main() {
	Game game{CAVESIZE};
	game.run();
}