#include "Game.h"

int main() {
    constexpr int CAVESIZE = 20;
    Game game{ CAVESIZE };
    game.run();
}