#include "Rosie.h"

int main()
{
    //engineSpeedBenchmark();
    //for (int i = 1; i < 5; i++) {
    //    positionBenchmark(i);
    //}
    //moveGenSpeedTest();
    //engineMatch();
    //Rosy bot{};
    //bot.play();
    rosy bot{};
    Position pos{};
    //pos.play(true);

    pos.print();
    while (pos.LegalGenerator()->size() != 0) {
        std::vector <Move> moves = *pos.LegalGenerator();
        //for (int i = 0; i < moves.size(); i++) {
        //    cout << moves[i].uci() << "\n";
        //}
        vector <string> legals;
        for (int i = 0; i < moves.size(); i++) {
            legals.push_back(moves[i].uci());
        }
        bool escaped=false;
        string moveString="";
        while (std::find(legals.begin(), legals.end(), moveString) == legals.end()) {
            cout << "Enter Move:";
            std::cin >> moveString;
            if (moveString == "quit") { break; }
            if (moveString == "resign") { break; }
            if (moveString == "undo") { break; }
            if (moveString == "switch") { break; }
        }
        if (moveString == "undo") {
            pos.unmakeMove();
            pos.turn = !pos.turn;
        }
        else if (moveString == "switch") {
            if ((*pos.LegalGenerator()).size() != 0) {
                std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                pos.makeMove(bot.bestMove(pos));
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                double time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
                time /= 1000;
                std::cout << "Time difference = " << time << "[seconds]\n";
                pos.print();
            }
        }
        else {
            Move move{};
            move.fromUci(moveString);
            if (find(moves.begin(), moves.end(), move) != moves.end()) {
                pos.makeMove(move);
                pos.print();
                if ((*pos.LegalGenerator()).size() != 0) {
                    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                    pos.makeMove(bot.bestMove(pos));
                    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                    double time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
                    time /= 1000;
                    cout << "Time difference = " << time << "[seconds]\n";
                    pos.print();
                }
            }
        }
    }
    
    
    return 0;
}