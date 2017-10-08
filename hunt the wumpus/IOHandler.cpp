#include "IOHandler.h"
#include "constant.h"
#include "utils.h"

#include <regex>
#include <sstream>

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
        std::cout << "You hear wings flapping.\n";
    }
}

void IOHandler::turnPrompt(const GameState& gameState) const {
    std::cout << "You are in room " << gameState.player.position << " of the cave. There are tunnels leading to rooms ";

    const auto& adjacentRooms = gameState.cave.adjacencyList[gameState.player.position];
    printNumbers(adjacentRooms.begin(), adjacentRooms.end());

    std::cout << ". You have " << gameState.player.arrows << " arrows.\n";

    wumpusWarning(gameState);
    pitWarning(gameState);
    batWarning(gameState);
}

void IOHandler::helpPrompt() const {
    std::cout << "(Enter '" << constant::HELP_KEY << "' if you need help!)\n";
}

namespace inputRegex {
    // e.g. h or q
    const std::string helpQuitRegex = std::string("([") + constant::HELP_KEY + constant::QUIT_KEY + "])";
    // e.g. m13
    const std::string moveRegex = std::string("(") + constant::MOVE_KEY + ")(\\d+)";
    // e.g. s1-3-5
    const std::string shootRegex = std::string("(") + constant::SHOOT_KEY + ")((?:\\d+-)*\\d+)";
    const std::string inputRegex = helpQuitRegex + '|' + moveRegex + '|' + shootRegex;
    const std::regex pattern{ inputRegex };
}

// uses regex to parse input
std::unique_ptr<Action> IOHandler::getAction() const {
    std::cout << "\n> ";
    std::unique_ptr<Action> result;
    while (true) {
        std::string input = "";
        std::cin >> input;
        std::smatch matches;
        if (std::regex_match(input, matches, inputRegex::pattern)) {
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

std::string getHelpPrompt() {
    std::ostringstream helpPrompt{};
    helpPrompt << "Welcome to Hunt the Wumpus!\nThe Wumpus is a mighty beast, living deep in a cave of dark rooms and\ntwisting tunnels."
        << " Your task is to find and kill it using your magical arrows.\n\n"
        << "Cave: the cave consists of interconnected rooms, with each room having\nthree neighbours.\n\n"
        << "Hazards: the Wumpus is not the only hazard in the cave. Some rooms give home to\nhuge bats,"
        << " while others may contain a bottomless pit. The bats pick you up upon\nentering and take you to a random room of the cave."
        << " The bottomless pit is\nsomething of a misnomer - the pit does have a bottom, but it's so far below\nthat falling into it means certain death."
        << " Luckily, your senses are sharp:\nwhen some of the neighbouring rooms have hazards,"
        << " you will be given an\nappropriate warning.\n\n"
        << "Moving: you can move into a neighbouring room by entering '" << constant::MOVE_KEY << "' and the appropriate room number,"
        << " for example: \"" << constant::MOVE_KEY << "13\".\n\n"
        << "Shooting: your arrows are magical: they can find their way through the\ntwisting tunnels. You can specify up to " << constant::MAXIMUM_ARROW_REACH        << " connected rooms, starting with a\nneighbouring one, and your arrows will fly through all of them"
        << " (provided that\nthey are indeed connected to each other).\nYou can do this by entering '"        << constant::SHOOT_KEY << "' and the room numbers connected by dashes,\nfor example: "        << "\"" << constant::SHOOT_KEY << "13-2-5\".\nNote that you lose if you run out of arrows before killing the Wumpus!\n\n"
        << "Quitting: enter '" << constant::QUIT_KEY << "' to quit the game at any time.";
    return helpPrompt.str();
}

void IOHandler::printResultOfAction(const ActionStatus& status) const {
    std::string output = "";
    switch (status) {
    case ActionStatus::INVALID_MOVE:
        output = "You search around thoroughly but can't find a tunnel into that room.";
        break;
    case ActionStatus::MOVED_INTO_WUMPUS:
        output = "You know you've made a mistake the moment you enter the room, but it's\nalready too late.\n"
            " The Wumpus lunges towards you, its terrible fangs ready to tear you apart!\n"
            "You will be remembered as one of the many heroes who tried to hunt the beast\nand failed.";
        break;
    case ActionStatus::MOVED_INTO_PITS:
        output = "You make a careless step and suddenly the ground disappears from under you!\n"
            "You have just enough time to realize your fate before you crash into the bottom\nof the pit.";
        break;
    case ActionStatus::MOVED_INTO_BATS_TO_WUMPUS:
        output = "As you enter the next room, you suddenly feel lifted into the air!\n"
            "You hear the sound of huge wings flapping around you and you find yourself\nflying through these endless caves.\n"
            "Then, just as quickly as they picked you up, the bats drop you -- straight in\nfront of the bloodthirsty Wumpus!"
            " You stand no chance against it from up close.\nYou will be remembered as one of the many heroes who tried to hunt the beast\nand failed.";
        break;
    case ActionStatus::MOVED_INTO_BATS_TO_PITS:
        output = "As you enter the next room, you suddenly feel lifted into the air!\n"
            "You hear the sound of huge wings flapping around you and you find yourself\nflying through these endless caves.\n"
            "After a few moments you feel the claws on your shoulders let go and you keep\nfalling and falling. . .\n"
            "You have just enough time to realize your fate before you crash into the bottom\nof the pit.";
        break;
    case ActionStatus::MOVED_INTO_BATS:
        output = "As you enter the next room, you suddenly feel lifted into the air!\n"
            "You hear the sound of huge wings flapping around you and you find yourself\nflying through these endless caves.\n"
            "Then, just as quickly as they picked you up, the bats release you and by\nthe time you get your bearings they're already gone.\n"
            "You're a bit disoriented but, thankfully, safe.";
        break;
    case ActionStatus::VALID_MOVE:
        output = "You hold your breath as you enter the tunnel, wondering if these will be your\nlast steps."
            " Finally, you reach the room on the other side only to realize that\n. . . it's empty.";
        break;
    case ActionStatus::INVALID_SHOT:
        output = "You feel a slight disturbance: your magical arrows cannot find their way\nto that room from here."
            " You decide not to waste them and put them away for now.";
        break;
    case ActionStatus::SHOT_WUMPUS:
        output = "It is done. Your arrow burrows itself straight into the heart of the beast.\nAs you stand before its huge, lifeless body,"
            " you contemplate for a moment\nthe weight of your deed.\nYou have succeeded where so many others have failed: the Wumpus is no more.\n"
            "Wherever your travels may take you, you will always be known as the hero\nwho freed the lands from this menace.";
        break;
    case ActionStatus::WUMPUS_MOVED_INTO_ROOM:
        output = "As the arrow flies down the empty tunnels you have a terrible premonition."
            " You turn\naround to see the frenzied Wumpus sprinting towards you at an improbable speed!\n"
            "Before you can react it has already taken hold of you, tearing you into pieces in mere moments.\n"
            "You will be remembered as one of the many heroes who tried to hunt the beast\nand failed.";
        break;
    case ActionStatus::NO_ARROWS_LEFT:
        output = "The arrow flies down the empty tunnels without hitting anything.\nYou realize with horror that your quiver is empty!\n"
            "Without arrows you stand no chance against the Wumpus.\n"
            "You will be remembered as one of the many heroes who tried to hunt the beast\nand failed.";
        break;
    case ActionStatus::VALID_SHOT:
        output = "The arrow flies down the empty tunnels without hitting anything.\n"
            "You hear the Wumpus stir somewhere deep in the cave before the stifling silence\nreturns.";
        break;
    case ActionStatus::HELP:
        output = getHelpPrompt();
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