// TODO: writing, input handling

#include "IOHandler.h"
#include "constants.h"
#include "utils.h"

#include <sstream> 

void IOHandler::startOfGame() {
	std::cout << "You step into the cave. The damp walls muffle the sound of your steps.\n";
	std::cout << "A faint, sour odor lingers in the air. This, finally, must be the home of the mighty Wumpus.\n";
	std::cout << "You know that only one of you can leave this cave alive. Whether it will be you or the beast, however, ";
	std::cout << "is not yet settled...\n\n";
}

void wumpusWarning(const GameState& gameState) {
	if (isAdjacent(gameState.wumpusPosition, gameState.player.position, gameState.cave)) {
		std::cout << "You smell the wumpus.\n";
	}
}

void pitWarning(const GameState& gameState) {
	const auto& adjacentRooms = gameState.cave.adjacencyList[gameState.player.position];
	if (std::any_of(adjacentRooms.begin(), adjacentRooms.end(),
		[&gameState](int i) {return gameState.cave.rooms[i].hasPit; })) {
		std::cout << "You feel a breeze.\n";
	}
}

void batWarning(const GameState& gameState) {
	const auto& adjacentRooms = gameState.cave.adjacencyList[gameState.player.position];
	if (std::any_of(adjacentRooms.begin(), adjacentRooms.end(),
		[&gameState](int i) {return gameState.cave.rooms[i].hasBats; })) {
		std::cout << "You hear a bat.\n";
	}
}

void IOHandler::prompt(const GameState& gameState) {
	std::cout << "You are in room " << gameState.player.position << " of the cave. There are tunnels leading to rooms ";

	const auto& adjacentRooms = gameState.cave.adjacencyList[gameState.player.position];
	printNumbers(adjacentRooms.begin(),adjacentRooms.end());
	
	std::cout << ". You have " << gameState.player.arrows << " arrows.\n";

	wumpusWarning(gameState);
	pitWarning(gameState);
	batWarning(gameState);
	std::cout << '\n';
}

void cleanUpCin() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

std::vector<int> getRoomIndices(std::istringstream& input) {
    std::vector<int> roomIndices;
    int roomIndex;
    input >> roomIndex;
    roomIndices.push_back(roomIndex);

    char ch;
    while (roomIndices.size() < constant::MAXIMUM_ARROW_REACH && input >> ch >> roomIndex) {
        if (ch != '-') break;
        else roomIndices.push_back(roomIndex);
    }
    return roomIndices;
}

std::unique_ptr<Action> IOHandler::getAction()
{
	std::string inp = "";
	std::unique_ptr<Action> result;
	while (true) {
		std::cin >> inp;
		char ch;
		std::istringstream input{ inp };
		input >> ch;
		if (ch == constant::QUIT_KEY) {
			result = std::make_unique<QuitAction>(QuitAction());
			break;
		}
		else if (ch == constant::HELP_KEY) {
			result = std::make_unique<HelpAction>(HelpAction());
			break;
		}
		else if (ch == constant::MOVE_KEY) {
			int roomIndex;
			input >> roomIndex;
			if (input) {
				result = std::make_unique<MoveAction>(MoveAction(roomIndex));
				break;
			}
		}
		// TODO
		else if (ch == constant::SHOOT_KEY) {
            auto roomIndices = getRoomIndices(input);
			if (!roomIndices.empty()) {
                result = std::make_unique<ShootAction>(ShootAction{roomIndices});
				break;
			}
		}
	}
    cleanUpCin();
    std::cout << '\n';
	return result;
}

void IOHandler::printResult(const ActionStatus& status) {
	std::string output = "";
	switch (status) {
	case ActionStatus::INVALID_MOVE:
		output = "You search around thoroughly but can't find a tunnel into that room.";
		break;
	case ActionStatus::MOVED_INTO_WUMPUS:
		output = "You know you've made a mistake the moment you enter the room, but it's already too late.\n"
					"The Wumpus lunges towards you, its terrible fangs ready to tear you apart.\n"
					"You will be remembered as one of the many heroes who tried to hunt the beast and failed.";
		break;
	case ActionStatus::MOVED_INTO_PITS:
		output = "You make a careless step and suddenly the ground disappears from under you.\n"
			"You have just enough time to realize your fate before you crash into the ground.";
		break;
	case ActionStatus::MOVED_INTO_BATS_TO_WUMPUS:
		output = "As you enter the next room, you suddenly feel lifted into the air.\n"
            "You hear the sound of huge wings flapping around you and you find yourself flying through these endless caves.\n"
            "";
		break;
	case ActionStatus::MOVED_INTO_BATS_TO_PITS:
		output = "As you enter the next room, you suddenly feel lifted into the air.\n"
            "You hear the sound of huge wings flapping around you and you find yourself flying through these endless caves.\n"
            "After a few moments you feel the claws on your shoulders let go ...";
		break;
	case ActionStatus::MOVED_INTO_BATS:
		output = "As you enter the next room, you suddenly feel lifted into the air.\n"
			"You hear the sound of huge wings flapping around you and you find yourself flying through these endless caves.\n"
			"Then, just as quickly as they picked you up, the bats release you and by the time you get your bearings they're already gone.\n"
			"You're a bit disoriented but, thankfully, safe.";
		break;
	case ActionStatus::VALID_MOVE:
		output = "You hold your breath as you enter the tunnel, wondering if these will be your last steps.\n"
			"Finally, you reach the room on the other side only to realize that...\nit's empty.";
		break;
	case ActionStatus::INVALID_SHOT:
		output = "You feel a slight disturbance:\neven your magical arrows cannot find their way to that room from here.\n"
			"You decide not to waste them and put them away for now.";
		break;
	case ActionStatus::SHOT_WUMPUS:
		output = "win";
		break;
	case ActionStatus::WUMPUS_MOVED_INTO_ROOM:
        output = "As the arrow flies down the empty tunnels you have a terrible premonition.\n"
            "You turn around to see the frenzied Wumpus sprinting towards you at an improbable speed.\n"
            "Before you can react it has already taken hold of you, tearing you into pieces in mere moments.\n"
            "You will be remembered as one of the many heroes who tried to hunt the beast and failed.";
		break;
	case ActionStatus::NO_ARROWS_LEFT:
		output = "noarrows";
		break;
	case ActionStatus::VALID_SHOT:
        output = "The arrow flies down the empty tunnels without hitting anything.\n"
            "You hear the Wumpus stir somewhere deep in the cave before the stifling silence returns.";
		break;
	case ActionStatus::HELP:
		output = "help";
		break;
	case ActionStatus::GAME_QUIT:
		break;
	default:
		break;
	}
	std::cout << output << '\n' << '\n';
}

void IOHandler::endOfGame() {
	std::cout << "GAME OVER.\nThank you for playing!\n";
	std::cin.get();
}
