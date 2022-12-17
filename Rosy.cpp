#include "Rosy.h"
U64 lookedAt = 0ULL;
U64 mates = 0ULL;

void tree(Position& pos,const int depth,const int ind, std::vector<unsigned long long>* nodes_ptr) {
    (nodes_ptr->at(ind)) += (depth == 0);
    std::vector<int> moves{};
    pos.get_legal_moves(&moves);
    mates += (depth == 0) * (moves.size() == 0);
    /*
    if (depth == 0) {
        mates += (moves.size() == 0);
        return;
    }
    */
    //print_position(pos);
    if (depth > 0) {
        for (int i = 0; i < moves.size(); i++) {
            pos.make_move(moves[i]);
            tree(pos,depth-1,ind,nodes_ptr);
            pos.unmake_move();
        }
    }
}
void perf(Position& pos,const int depth){
    print_position(pos);
    std::cout << "\tNodes from different branches:\n";
    std::vector<unsigned long long> nodes_from_branches{};
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::vector<int> moves{};
    pos.get_legal_moves(&moves);
    unsigned long long total_nodes = 0ULL;
    for (int i = 0; i < moves.size(); i++) {
        nodes_from_branches.push_back(0ULL);
        pos.make_move(moves[i]);
        tree(pos, depth - 1, i, &nodes_from_branches);
        pos.unmake_move();
        total_nodes += nodes_from_branches[i];
        std::cout << "\t\t" << uci(moves[i]) << ": " << nodes_from_branches[i] << " Nodes\n";
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    U64 totalTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "\tTotal Nodes: " << total_nodes << "\n";
    std::cout << "\tTime: " << totalTime << "  (" << 1000000000 * total_nodes / totalTime << " 1/s)\n";
}

void test() {
    Position pos;
    pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    perf(pos, 3);
    std::cout << "Positions: " << lookedAt << "\nMates: " << mates << "\n";
    lookedAt = 0;
    mates = 0;
    pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    perf(pos, 4);
    std::cout << "Positions: " << lookedAt << "\nMates: " << mates << "\n";
    lookedAt = 0;
    mates = 0;
    pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    perf(pos, 5);
    std::cout << "Positions: " << lookedAt << "\nMates: " << mates << "\n";
    lookedAt = 0;
    mates = 0;
    pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    perf(pos, 6);
    std::cout << "Positions: " << lookedAt << "\nMates: " << mates << "\n";
    /*
    */
}

void perft() {
    Position pos;
    pos = Position{ "8/8/8/K2Pp2q/8/8/2k5/8 w - e6 0 1" };
    perf(pos, 1);
    pos = Position{ "8/8/8/q2Pp2K/8/8/2k5/8 w - e6 0 1" };
    perf(pos, 1);
    pos = Position{ "8/8/8/8/k2pP2Q/8/2K5/8 b - e3 0 1" };
    perf(pos, 1);
    pos = Position{ "8/8/8/8/Q2pP2k/8/2K5/8 b - e3 0 1" };
    perf(pos, 1);
    pos = Position{ "r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2" };
    perf(pos, 1);
    pos = Position{ "8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3" };
    perf(pos, 1);
    pos = Position{ "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2" };
    perf(pos, 1);
    pos = Position{ "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2" };
    perf(pos, 1);
    pos = Position{ "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2" };
    perf(pos, 1);
    pos = Position{ "rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9" };
    perf(pos, 1);
    pos = Position{ "2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4" };
    perf(pos, 1);
    pos = Position{ "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" };
    perf(pos, 3);
    pos = Position{ "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10" };
    perf(pos, 3);
    pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    perf(pos, 2);
    pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    perf(pos, 3);
    pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    perf(pos, 4);
    pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    perf(pos, 5);
    pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    perf(pos, 6);
}
void play(std::string fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
    Position pos{ fen };
    print_position(pos);
    while (true) {
        std::vector<int> moves{};
        pos.get_legal_moves(&moves);
        std::vector<std::string> move_names{};
        for (int i = 0; i < moves.size(); i++) {
            std::cout << uci(moves[i]) << "\n";
            //print_move(moves[i]);
            move_names.push_back(uci(moves[i]));
        }
        /*
        for (int i = 0; i < pos.hash_stack.size(); i++) {
            std::cout << pos.hash_stack[i] << "\n";
        }
        */
        std::string moveString = "";
        int move=0;
        bool special_command = false;
        while (std::find(move_names.begin(), move_names.end(), moveString) == move_names.end()) {
            std::cout << "Enter Move:";
            std::cin >> moveString;
            if (moveString == "quit") {
                special_command = true;
                break;
            }
            else if ((moveString == "unmake") || (moveString == "undo")) {
                pos.unmake_move();
                print_position(pos);
                special_command = true;
                break;
            }
            else if (moveString == "null") {
                pos.make_nullmove();
                print_position(pos);
                special_command = true;
                break;
            }
            else if ((moveString == "unmakeNull") || (moveString == "undoNull")) {
                pos.unmake_nullmove();
                print_position(pos);
                special_command = true;
                break;
            }
        }
        if (!special_command) {
            for (int i = 0; i < moves.size(); i++) {
                if (uci(moves[i]) == moveString) {
                    move = moves[i];
                }
            }
            print_move(move);
            pos.make_move(move);
            print_position(pos);
        }
        else {
            if (moveString == "quit") { break; }
        }
    }
}

void traversal(Position& pos, const int depth) {
    if (depth == 0) {
        std::cout << pos.current_hash << "---" << get_hash(pos) << std::endl;
        return;
    }
    std::vector<int> moves{};
    pos.get_legal_moves(&moves);
    for (int i = 0; i < moves.size(); i++) {
        pos.make_move(moves[i]);
        traversal(pos, depth - 1);
        pos.unmake_move();
    }
}
void position_tree_traversal() {
    Position pos{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    std::cout << pos.current_hash << "---" << get_hash(pos) << std::endl;
    traversal(pos, 1);

}
void build_opening_book(char* file_path,char* save_to) {
    std::vector<book_entry> entries{};
    std::fstream table;
    table.open(file_path, std::ios::in);
    std::string s1{ file_path };
    std::string s2{ save_to };
    if (s1 == s2) {
        std::cout << "You are not allowed to provide the same path twice" << std::endl;
        return;
    }
    if (table.is_open()) {
        std::string str;
        while (std::getline(table, str)) {
            std::string fen = str.substr(0, str.find(":", 0)) + " 0 1";
            //std::cout << fen << "\n";
            str = str.substr(str.find(":", 0) + 1);
            Position pos{ fen };
            //print_position(pos);
            int move=0;
            std::vector<int> legal_moves{};
            pos.get_legal_moves(&legal_moves);
            //std::cout << str << "\n";
            std::array<int,5>moves{};
            std::array<int,5>weights{};
            int total_weights = 0;
            int number_of_good_moves = 0;
            while (str.find(",", 0) != std::string::npos) {
                if (number_of_good_moves >= 4) {
                    break;
                }//at most save the best five moves
                std::string moveString = str.substr(0, str.find(",", 0));
                str = str.substr(str.find(",", 0) + 1);
                for (int i = 0; i < legal_moves.size(); i++) {
                    if (uci(legal_moves[i]) == moveString) {
                        move = legal_moves[i];
                        break;
                    }
                }
                moves[number_of_good_moves] = move;
                //std::cout << uci(move) << "\n";
                int weight;
                if (str.find(":", 0) == std::string::npos) {
                    weight = std::stoi(str);
                }
                else {
                    weight = std::stoi(str.substr(0, str.find(":", 0)));
                }
                total_weights += weight;
                weights[number_of_good_moves]=weight;
                str = str.substr(str.find(":", 0) + 1);
                //std::cout << weight << "\n";
                number_of_good_moves++;
            }
            int new_total_weights = 0;
            //std::cout << "ooo"<<total_weights << "\n";
            for (int i = 0; i < weights.size(); i++) {
                //std::cout << "---" << weights[i];
                weights[i] = (int)std::floor(100 * (float)weights[i] / total_weights);
                //std::cout << "---" << weights[i];
                new_total_weights += weights[i];
            }
            weights[0] = weights[0] + 100 - new_total_weights;
            std::array<move_weight_pair,5> pairs{};
            for (int i = 0; i < 5; i++) {
                pairs[i]=move_weight_pair{moves[i],weights[i]};
            }
            entries.push_back(book_entry{ pos.current_hash,pairs});
            //std::cout << pos.current_hash << "\n";
        }
        table.close();
        OpeningBook book{};
        book.set_entries(entries);
        book.save(save_to);
    }
    else {
        std::cout << "Failed to open the file" << std::endl;
    }
}
bool game_ended(Position& pos) {
    std::vector<int> moves{};
    pos.get_legal_moves(&moves);
    const int number_of_moves = (int)moves.size();

    const bool draw_by_repetition = std::count(pos.hash_stack.begin(), pos.hash_stack.end(), pos.current_hash) >= 3;
    const bool draw_by_fifty_move_rule = pos.moves_without_pawns >= 50;
    const bool in_check = pos.is_attacked_by(bitscan(pos.bitboards[5 + (pos.side) * 6]), !pos.side);
    const bool no_moves_left = number_of_moves == 0;
    const bool draw_by_stalemate = no_moves_left && (!in_check);
    const bool is_draw = draw_by_fifty_move_rule || draw_by_repetition || draw_by_stalemate;
    const bool is_lost = no_moves_left && in_check;
    return is_draw || is_lost;
}
int engine_match(Position& startpos, Rosy& bot1, Rosy& bot2) {
    bool ended_after_bot1_turn = false;
    while (!game_ended(startpos)) {
        bot1.pos = startpos;
        int move = bot1.get_best_move(2);
        startpos.make_move(move);
        if (game_ended(startpos)) {
            ended_after_bot1_turn = true;
            break;
        }
        bot2.pos = startpos;
        startpos.make_move(bot2.get_best_move(2));
    }
    std::vector<int> moves{};
    startpos.get_legal_moves(&moves);
    const int number_of_moves = (int)moves.size();

    const bool draw_by_repetition = std::count(startpos.hash_stack.begin(), startpos.hash_stack.end(), startpos.current_hash) >= 3;
    const bool draw_by_fifty_move_rule = startpos.moves_without_pawns >= 50;
    const bool in_check = startpos.is_attacked_by(bitscan(startpos.bitboards[5 + (startpos.side) * 6]), !startpos.side);
    const bool no_moves_left = number_of_moves == 0;
    const bool draw_by_stalemate = no_moves_left && (!in_check);
    const bool is_draw = draw_by_fifty_move_rule || draw_by_repetition || draw_by_stalemate;
    const bool is_lost = no_moves_left && in_check;
    if (is_draw) {
        return 0;
    }
    if (is_lost) {
        if (ended_after_bot1_turn) {
            return 1;
        }
        return -1;
    }
    else { throw(OtherException()); }
}
void tune() {
    const char* starting_fens[15] = { "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2","rnbqkbnr/pppp1ppp/4p3/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2", "r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 3 3",
    "r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 3 3", "rnbqkbnr/pp1ppppp/2p5/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2" ,"rnbqkb1r/ppp1pppp/3p1n2/8/3PP3/8/PPP2PPP/RNBQKBNR w KQkq - 1 3",
    "r1bqkbnr/pppp1ppp/2n5/4p3/3PP3/5N2/PPP2PPP/RNBQKB1R b KQkq d3 0 3", "rnbqkbnr/ppp1pppp/8/3p4/2PP4/8/PP2PPPP/RNBQKBNR b KQkq c3 0 2", "rnbqkb1r/pppppp1p/5np1/8/2PP4/8/PP2PPPP/RNBQKBNR w KQkq - 0 3",
    "rnbqk2r/pppp1ppp/4pn2/8/1bPP4/2N5/PP2PPPP/R1BQKBNR w KQkq - 2 4", "rnbqkb1r/pppp1ppp/4pn2/8/2PP4/6P1/PP2PP1P/RNBQKBNR b KQkq - 0 3", "rnbqkb1r/ppp1pp1p/5np1/3p4/2PP4/2N5/PP2PPPP/R1BQKBNR w KQkq d6 0 4",
    "rnbqkbnr/ppppp1pp/8/5p2/3P4/8/PPP1PPPP/RNBQKBNR w KQkq f6 0 2", "rnbqkb1r/ppp1pppp/5n2/3p4/3P1B2/5N2/PPP1PPPP/RN1QKB1R b KQkq - 3 3", "rnbqkbnr/ppp1pppp/8/3p4/8/5NP1/PPPPPP1P/RNBQKB1R b KQkq - 0 2" };

    int weights[14] = {
       100,305,333,563,950,2,2,3,5,20,10,
       20,10,15
    };
    int second_weights[14] = {
       83 , 302 , 351 , 550 , 949 , 6 , 2 , 7 , 4 , 19 , 10 , 24 , 6 , 13
    };
    int third_weights[14] = {
       100,305,333,563,950,2,2,3,5,20,10,
       20,10,15
    };
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 0; i < 200; i++) {
        std::uniform_int_distribution<> big_distr(-5, 5);
        std::uniform_int_distribution<> small_distr(-1, 1);
        int params[10][14];
        for (int j = 0; j < 14; j++) {
            params[0][j] = weights[j];
        }
        for (int j = 1; j < 10; j++) {
            int adjustments[14] = { big_distr(gen), big_distr(gen), big_distr(gen), big_distr(gen), big_distr(gen), small_distr(gen),small_distr(gen),small_distr(gen),small_distr(gen),small_distr(gen),small_distr(gen),small_distr(gen),small_distr(gen),small_distr(gen) };

            int arr[14];
            if (j < 5) {
                std::copy(std::begin(weights), std::end(weights), std::begin(arr));
            }
            else if (j < 8) {
                std::copy(std::begin(second_weights), std::end(second_weights), std::begin(arr));
            }
            else {
                std::copy(std::begin(third_weights), std::end(third_weights), std::begin(arr));
            }
            for (int k = 0; k < 14; k++) {
                params[j][k] = arr[k] + adjustments[k];
            }
        }
        int results[10][10] = { {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0} };
        for (int j = 0; j < 10; j++) {
            //std::uniform_int_distribution<> distr(0, 30);
            for (int k = 0; k < 10; k++) {
                for (int l = 0; l < 15; l++) {
                    //Position pos{ starting_fens[distr(gen)] };
                    Position pos{ starting_fens[l] };
                    Rosy bot1{};
                    std::copy(std::begin(params[j]), std::end(params[j]), std::begin(bot1.weights));
                    Rosy bot2{};
                    std::copy(std::begin(params[k]), std::end(params[k]), std::begin(bot2.weights));
                    int result = engine_match(pos, bot1, bot2);
                    if (result == 0) {
                        results[j][k] += 1;
                    }
                    else if (result == 1) {
                        results[j][k] += 2;
                    }
                }
            }
        }
        for (int l = 0; l < 10; l++) {
            for (int m = 0; m < 10; m++) {
                std::cout << results[l][m] << "  ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        int overall_score[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        for (int j = 0; j < 10; j++) {
            for (int k = 0; k < 10; k++) {
                overall_score[j] += results[j][k];
                overall_score[j] += results[k][j];
            }
        }
        std::cout << "total scores: ";
        for (int j = 0; j < 10; j++) {
            std::cout << overall_score[j] << "  ";
        }
        std::cout << "\n";
        int best = (int)std::distance(std::begin(overall_score), std::max_element(std::begin(overall_score), std::end(overall_score)));
        std::copy(std::begin(params[best]), std::end(params[best]), std::begin(weights));
        std::cout << "engine " << best << " won with parameters { ";
        for (int j = 0; j < 13; j++) {
            std::cout << weights[j] << " , ";
        }
        std::cout << weights[13] << " }\n";
        overall_score[best] = 0;
        best = (int)std::distance(std::begin(overall_score), std::max_element(std::begin(overall_score), std::end(overall_score)));
        std::copy(std::begin(params[best]), std::end(params[best]), std::begin(second_weights));
        overall_score[best] = 0;
        best = (int)std::distance(std::begin(overall_score), std::max_element(std::begin(overall_score), std::end(overall_score)));
        std::copy(std::begin(params[best]), std::end(params[best]), std::begin(third_weights));
        if (best == 0) {
            i--;
        }
    }
}

void pinned_pieces_test() {
    Position pos{ "3k4/8/3q4/8/3R4/8/3K4/8 w - - 0 1" };
    print_position(pos);
    int kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    U64 enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_pinned_pieces(kingpos,enemy_attacks));
    pos = Position{ "3k4/8/3q4/8/3R4/8/3K4/8 b - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_pinned_pieces(kingpos, enemy_attacks));
    pos = Position{ "8/6k1/5q2/8/3R4/8/1K6/8 b - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_pinned_pieces(kingpos, enemy_attacks));
    pos = Position{ "8/6k1/5q2/8/3R4/8/1K6/8 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_pinned_pieces(kingpos, enemy_attacks));
    pos = Position{ "8/6k1/5q2/8/3R4/2R5/1K6/8 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_pinned_pieces(kingpos, enemy_attacks));
    pos = Position{ "3R4/6k1/5q2/8/3R4/8/1K6/8 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_pinned_pieces(kingpos, enemy_attacks));
    pos = Position{ "8/6k1/1R3q2/8/3R4/8/1K6/8 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_pinned_pieces(kingpos, enemy_attacks));
    pos = Position{ "8/6k1/1R3q2/8/3R4/8/1K6/8 b - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_pinned_pieces(kingpos, enemy_attacks));

}
void pinned_pieces_move_test() {
    Position pos{ "3k4/8/3q4/8/3R4/8/3K4/8 w - - 0 1" };
    print_position(pos);
    int kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    U64 enemy_attacks = pos.get_attacks_by(!pos.side);
    std::vector<int> moves{};
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "3k4/8/3q4/8/3R4/8/3K4/8 b - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "8/6k1/5q2/8/3R4/8/1K6/8 b - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "8/6k1/5q2/8/3R4/8/1K6/8 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "8/6k1/5q2/8/3R4/2R5/1K6/8 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "3R4/6k1/5q2/8/3R4/8/1K6/8 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "8/6k1/1R3q2/8/3R4/8/1K6/8 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "8/6k1/1R3q2/8/3R4/8/1K6/8 b - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "8/6k1/5q2/8/3B4/8/1K6/8 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "8/6k1/5q2/8/3B4/8/1K6/8 b - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "1b6/k7/8/3pP3/5K2/8/8/8 w - d6 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "b7/1P6/8/k2K4/8/8/8/8 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "8/8/8/k7/8/3r4/3P4/3K4 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "8/8/8/k7/3r4/8/3P4/3K4 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "8/8/3r4/k7/3P4/8/3K4/8 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "8/3r4/8/k7/3P4/8/3K4/8 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "8/3r4/8/k7/8/8/3P4/3K4 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "8/3rP1K1/8/k7/8/8/8/8 w - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
    pos = Position{ "4k3/5p2/8/4p2Q/8/8/8/4K3 b - - 0 1" };
    print_position(pos);
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    enemy_attacks = pos.get_attacks_by(!pos.side);
    print_bitboard(pos.get_moves_for_pinned_pieces(&moves, kingpos, enemy_attacks));
    for (int i = 0; i < moves.size(); i++) {
        print_move(moves[i]);
    }
    moves.clear();
}
void checking_ray_test(){
    Position pos{};
    pos = Position{ "8/4Q3/8/K6k/8/8/4B3/8 b - - 0 1" };
    int kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    U64 kings_queen_scope = get_queen_attacks(pos.occupancies[both], kingpos);
    pos.print();
    print_bitboard(pos.get_checking_rays(kingpos));
    pos = Position{ "8/5B2/8/K1R4k/8/8/8/8 b - - 0 1" };
    kingpos = bitscan(pos.bitboards[K + (pos.side) * 6]);
    pos.print();
    print_bitboard(pos.get_checking_rays(kingpos));

}
unsigned long long test2() {
    unsigned long long arr[12] = {71776119061217280ULL, 4755801206503243776ULL, 2594073385365405696ULL, 9295429630892703744ULL, 576460752303423488ULL, 1152921504606846976ULL, 65280ULL, 66ULL, 36ULL, 129ULL, 8ULL, 16ULL};
    __m128i data = _mm_load_si128((__m128i*)arr);
    __m128i data2 = _mm_load_si128((__m128i*)(&arr+3));
    data = _mm_or_si128(data, data2);
    //std::cout << data << "\n";
    return 0;
}
int main()
{
    //perft();
    test();
    //position_tree_traversal();
    //build_opening_book((char*)"C:\\Users\\marvi\\source\\repos\\Rosy\\opening_data.txt", (char*)"C:\\Users\\marvi\\source\\repos\\Rosy\\opening_part1.txt");
    //OpeningBook book{ (char*)"C:\\Users\\marvi\\source\\repos\\Rosy\\openingBook.txt" };
    //OpeningBook book2{ (char*)"C:\\Users\\marvi\\source\\repos\\Rosy\\opening_part1.txt" };
    //book.join(book2);
    //std::cout << "joined\n";
    //book.save((char*)"C:\\Users\\marvi\\source\\repos\\Rosy\\openingBook.txt");
    //pinned_pieces_test();
    //pinned_pieces_move_test();go wtime 60000 btime 60000 winc 1000 binc 1000
    //checking_ray_test();position startpos moves e2e4 e7e5 g1f3 d8e7 f1c4 e7e5 d2d4 e7e6 d4e5 e7e5
    //Rosy bot{};
    //bot.uci_loop();
    //play("1k1r3q/1ppn3p/p4b2/4p3/8/P2N2P1/1PP1R1BP/2K1Q3 w - - 0 1");
    //perft();
    return 0;
}
/*
position startpos moves e2e4 d7d5 e4d5 d8d5 g1f3 d5e4 f1e2 g8f6 b1c3 e4g4 e1g1 g4b4 d2d4 c8g4 f3e5 g4e2 d1e2 b8c6 d4d5 c6e5 e2e5 f6d7 e5c7 b4b6 c7f4 b6f6 f4a4 a8c8 f1e1 f6g6 c1f4 g6a6 a4e4 c8c4 e4f5 a6f6 f5f6 g7f6 f4e3 a7a6 a1d1 h8g8 g2g3 d7e5 g1g2 g8g6 f2f4 e5g4 f4f5 g4e3 e1e3 g6g5 b2b3 c4c5 d1f1 g5g8 f1e1 g8g5 d5d6 e7e6 c3e4 c5c2 e3e2 c2e2 e1e2 g5f5 e2c2 e8d8 c2c7 f8d6 c7f7 f5a5 e4d6 a5a2 g2h3 a2d2 d6e4 d2c2 e4f6 c2e2 f7b7
83 , 302 , 351 , 550 , 949 , 6 , 2 , 7 , 4 , 19 , 10 , 24 , 6 , 13
*/
/*std::array<std::array<std::array<int,64>,12>,206>
position startpos moves e2e4 d7d5 e4d5 d8d5 b1c3 d5e6 f1e2 b8c6 g1f3
position startpos moves e2e4 c7c5 g1f3 d7d6 d2d4 c5d4 f3d4 g8f6 f1c4 e7e5 d4f5 d6d5 e4d5 c8f5 a2a3 d8c7 b1d2 b8d7 d1e2 e8c8 d2f3 e5e4 e1g1 e4f3 e2f3 f5e4 f3e2 d8e8 f1e1 e4g6 e2e8 f6e8 e1e8 c7d8 e8d8 c8d8 c4b3 f8d6 a3a4 d7c5 c1e3 c5b3 c2b3 g6c2 e3a7 c2b3 a4a5 h8e8 a7e3 f7f5 a5a6 b7a6 e3d2 e8e2 d2e3 b3d5 b2b3 d5b3 g2g3 d8c8 e3d4 g7g5 f2f4 g5f4 a1a6 f4g3 a6d6 g3h2 g1h1 c8c7 d4c5 b3a4 d6d5 a4c6 c5d6 c7d7 d6h2 c6d5 h1g1
*/