#pragma once

#include "GameState.h"
#include "Action.h"
#include "IOHandler.h"

// GAME class
// handles game events
class Game {
private:
    GameState gameState;
    const IOHandler ioHandler;
    int turnCount;

public:
    explicit Game(int caveSize)
        :gameState{ caveSize },
        ioHandler{},
        turnCount{ 0 } {}

    // keeps running turns until the game ends (the player wins, loses or quits)
    void run();

private:
    // prompts user for action and then executes action
    void playTurn();
};