#include "Action.h"

bool validMove(int targetIndex, const GameState& gs) {
	const auto& adjacentRooms = gs.cave.adjacencyList[gs.player.position];
	return std::find(adjacentRooms.begin(), adjacentRooms.end(), targetIndex) != adjacentRooms.end();
}

ActionStatus handleBatEncounter(GameState& gameState) {
	int newIndex = 0;	// TODO: generate random number!
	while (gameState.cave.bats[newIndex]) newIndex = 0; // generate new number
	if (newIndex == gameState.wumpusPosition) {
		gameState.gameOver = true;
		return ActionStatus::MOVED_INTO_BATS_TO_WUMPUS;
	}
	else if (gameState.cave.pits[newIndex]) {
		gameState.gameOver = true;
		return ActionStatus::MOVED_INTO_BATS_TO_PITS;
	}
	else {
		gameState.player.position = newIndex;
		return ActionStatus::MOVED_INTO_BATS;
	}
}

ActionStatus MoveAction::execute(GameState& gameState)
{
	if (!validMove(targetIndex_, gameState)) return ActionStatus::INVALID_MOVE;
	else if (targetIndex_ == gameState.wumpusPosition) {
		gameState.gameOver = true;
		return ActionStatus::MOVED_INTO_WUMPUS;
	}
	else if (gameState.cave.pits[targetIndex_]) {
		gameState.gameOver = true;
		return ActionStatus::MOVED_INTO_PITS;
	}
	else if (gameState.cave.bats[targetIndex_]) {
		return handleBatEncounter(gameState);
	}
	else {
		gameState.player.position = targetIndex_;
		return ActionStatus::VALID_MOVE;
	}
}

// randomly moves the Wumpus into an adjacent room
void moveWumpus(GameState& gameState);

// targets[i] is valid if it's the index of an adjacent room to targets[i-1]
// (for i==0 adjacent to the player) and if targets[i] is not the player's position
// if an index is invalid then so are the ones following it
std::vector<int> getValidTargets(std::vector<int> targets, const GameState& gameState);

// TODO: if no arrows and valid, exception
ActionStatus ShootAction::execute(GameState& gameState)
{
	const auto& validTargets = getValidTargets(targets_, gameState);
	if (validTargets.empty()) return ActionStatus::INVALID_SHOT;

	// Wumpus shot
	else if (std::find(validTargets.begin(), validTargets.end(), gameState.wumpusPosition)
		!= validTargets.end()) {
		gameState.gameOver = true;
		return ActionStatus::SHOT_WUMPUS;
	}
	else {
		moveWumpus(gameState);
		if (gameState.wumpusPosition = gameState.player.position) {
			gameState.gameOver = true;
			return ActionStatus::WUMPUS_MOVED_INTO_ROOM;
		}

		--gameState.player.arrows;
		if (gameState.player.arrows == 0) {
			gameState.gameOver = true;
			return ActionStatus::NO_ARROWS_LEFT;
		}

		else return ActionStatus::VALID_SHOT;

	}
}

ActionStatus QuitAction::execute(GameState& gameState)
{
	gameState.gameOver = true;
	return ActionStatus::GAME_QUIT;
}
