#include "Action.h"
#include "utils.h"

bool validMove(int targetIndex, const GameState& gs) {
	const auto& adjacentRooms = gs.cave.adjacencyList[gs.player.position];
	return std::find(adjacentRooms.begin(), adjacentRooms.end(), targetIndex) != adjacentRooms.end();
}

int getIndexWithoutBats(GameState& gameState) {
    int newIndex;
    do {
        newIndex = getRandomNumber(0, gameState.cave.mapSize - 1);
    } while (gameState.cave.bats[newIndex]);
    return newIndex;
}

ActionStatus handleBatEncounter(GameState& gameState) {
    int newIndex = getIndexWithoutBats(gameState);
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

// moves the Wumpus into a random adjacent room
void moveWumpus(GameState& gameState) {
	const auto& adjacentRooms = gameState.cave.adjacencyList[gameState.wumpusPosition];
	gameState.wumpusPosition = adjacentRooms[getRandomNumber(0, adjacentRooms.size() - 1)];
}

// targets[i] is valid if it's the index of an adjacent room to targets[i-1]
// (for i==0 adjacent to the player) and if targets[i] is not the player's position
// if an index is invalid then so are the ones following it
std::vector<int> getValidTargets(std::vector<int> targets, const GameState& gameState) {
	std::vector<int> validTargets;
	if (targets.empty() ||
		!isAdjacent(gameState.player.position, targets[0], gameState.cave)) return validTargets;

	validTargets.push_back(targets[0]);
	
	for (std::size_t i = 1; i < targets.size(); ++i) {
		if (targets.size() == i ||
			!isAdjacent(targets[i-1], targets[i], gameState.cave) ||
			targets[i] == gameState.player.position) break;
		validTargets.push_back(targets[i]);
	}

	return validTargets;
}

// TODO: if the shot is valid but there , throw exception
ActionStatus ShootAction::execute(GameState& gameState)
{
	const auto& validTargets = getValidTargets(targets_, gameState);
	
	// no valid targets
	if (validTargets.empty()) return ActionStatus::INVALID_SHOT;

	// valid shot but no arrows -- shouldn't happen!
	if (gameState.player.arrows == 0) {
		gameState.gameOver = true;
		return ActionStatus::NO_ARROWS_LEFT;
	}

	// wumpus shot
	else if (std::find(validTargets.begin(), validTargets.end(), gameState.wumpusPosition)
		!= validTargets.end()) {
		gameState.gameOver = true;
		return ActionStatus::SHOT_WUMPUS;
	}
	else {
		moveWumpus(gameState);
		// wumpus eats player
		if (gameState.wumpusPosition == gameState.player.position) {
			gameState.gameOver = true;
			return ActionStatus::WUMPUS_MOVED_INTO_ROOM;
		}

		// player runs out of arrows
		--gameState.player.arrows;
		if (gameState.player.arrows == 0) {
			gameState.gameOver = true;
			return ActionStatus::NO_ARROWS_LEFT;
		}

		else return ActionStatus::VALID_SHOT;

	}
}

ActionStatus HelpAction::execute(GameState& gameState)
{
	return ActionStatus::HELP;
}

ActionStatus QuitAction::execute(GameState& gameState)
{
	gameState.gameOver = true;
	return ActionStatus::GAME_QUIT;
}

