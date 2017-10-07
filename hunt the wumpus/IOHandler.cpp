#include "IOHandler.h"
#include "constant.h"
#include "utils.h"


#include <regex>

void IOHandler::startOfGamePrompt() const {
	const std::string startPrompt = "You step into the cave. The damp walls muffle the sound of your steps.\n"
	"A faint, sour odor lingers in the air. This, finally, must be the home of the mighty Wumpus.\n"
	"You know that only one of you can leave this cave alive. Whether it will be you or the beast, however, "
	 "is not yet settled...\n\n";
    std::cout << startPrompt;
}

void wumpusWarning(const GameState& gameState) {
	if (isAdjacent(gameState.wumpusPosition, gameState.player.position, gameState.cave)) {
		std::cout << "You smell the Wumpus.\n";
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

void IOHandler::turnPrompt(const GameState& gameState) const {
	std::cout << "You are in room " << gameState.player.position << " of the cave. There are tunnels leading to rooms ";

	const auto& adjacentRooms = gameState.cave.adjacencyList[gameState.player.position];
	printNumbers(adjacentRooms.begin(),adjacentRooms.end());
	
	std::cout << ". You have " << gameState.player.arrows << " arrows.\n";

	wumpusWarning(gameState);
	pitWarning(gameState);
	batWarning(gameState);
	std::cout << "\n> ";
}

const std::regex& getInputPattern() {
    // e.g. h or q
    static const std::string helpQuitRegex = std::string("([") + constant::HELP_KEY + constant::QUIT_KEY + "])";
    // e.g. m13
    static const std::string moveRegex = std::string("(") + constant::MOVE_KEY + ")(\\d+)";
    // e.g. s1-3-5
    static const std::string shootRegex = std::string("(") + constant::SHOOT_KEY + ")((?:\\d+-)*\\d+)";
    static const std::string inputRegex = helpQuitRegex + '|' + moveRegex + '|' + shootRegex;
    static const std::regex inputPattern{ inputRegex };
    return inputPattern;
}

// uses regex to parse input
std::unique_ptr<Action> IOHandler::getAction() const {
    std::unique_ptr<Action> result;
    const std::regex& inputPattern = getInputPattern();
    while (true) {
        std::string input = "";
        std::cin >> input;
        std::smatch matches;
        if (std::regex_match(input, matches, inputPattern)) {
            if (matches[1] == constant::HELP_KEY) {
                result = std::make_unique<HelpAction>();
            }
            else if (matches[1] == constant::QUIT_KEY) {
                result = std::make_unique<QuitAction>();
            }
            else if (matches[2] == constant::MOVE_KEY) {
                result = std::make_unique<MoveAction>(std::stoi(matches[3]));
            }
            else {
                result = std::make_unique<ShootAction>(parseNumberSequence(matches[5]));
            }
            break;
        }
        else {
            std::cout << "Invalid input! Please try again.\n> ";
            cleanUpCin();
        }
    }
    cleanUpCin();
    return result;
}

void IOHandler::printResultOfAction(const ActionStatus& status) const {
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

void IOHandler::endOfGamePrompt() const {
	std::cout << "GAME OVER.\nThank you for playing!\n";
	std::cin.get();
}
