// TODO: IOHandler implementation, Cave generation, gameStatus debug output
// TODO LATER: add exceptions (gamestate validity)

#include "Game.h"

int main() {

    constexpr int CAVESIZE = 20;
	Game game{CAVESIZE};
	game.run();
}