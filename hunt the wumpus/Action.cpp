#include "Action.h"
#include "utils.h"
#include <assert.h>

bool validMove(int targetIndex, const GameState& gs) {
	const auto& adjacentRooms = gs.cave.adjacencyList[gs.player.position];
	return std::find(adjacentRooms.begin(), adjacentRooms.end(), targetIndex) != adjacentRooms.end();
}

int getRandomIndexWithoutBats(GameState& gameState) {
    int newIndex;
    do {
        newIndex = getRandomNumber(0, gameState.cave.mapSize - 1);
    } while (gameState.cave.rooms[newIndex].hasBats);
    return newIndex;
}

ActionStatus handleBatEncounter(GameState& gameState) {
    int newIndex = getRandomIndexWithoutBats(gameState);
    gameState.player.position = newIndex;

    if (newIndex == gameState.wumpusPosition) {
		gameState.gameOver = true;
		return ActionStatus::MOVED_INTO_BATS_TO_WUMPUS;
	}
	else if (gameState.cave.rooms[newIndex].hasPit) {
		gameState.gameOver = true;
		return ActionStatus::MOVED_INTO_BATS_TO_PITS;
	}
	else {
		return ActionStatus::MOVED_INTO_BATS;
	}
}

ActionStatus MoveAction::execute(GameState& gameState) const {
	if (!validMove(targetIndex, gameState)) return ActionStatus::INVALID_MOVE;
    
    gameState.player.position = targetIndex;
    const Room& targetRoom = gameState.cave.rooms[targetIndex];

    if (targetIndex == gameState.wumpusPosition) {
		gameState.gameOver = true;
		return ActionStatus::MOVED_INTO_WUMPUS;
	}
	else if (targetRoom.hasPit) {
		gameState.gameOver = true;
		return ActionStatus::MOVED_INTO_PITS;
	}
	else if (targetRoom.hasBats) {
		return handleBatEncounter(gameState);
	}
	else {
		return ActionStatus::VALID_MOVE;
	}
}

void moveWumpusToRandomAdjacentRoom(GameState& gameState) {
	const auto& adjacentRooms = gameState.cave.adjacencyList[gameState.wumpusPosition];
	gameState.wumpusPosition = adjacentRooms[getRandomNumber(0, adjacentRooms.size() - 1)];
}

bool noValidTargets(const std::vector<int>& targets, const GameState& gameState) {
    return targets.empty() ||
        !isAdjacent(gameState.player.position, targets[0], gameState.cave);
}

// check whether targets[i] is a valid target, assuming that the previous targets were valid
bool isValidTarget(size_t i, const std::vector<int>& targets, const GameState& gameState) {
    return i < targets.size() &&
        isAdjacent(targets[i - 1], targets[i], gameState.cave) &&
        targets[i] != gameState.player.position;
}

// targets[i] is valid if it's the index of an adjacent room to targets[i-1]
// (for i==0 adjacent to the player) and if targets[i] is not the player's position
// if an index is invalid then so are the ones following it
std::vector<int> getValidTargets(const std::vector<int>& targets, const GameState& gameState) {
    std::vector<int> validTargets{};
    if (noValidTargets(targets, gameState)) return validTargets;
    else {
        validTargets.push_back(targets[0]);

        size_t i = 1;
        while (isValidTarget(i, targets, gameState)) {
            validTargets.push_back(targets[i]);
            ++i;
        }
        return validTargets;
    }
}

bool wumpusShot(const std::vector<int>& validTargets, const GameState& gameState) {
    return std::find(validTargets.begin(), validTargets.end(), gameState.wumpusPosition)
            != validTargets.end();
}

ActionStatus ShootAction::execute(GameState& gameState) const {
	const auto& validTargets = getValidTargets(targets, gameState);
	
	// no valid targets
	if (validTargets.empty()) return ActionStatus::INVALID_SHOT;

	// if the player has no arrows it should already have been game over
    assert(gameState.player.arrows != 0);
	--gameState.player.arrows;
	
	// wumpus shot
	if (wumpusShot(validTargets,gameState)) {
		gameState.gameOver = true;
		return ActionStatus::SHOT_WUMPUS;
	}

	else {
        moveWumpusToRandomAdjacentRoom(gameState);
		// wumpus eats player
		if (gameState.wumpusPosition == gameState.player.position) {
			gameState.gameOver = true;
			return ActionStatus::WUMPUS_MOVED_INTO_ROOM;
		}

		// player ran out of arrows
		if (gameState.player.arrows == 0) {
			gameState.gameOver = true;
			return ActionStatus::NO_ARROWS_LEFT;
		}

		else return ActionStatus::VALID_SHOT;
	}
}

ActionStatus HelpAction::execute(GameState& gameState) const {
	return ActionStatus::HELP;
}

ActionStatus QuitAction::execute(GameState& gameState) const {
	gameState.gameOver = true;
	return ActionStatus::GAME_QUIT;
}