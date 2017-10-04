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
		[&gameState](int i) {return gameState.cave.pits[i]; })) {
		std::cout << "You feel a breeze.\n";
	}
}

void batWarning(const GameState& gameState) {
	const auto& adjacentRooms = gameState.cave.adjacencyList[gameState.player.position];
	if (std::any_of(adjacentRooms.begin(), adjacentRooms.end(),
		[&gameState](int i) {return gameState.cave.bats[i]; })) {
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
			int roomIndex;
			input >> roomIndex;
			if (input) {
				result = std::make_unique<ShootAction>(ShootAction({ roomIndex }));
				break;
			}
		}
	}
	std::cin.clear();
	std::cin.ignore(10000, '\n');
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
		output = "batwump";
		break;
	case ActionStatus::MOVED_INTO_BATS_TO_PITS:
		output = "batpit";
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
		output = "As the arrow flies down the empty tunnels you have a terrible premonition. ...";
		break;
	case ActionStatus::NO_ARROWS_LEFT:
		output = "noarrows";
		break;
	case ActionStatus::VALID_SHOT:
		output = "noluck";
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
