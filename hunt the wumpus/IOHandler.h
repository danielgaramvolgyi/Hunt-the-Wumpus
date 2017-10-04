#pragma once

#include "Action.h"
#include "GameState.h"

#include<iostream>
#include<string>
#include<memory>

class IOHandler {
public:

	void startOfGame();

	// outputs relevant information at the start of a turn
	// (e.g. current room, number of arrows, hints about hazards
	// in adjacent rooms)
	void prompt(const GameState& gameState);

	// parses user input into an Action
	std::unique_ptr<Action> getAction();

	// prints appropriate message after an action is executed
	void printResult(const ActionStatus& status);

	void endOfGame();
};