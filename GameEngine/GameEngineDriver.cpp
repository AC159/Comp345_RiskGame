#include "GameEngine.h"

// demonstrates part 2: tournament mode
void GameEngine::tournamentModeDriver() {
    auto game = new GameEngine();
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

    cout << "Command entry methods:\n1. Read from file\n2. Enter in console\nEnter option number: ";
    string str;
    getline(cin, str);
    cout << endl;

    //ensuring that user input is either 1 or 2
    while (str != "1" && str != "2") {
        cout << "Invalid selection! Please enter a valid option number: ";
        getline(cin, str);
        cout << endl;
    }

    if (str == "1") { //if input is 1, execute logic to read tournament commands from file
        cout << "Available files: " << endl;
        cout << "   commands" << endl;
        cout << "Enter file name: ";
        getline(cin, str); //reading command file path and storing it in the str variable
        str = "../" + str + ".txt";

        auto fcp = new FileCommandProcessorAdapter(str);

        //prompt user to re-enter a file name until an existing one is given
        while (!fcp->isFLRFileOpen()) {
            cout << "Try again. Enter file name: ";
            getline(cin, str);
            str = "../" + str + ".txt";
            delete fcp;
            fcp = new FileCommandProcessorAdapter(str);
        }

        while (fcp->moreCommands()) { //if not at the end of the file, we read line by line and execute each tournament command sequentially
            auto command = fcp->getCommand();
            bool validateTournament = game->processor->validate(command.command, *game);
            if (validateTournament) {
                command.saveEffect("Tournament command valid.");
                game->tournamentMode(command);
            } else {
                cout << "Tournament command invalid." << endl;
                command.saveEffect("Invalid command.");
            }

            delete game;
            game = new GameEngine();
        }
        delete fcp;
    } else if (str == "2") { //if input is 2, execute logic for reading command from console
        cout << "Tournament command: tournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>" << endl;
        cout << "List of available valid map files(at least 1, not more than 5): " << endl;
        cout << "   canada" << endl;
        cout << "   solar" << endl;
        cout << "   bigeurope" << endl;
        cout << "List of player strategies(at least 2, not more than 4): " << endl;
        cout << "   Aggressive" << endl;
        cout << "   Benevolent" << endl;
        cout << "   Neutral" << endl;
        cout << "   Cheater" << endl;
        cout << "Valid number of games: " << endl;
        cout << "   1 to 5" << endl;
        cout << "Valid max number of turns: " << endl;
        cout << "   10 to 50" << endl;
        cout << "*Map files and player strategies are separated by spaces" << endl;
        cout << "*(e.g tournament -M canada solar -P Aggressive Cheater -G 2 -D 15)." << endl;

        Command command = game->processor->getCommand();

        //prompt user to re-enter a tournament command until a valid one is given
        while (!game->processor->validate(command.command, *game)) {
            command.saveEffect("Tournament command invalid.");
            cout << "Try again. ";
            command = game->processor->getCommand();
        }

        command.saveEffect("Tournament command valid.");
        game->tournamentMode(command);
    }
    delete game;
}