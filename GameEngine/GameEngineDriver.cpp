#include "GameEngine.h"

// demonstrates part 2: the startup phase of the game
void GameEngine::gameStartupDriver() {
    auto *gameEngine = new GameEngine();
    gameEngine->startupPhase();
    delete gameEngine;
}

// demonstrates part 3 (reinforcement, issuing orders, order execution and win phases) in addition to part 2
void GameEngine::gamePlayDriver() {
    string userInput;
    do {
        GameEngine game;
        game.startupPhase();
        game.mainGameLoop();
        game.winStateChange();
        cout << "Use the 'replay' command to play again or the 'quit' command to exit" << endl;
        Command command = game.processor->getCommand();
        userInput = command.command;
        while (!game.processor->validate(userInput, game)) {
            cout << "Invalid command! Please try again." << endl;
            command = game.processor->getCommand();
            userInput = command.command;
        }
    } while (userInput == "replay");
    cout << endl;
}

// demonstrates part 2: tournament mode
void GameEngine::tournamentModeDriver() {
    GameEngine *game = new GameEngine();
    LogObserver *logObserver = new LogObserver();
    Subject::attach(logObserver);
    string welcomeBanner =
            "WWWWWWWW                           WWWWWWWW                                                                                                          \n"
            "W::::::W                           W::::::W                                                                                                          \n"
            "W::::::W                           W::::::W                                                                                                          \n"
            "W::::::W                           W::::::W                                                                                                          \n"
            " W:::::W           WWWWW           W:::::Waaaaaaaaaaaaa  rrrrr   rrrrrrrrr   zzzzzzzzzzzzzzzzz   ooooooooooo   nnnn  nnnnnnnn        eeeeeeeeeeee    \n"
            "  W:::::W         W:::::W         W:::::W a::::::::::::a r::::rrr:::::::::r  z:::::::::::::::z oo:::::::::::oo n:::nn::::::::nn    ee::::::::::::ee  \n"
            "   W:::::W       W:::::::W       W:::::W  aaaaaaaaa:::::ar:::::::::::::::::r z::::::::::::::z o:::::::::::::::on::::::::::::::nn  e::::::eeeee:::::ee\n"
            "    W:::::W     W:::::::::W     W:::::W            a::::arr::::::rrrrr::::::rzzzzzzzz::::::z  o:::::ooooo:::::onn:::::::::::::::ne::::::e     e:::::e\n"
            "     W:::::W   W:::::W:::::W   W:::::W      aaaaaaa:::::a r:::::r     r:::::r      z::::::z   o::::o     o::::o  n:::::nnnn:::::ne:::::::eeeee::::::e\n"
            "      W:::::W W:::::W W:::::W W:::::W     aa::::::::::::a r:::::r     rrrrrrr     z::::::z    o::::o     o::::o  n::::n    n::::ne:::::::::::::::::e \n"
            "       W:::::W:::::W   W:::::W:::::W     a::::aaaa::::::a r:::::r                z::::::z     o::::o     o::::o  n::::n    n::::ne::::::eeeeeeeeeee  \n"
            "        W:::::::::W     W:::::::::W     a::::a    a:::::a r:::::r               z::::::z      o::::o     o::::o  n::::n    n::::ne:::::::e           \n"
            "         W:::::::W       W:::::::W      a::::a    a:::::a r:::::r              z::::::zzzzzzzzo:::::ooooo:::::o  n::::n    n::::ne::::::::e          \n"
            "          W:::::W         W:::::W       a:::::aaaa::::::a r:::::r             z::::::::::::::zo:::::::::::::::o  n::::n    n::::n e::::::::eeeeeeee  \n"
            "           W:::W           W:::W         a::::::::::aa:::ar:::::r            z:::::::::::::::z oo:::::::::::oo   n::::n    n::::n  ee:::::::::::::e  \n"
            "            WWW             WWW           aaaaaaaaaa  aaaarrrrrrr            zzzzzzzzzzzzzzzzz   ooooooooooo     nnnnnn    nnnnnn    eeeeeeeeeeeeee  ";

    cout << welcomeBanner << endl << endl;

    CommandProcessor *cp = new CommandProcessor();

    cout << "Command entry methods:\n1. Read from file\n2. Enter in console\nEnter option number: ";
    string str = "";
    getline(cin, str);
    cout << endl;

    while (str != "1" && str != "2") {
        cout << "Invalid selection! Please enter a valid option number: ";
        getline(cin, str);
        cout << endl;
    }

    if (str == "1") {
        cout << "Enter command file path: ";
        getline(cin, str);
        FileLineReader *flr = new FileLineReader(str);
        flr->openFile();
        if (flr->isFileOpen())
            while (!flr->checkEOF()) {
                string fileLineCommand = flr->readLineFromFile();
                bool validateTournament = cp->validate(fileLineCommand, *game);
                Command *command = new Command(fileLineCommand);

                if (validateTournament) {
                    command->saveEffect("Tournament command valid.");
                    game->tournamentMode(*command);
                } else {
                    cout << "Tournament command invalid." << endl;
                    command->saveEffect("Invalid command.");
                }

                delete command;
                delete game;
                game = new GameEngine();
            }

    } else if (str == "2") {
        cout
                << "Enter tournament command (tournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>: "
                << endl;
//        CommandProcessor *cp = new CommandProcessor();
        Command *command = &cp->getCommand();
        cout << command->command << endl;

        string temp = command->command;

        bool validateTournament = cp->validate(temp, *game);

        if (validateTournament) {
            command->saveEffect("Tournament command valid.");
            game->tournamentMode(*command);
        } else {
            cout << "Tournament command invalid." << endl;
            command->saveEffect("Invalid command.");
        }
    }
}