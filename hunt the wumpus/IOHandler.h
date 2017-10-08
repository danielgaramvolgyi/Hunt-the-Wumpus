#pragma once

#include "Action.h"
#include "ActionStatus.h"
#include "GameState.h"

#include <iostream>
#include <string>
#include <memory>

// IOHANDLER CLASS
// handles output (at the start and end of the game and in each turn)
// and user input (in each turn)
class IOHandler {
public:
    void startOfGamePrompt() const;
    void endOfGamePrompt() const;
    void printResultOfAction(const ActionStatus& status) const;

    void IOHandler::helpPrompt() const;

    // outputs relevant information at the start of a turn
    // (e.g. current room, number of arrows, hints about hazards
    // in adjacent rooms)
    void turnPrompt(const GameState& gameState) const;

    // parses user input into an Action
    std::unique_ptr<Action> getAction() const;

};