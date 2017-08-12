#pragma once

#include "GameState.h"

#include<vector>

// ACTIONSTATUS enum class
// signals result of player event
// only for output; internal logic handled by Action class
enum class ActionStatus {
	
	// move action

	INVALID_MOVE,
	MOVED_INTO_WUMPUS, // lose condition
	MOVED_INTO_PITS, // lose condition
	// bats can carry the player into a room with other hazards:
	MOVED_INTO_BATS_TO_WUMPUS,	 // lose condition
	MOVED_INTO_BATS_TO_PITS,	 // lose condition
	MOVED_INTO_BATS,		// note: bats don't move player into another bat room
	VALID_MOVE,	// if nothing else happens

	// shoot action

	INVALID_SHOT,
	SHOT_WUMPUS,	// win condition
	WUMPUS_MOVED_INTO_ROOM, // lose condition
	NO_ARROWS_LEFT, // lose condition
 	VALID_SHOT, // if nothing else happens

	// quit action

	GAME_QUIT // lose condition
};


// ACTION class
// interface for player actions
// note: actions are guaranteed to leave the game in a valid state
// (if it was in a valid state before the action)
class Action abstract {
public:
	virtual ActionStatus execute(GameState& gameState) =0;
};


// MOVEACTION class
// handles moving player
// only executes valid movements!
class MoveAction : public Action {
public:
	explicit MoveAction(int targetIndex) :targetIndex_{targetIndex} {}

	// changes player position, handles encounters with wumpus/pit/bats
	ActionStatus execute(GameState& gameState) override;
private:
	int targetIndex_;
};


// SHOOTACTION class
// handles shooting arrows
// shoots if the first target is valid 
class ShootAction : public Action {
public:
	ShootAction(std::vector<int> targets) :targets_{targets} {}

	// decreases arrows, checks if wumpus stays alive, moves it if so and checks if it's game over
	ActionStatus execute(GameState& gameState) override;
private:
	std::vector<int> targets_;
};


// QUITACTION class
// ends the game
class QuitAction : public Action {
public: 
	// sets gameOver flag to true
	ActionStatus execute(GameState& gameState) override;
};