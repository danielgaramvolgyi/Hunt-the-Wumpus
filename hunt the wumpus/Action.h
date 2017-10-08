#pragma once

#include "ActionStatus.h"
#include "GameState.h"

#include <vector>
#include <array>

// ACTION class
// interface for player actions
// actions are guaranteed to leave the game in a valid state
// (if it was in a valid state before the action)
class Action abstract {
public:
    virtual ActionStatus execute(GameState& gameState) const = 0;
};

class MoveAction : public Action {
public:
    explicit MoveAction(int targetIndex)
        :targetIndex{ targetIndex } {}

    // changes player position if the target index is valid, handles encounters with Wumpus/pit/bats
    ActionStatus execute(GameState& gameState) const override;
private:
    int targetIndex;
};

class ShootAction : public Action {
public:
    explicit ShootAction(const std::vector<int>& targets)
        :targets{ targets } {}

    // shoots if the first target is valid, decreases arrows, checks if Wumpus stays alive, moves it if so and checks if it's game over
    ActionStatus execute(GameState& gameState) const override;
private:
    std::vector<int> targets;
};

class HelpAction : public Action {
public:
    ActionStatus execute(GameState& gameState) const override;
};

class QuitAction : public Action {
public:
    ActionStatus execute(GameState& gameState) const override;
};