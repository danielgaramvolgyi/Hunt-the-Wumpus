#pragma once

// ACTIONSTATUS enum class
// signals result of player event
// used for communication between Action and IOHandler classes
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

    // help action

    HELP,

    // quit action

    GAME_QUIT // lose condition
};
