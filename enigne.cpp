#include "engine.h"
struct StopException : public std::exception {
	const char* what() const throw() {
		return "Stop evaluation exception";
	}
};
struct OutOfTimeException : public std::exception {
	const char* what() const throw() {
		return "Out of time for evaluation exception";
	}
};
struct OutOfNodesException : public std::exception {
	const char* what() const throw() {
		return "Stop evaluation exception";
	}
};
struct PonderhitException : public std::exception {
	const char* what() const throw() {
		return "Ponderhit exception";
	}
};
static U64 history[2][12][64] = { {
	{0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},
	{ 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0 }
},{
	{0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},
	{ 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0 }
}};
namespace std {
	string getCurDir() {
		char buff[512];
		_getcwd(buff, 512);
		string current_working_dir(buff);
		return current_working_dir;
	}
}


OpeningBook book{ (std::getCurDir() + std::string("\\engines\\openingBook.txt")).c_str() };


Rosy::Rosy() {
	pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
	maxDepth = 7;
	set_piece_square_table(piece_square_table);
}
Rosy::Rosy(const Rosy& other) {
	std::memcpy(this, &other, sizeof(other));
	set_piece_square_table(piece_square_table);
}
inline void Rosy::parse_go(char* command, std::atomic<bool>* run) {
#if set_hashtable_size
	if ((hash_map.size()*(sizeof(TableEntrie{}))) >= max_HashTableSize) {
		hash_map.clear();
	}
	//std::cout << hash_map.size() << "B\n";
#endif
	int depth = -1;
	char* current_depth = nullptr;
	pondering = false;
	if (current_depth = strstr(command, "wtime")) {
		depth = 7;
		char* current_char = current_depth + 6;
		std::string time_str = "";
		while ((current_char != nullptr) && (*current_char != ' ')) {
			time_str += *current_char;
			current_char++;
		}
		const int wtime = stoi(time_str);
		current_char = strstr(command, "btime") + 6;
		time_str = "";
		while ((current_char != nullptr) && (*current_char != ' ')) {
			time_str += *current_char;
			current_char++;
		}
		const int btime = stoi(time_str);
		current_char = strstr(command, "winc") + 5;
		time_str = "";
		while ((current_char != nullptr) && (*current_char != ' ')) {
			time_str += *current_char;
			current_char++;
		}
		const int winc = stoi(time_str);
		current_char = strstr(command, "binc") + 5;
		time_str = "";
		while ((current_char != nullptr) && (*current_char != ' ') && (*current_char != '\n')) {
			time_str += *current_char;
			current_char++;
		}
		const int binc = stoi(time_str);
		const bool increment = ((pos.side) * binc + (!pos.side) * winc) != 0;
		max_search_time = (U64)((pos.side) * binc + (!pos.side) * winc);
		U64 totalTime = (U64)((!pos.side) * wtime + (pos.side) * btime);
		if (totalTime > last_search_time) {
			time_at_begining_of_game = totalTime;
		}
		last_search_time = totalTime;
		if (totalTime > time_at_begining_of_game) {
			max_search_time = 2ULL * max_search_time + (U64)std::floor(0.3 * (totalTime - time_at_begining_of_game));
		}
		else if (totalTime > 0.8 * time_at_begining_of_game) {
			max_search_time = (U64)std::floor(2.5 * max_search_time);
		}
		else if (totalTime > 0.5 * time_at_begining_of_game) {
			max_search_time = 2ULL * max_search_time;
		}
		else if (totalTime > 0.3 * time_at_begining_of_game) {
			max_search_time = (U64)std::floor(1.5 * max_search_time);
		}
		else if (totalTime < max_search_time) {
			if (totalTime < 0.5 * (long double)max_search_time) {
				max_search_time = (U64)std::floor(0.25 * max_search_time);
			}
			else max_search_time = (U64)std::floor(0.5 * max_search_time);
		}
		std::cout << max_search_time << "\n";
		movetime_search(run);
		return;

	}
	else if (current_depth = strstr(command, "infinite")) {
		infinite_search(run);
		return;
	}
	else if (current_depth = strstr(command, "ponderhit")) {
		maxDepth = 8;
		best_move(run);
		return;
	}
	else if (current_depth = strstr(command, "ponder")) {
		pondering = true;
		ponder_search(run);
		return;
	}
	else if (current_depth = strstr(command, "movetime")) {
		char* current_char = current_depth + 9;
		std::string time_str = "";
		while ((current_char != nullptr) && (*current_char != ' ') && (*current_char != '\n')) {
			time_str += *current_char;
			current_char++;
		}
		max_search_time = stoi(time_str);
		movetime_search(run);
		return;
	}
	else if (current_depth = strstr(command, "nodes")) {
		char* current_char = current_depth + 6;
		std::string node_str = "";
		while ((current_char != nullptr) && (*current_char != ' ') && (*current_char != '\n')) {
			node_str += *current_char;
			current_char++;
		}
		max_search_nodes = stoi(node_str);
		max_nodes_search(run);
		return;
	}
	else if (current_depth = strstr(command, "mate")) {
		char* current_char = current_depth + 5;
		std::string number_str = "";
		while ((current_char != nullptr) && (*current_char != ' ') && (*current_char != '\n')) {
			number_str += *current_char;
			current_char++;
		}
		maxDepth = stoi(number_str);
		find_mate(run);
		return;
	}
	else if (current_depth = strstr(command, "movestogo")) {
		//std::cout << "not currently not supported" << std::endl;
		depth = 6;
	}
	else if (current_depth = strstr(command, "depth")) {
		depth = std::stoi(current_depth + 6);
	}
	else { depth = 7; }
	maxDepth = depth;
	best_move(run);
}
void Rosy::uci_loop() {
	fflush(stdin);
	fflush(stdout);
	char input[2000];

	std::vector<std::thread> workers{};
	std::atomic<bool> run = false;
	char* command = nullptr;
	std::cout << "option name Move Overhead type spin default 100 min 0 max 20000\noption name Threads type spin default 2 min 2 max 2\noption name Hash type spin default 512 min 256" << std::endl;
	while (true) {
		memset(input, 0, sizeof(input));
		fflush(stdout);
		if (!fgets(input, 2000, stdin)) {
			continue;
		}

		if (!run) {
			for (int i = 0; i < workers.size(); i++) {
				if (workers[i].joinable()) {
					workers[i].join();
					workers.erase(workers.begin() + i);
				}
			}
			pondering = false;
		}//if no thread is running join all threads and delete joined threads from workers

		if (input[0] == '\n') {
			continue;
		}
		if (strncmp(input, "isready", 7) == 0) {
			std::cout << "readyok\n";
		}
		else if (strncmp(input, "position", 8) == 0) {
			parse_position(input);
		}
		else if (strncmp(input, "ucinewgame", 10) == 0) {
			parse_position((char*)"position startpos");
		}
		else if (strncmp(input, "go", 2) == 0) {
			if (!run) {
				run = true;
				workers.push_back(std::thread(&Rosy::parse_go, this, input, &run));
			}

			using namespace std::literals::chrono_literals;
			std::this_thread::sleep_for(500us);
		}
		else if (strncmp(input, "quit", 4) == 0) {
			pondering = false;
			run = false;
			for (int i = 0; i < workers.size(); i++) {
				if (workers[i].joinable()) {
					workers[i].join();
					workers.erase(workers.begin() + i);
				}
			}
			break;
		}
		else if (strncmp(input, "stop", 4) == 0) {
			if (pondering) {
				pondering = false;
				run = false;
				for (int i = 0; i < workers.size(); i++) {
					if (workers[i].joinable()) {
						workers[i].join();
						workers.erase(workers.begin() + i);
					}
				}
				if (!run) {
					run = true;
					workers.push_back(std::thread(&Rosy::parse_go, this, (char*)("go depth 7"), &run));
				}

				using namespace std::literals::chrono_literals;
				std::this_thread::sleep_for(500us);
			}
			else {
				run = false;
				for (int i = 0; i < workers.size(); i++) {
					if (workers[i].joinable()) {
						workers[i].join();
						workers.erase(workers.begin() + i);
					}
				}
				pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
			}
		}
		else if (strncmp(input, "uci", 3) == 0) {
			std::cout<<"id name Rosy\n";
			std::cout<<"id author name disappointed_lama \n";
			std::cout<<"uciok\n";
		}
		else if (strncmp(input, "ponderhit", 9) == 0) {
			run = false;
			for (int i = 0; i < workers.size(); i++) {
				if (workers[i].joinable()) {
					workers[i].join();
					workers.erase(workers.begin() + i);
				}
			}
			if (!run) {
				run = true;
				workers.push_back(std::thread(&Rosy::parse_go, this, input, &run));
			}
			using namespace std::literals::chrono_literals;
			std::this_thread::sleep_for(500us);
		}
	}
}

int Rosy::get_best_move(const int depth) {
	maxDepth = depth;
	potential_ponder_move = 0;
	std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	assert(size(moves) >= 1);
	int best = moves[0];
	int old_best = best;
	number_of_calls++;
	const int aspirationWindow = 80;
	const int infinity = 1000000;
	int alpha = -infinity;
	int beta = infinity;
	try {
		last_search_depth = maxDepth;
		for (current_desired_depth = 1; current_desired_depth <= maxDepth; current_desired_depth++) {
			nodes = 0;
			old_best = best;
			best = quiet_root_call(current_desired_depth, alpha, beta);
			const bool fell_outside_window = (current_eval <= alpha) || (current_eval >= beta);
			if (fell_outside_window) {
				nodes = 0;
				best = quiet_root_call(current_desired_depth, -infinity, infinity);
			}
			alpha = current_eval - aspirationWindow;//[current_desired_depth];
			beta = current_eval + aspirationWindow;//[current_desired_depth];
			if ((current_eval == 1000000) || (current_eval == -1000000)||(current_eval == -1000001)||(current_eval == 1000001)) {
				last_search_depth = current_desired_depth;
				return best;
			}
		}
		//std::cout << last_search_time << std::endl;
		if (best != -1) {
			return best;
		}
	}
	catch (StopException e) {
		if (DEBUG) {
			std::cout << "Caught StopException. Execution of command stopped\n";
		}
		std::cout << "bestmove " << uci(old_best) << std::endl;
	}
}
void Rosy::best_move(std::atomic<bool>* run) {
	if (use_opening_book) {
		const int move = book.find_move(pos.current_hash);
		if (move) {
			std::cout << "bestmove " << uci(move) << std::endl;
			*run = false;
			pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
			return;
		}
	}
	potential_ponder_move = 0;
	std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	assert(size(moves) >= 1);
	int best = moves[0];
	int old_best = best;
	number_of_calls++;
	const int aspirationWindow = 100;
	//const std::array<int, 20> aspirationWindow = { 0, 100, 90, 80, 70, 60, 50, 40, 30, 25, 20, 18, 15, 12, 10, 10, 10, 10, 10, 10 };
	const int infinity = 1000000;
	int alpha = -infinity;
	int beta = infinity;
	try {
		last_search_depth = maxDepth;
		for (current_desired_depth = 1; current_desired_depth <= maxDepth; current_desired_depth++) {
			nodes = 0;
			old_best = best;
			best = root_call(current_desired_depth, alpha, beta, run);
			const bool fell_outside_window = (current_eval <= alpha) || (current_eval >= beta);
			if (fell_outside_window) {
				nodes = 0;
				best = root_call(current_desired_depth, -infinity, infinity, run);
			}
			alpha = current_eval - aspirationWindow;//[current_desired_depth];
			beta = current_eval + aspirationWindow;//[current_desired_depth];
			if (current_eval == 1000000) {
				last_search_depth = current_desired_depth;
				std::cout << "\nbestmove " << uci(best) << std::endl;
				*run = false;
				pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
				return;
			}
			else if (current_eval == -1000000) {
				last_search_depth = current_desired_depth;
				std::cout << "\nbestmove " << uci(old_best) << std::endl;
				*run = false;
				pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
				return;
			}
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		if (best != -1) {
			std::cout << "bestmove " << uci(best);
			if (potential_ponder_move) {
				std::cout << " [ ponder " << uci(potential_ponder_move) << " ]" << std::endl;
			}
			std::cout << std::endl;
		}
		else {
			std::cout << "\n\n\nUnable to find a good move, researching shallower tree\n\n\n";
			std::vector<int> moves{};
			pos.get_legal_moves(&moves);
			int move = 0;
			if (moves.size() != 0) {
				//move = moves[0];
				bool found_longest_lasting_move = false;
				for (int i = 1; i < maxDepth; i++) {
					move = root_call(maxDepth - i, -1000000, 1000000, run);
					if (move != -1) {
						std::cout << "Found move at depth " << i << "\n";
						std::cout << "bestmove " << uci(move) << "\n";
						found_longest_lasting_move = true;
						break;
					}
				}
				if (!found_longest_lasting_move) {
					std::cout << "bestmove " << uci(moves[0]) << "\n";
				}
			}

		}
	}
	catch (StopException e) {
		if (DEBUG) {
			std::cout << "Caught StopException. Execution of command stopped\n";
		}
		std::cout << "bestmove " << uci(old_best) << std::endl;
	}
	*run = false;
	pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
}
void Rosy::infinite_search(std::atomic<bool>* run) {
	std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	assert(size(moves) >= 1);
	int best = moves[0];
	int old_best = best;
	number_of_calls++;
	const int aspirationWindow = 100;
	//const std::array<int, 20> aspirationWindow = { 0, 100, 90, 80, 70, 60, 50, 40, 30, 25, 20, 18, 15, 12, 10, 10, 10, 10, 10, 10 };
	const int infinity = 1000000;
	int alpha = -infinity;
	int beta = infinity;
	try {
		maxDepth = 20;
		for (current_desired_depth = 1; current_desired_depth <= maxDepth; current_desired_depth++) {
			nodes = 0;
			old_best = best;
			best = infinite_root_call(current_desired_depth, alpha, beta, run);
			const bool fell_outside_window = (current_eval <= alpha) || (current_eval >= beta);
			if (fell_outside_window) {
				nodes = 0;
				best = infinite_root_call(current_desired_depth, -infinity, infinity, run);
			}
			alpha = current_eval - aspirationWindow;//[current_desired_depth];
			beta = current_eval + aspirationWindow;//[current_desired_depth];
			if (current_eval == 1000000) {
				last_search_depth = current_desired_depth;
				std::cout << "\nbestmove " << uci(best) << std::endl;
				*run = false;
				pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
				return;
			}
			else if (current_eval == -1000000) {
				last_search_depth = current_desired_depth;
				std::cout << "\nbestmove " << uci(old_best) << std::endl;
				*run = false;
				pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
				return;
			}
		}
		std::cout << "bestmove " << uci(best) << std::endl;
	}
	catch (StopException e) {
		if (DEBUG) {
			std::cout << "Caught StopException. Execution of command stopped\n";
		}
		std::cout << "bestmove " << uci(old_best) << std::endl;
	}
	*run = false;
	pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
}
void Rosy::ponder_search(std::atomic<bool>* run) {
	std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	assert(size(moves) >= 1);
	int best = moves[0];
	int old_best = best;
	number_of_calls++;
	const int aspirationWindow = 100;
	//const std::array<int, 20> aspirationWindow = { 0, 100, 90, 80, 70, 60, 50, 40, 30, 25, 20, 18, 15, 12, 10, 10, 10, 10, 10, 10 };
	const int infinity = 1000000;
	int alpha = -infinity;
	int beta = infinity;
	Position old_pos = pos;
	try {
		maxDepth = 20;
		for (current_desired_depth = 1; current_desired_depth <= maxDepth; current_desired_depth++) {
			nodes = 0;
			old_best = best;
			best = infinite_root_call(current_desired_depth, alpha, beta, run);
			const bool fell_outside_window = (current_eval <= alpha) || (current_eval >= beta);
			if (fell_outside_window) {
				nodes = 0;
				best = infinite_root_call(current_desired_depth, -infinity, infinity, run);
			}
			alpha = current_eval - aspirationWindow;//[current_desired_depth];
			beta = current_eval + aspirationWindow;//[current_desired_depth];
			if ((current_eval == 1000000) || (current_eval == -1000000)) {
				*run = false;
				pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
				return;
			}
		}
	}
	catch (StopException e) {
		if (DEBUG) {
			std::cout << "Caught StopException. Execution of command stopped\n";
		}
		std::cout << "bestmove " << uci(old_best) << std::endl;
	}
	*run = false;
	pos = old_pos;
}
void Rosy::movetime_search(std::atomic<bool>* run) {
	if (use_opening_book) {
		const int move = book.find_move(pos.current_hash);
		if (move) {
			std::cout << "bestmove " << uci(move) << "\n";
			*run = false;
			pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
			return;
		}
	}
	std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	assert(size(moves) >= 1);
	int best = moves[0];
	int old_best = best;
	number_of_calls++;
	const int aspirationWindow = 100;
	//const std::array<int, 20> aspirationWindow = { 0, 100, 90, 80, 70, 60, 50, 40, 30, 25, 20, 18, 15, 12, 10, 10, 10, 10, 10, 10 };
	const int infinity = 1000000;
	int alpha = -infinity;
	int beta = infinity;
	try {
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		maxDepth = 20;
		max_search_time -= move_overhead;
		for (current_desired_depth = 1; current_desired_depth <= maxDepth; current_desired_depth++) {
			nodes = 0;
			old_best = best;
			best = movetime_root_call(current_desired_depth, alpha, beta, run, begin);
			const bool fell_outside_window = (current_eval <= alpha) || (current_eval >= beta);
			if (fell_outside_window) {
				nodes = 0;
				best = movetime_root_call(current_desired_depth, -infinity, infinity, run, begin);
			}
			alpha = current_eval - aspirationWindow;//[current_desired_depth];
			beta = current_eval + aspirationWindow;//[current_desired_depth];
			if (current_eval == 1000000) {
				last_search_depth = current_desired_depth;
				std::cout << "\nbestmove " << uci(best) << std::endl;
				*run = false;
				pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
				return;
			}
			else if (current_eval == -1000000) {
				last_search_depth = current_desired_depth;
				std::cout << "\nbestmove " << uci(old_best) << std::endl;
				*run = false;
				pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
				return;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
		std::cout << "bestmove " << uci(best) << std::endl;
	}
	catch (StopException e) {
		if (DEBUG) {
			std::cout << "Caught StopException. Execution of command stopped\n";
		}
		std::cout << "bestmove " << uci(old_best) << std::endl;
	}
	catch (OutOfTimeException e) {
		std::cout << "bestmove " << uci(old_best) << std::endl;
	}
	*run = false;
	pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
}
void Rosy::max_nodes_search(std::atomic<bool>* run) {
	if (use_opening_book) {
		const int move = book.find_move(pos.current_hash);
		if (move) {
			std::cout << "bestmove " << uci(move) << std::endl;
			*run = false;
			pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
			return;
		}
	}std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	assert(size(moves) >= 1);
	int best = moves[0];
	int old_best = best;
	number_of_calls++;
	const int aspirationWindow = 100;
	//const std::array<int, 20> aspirationWindow = { 0, 100, 90, 80, 70, 60, 50, 40, 30, 25, 20, 18, 15, 12, 10, 10, 10, 10, 10, 10 };
	const int infinity = 1000000;
	int alpha = -infinity;
	int beta = infinity;
	try {
		maxDepth = 20;
		for (current_desired_depth = 1; current_desired_depth <= maxDepth; current_desired_depth++) {
			nodes = 0;
			old_best = best;
			best = nodes_root_call(current_desired_depth, alpha, beta, run);
			const bool fell_outside_window = (current_eval <= alpha) || (current_eval >= beta);
			if (fell_outside_window) {
				nodes = 0;
				best = nodes_root_call(current_desired_depth, -infinity, infinity, run);
			}
			alpha = current_eval - aspirationWindow;//[current_desired_depth];
			beta = current_eval + aspirationWindow;//[current_desired_depth];
			if (current_eval == 1000000) {
				last_search_depth = current_desired_depth;
				std::cout << "\nbestmove " << uci(best) << std::endl;
				*run = false;
				pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
				return;
			}
			else if (current_eval == -1000000) {
				last_search_depth = current_desired_depth;
				std::cout << "\nbestmove " << uci(old_best) << std::endl;
				*run = false;
				pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
				return;
			}
		}
		std::cout << "bestmove " << uci(best) << std::endl;
	}
	catch (StopException e) {
		if (DEBUG) {
			std::cout << "Caught StopException. Execution of command stopped\n";
		}
		std::cout << "bestmove " << uci(old_best) << std::endl;
	}
	catch (OutOfNodesException e) {
		std::cout << "bestmove " << uci(old_best) << std::endl;
	}
	*run = false;
	pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
}
void Rosy::find_mate(std::atomic<bool>* run) {
	std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	assert(size(moves) >= 1);
	int best = moves[0];
	int old_best = best;
	number_of_calls++;
	const int aspirationWindow = 100;
	//const std::array<int, 20> aspirationWindow = { 0, 100, 90, 80, 70, 60, 50, 40, 30, 25, 20, 18, 15, 12, 10, 10, 10, 10, 10, 10 };
	const int infinity = 1000000;
	int alpha = -infinity;
	int beta = infinity;
	try {
		for (current_desired_depth = 1; current_desired_depth <= maxDepth; current_desired_depth++) {
			nodes = 0;
			old_best = best;
			best = root_call(current_desired_depth, alpha, beta, run);
			const bool fell_outside_window = (current_eval <= alpha) || (current_eval >= beta);
			if (fell_outside_window) {
				nodes = 0;
				best = root_call(current_desired_depth, -infinity, infinity, run);
			}
			alpha = current_eval - aspirationWindow;//[current_desired_depth];
			beta = current_eval + aspirationWindow;//[current_desired_depth];
			if ((current_eval == 1000000) || (current_eval == -1000000)) {
				last_search_depth = current_desired_depth;
				std::cout << "\nbestmove " << uci(best) << std::endl;
				*run = false;
				pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
				return;
			}
		}
		std::cout << "no mate found" << std::endl;
	}
	catch (StopException e) {
		if (DEBUG) {
			std::cout << "Caught StopException. Execution of command stopped\n";
		}
		std::cout << "bestmove " << uci(old_best) << std::endl;
	}
	*run = false;
	pos = Position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
}

int Rosy::quiet_root_call(const int depth, int alpha, int beta) {
	const double alphaOrigin = alpha;
	TableEntrie ttEntry = lookUp();
	std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	const int number_of_moves = (int)moves.size();

	const int ply = current_desired_depth - depth;
	assert(PV_TABLE_SIZE > ply);
	pv_length[ply] = ply;

	moves = order(moves, ttEntry);
	assert(moves.size() == number_of_moves);
	int current_best_move = -1;
	int current_best_eval = -1000000;
	for (int i = 0; i < number_of_moves; i++) {
		pos.make_move(moves[i]);
		int value = -pv_search(depth - 1, -beta, -alpha);
		pos.unmake_move();
		const bool is_best_move = (value > current_best_eval);
		current_best_move = (is_best_move)*moves[i] + (!is_best_move) * current_best_move;
		current_best_eval = (is_best_move)*value + (!is_best_move) * current_best_eval;

		if (value > alpha) {
			alpha = value;
			pv_table[ply][ply] = moves[i];
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
				pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
			}
			pv_length[ply] = pv_length[ply + 1];
			if (value == 1000000) {
				current_eval = value;
				hash_map[pos.current_hash] = TableEntrie{ current_best_eval,EXACT,current_best_move,depth };

				return moves[i];
			}
		}
	}
	hash_map[pos.current_hash] = TableEntrie{ current_best_eval,EXACT,current_best_move,depth };
	if (current_best_eval == -1000000) {
		current_eval = current_best_eval;
		return current_best_move;
	}
	else {
	}

	current_eval = current_best_eval;
	return current_best_move;
}
int Rosy::root_call(const int depth, int alpha, int beta, std::atomic<bool>* run) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	const double alphaOrigin = alpha;
	TableEntrie ttEntry = lookUp();
	std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	const int number_of_moves = (int)moves.size();

	const int ply = current_desired_depth - depth;
	assert(ply < PV_TABLE_SIZE);
	pv_length[ply] = ply;

	moves = order(moves,ttEntry);
	assert(moves.size() == number_of_moves);

	int current_best_move = -1;
	int current_best_eval = -1000000;
	for (int i = 0; i < number_of_moves; i++) {
		if (!(*run)) {
			throw(StopException());
		}
		pos.make_move(moves[i]);
		int value = -pv_search(depth - 1, -beta, -alpha);
		pos.unmake_move();
		const bool is_best_move = (value > current_best_eval);
		current_best_move = (is_best_move) * moves[i]+ (!is_best_move)*current_best_move;
		current_best_eval = (is_best_move)*value + (!is_best_move) * current_best_eval;
		if (DEBUG) {
			if (depth == maxDepth) {
				if (is_best_move) {
					std::cout << "Found new best move in: " << uci(moves[i]) << " (" << value << ")" << "\n";
				}
				else {
					std::cout << "finished " << uci(moves[i]) << "\n";
				}
			}
		}

		if (value > alpha) {
			alpha = value;
			pv_table[ply][ply] = moves[i];
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
				pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
			}
			pv_length[ply] = pv_length[ply + 1];
			if (value == 1000000) {
				std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
				const U64 time = (U64)std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
				std::cout << "info score mate " << current_desired_depth << " depth " << depth << " nodes " << nodes <<" nps " << 1000000000 * nodes/time << std::endl;
				current_eval = value;
				hash_map[pos.current_hash] = TableEntrie{ current_best_eval,EXACT,current_best_move,depth };

				return moves[i];
			}
		}
	}
	hash_map[pos.current_hash] = TableEntrie{ current_best_eval,EXACT,current_best_move,depth };
	if (current_best_eval==-1000000) {
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		const U64 time = (U64)std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		std::cout << "info score mate " << current_desired_depth << " depth " << depth << " nodes " << nodes <<" nps " << 1000000000 * nodes / time << " pv ";
		current_eval = current_best_eval;
		return current_best_move;
	}
	else {
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		const U64 time = (U64)std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		std::cout << "info score cp " << current_best_eval << " depth " << depth << " nodes " << nodes <<" nps "<< 1000000000 * nodes / time << " pv ";
	}
	if (current_desired_depth >= 2) {
		potential_ponder_move = pv_table[0][1];
	}
	/*
	for (int i = 0; i < pv_length[0]; i++) {
		std::cout << uci(pv_table[0][i]) << " ";
	}
	*/

	int j = 0;
	for (int i = 0; i < current_desired_depth; i++) {
		TableEntrie entry = lookUp();
		if (entry.flag != EXACT) break;
		j++;
		std::cout << uci(entry.move) << " ";
		pos.make_move(entry.move);
	}
	for (int i = 0; i < j; i++) {
		pos.unmake_move();
	}
	std::cout << std::endl;


	current_eval = current_best_eval;

	return current_best_move;
}
int Rosy::pv_search(const int depth, int alpha, int beta) {
	nodes++;
	/*
	* used to use >= as condition for table entry being used
	*/
	const double alphaOrigin = alpha;
	TableEntrie ttEntry = lookUp();

	const int ply = current_desired_depth - depth;
	assert(ply < PV_TABLE_SIZE);
	pv_length[ply] = ply;

	if ((ttEntry.depth > depth)) {
		if (ttEntry.flag == EXACT) {
			if (ttEntry.eval > alpha) {
				pv_table[ply][ply] = ttEntry.move;
				for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
					pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
				}
				pv_length[ply] = pv_length[ply + 1];
			}
			return ttEntry.eval;
		}
		else if ((ttEntry.flag == LOWER) && (ttEntry.eval > alpha)) {
			alpha = ttEntry.eval;
			pv_table[ply][ply] = ttEntry.move;
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
				pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
			}
			pv_length[ply] = pv_length[ply + 1];
		}
		else if (ttEntry.flag == UPPER) {
			beta = std::min(beta, ttEntry.eval);
		}
		if (alpha >= beta) {
			return ttEntry.eval;
		}
	}


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

	if (is_draw || is_lost) {
		const int eval = evaluate(is_draw, is_lost);
		hash_map[pos.current_hash] = TableEntrie{ eval,EXACT,0,depth };
		return eval;
	}
	if (depth == 0) {
		return quiescence(alpha, beta);
	}

	if ((depth >= 1 + Red) && (!in_check)) {
		pos.make_nullmove();
		int nm_value = -pv_search(depth - 1 - Red, -beta, -beta + 1);
		pos.unmake_nullmove();
		if (nm_value >= beta) {
			return beta;
		}
	}
	moves = order(moves, ttEntry);
	assert(moves.size() == number_of_moves);

	int current_best_move = -1;
	int current_best_eval = -1000000;

	pos.make_move(moves[0]);
	bool now_in_check = pos.is_attacked_by(bitscan(pos.bitboards[K + (pos.side) * 6]), !pos.side);
	int value = -pv_search(depth - (!now_in_check), -beta, -alpha);
	pos.unmake_move();

	const bool is_new_best = (current_best_eval < value);
	current_best_eval = (is_new_best)*value + (!is_new_best) * current_best_eval;
	current_best_move = (is_new_best)*moves[0] + (!is_new_best) * current_best_move;

	if (value > alpha) {
		if (value >= beta) {
			const bool not_capture = !get_capture_flag(moves[0]);
			if (not_capture) {
				killer_table.push_move(moves[0], pos.turn);
				history[pos.side][(int)get_piece_type(moves[0])][(int)get_to_square(moves[0])] += (!get_capture_flag(moves[0])) * (unsigned long long)(int_power(2, depth));
			}
			if (depth >= ttEntry.depth) {
				hash_map[pos.current_hash] = TableEntrie{ value,LOWER,moves[0],depth };
			}
			return beta;
		}
		alpha = value;
		pv_table[ply][ply] = moves[0];
		for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
			pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
		}
		pv_length[ply] = pv_length[ply + 1];
	}
	const int full_depth_moves = 8;
	const int reduction_limit = 3;
	for (int i = 1; i < number_of_moves; i++) {

		pos.make_move(moves[i]);
		now_in_check = pos.is_attacked_by(bitscan(pos.bitboards[K + (pos.side) * 6]), !pos.side);
		value = -pv_search(depth - 1 - (!now_in_check) * ((i >= full_depth_moves) && (depth >= reduction_limit) && (!in_check)), -alpha - 1, -alpha);
		if ((value > alpha) && (value < beta)) {
			value = -pv_search(depth - (!now_in_check), -beta, -alpha);
		}
		pos.unmake_move();

		const bool is_new_best = (current_best_eval < value);
		current_best_eval = (is_new_best)*value + (!is_new_best) * current_best_eval;
		current_best_move = (is_new_best)*moves[i] + (!is_new_best) * current_best_move;

		if (value > alpha) {
			if (value >= beta) {
				const bool not_capture = !get_capture_flag(moves[i]);
				if (not_capture){
					killer_table.push_move(moves[i], pos.turn);
					history[(size_t)((pos.side))][(size_t)get_piece_type(moves[i])][(size_t)get_to_square(moves[i])] += (!get_capture_flag(moves[i])) * (unsigned long long)(int_power(2, depth));
				}
				if (depth >= ttEntry.depth) {
					hash_map[pos.current_hash] = TableEntrie{ value,LOWER,moves[i],depth };
				}
				return beta;
			}
			alpha = value;
			pv_table[ply][ply] = moves[i];
			assert(ply < PV_TABLE_SIZE - 2);
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
				pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
			}
			pv_length[ply] = pv_length[ply + 1];
		}
	}
	if (depth >= ttEntry.depth) {
		hash_map[pos.current_hash] = TableEntrie{ alpha,(current_best_eval <= alphaOrigin) * UPPER,current_best_move,depth };
	}
	return alpha;
}
int Rosy::quiescence(int alpha, int beta){
	nodes++;

	const double alphaOrigin = alpha;
	TableEntrie ttEntry = lookUp();

	std::vector<int> captures{};
	pos.get_legal_captures(&captures);
	const int number_of_captures = (const int)(captures.size());

	const bool draw_by_repetition = std::count(pos.hash_stack.begin(), pos.hash_stack.end(), pos.current_hash) >= 3;
	const bool draw_by_fifty_move_rule = pos.moves_without_pawns >= 50;
	bool is_draw = draw_by_fifty_move_rule || draw_by_repetition;
	//if the side is in check do a pv search of depth 1 
	//this should not take much extra time because checks are rare and there are only a few valid moves per check
	/*
	if (in_check) {
		return pv_search(1, alpha, beta);
	}
	*/

	if (is_draw) {
		const int eval = evaluate(is_draw, false);
		//hash_map[pos.current_hash] = TableEntrie{ eval,EXACT,0,0 };
		return eval;
	}

	if (number_of_captures == 0) {
		std::vector<int> moves{};
		pos.get_legal_moves(&moves);
		const int number_of_moves = (const int)moves.size();
		const bool no_moves_left = number_of_moves == 0;
		const bool in_check = pos.is_attacked_by(bitscan(pos.bitboards[5 + (pos.side) * 6]), !pos.side);
		const bool draw_by_stalemate = no_moves_left && (!in_check);
		is_draw = is_draw || draw_by_stalemate;
		const bool is_lost = no_moves_left && in_check;
		const int eval = evaluate(is_draw, is_lost);
		//hash_map[pos.current_hash] = TableEntrie{ eval,EXACT,0,0 };
		return eval;
	}
	int eval = evaluate(false, false);
	/*
	*/
	if (eval < alpha - 950) {
		return alpha;
	}
	if (eval > alpha) {
		if (eval >= beta) {
			return eval;
		}
		alpha = eval;
	}

	std::sort(captures.begin(), captures.end(), [&](const int& lhs, const int& rhs)
		{
			//return (pos.see(get_to_square(lhs),pos.side)) < (pos.see(get_to_square(rhs), pos.side));
			return (get_captured_type(lhs) - get_piece_type(lhs)) > (get_captured_type(rhs) - get_piece_type(rhs));
		});

	int current_best_move = -1;
	int current_best_eval = -1000000;

	for (int i = 0; i < number_of_captures; i++) {
		const int static_exchange_eval = pos.see(get_to_square(captures[i]), pos.side);
		if (static_exchange_eval<=0) break;
		if (eval < alpha - 200 - static_exchange_eval) continue;
		/*
		if ((!(pos.see(get_to_square(captures[i]), pos.side))) || ((eval < alpha - 200 - weights[get_captured_type(captures[i]) % 6]) < 0)) continue;
		*/
		pos.make_move(captures[i]);
		const int value = -quiescence(-beta, -alpha);
		pos.unmake_move();

		const bool is_new_best = (current_best_eval < value);
		current_best_eval = (is_new_best)*value + (!is_new_best) * current_best_eval;
		current_best_move = (is_new_best)*captures[i] + (!is_new_best) * current_best_move;

		if (value > alpha) {
			if (value >= beta) {
				hash_map[pos.current_hash] = TableEntrie{ value,UPPER,captures[i],0 };
				return beta;
			}
			alpha = value;
		}
	}
	hash_map[pos.current_hash] = TableEntrie{ alpha,(current_best_eval <= alphaOrigin) * LOWER,current_best_move,0 };
	return alpha;
}

int Rosy::infinite_root_call(const int depth, int alpha, int beta, std::atomic<bool>* run) {
	if (!(*run)) {
		throw(StopException());
	}
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	const double alphaOrigin = alpha;
	TableEntrie ttEntry = lookUp();
	std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	const int number_of_moves = (const int)moves.size();

	const int ply = current_desired_depth - depth;
	assert(ply < PV_TABLE_SIZE);
	pv_length[ply] = ply;

	moves = order(moves, ttEntry);
	assert(moves.size() == number_of_moves);

	int current_best_move = -1;
	int current_best_eval = -1000000;
	for (int i = 0; i < number_of_moves; i++) {
		if (!(*run)) {
			throw(StopException());
		}
		pos.make_move(moves[i]);
		int value = -infinite_pv_search(depth - 1, -beta, -alpha, run);
		pos.unmake_move();
		const bool is_best_move = (value > current_best_eval);
		current_best_move = (is_best_move)*moves[i] + (!is_best_move) * current_best_move;
		current_best_eval = (is_best_move)*value + (!is_best_move) * current_best_eval;

		if (value > alpha) {
			alpha = value;
			pv_table[ply][ply] = moves[i];
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
				pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
			}
			pv_length[ply] = pv_length[ply + 1];
			if (value == 1000000) {
				std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
				const U64 time = (U64)std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
				std::cout << "info score mate " << current_desired_depth << " depth " << depth << " nodes " << nodes << " nps " << 1000000000 * nodes / time << std::endl;
				current_eval = value;

				hash_map[pos.current_hash] = TableEntrie{ current_best_eval,EXACT,current_best_move,depth };
				return moves[i];
			}
		}
	}
	hash_map[pos.current_hash] = TableEntrie{ current_best_eval,EXACT,current_best_move,depth };
	if (current_best_eval == -1000000) {
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		const U64 time = (U64)std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		std::cout << "info score mate " << current_desired_depth << " depth " << depth << " nodes " << nodes << " nps " << 1000000000 * nodes / time << " pv ";
		current_eval = current_best_eval;
		return current_best_move;
	}
	else {
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		const U64 time = (U64)std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		std::cout << "info score cp " << current_best_eval << " depth " << depth << " nodes " << nodes << " nps " << 1000000000 * nodes / time << " pv ";
	}
	/*
	for (int i = 0; i < pv_length[0]; i++) {
		std::cout << uci(pv_table[0][i]) << " ";
	}
	*/

	int j = 0;
	for (int i = 0; i < current_desired_depth; i++) {
		TableEntrie entry = lookUp();
		if (entry.flag != EXACT) break;
		j++;
		std::cout << uci(entry.move) << " ";
		pos.make_move(entry.move);
	}
	for (int i = 0; i < j; i++) {
		pos.unmake_move();
	}
	std::cout << std::endl;


	current_eval = current_best_eval;
	return current_best_move;
}
int Rosy::infinite_pv_search(const int depth, int alpha, int beta, std::atomic<bool>* run) {
	if (!(*run)) {
		throw(StopException());
	}
	nodes++;
	/*
	* used to use >= as condition for table entry being used
	*/
	const double alphaOrigin = alpha;
	TableEntrie ttEntry = lookUp();

	const int ply = current_desired_depth - depth;
	assert(ply < PV_TABLE_SIZE);
	pv_length[ply] = ply;

	if ((ttEntry.depth > depth)) {
		if (ttEntry.flag == EXACT) {
			if (ttEntry.eval > alpha) {
				pv_table[ply][ply] = ttEntry.move;
				for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
					pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
				}
				pv_length[ply] = pv_length[ply + 1];
			}
			return ttEntry.eval;
		}
		else if ((ttEntry.flag == LOWER) && (ttEntry.eval > alpha)) {
			alpha = ttEntry.eval;
			pv_table[ply][ply] = ttEntry.move;
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
				pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
			}
			pv_length[ply] = pv_length[ply + 1];
		}
		else if (ttEntry.flag == UPPER) {
			beta = std::min(beta, ttEntry.eval);
		}
		if (alpha >= beta) {
			return ttEntry.eval;
		}
	}

	std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	const int number_of_moves = (const int)moves.size();

	const bool draw_by_repetition = std::count(pos.hash_stack.begin(), pos.hash_stack.end(), pos.current_hash) >= 3;
	const bool draw_by_fifty_move_rule = pos.moves_without_pawns >= 50;
	const bool in_check = pos.is_attacked_by(bitscan(pos.bitboards[5 + (pos.side) * 6]), !pos.side);
	const bool no_moves_left = number_of_moves == 0;
	const bool draw_by_stalemate = no_moves_left && (!in_check);
	const bool is_draw = draw_by_fifty_move_rule || draw_by_repetition || draw_by_stalemate;
	const bool is_lost = no_moves_left && in_check;

	if (is_draw || is_lost) {
		const int eval = evaluate(is_draw, is_lost);
		hash_map[pos.current_hash] = TableEntrie{ eval,EXACT,0,depth };
		return eval;
	}
	if (depth == 0) {
		return quiescence(alpha, beta);
	}
	if ((depth >= 1 + Red) && (!in_check)) {
		pos.make_nullmove();
		int nm_value = -infinite_pv_search(depth - 1 - Red, -beta, -beta + 1, run);
		pos.unmake_nullmove();
		if (nm_value >= beta) {
			return beta;
		}
	}

	moves = order(moves, ttEntry);
	assert(moves.size() == number_of_moves);

	int current_best_move = -1;
	int current_best_eval = -1000000;

	pos.make_move(moves[0]);
	bool now_in_check = pos.is_attacked_by(bitscan(pos.bitboards[K + (pos.side) * 6]), !pos.side);
	int value = -infinite_pv_search(depth - (!now_in_check), -beta, -alpha, run);
	pos.unmake_move();

	const bool is_new_best = (current_best_eval < value);
	current_best_eval = (is_new_best)*value + (!is_new_best) * current_best_eval;
	current_best_move = (is_new_best)*moves[0] + (!is_new_best) * current_best_move;

	if (value > alpha) {
		if (value >= beta) {
			const bool not_capture = !get_capture_flag(moves[0]);
			if (not_capture) {
				killer_table.push_move(moves[0], pos.turn);
				history[pos.side][(int)get_piece_type(moves[0])][(int)get_to_square(moves[0])] += (!get_capture_flag(moves[0])) * (unsigned long long)(int_power(2, depth));
			}
			if (depth >= ttEntry.depth) {
				hash_map[pos.current_hash] = TableEntrie{ value,LOWER,moves[0],depth };
			}
			return beta;
		}
		alpha = value;
		pv_table[ply][ply] = moves[0];
		for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
			pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
		}
		pv_length[ply] = pv_length[ply + 1];
	}
	const int full_depth_moves = 8;
	const int reduction_limit = 3;
	for (int i = 1; i < number_of_moves; i++) {

		pos.make_move(moves[i]);
		/*
		if ((i >= full_depth_moves) && (depth >= reduction_limit) && (!in_check)) {
			value = -pv_search(depth - 2, -alpha - 1, -alpha);
		}
		else {
			value = -pv_search(depth - 1, -alpha - 1, -alpha);
		}
		*/
		now_in_check = pos.is_attacked_by(bitscan(pos.bitboards[K + (pos.side) * 6]), !pos.side);
		value = -infinite_pv_search(depth - 1 - (!now_in_check) * ((i >= full_depth_moves) && (depth >= reduction_limit) && (!in_check)), -alpha - 1, -alpha, run);
		if ((value > alpha) && (value < beta)) {
			value = -infinite_pv_search(depth - (!now_in_check), -beta, -alpha, run);
		}
		pos.unmake_move();

		const bool is_new_best = (current_best_eval < value);
		current_best_eval = (is_new_best)*value + (!is_new_best) * current_best_eval;
		current_best_move = (is_new_best)*moves[i] + (!is_new_best) * current_best_move;

		if (value > alpha) {
			if (value >= beta) {
				const bool not_capture = !get_capture_flag(moves[i]);
				if (not_capture) {
					killer_table.push_move(moves[i], pos.turn);
					history[(pos.side)][(int)get_piece_type(moves[i])][(int)get_to_square(moves[i])] += (!get_capture_flag(moves[i])) * (unsigned long long)(int_power(2, depth));
				}
				if (depth >= ttEntry.depth) {
					hash_map[pos.current_hash] = TableEntrie{ value,LOWER,moves[i],depth };
				}
				return beta;
			}
			alpha = value;
			pv_table[ply][ply] = moves[i];
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
				pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
			}
			pv_length[ply] = pv_length[ply + 1];
		}
	}
	/*
	*/
	if (depth >= ttEntry.depth) {
		hash_map[pos.current_hash] = TableEntrie{ alpha,(current_best_eval <= alphaOrigin) * UPPER,current_best_move,depth };
	}
	return alpha;
}
int Rosy::nodes_root_call(const int depth, int alpha, int beta, std::atomic<bool>* run) {
	if (!(*run)) {
		throw(StopException());
	}
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	const double alphaOrigin = alpha;
	TableEntrie ttEntry = lookUp();
	std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	const int number_of_moves = (const int)moves.size();

	const int ply = current_desired_depth - depth;
	assert(ply < PV_TABLE_SIZE);
	pv_length[ply] = ply;

	moves = order(moves, ttEntry);
	assert(moves.size() == number_of_moves);

	int current_best_move = -1;
	int current_best_eval = -1000000;
	for (int i = 0; i < number_of_moves; i++) {
		if (!(*run)) {
			throw(StopException());
		}
		pos.make_move(moves[i]);
		int value = -nodes_pv_search(depth - 1, -beta, -alpha, run);
		pos.unmake_move();
		const bool is_best_move = (value > current_best_eval);
		current_best_move = (is_best_move)*moves[i] + (!is_best_move) * current_best_move;
		current_best_eval = (is_best_move)*value + (!is_best_move) * current_best_eval;

		if (value > alpha) {
			alpha = value;
			pv_table[ply][ply] = moves[i];
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
				pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
			}
			pv_length[ply] = pv_length[ply + 1];
			if (value == 1000000) {
				std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
				const U64 time = (U64)std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
				std::cout << "info score mate " << current_desired_depth << " depth " << depth << " nodes " << nodes << " nps " << 1000000000 * nodes / time << std::endl;
				current_eval = value;

				hash_map[pos.current_hash] = TableEntrie{ current_best_eval,EXACT,current_best_move,depth };
				return moves[i];
			}
		}
	}
	hash_map[pos.current_hash] = TableEntrie{ current_best_eval,EXACT,current_best_move,depth };
	if (current_best_eval == -1000000) {
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		const U64 time = (U64)std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		std::cout << "info score mate " << current_desired_depth << " depth " << depth << " nodes " << nodes << " nps " << 1000000000 * nodes / time << " pv ";
	}
	else {
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		const U64 time = (U64)std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		std::cout << "info score cp " << current_best_eval << " depth " << depth << " nodes " << nodes << " nps " << 1000000000 * nodes / time << " pv ";
	}
	/*
	for (int i = 0; i < pv_length[0]; i++) {
		std::cout << uci(pv_table[0][i]) << " ";
	}
	*/

	int j = 0;
	for (int i = 0; i < current_desired_depth; i++) {
		TableEntrie entry = lookUp();
		if (entry.flag != EXACT) break;
		j++;
		std::cout << uci(entry.move) << " ";
		pos.make_move(entry.move);
	}
	for (int i = 0; i < j; i++) {
		pos.unmake_move();
	}
	std::cout << std::endl;

	current_eval = current_best_eval;
	return current_best_move;
}
int Rosy::nodes_pv_search(const int depth, int alpha, int beta, std::atomic<bool>* run) {
	if (!(*run)) {
		throw(StopException());
	}
	nodes++;
	if (nodes >= max_search_nodes) {
		throw(OutOfNodesException());
	}
	/*
	* used to use >= as condition for table entry being used
	*/
	const double alphaOrigin = alpha;
	TableEntrie ttEntry = lookUp();

	const int ply = current_desired_depth - depth;
	assert(ply < PV_TABLE_SIZE);
	pv_length[ply] = ply;

	if ((ttEntry.depth > depth)) {
		if (ttEntry.flag == EXACT) {
			if (ttEntry.eval > alpha) {
				pv_table[ply][ply] = ttEntry.move;
				for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
					pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
				}
				pv_length[ply] = pv_length[ply + 1];
			}
			return ttEntry.eval;
		}
		else if ((ttEntry.flag == LOWER) && (ttEntry.eval > alpha)) {
			alpha = ttEntry.eval;
			pv_table[ply][ply] = ttEntry.move;
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
				pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
			}
			pv_length[ply] = pv_length[ply + 1];
		}
		else if (ttEntry.flag == UPPER) {
			beta = std::min(beta, ttEntry.eval);
		}
		if (alpha >= beta) {
			return ttEntry.eval;
		}
	}

	std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	const int number_of_moves = (const int)moves.size();

	const bool draw_by_repetition = std::count(pos.hash_stack.begin(), pos.hash_stack.end(), pos.current_hash) >= 3;
	const bool draw_by_fifty_move_rule = pos.moves_without_pawns >= 50;
	const bool in_check = pos.is_attacked_by(bitscan(pos.bitboards[5 + (pos.side) * 6]), !pos.side);
	const bool no_moves_left = number_of_moves == 0;
	const bool draw_by_stalemate = no_moves_left && (!in_check);
	const bool is_draw = draw_by_fifty_move_rule || draw_by_repetition || draw_by_stalemate;
	const bool is_lost = no_moves_left && in_check;

	if (is_draw || is_lost) {
		const int eval = evaluate(is_draw, is_lost);
		hash_map[pos.current_hash] = TableEntrie{ eval,EXACT,0,depth };
		return eval;
	}
	if (depth == 0) {
		return quiescence(alpha, beta);
	}
	if ((depth >= 1 + Red) && (!in_check)) {
		pos.make_nullmove();
		int nm_value = -nodes_pv_search(depth - 1 - Red, -beta, -beta + 1, run);
		pos.unmake_nullmove();
		if (nm_value >= beta) {
			return beta;
		}
	}

	moves = order(moves, ttEntry);
	assert(moves.size() == number_of_moves);

	int current_best_move = -1;
	int current_best_eval = -1000000;

	pos.make_move(moves[0]);
	bool now_in_check = pos.is_attacked_by(bitscan(pos.bitboards[K + (pos.side) * 6]), !pos.side);
	int value = -nodes_pv_search(depth - (!now_in_check), -beta, -alpha, run);
	pos.unmake_move();

	const bool is_new_best = (current_best_eval < value);
	current_best_eval = (is_new_best)*value + (!is_new_best) * current_best_eval;
	current_best_move = (is_new_best)*moves[0] + (!is_new_best) * current_best_move;

	if (value > alpha) {
		if (value >= beta) {
			const bool not_capture = !get_capture_flag(moves[0]);
			if (not_capture) {
				killer_table.push_move(moves[0], pos.turn);
				history[pos.side][(int)get_piece_type(moves[0])][(int)get_to_square(moves[0])] += (!get_capture_flag(moves[0])) * (unsigned long long)(int_power(2, depth));
			}
			if (depth >= ttEntry.depth) {
				hash_map[pos.current_hash] = TableEntrie{ value,LOWER,moves[0],depth };
			}
			return beta;
		}
		alpha = value;
		pv_table[ply][ply] = moves[0];
		for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
			pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
		}
		pv_length[ply] = pv_length[ply + 1];
	}
	const int full_depth_moves = 8;
	const int reduction_limit = 3;
	for (int i = 1; i < number_of_moves; i++) {

		pos.make_move(moves[i]);
		now_in_check = pos.is_attacked_by(bitscan(pos.bitboards[K + (pos.side) * 6]), !pos.side);
		value = -nodes_pv_search(depth - 1 - (!now_in_check) * ((i >= full_depth_moves) && (depth >= reduction_limit) && (!in_check)), -alpha - 1, -alpha, run);
		if ((value > alpha) && (value < beta)) {
			value = -nodes_pv_search(depth - (!now_in_check), -beta, -alpha, run);
		}
		pos.unmake_move();

		const bool is_new_best = (current_best_eval < value);
		current_best_eval = (is_new_best)*value + (!is_new_best) * current_best_eval;
		current_best_move = (is_new_best)*moves[i] + (!is_new_best) * current_best_move;

		if (value > alpha) {
			if (value >= beta) {
				const bool not_capture = !get_capture_flag(moves[i]);
				if (not_capture) {
					killer_table.push_move(moves[i], pos.turn);
					history[pos.side][(int)get_piece_type(moves[i])][(int)get_to_square(moves[i])] += (!get_capture_flag(moves[i])) * (unsigned long long)(int_power(2, depth));
				}
				if (depth >= ttEntry.depth) {
					hash_map[pos.current_hash] = TableEntrie{ value,LOWER,moves[i],depth };
				}
				return beta;
			}
			alpha = value;
			pv_table[ply][ply] = moves[i];
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
				pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
			}
			pv_length[ply] = pv_length[ply + 1];
		}
	}
	/*
	*/
	if (depth >= ttEntry.depth) {
		hash_map[pos.current_hash] = TableEntrie{ alpha,(current_best_eval <= alphaOrigin) * UPPER,current_best_move,depth };
	}
	return alpha;
}
int Rosy::movetime_root_call(const int depth, int alpha, int beta, std::atomic<bool>* run, std::chrono::steady_clock::time_point& search_begin) {
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	if ((((U64)std::chrono::duration_cast<std::chrono::milliseconds>(end - search_begin).count()) >= max_search_time)) {
		throw(OutOfTimeException());
	}
	if (!(*run)) {
		throw(StopException());
	}
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	const double alphaOrigin = alpha;
	TableEntrie ttEntry = lookUp();
	std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	const int number_of_moves = (const int)moves.size();

	const int ply = current_desired_depth - depth;
	assert(ply < PV_TABLE_SIZE);
	pv_length[ply] = ply;

	moves = order(moves, ttEntry);
	assert(moves.size() == number_of_moves);

	int current_best_move = -1;
	int current_best_eval = -1000000;
	for (int i = 0; i < number_of_moves; i++) {
		if (!(*run)) {
			throw(StopException());
		}
		pos.make_move(moves[i]);
		int value = -movetime_pv_search(depth - 1, -beta, -alpha, run, search_begin);
		pos.unmake_move();
		const bool is_best_move = (value > current_best_eval);
		current_best_move = (is_best_move)*moves[i] + (!is_best_move) * current_best_move;
		current_best_eval = (is_best_move)*value + (!is_best_move) * current_best_eval;

		if (value > alpha) {
			alpha = value;
			pv_table[ply][ply] = moves[i];
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
				pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
			}
			pv_length[ply] = pv_length[ply + 1];
			if (value == 1000000) {
				std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
				const U64 time = (U64)std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
				std::cout << "info score mate " << current_desired_depth << " depth " << depth << " nodes " << nodes << " nps " << 1000000000 * nodes / time << " pv ";
				current_eval = value;

				for (int i = 0; i < pv_length[0]; i++) {
					std::cout << uci(pv_table[0][i]) << " ";
				}
				std::cout << std::endl;

				hash_map[pos.current_hash] = TableEntrie{ current_best_eval,EXACT,current_best_move,depth };
				return moves[i];
			}
		}
	}
	hash_map[pos.current_hash] = TableEntrie{ current_best_eval,EXACT,current_best_move,depth };
	if (current_best_eval == -1000000) {
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		const U64 time = (U64)std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		std::cout << "info score mate " << current_desired_depth << " depth " << depth << " nodes " << nodes << " nps " << 1000000000 * nodes / time << " pv ";
	}
	else {
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		const U64 time = (U64)std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		std::cout << "info score cp " << current_best_eval << " depth " << depth << " nodes " << nodes << " nps " << 1000000000 * nodes / time << " pv ";
	}
	/*
	for (int i = 0; i < pv_length[0]; i++) {
		std::cout << uci(pv_table[0][i]) << " ";
	}
	*/
	int j = 0;
	for (int i = 0; i < current_desired_depth; i++) {
		TableEntrie entry = lookUp();
		if (entry.flag != EXACT) break;
		j++;
		std::cout << uci(entry.move) << " ";
		pos.make_move(entry.move);
	}
	for (int i = 0; i < j; i++) {
		pos.unmake_move();
	}
	std::cout << std::endl;
	current_eval = current_best_eval;

	return current_best_move;
}
int Rosy::movetime_pv_search(const int depth, int alpha, int beta, std::atomic<bool>* run, std::chrono::steady_clock::time_point& begin) {
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	if ((((U64)std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) >= max_search_time)) {
		throw(OutOfTimeException());
	}
	if (!(*run)) {
		throw(StopException());
	}
	nodes++;
	/*
	used to use >= as condition for table entry being used
	*/
	const double alphaOrigin = alpha;
	TableEntrie ttEntry = lookUp();

	const int ply = current_desired_depth - depth;
	assert(ply < PV_TABLE_SIZE);
	pv_length[ply] = ply;

	if ((ttEntry.depth > depth)) {
		if (ttEntry.flag == EXACT) {
			if (ttEntry.eval > alpha) {
				pv_table[ply][ply] = ttEntry.move;
				for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
					pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
				}
				pv_length[ply] = pv_length[ply + 1];
			}
			return ttEntry.eval;
		}
		else if ((ttEntry.flag == LOWER) && (ttEntry.eval > alpha)) {
			alpha = ttEntry.eval;
			pv_table[ply][ply] = ttEntry.move;
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
				pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
			}
			pv_length[ply] = pv_length[ply + 1];
		}
		else if (ttEntry.flag == UPPER) {
			beta = std::min(beta, ttEntry.eval);
		}
		if (alpha >= beta) {
			return ttEntry.eval;
		}
	}

	std::vector<int> moves{};
	pos.get_legal_moves(&moves);
	const int number_of_moves = (const int)moves.size();

	const bool draw_by_repetition = std::count(pos.hash_stack.begin(), pos.hash_stack.end(), pos.current_hash) >= 3;
	const bool draw_by_fifty_move_rule = pos.moves_without_pawns >= 50;
	const bool in_check = pos.is_attacked_by(bitscan(pos.bitboards[5 + (pos.side) * 6]), !pos.side);
	const bool no_moves_left = number_of_moves == 0;
	const bool draw_by_stalemate = no_moves_left && (!in_check);
	const bool is_draw = draw_by_fifty_move_rule || draw_by_repetition || draw_by_stalemate;
	const bool is_lost = no_moves_left && in_check;

	if (is_draw || is_lost) {
		const int eval = evaluate(is_draw, is_lost);
		hash_map[pos.current_hash] = TableEntrie{ eval,EXACT,0,depth };
		return eval;
	}
	if (depth == 0) {
		return quiescence(alpha, beta);
	}
	if ((depth >= 1 + Red) && (!in_check)) {
		pos.make_nullmove();
		int nm_value = -movetime_pv_search(depth - 1 - Red, -beta, -beta + 1, run, begin);
		pos.unmake_nullmove();
		if (nm_value >= beta) {
			return beta;
		}
	}

	moves = order(moves, ttEntry);
	assert(moves.size() == number_of_moves);

	int current_best_move = -1;
	int current_best_eval = -1000000;

	pos.make_move(moves[0]);
	bool now_in_check = pos.is_attacked_by(bitscan(pos.bitboards[K + (pos.side) * 6]), !pos.side);
	int value = -movetime_pv_search(depth - (!now_in_check), -beta, -alpha, run, begin);
	pos.unmake_move();

	const bool is_new_best = (current_best_eval < value);
	current_best_eval = (is_new_best)*value + (!is_new_best) * current_best_eval;
	current_best_move = (is_new_best)*moves[0] + (!is_new_best) * current_best_move;

	if (value > alpha) {
		if (value >= beta) {
			const bool not_capture = !get_capture_flag(moves[0]);
			if (not_capture) {
				killer_table.push_move(moves[0], pos.turn);
				history[pos.side][(int)get_piece_type(moves[0])][(int)get_to_square(moves[0])] += (!get_capture_flag(moves[0])) * (unsigned long long)(int_power(2, depth));
			}
			if (depth >= ttEntry.depth) {
				hash_map[pos.current_hash] = TableEntrie{ value,LOWER,moves[0],depth };
			}
			return beta;
		}
		alpha = value;
		pv_table[ply][ply] = moves[0];
		for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
			pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
		}
		pv_length[ply] = pv_length[ply + 1];
	}
	for (int i = 1; i < number_of_moves; i++) {

		pos.make_move(moves[i]);
		now_in_check = pos.is_attacked_by(bitscan(pos.bitboards[K + (pos.side) * 6]), !pos.side);
		value = -movetime_pv_search(depth - 1 - (!now_in_check) * ((i >= full_depth_moves) && (depth >= reduction_limit) && (!in_check)), -alpha - 1, -alpha, run, begin);
		if ((value > alpha) && (value < beta)) {
			value = -movetime_pv_search(depth - (!now_in_check), -beta, -alpha, run, begin);
		}
		pos.unmake_move();

		const bool is_new_best = (current_best_eval < value);
		current_best_eval = (is_new_best)*value + (!is_new_best) * current_best_eval;
		current_best_move = (is_new_best)*moves[i] + (!is_new_best) * current_best_move;

		if (value > alpha) {
			if (value >= beta) {
				const bool not_capture = !get_capture_flag(moves[i]);
				if (not_capture) {
					killer_table.push_move(moves[i], pos.turn);
					history[pos.side][(int)get_piece_type(moves[i])][(int)get_to_square(moves[i])] += (!get_capture_flag(moves[i])) * (unsigned long long)(int_power(2, depth));
				}
				if (depth >= ttEntry.depth) {
					hash_map[pos.current_hash] = TableEntrie{ value,LOWER,moves[i],depth };
				}
				return beta;
			}
			alpha = value;
			pv_table[ply][ply] = moves[i];
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
				pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
			}
			pv_length[ply] = pv_length[ply + 1];
		}
	}
	/*
	*/
	if (depth >= ttEntry.depth) {
		hash_map[pos.current_hash] = TableEntrie{ alpha,(current_best_eval <= alphaOrigin) * UPPER,current_best_move,depth };
	}
	return alpha;
}

TableEntrie Rosy::lookUp() {
	auto yield = hash_map.find(pos.current_hash);
	if (yield != hash_map.end()) {
		return yield->second;
	}
	return TableEntrie { 0,0,0,-9999 };
}
std::vector<int> Rosy::order(std::vector<int>& moves,TableEntrie& entry) {
	std::vector<int> ret{};
	ret.reserve(64);
	int hash_move = 0;
	if (((bool)(entry.move)) && (std::find(moves.begin(), moves.end(), entry.move) != moves.end())) {
		hash_move = entry.move;
	}
	std::vector<int> killers{};
	killers.reserve(32);
	std::vector<int> capture{};
	capture.reserve(32);
	std::vector<int> other{};
	other.reserve(32);
	for (int i = 0; i < moves.size(); i++) {
		assert(i >= 0);
		assert(i < moves.size());
		if (moves[i] == hash_move) {
			ret.push_back(entry.move);
		}
		else if (killer_table.find(moves[i], pos.turn)) {
			killers.push_back(moves[i]);
		}
		else if (get_capture_flag(moves[i])) {
			capture.push_back(moves[i]);
			continue;
		}
		else {
			other.push_back(moves[i]);
		}
	}
	std::sort(capture.begin(), capture.end(), [&](const int& lhs, const int& rhs)
		{
			//return (pos.see(get_to_square(lhs), pos.side)) < (pos.see(get_to_square(rhs), pos.side));
			return (get_captured_type(lhs) - get_piece_type(lhs)) > (get_captured_type(rhs) - get_piece_type(rhs));
		});
	ret.insert(ret.end(), capture.begin(), capture.end());
	ret.insert(ret.end(), killers.begin(), killers.end());
	std::sort(other.begin(), other.end(), [&](const int& lhs, const int& rhs)
		{
		return (history[(size_t)(pos.side)][(size_t)(get_piece_type(lhs))][(size_t)(get_to_square(lhs))] > history[(size_t)(pos.side)][(size_t)(get_piece_type(rhs))][(size_t)(get_to_square(rhs))]);
		});
	ret.insert(ret.end(), other.begin(), other.end());
	return ret;
}

inline int Rosy::evaluate(const bool is_draw, const bool is_lost) {
	const int phase = get_phase();
	const int sign = (!pos.side) - (pos.side);
	const bool is_endgame = (phase < 30);
	const int endgame_kingpos = (is_endgame) * (endgameKingTable[bitscan(pos.bitboards[5])] - endgameKingTable[bitscan(pos.bitboards[11])]);
	const int ret = sign * (raw_material((is_endgame)) + endgame_kingpos + (!is_endgame) * king_pawn_shield() + king_attack_zones() + pawn_eval() + get_white_mobility_conectivity(pos) - get_black_mobility_conectivity(pos));
	return (!is_draw) * ((is_lost) * (-1000000) + (!(is_draw || is_lost)) * ret);
}
inline int Rosy::get_phase() {
	int ret = count_bits(pos.bitboards[P] | pos.bitboards[p]);
	ret += 3 * count_bits(pos.bitboards[N] | pos.bitboards[n] | pos.bitboards[B] | pos.bitboards[b]);
	ret += 5 * count_bits(pos.bitboards[R] | pos.bitboards[r]);
	ret += 9 * count_bits(pos.bitboards[Q] | pos.bitboards[q]);
	return ret;
}
inline int Rosy::raw_material(const bool is_endgame) {
	int ret = openingKingTableWhite[bitscan(pos.bitboards[K])] - openingKingTableBlack[bitscan(pos.bitboards[k])];
	U64 whitePawns = pos.bitboards[P];
	while (whitePawns) {
		const U64 isolated = whitePawns & twos_complement(whitePawns);
		const int ind = bitscan(isolated);
		ret += weights[P] + (!is_endgame) * openingPawnTableWhite[ind] + (is_endgame)*endgamePawnTableWhite[ind];
		whitePawns &= ones_decrement(whitePawns);
	}
	U64 blackPawns = pos.bitboards[p];
	while (blackPawns) {
		const U64 isolated = blackPawns & twos_complement(blackPawns);
		const int ind = bitscan(isolated);
		ret -= weights[P] + (!is_endgame) * openingPawnTableBlack[ind] + (is_endgame) * endgamePawnTableBlack[ind];
		blackPawns &= ones_decrement(blackPawns);
	}

	U64 whiteKnights = pos.bitboards[N];
	while (whiteKnights) {
		const U64 isolated = whiteKnights & twos_complement(whiteKnights);
		ret += weights[N] + openingKnightsTable[bitscan(isolated)];
		whiteKnights &= ones_decrement(whiteKnights);
	}
	U64 blackKnights = pos.bitboards[n];
	while (blackKnights) {
		const U64 isolated = blackKnights & twos_complement(blackKnights);
		ret -= weights[N] + openingKnightsTable[bitscan(isolated)];
		blackKnights &= ones_decrement(blackKnights);
	}

	U64 whiteBishops = pos.bitboards[B];
	while (whiteBishops) {
		const U64 isolated = whiteBishops & twos_complement(whiteBishops);
		ret += weights[B] + openingBishopTableWhite[bitscan(isolated)];
		whiteBishops &= ones_decrement(whiteBishops);
	}
	U64 blackBishops = pos.bitboards[b];
	while (blackBishops) {
		const U64 isolated = blackBishops & twos_complement(blackBishops);
		ret -= weights[B] + openingBishopTableBlack[bitscan(isolated)];
		blackBishops &= ones_decrement(blackBishops);
	}

	U64 whiteRooks = pos.bitboards[R];
	while (whiteRooks) {
		const U64 isolated = whiteRooks & twos_complement(whiteRooks);
		ret += weights[R] + openingRookTableWhite[bitscan(isolated)];
		whiteRooks &= ones_decrement(whiteRooks);
	}
	U64 blackRooks = pos.bitboards[r];
	while (blackRooks) {
		const U64 isolated = blackRooks & twos_complement(blackRooks);
		ret -= weights[R] + openingQueenTableBlack[bitscan(isolated)];
		blackRooks &= ones_decrement(blackRooks);
	}

	U64 whiteQueens = pos.bitboards[Q];
	while (whiteQueens) {
		const U64 isolated = whiteQueens & twos_complement(whiteQueens);
		ret += weights[Q] + openingQueenTableWhite[bitscan(isolated)];
		whiteQueens &= ones_decrement(whiteQueens);
	}
	U64 blackQueens = pos.bitboards[q];
	while (blackQueens) {
		const U64 isolated = blackQueens & twos_complement(blackQueens);
		ret -= weights[Q] + openingQueenTableBlack[bitscan(isolated)];
		blackQueens &= ones_decrement(blackQueens);
	}
	return ret;
}
inline int Rosy::king_attack_zones() {
	U64 black_king_zone = blackKingZones[bitscan(pos.bitboards[k])];

	int table_index = 0;
	U64 tempKnights = pos.bitboards[N];
	while (tempKnights) {
		U64 isolated = tempKnights & twos_complement(tempKnights);
		table_index += weights[5] * count_bits(knight_attacks[bitscan(isolated)] & black_king_zone);
		tempKnights = tempKnights & ones_decrement(tempKnights);
	}
	U64 tempBishops = pos.bitboards[B];
	while (tempBishops) {
		U64 isolated = tempBishops & twos_complement(tempBishops);
		table_index += weights[6] * count_bits(get_bishop_attacks(pos.occupancies[both], bitscan(isolated)) & black_king_zone);
		tempBishops = tempBishops & ones_decrement(tempBishops);
	}
	U64 tempRooks = pos.bitboards[R];
	while (tempRooks) {
		U64 isolated = tempRooks & twos_complement(tempRooks);
		table_index += weights[7] * count_bits(get_bishop_attacks(pos.occupancies[both], bitscan(isolated)) & black_king_zone);
		tempRooks = tempRooks & ones_decrement(tempRooks);
	}
	U64 tempQueens = pos.bitboards[Q];
	while (tempQueens) {
		U64 isolated = tempQueens & twos_complement(tempQueens);
		const int ind = bitscan(isolated);
		table_index += weights[8] * count_bits((get_bishop_attacks(pos.occupancies[both], ind) | get_bishop_attacks(pos.occupancies[both], ind)) & black_king_zone);
		tempQueens = tempQueens & ones_decrement(tempQueens);
	}

	int ret = SafetyTable[table_index];
	
	U64 white_king_zone = whiteKingZones[bitscan(pos.bitboards[K])];
	table_index = 0;
	tempKnights = pos.bitboards[n];
	while (tempKnights) {
		U64 isolated = tempKnights & twos_complement(tempKnights);
		table_index += weights[5] * count_bits(knight_attacks[bitscan(isolated)] & white_king_zone);
		tempKnights = tempKnights & ones_decrement(tempKnights);
	}
	tempBishops = pos.bitboards[b];
	while (tempBishops) {
		U64 isolated = tempBishops & twos_complement(tempBishops);
		table_index += weights[6] * count_bits(get_bishop_attacks(pos.occupancies[both], bitscan(isolated)) & white_king_zone);
		tempBishops = tempBishops & ones_decrement(tempBishops);
	}
	tempRooks = pos.bitboards[r];
	while (tempRooks) {
		U64 isolated = tempRooks & twos_complement(tempRooks);
		table_index += weights[7] * count_bits(get_rook_attacks(pos.occupancies[both], bitscan(isolated)) & white_king_zone);
		tempRooks = tempRooks & ones_decrement(tempRooks);
	}
	tempQueens = pos.bitboards[q];
	while (tempQueens) {
		U64 isolated = tempQueens & twos_complement(tempQueens);
		const int ind = bitscan(isolated);
		table_index += weights[8] * count_bits((get_bishop_attacks(pos.occupancies[both], ind) | get_bishop_attacks(pos.occupancies[both], ind)) & white_king_zone);
		tempQueens = tempQueens & ones_decrement(tempQueens);
	}

	return ret - SafetyTable[table_index];
}
inline int Rosy::king_pawn_shield() {
	/*
    ((black_queenside_shield >> 8) | (black_queenside_shield >> 13)) & 0x3F;
    ((black_kingside_shield >> 13) | (black_kingside_shield >> 18)) & 0x3F;
    ((white_queenside_shield >> 40) | (white_queenside_shield >> 45)) & 0x3F;
    ((white_kingside_shield >> 45) | (white_kingside_shield >> 50)) & 0x3F;
	later use these as indexes for precalculated tables which hold the bonuses for the corresponding pawn formations
	*/
	const U64 black_queenside_shield = 0x70700ULL;
	const U64 black_kingside_shield = 0xE0E000ULL;
	const U64 white_queenside_shield = 0x7070000000000ULL;
	const U64 white_kingside_shield = 0xE0E00000000000ULL;
	const bool white_queenside = (bool)(pos.bitboards[5] & 0x700000000000000ULL);
	const bool white_kingside = (bool)(pos.bitboards[5] & 0xE000000000000000ULL);
	const bool black_queenside = (bool)(pos.bitboards[5] & 7ULL);
	const bool black_kingside = (bool)(pos.bitboards[5] & 0xE0ULL);
	return weights[9] * ((white_queenside)*count_bits(white_queenside_shield & pos.bitboards[0]) + (white_kingside)*count_bits(white_kingside_shield & pos.bitboards[0]) - (black_queenside)*count_bits(black_queenside_shield & pos.bitboards[6]) - (black_kingside)*count_bits(black_kingside_shield & pos.bitboards[6]));
}
inline int Rosy::pawn_chains() {
	U64 white = pos.bitboards[0];
	int ret = 0;
	while (white) {
		U64 isolated = white & twos_complement(white);
		U64 isolated2 = isolated;
		int root = 1 + (int)floor(bitscan(isolated) / 8);
		isolated |= ((isolated << 9) & (white & (notAFile)));
		isolated |= ((isolated << 9) & (white & (notAFile)));
		isolated |= ((isolated << 9) & (white & (notAFile)));
		isolated |= ((isolated << 9) & (white & (notAFile)));
		isolated |= ((isolated << 9) & (white & (notAFile)));
		ret += root * (count_bits(isolated) - 1);
		isolated2 |= ((isolated2 << 7) & (white & (notHFile)));
		isolated2 |= ((isolated2 << 7) & (white & (notHFile)));
		isolated2 |= ((isolated2 << 7) & (white & (notHFile)));
		isolated2 |= ((isolated2 << 7) & (white & (notHFile)));
		isolated2 |= ((isolated2 << 7) & (white & (notHFile)));
		ret += root * (count_bits(isolated2) - 1);
		white &= (~isolated & ~isolated2);
	}
	U64 black = pos.bitboards[6];
	while (black) {
		U64 isolated = black & twos_complement(black);
		U64 isolated2 = isolated;
		int root = 1 + (int)abs(floor(bitscan(isolated) / 8) - 7);
		isolated |= ((isolated >> 9) & (black & (notHFile)));
		isolated |= ((isolated >> 9) & (black & (notHFile)));
		isolated |= ((isolated >> 9) & (black & (notHFile)));
		isolated |= ((isolated >> 9) & (black & (notHFile)));
		isolated |= ((isolated >> 9) & (black & (notHFile)));
		ret -= root * (count_bits(isolated) - 1);
		isolated2 |= ((isolated2 >> 7) & (black & (notAFile)));
		isolated2 |= ((isolated2 >> 7) & (black & (notAFile)));
		isolated2 |= ((isolated2 >> 7) & (black & (notAFile)));
		isolated2 |= ((isolated2 >> 7) & (black & (notAFile)));
		isolated2 |= ((isolated2 >> 7) & (black & (notAFile)));
		ret -= root * (count_bits(isolated2) - 1);
		black &= (~isolated & ~isolated2);
	}
	return weights[10] * ret;
}
inline int Rosy::passed_pawns() {
	/*
	U64 black = pos.bitboards[6];
	U64 white = pos.bitboards[0];
	int ret = ((count_bits(white & hFile) != 0) && (count_bits(black & (hFile | gFile)) == 0)) - ((count_bits(black & hFile) != 0) && (count_bits(white & (hFile | gFile)) == 0));
	ret += ((count_bits(white & gFile) != 0) && (count_bits(black & (hFile | gFile | fFile)) == 0)) - ((count_bits(black & gFile) != 0) && (count_bits(white & (hFile | gFile | fFile)) == 0));
	ret += ((count_bits(white & fFile) != 0) && (count_bits(black & (gFile | fFile | eFile)) == 0)) - ((count_bits(black & fFile) != 0) && (count_bits(white & (gFile | fFile | eFile)) == 0));
	ret += ((count_bits(white & eFile) != 0) && (count_bits(black & (fFile | eFile | dFile)) == 0)) - ((count_bits(black & eFile) != 0) && (count_bits(white & (fFile | eFile | dFile)) == 0));
	ret += ((count_bits(white & dFile) != 0) && (count_bits(black & (eFile | dFile | cFile)) == 0)) - ((count_bits(black & dFile) != 0) && (count_bits(white & (eFile | dFile | cFile)) == 0));
	ret += ((count_bits(white & cFile) != 0) && (count_bits(black & (bFile | cFile | dFile)) == 0)) - ((count_bits(black & cFile) != 0) && (count_bits(white & (bFile | cFile | dFile)) == 0));
	ret += ((count_bits(white & bFile) != 0) && (count_bits(black & (cFile | bFile | aFile)) == 0)) - ((count_bits(black & bFile) != 0) && (count_bits(white & (cFile | bFile | aFile)) == 0));
	ret += ((count_bits(white & aFile) != 0) && (count_bits(black & (aFile | bFile)) == 0)) - ((count_bits(black & aFile) != 0) && (count_bits(white & (aFile | bFile)) == 0));*/
	int ret = 0;
	U64 tempPawns = pos.bitboards[0];
	while (tempPawns) {
		U64 isolated = tempPawns & twos_complement(tempPawns);
		ret += (!((bool)(passed_pawn_masks[0][bitscan(isolated)] & pos.bitboards[6])));
		tempPawns = tempPawns & ones_decrement(tempPawns);
	}
	tempPawns = pos.bitboards[6];
	while (tempPawns) {
		U64 isolated = tempPawns & twos_complement(tempPawns);
		ret -= (!((bool)(passed_pawn_masks[1][bitscan(isolated)] & pos.bitboards[0])));
		tempPawns = tempPawns & ones_decrement(tempPawns);
	}
	return weights[11] * ret;
}
inline int Rosy::isolated_pawns() {
	int ret = 0;
	U64 tempPawns = pos.bitboards[0];
	while (tempPawns) {
		U64 isolated = tempPawns & twos_complement(tempPawns);
		ret -= ((neighbour_pawn_masks[bitscan(isolated)%8] & pos.bitboards[0])==0);
		tempPawns = tempPawns & ones_decrement(tempPawns);
	}
	tempPawns = pos.bitboards[6];
	while (tempPawns) {
		U64 isolated = tempPawns & twos_complement(tempPawns);
		ret += ((neighbour_pawn_masks[bitscan(isolated) % 8] & pos.bitboards[6]) == 0);
		tempPawns = tempPawns & ones_decrement(tempPawns);
	}
	return weights[12] * ret;
}
inline int Rosy::doubledPawns() {
	int ret = 0;
	U64 whitePawns = pos.bitboards[P];
	while (whitePawns) {
		const U64 isolated = whitePawns & twos_complement(whitePawns);
		const int sq = bitscan(isolated);
		ret -= count_bits(whitePawns & doubled_pawn_masks[sq]);
		whitePawns = whitePawns & (~doubled_pawn_reset_masks[sq]);
	}
	U64 blackPawns = pos.bitboards[p];
	while (blackPawns) {
		const U64 isolated = blackPawns & twos_complement(blackPawns);
		const int sq = bitscan(isolated);
		ret += count_bits(blackPawns & doubled_pawn_masks[sq]);
		blackPawns = blackPawns & (~doubled_pawn_reset_masks[sq]);
	}
	return weights[13] * ret;
}