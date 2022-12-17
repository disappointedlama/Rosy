#pragma once
#include "position.h"
#include "openingBook.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>
#include <thread>

#include <direct.h>
#include <limits.h>
constexpr int LOWER_OPENING_LIMIT = 70;
constexpr int UPPER_ENDGAME_LIMIT = 30;
namespace std {
	string getCurDir();
}
static inline int weighing_fkt(const int phase, const int opening_score, const int endgame_score) {
	return (int)std::floor(((double)(opening_score*phase + endgame_score * (LOWER_OPENING_LIMIT - phase)))/(double)LOWER_OPENING_LIMIT);
}
static void set_piece_square_table (std::array<std::array<std::array<int, 64>, 12>, 206>&arr) {
	std::array<std::array<int, 64>, 12> opening_table = {
		std::array<int,64>{
			0,  0,  0,  0,  0,  0,  0,  0,
			50, 50, 50, 50, 50, 50, 50, 50,
			10, 10, 20, 30, 30, 20, 10, 10,
			5,  5, 10, 25, 25, 10,  5,  5,
			0,  0,  0, 20, 20,  0,  0,  0,
			5, -5,-10,  0,  0,-10, -5,  5,
			5, 10, 10,-20,-20, 10, 10,  5,
			0,  0,  0,  0,  0,  0,  0,  0
		},
		std::array<int,64>{
			-50,-40,-30,-30,-30,-30,-40,-50,
			-40,-20,0,0,0,0,-20,-40,
			-30,0,10,15,15,10,0,-30,
			-30,0,15,20,20,15,0,-30,
			-30,0,15,20,20,15,0,-30,
			-30,0,10,15,15,10,0,-30,
			-40,-20,0,0,0,0,-20,-40,
			-50,-30,-30,-30,-30,-30,-40,-50,
		},
		std::array<int,64>{
			-20,-10,-10,-10,-10,-10,-10,-20,
			-10,  0,  0,  0,  0,  0,  0,-10,
			-10,  0,  5, 10, 10,  5,  0,-10,
			-10,  5,  5, 10, 10,  5,  5,-10,
			-10,  0, 10, 10, 10, 10,  0,-10,
			-10, 10, 10, 10, 10, 10, 10,-10,
			-10,  5,  0,  0,  0,  0,  5,-10,
			-20,-10,-10,-10,-10,-10,-10,-20
		},
		std::array<int,64>{
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,5,5,5,5,0,0,
			10,0,20,30,30,20,0,10,
		},
		std::array<int,64>{
			-20,-10,-10, -5, -5,-10,-10,-20,
			-10,  0,  0,  0,  0,  0,  0,-10,
			-10,  0,  5,  5,  5,  5,  0,-10,
			 -5,  0,  5,  5,  5,  5,  0, -5,
			  0,  0,  5,  5,  5,  5,  0, -5,
			-10,  5,  5,  5,  5,  5,  0,-10,
			-10,  0,  5,  0,  0,  0,  0,-10,
			-20,-10,-10, -5, -5,-10,-10,-20
		},
		std::array<int,64>{
			-10,-10,-10,-10,-10,-10,-10,-10,
			-10,-10,-10,-10,-10,-10,-10,-10,
			-10,-10,-10,-10,-10,-10,-10,-10,
			-10,-10,-10,-10,-10,-10,-10,-10,
			-10,-10,-10,-10,-10,-10,-10,-10,
			-10,-10,-10,-10,-10,-10,-10,-10,
			0,2,0,0,0,0,2,0,
			7,20,10,0,0,0,20,7,
		},
		//############################//
		//############################//
		std::array<int,64>{
			0,  0,  0,  0,  0,  0,  0,  0,
			5, 10, 10,-20,-20, 10, 10,  5,
			5, -5,-10,  0,  0,-10, -5,  5,
			0,  0,  0, 20, 20,  0,  0,  0,
			5,  5, 10, 25, 25, 10,  5,  5,
			10, 10, 20, 30, 30, 20, 10, 10,
			50, 50, 50, 50, 50, 50, 50, 50,
			0,  0,  0,  0,  0,  0,  0,  0
		},
		std::array<int,64>{
			-50,-40,-30,-30,-30,-30,-40,-50,
			-40,-20,0,0,0,0,-20,-40,
			-30,0,10,15,15,10,0,-30,
			-30,0,15,20,20,15,0,-30,
			-30,0,15,20,20,15,0,-30,
			-30,0,10,15,15,10,0,-30,
			-40,-20,0,0,0,0,-20,-40,
			-50,-30,-30,-30,-30,-30,-40,-50,
		},
		std::array<int,64>{
			-20,-10,-10,-10,-10,-10,-10,-20
			- 10,  5,  0,  0,  0,  0,  5,-10,
			-10, 10, 10, 10, 10, 10, 10,-10,
			-10,  0, 10, 10, 10, 10,  0,-10,
			-10,  5,  5, 10, 10,  5,  5,-10,
			-10,  0,  5, 10, 10,  5,  0,-10,
			-10,  0,  0,  0,  0,  0,  0,-10,
			-20,-10,-10,-10,-10,-10,-10,-20,
		},
		std::array<int,64>{
			10,0,20,30,30,20,0,10,
			0,0,5,5,5,5,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
		},
		std::array<int, 64>{
			-20,-10,-10, -5, -5,-10,-10,-20,
			-10,  0,  5,  0,  0,  0,  0,-10,
			-10,  0,  5,  5,  5,  5,  5,-10,
			-10,  0,  5,  5,  5,  5,  0,-10,
			 -5,  0,  5,  5,  5,  5,  0, -5,
			  0,  0,  5,  5,  5,  5,  0, -5,
			-10,  0,  0,  0,  0,  0,  0,-10,
			-20,-10,-10, -5, -5,-10,-10,-20
		},
		std::array<int,64>{
		7,20,10,0,0,0,20,7,
		0,2,0,0,0,0,2,0,
		-10,-10,-10,-10,-10,-10,-10,-10,
		-10,-10,-10,-10,-10,-10,-10,-10,
		-10,-10,-10,-10,-10,-10,-10,-10,
		-10,-10,-10,-10,-10,-10,-10,-10,
		-10,-10,-10,-10,-10,-10,-10,-10,
		-10,-10,-10,-10,-10,-10,-10,-10,
		}
	};
	std::array<std::array<int, 64>, 12> endgame_table = {
		std::array<int, 64>{
			0,0,0,0,0,0,0,0,
			100,100,100,100,100,100,100,100,
			80,80,80,80,80,80,80,80,
			60,60,60,60,60,60,60,60,
			40,40,40,40,40,40,40,40,
			20,20,20,20,20,20,20,20,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
		},
		std::array<int,64>{
			-50,-40,-30,-30,-30,-30,-40,-50,
			-40,-20,0,0,0,0,-20,-40,
			-30,0,10,15,15,10,0,-30,
			-30,0,15,20,20,15,0,-30,
			-30,0,15,20,20,15,0,-30,
			-30,0,10,15,15,10,0,-30,
			-40,-20,0,0,0,0,-20,-40,
			-50,-30,-30,-30,-30,-30,-40,-50,
		},
		std::array<int,64>{
			-20,-10,-10,-10,-10,-10,-10,-20,
			-10,  0,  0,  0,  0,  0,  0,-10,
			-10,  0,  5, 10, 10,  5,  0,-10,
			-10,  5,  5, 10, 10,  5,  5,-10,
			-10,  0, 10, 10, 10, 10,  0,-10,
			-10, 10, 10, 10, 10, 10, 10,-10,
			-10,  5,  0,  0,  0,  0,  5,-10,
			-20,-10,-10,-10,-10,-10,-10,-20
		},
		std::array<int, 64>{
			0,0,0,0,0,0,0,0,
			20,20,20,20,20,20,20,20,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
		},
		std::array<int,64>{
			-20,-10,-10, -5, -5,-10,-10,-20,
			-10,  0,  0,  0,  0,  0,  0,-10,
			-10,  0,  5,  5,  5,  5,  0,-10,
			 -5,  0,  5,  5,  5,  5,  0, -5,
			  0,  0,  5,  5,  5,  5,  0, -5,
			-10,  5,  5,  5,  5,  5,  0,-10,
			-10,  0,  5,  0,  0,  0,  0,-10,
			-20,-10,-10, -5, -5,-10,-10,-20
		},
		std::array<int, 64>{
		-5,-3,-3,-3,-3,-3,-3,-5
		-3,-1,-1,-1,-1,-1,-1,-3,
		-3,-1,2,2,2,2,-1,-3,
		-3,-1,2,2,2,2,-1,-3,
		-3,-1,2,2,2,2,-1,-3,
		-3,-1,2,2,2,2,-1,-3,
		-3,-1,-1,-1,-1,-1,-1,-3,
		-5,-3,-3,-3,-3,-3,-3,-5
		},
		//############################//
		//############################//
		std::array<int, 64>{
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		20,20,20,20,20,20,20,20,
		40,40,40,40,40,40,40,40,
		60,60,60,60,60,60,60,60,
		80,80,80,80,80,80,80,80,
		100,100,100,100,100,100,100,100,
		0,0,0,0,0,0,0,0,
		},
		std::array<int,64>{
			-50,-40,-30,-30,-30,-30,-40,-50,
			-40,-20,0,0,0,0,-20,-40,
			-30,0,10,15,15,10,0,-30,
			-30,0,15,20,20,15,0,-30,
			-30,0,15,20,20,15,0,-30,
			-30,0,10,15,15,10,0,-30,
			-40,-20,0,0,0,0,-20,-40,
			-50,-30,-30,-30,-30,-30,-40,-50,
		},
		std::array<int,64>{
			-20,-10,-10,-10,-10,-10,-10,-20
			- 10,  5,  0,  0,  0,  0,  5,-10,
			-10, 10, 10, 10, 10, 10, 10,-10,
			-10,  0, 10, 10, 10, 10,  0,-10,
			-10,  5,  5, 10, 10,  5,  5,-10,
			-10,  0,  5, 10, 10,  5,  0,-10,
			-10,  0,  0,  0,  0,  0,  0,-10,
			-20,-10,-10,-10,-10,-10,-10,-20,
		},
		std::array<int, 64>{
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			20,20,20,20,20,20,20,20,
			0,0,0,0,0,0,0,0,
		},
		std::array<int, 64>{
			-20,-10,-10, -5, -5,-10,-10,-20,
			-10,  0,  5,  0,  0,  0,  0,-10,
			-10,  0,  5,  5,  5,  5,  5,-10,
			-10,  0,  5,  5,  5,  5,  0,-10,
			 -5,  0,  5,  5,  5,  5,  0, -5,
			  0,  0,  5,  5,  5,  5,  0, -5,
			-10,  0,  0,  0,  0,  0,  0,-10,
			-20,-10,-10, -5, -5,-10,-10,-20
		},
		std::array<int, 64>{
		-5,-3,-3,-3,-3,-3,-3,-5
		-3,-1,-1,-1,-1,-1,-1,-3,
		-3,-1,2,2,2,2,-1,-3,
		-3,-1,2,2,2,2,-1,-3,
		-3,-1,2,2,2,2,-1,-3,
		-3,-1,2,2,2,2,-1,-3,
		-3,-1,-1,-1,-1,-1,-1,-3,
		-5,-3,-3,-3,-3,-3,-3,-5
		}
	};
	for (int phase = 0; phase < 206; phase++) {
		for (int piece = 0; piece < 12; piece++) {
			for (int square = 0; square < 64; square++) {
				if (phase > LOWER_OPENING_LIMIT) {
					arr[phase][piece][square] = opening_table[piece][square];
				}
				else if (phase < UPPER_ENDGAME_LIMIT) {
					arr[phase][piece][square] = endgame_table[piece][square];
				}
				else {
					arr[phase][piece][square] = weighing_fkt(phase, opening_table[piece][square], endgame_table[piece][square]);
				}
			}
		}
	}
}
static void print_piece_square_table (std::array<std::array<std::array<int, 64>, 12>, 206>&arr) {
	std::cout << "{\n";
	for (int phase = 0; phase < 206; phase++) {
		std::cout << "{\n";
		for (int piece = 0; piece < 12; piece++) {
			std::cout << "{";
			for (int square = 0; square < 64; square++) {
				if (phase > LOWER_OPENING_LIMIT) {
					std::cout<< arr[phase][piece][square];
				}
				else if (phase < UPPER_ENDGAME_LIMIT) {
					std::cout << arr[phase][piece][square];
				}
				else {
					std::cout << arr[phase][piece][square];
				}
				if (square != 63) std::cout << ", ";
			}
			if (piece != 11) std::cout << "},\n";
			else std::cout << "}\n";
		}
		if (phase != 205) std::cout << "},\n";
		else std::cout << "}\n";
	}
	std::cout << "}\n";
}
static std::array<std::array<std::array<int, 64>, 12>, 206> piece_square_table{};
static constexpr int EXACT = 0;
static constexpr int UPPER = 1;
static constexpr int LOWER = 2;
static constexpr int Red = 1;
static constexpr int PV_TABLE_SIZE = 64;

static constexpr int full_depth_moves = 8;
static constexpr int reduction_limit = 3;

static constexpr int SafetyTable[100] = {
	0,  0,   1,   2,   3,   5,   7,   9,  12,  15,
  18,  22,  26,  30,  35,  39,  44,  50,  56,  62,
  68,  75,  82,  85,  89,  97, 105, 113, 122, 131,
 140, 150, 169, 180, 191, 202, 213, 225, 237, 248,
 260, 272, 283, 295, 307, 319, 330, 342, 354, 366,
 377, 389, 401, 412, 424, 436, 448, 459, 471, 483,
 494, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500
};
static constexpr U64 blackKingZones[64] = {
	50529026,117901061,235802122,471604244,943208488,1886416976,3772833952,3233857600,
	12935430915,30182672135,60365344270,120730688540,241461377080,482922754160,965845508320,827867578560,
	3311470314240,7726764066560,15453528133120,30907056266240,61814112532480,123628225064960,247256450129920,211934100111360,
	847736400445440,1978051601039360,3956103202078720,7912206404157440,15824412808314880,31648825616629760,63297651233259520,54255129628508160,
	217020518514032640,506381209866076160,1012762419732152320,2025524839464304640,4051049678928609280,8102099357857218560,16204198715714437120,13889313184898088960,
	217020518463700992,506381209748635648,1012762419497271296,2025524838994542592,4051049677989085184,8102099355978170368,16204198711956340736,13889313181676863488,
	217020505578799104,506381179683864576,1012762359367729152,2025524718735458304,4051049437470916608,8102098874941833216,16204197749883666432,13889312357043142656,
	217017207043915776,506373483102470144,1012746966204940288,2025493932409880576,4050987864819761152,8101975729639522304,16203951459279044608,13889101250810609664
};
static constexpr U64 whiteKingZones[64] = {
	771,1799,3598,7196,14392,28784,57568,49344,
	197379,460551,921102,1842204,3684408,7368816,14737632,12632256,
	50529027,117901063,235802126,471604252,943208504,1886417008,3772834016,3233857728,
	12935430915,30182672135,60365344270,120730688540,241461377080,482922754160,965845508320,827867578560,
	3311470314240,7726764066560,15453528133120,30907056266240,61814112532480,123628225064960,247256450129920,211934100111360,
	847736400445440,1978051601039360,3956103202078720,7912206404157440,15824412808314880,31648825616629760,63297651233259520,54255129628508160,
	217020518514032640,506381209866076160,1012762419732152320,2025524839464304640,4051049678928609280,8102099357857218560,16204198715714437120,13889313184898088960,
	144962924425773056,362266021672779776,724532043345559552,1449064086691119104,2898128173382238208,5796256346764476416,11592512693528952832,4665941144822087680
};

static constexpr int openingKingTableBlack[64] = {
	7,20,10,0,0,0,20,7,
	0,2,0,0,0,0,2,0,
	-10,-10,-10,-10,-10,-10,-10,-10,
	-10,-10,-10,-10,-10,-10,-10,-10,
	-10,-10,-10,-10,-10,-10,-10,-10,
	-10,-10,-10,-10,-10,-10,-10,-10,
	-10,-10,-10,-10,-10,-10,-10,-10,
	-10,-10,-10,-10,-10,-10,-10,-10,
};
static constexpr int openingKingTableWhite[64] = {
	-10,-10,-10,-10,-10,-10,-10,-10,
	-10,-10,-10,-10,-10,-10,-10,-10,
	-10,-10,-10,-10,-10,-10,-10,-10,
	-10,-10,-10,-10,-10,-10,-10,-10,
	-10,-10,-10,-10,-10,-10,-10,-10,
	-10,-10,-10,-10,-10,-10,-10,-10,
	0,2,0,0,0,0,2,0,
	7,20,10,0,0,0,20,7,
};
static constexpr int openingRookTableWhite[64] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,5,5,5,5,0,0,
	10,0,20,30,30,20,0,10,
};
static constexpr int openingRookTableBlack[64] = {
	10,0,20,30,30,20,0,10,
	0,0,5,5,5,5,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
};
static constexpr int openingKnightsTable[64] = {
	-50,-40,-30,-30,-30,-30,-40,-50,
	-40,-20,0,0,0,0,-20,-40,
	-30,0,10,15,15,10,0,-30,
	-30,0,15,20,20,15,0,-30,
	-30,0,15,20,20,15,0,-30,
	-30,0,10,15,15,10,0,-30,
	-40,-20,0,0,0,0,-20,-40,
	-50,-30,-30,-30,-30,-30,-40,-50,
};
static constexpr int openingBishopTableWhite[64] = {
	-20,-10,-10,-10,-10,-10,-10,-20,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-10,  0,  5, 10, 10,  5,  0,-10,
	-10,  5,  5, 10, 10,  5,  5,-10,
	-10,  0, 10, 10, 10, 10,  0,-10,
	-10, 10, 10, 10, 10, 10, 10,-10,
	-10,  5,  0,  0,  0,  0,  5,-10,
	-20,-10,-10,-10,-10,-10,-10,-20
};
static constexpr int openingBishopTableBlack[64] = {
	-20,-10,-10,-10,-10,-10,-10,-20
	- 10,  5,  0,  0,  0,  0,  5,-10,
	-10, 10, 10, 10, 10, 10, 10,-10,
	-10,  0, 10, 10, 10, 10,  0,-10,
	-10,  5,  5, 10, 10,  5,  5,-10,
	-10,  0,  5, 10, 10,  5,  0,-10,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-20,-10,-10,-10,-10,-10,-10,-20,
};
static constexpr int openingPawnTableWhite[64] = {
	0,  0,  0,  0,  0,  0,  0,  0,
	50, 50, 50, 50, 50, 50, 50, 50,
	10, 10, 20, 30, 30, 20, 10, 10,
	5,  5, 10, 25, 25, 10,  5,  5,
	0,  0,  0, 20, 20,  0,  0,  0,
	5, -5,-10,  0,  0,-10, -5,  5,
	5, 10, 10,-20,-20, 10, 10,  5,
	0,  0,  0,  0,  0,  0,  0,  0
};
static constexpr int openingPawnTableBlack[64] = {
	0,  0,  0,  0,  0,  0,  0,  0,
	5, 10, 10,-20,-20, 10, 10,  5,
	5, -5,-10,  0,  0,-10, -5,  5,
	0,  0,  0, 20, 20,  0,  0,  0,
	5,  5, 10, 25, 25, 10,  5,  5,
	10, 10, 20, 30, 30, 20, 10, 10,
	50, 50, 50, 50, 50, 50, 50, 50,
	0,  0,  0,  0,  0,  0,  0,  0
};
static constexpr int openingQueenTableWhite[64] = {
	-20,-10,-10, -5, -5,-10,-10,-20,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-10,  0,  5,  5,  5,  5,  0,-10,
	 -5,  0,  5,  5,  5,  5,  0, -5,
	  0,  0,  5,  5,  5,  5,  0, -5,
	-10,  5,  5,  5,  5,  5,  0,-10,
	-10,  0,  5,  0,  0,  0,  0,-10,
	-20,-10,-10, -5, -5,-10,-10,-20
};
static constexpr int openingQueenTableBlack[64] = {
	-20,-10,-10, -5, -5,-10,-10,-20,
	-10,  0,  5,  0,  0,  0,  0,-10,
	-10,  0,  5,  5,  5,  5,  5,-10,
	-10,  0,  5,  5,  5,  5,  0,-10,
	 -5,  0,  5,  5,  5,  5,  0, -5,
	  0,  0,  5,  5,  5,  5,  0, -5,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-20,-10,-10, -5, -5,-10,-10,-20
};

static constexpr int endgameKingTable[64] = {
	-20,-15,-15,-15,-15,-15,-15,-20,
	-15,-5,-5,-5,-5,-5,-5,-15,
	-15,-5,5,5,5,5,-5,-15,
	-15,-5,5,5,5,5,-5,-15,
	-15,-5,5,5,5,5,-5,-15,
	-15,-5,5,5,5,5,-5,-15,
	-15,-5,-5,-5,-5,-5,-5,-15,
	-20,-15,-15,-15,-15,-15,-15,-20
};
static constexpr int endgamePawnTableWhite[64] = {
	0,0,0,0,0,0,0,0,
	100,100,100,100,100,100,100,100,
	80,80,80,80,80,80,80,80,
	60,60,60,60,60,60,60,60,
	40,40,40,40,40,40,40,40,
	20,20,20,20,20,20,20,20,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
};
static constexpr int endgamePawnTableBlack[64] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	20,20,20,20,20,20,20,20,
	40,40,40,40,40,40,40,40,
	60,60,60,60,60,60,60,60,
	80,80,80,80,80,80,80,80,
	100,100,100,100,100,100,100,100,
	0,0,0,0,0,0,0,0,
};

static constexpr U64 passed_pawn_masks[2][64] = {
	{0, 0, 0, 0, 0, 0, 0, 0,
	3, 7, 14, 28, 56, 112, 224, 192,
	771, 1799, 3598, 7196, 14392, 28784, 57568, 49344,
	197379, 460551, 921102, 1842204, 3684408, 7368816, 14737632, 12632256,
	50529027, 117901063, 235802126, 471604252, 943208504, 1886417008, 3772834016, 3233857728,
	12935430915, 30182672135, 60365344270, 120730688540, 241461377080, 482922754160, 965845508320, 827867578560,
	3311470314243, 7726764066567, 15453528133134, 30907056266268, 61814112532536, 123628225065072, 247256450130144, 211934100111552,
	847736400446211, 1978051601041159, 3956103202082318, 7912206404164636, 15824412808329272, 31648825616658544, 63297651233317088, 54255129628557504},
	{217020518514230016, 506381209866536704, 1012762419733073408, 2025524839466146816, 4051049678932293632, 8102099357864587264, 16204198715729174528, 13889313184910721024,
	217020518514229248, 506381209866534912, 1012762419733069824, 2025524839466139648, 4051049678932279296, 8102099357864558592, 16204198715729117184, 13889313184910671872,
	217020518514032640, 506381209866076160, 1012762419732152320, 2025524839464304640, 4051049678928609280, 8102099357857218560, 16204198715714437120, 13889313184898088960,
	217020518463700992, 506381209748635648, 1012762419497271296, 2025524838994542592, 4051049677989085184, 8102099355978170368, 16204198711956340736, 13889313181676863488,
	217020505578799104, 506381179683864576, 1012762359367729152, 2025524718735458304, 4051049437470916608, 8102098874941833216, 16204197749883666432, 13889312357043142656,
	217017207043915776, 506373483102470144, 1012746966204940288, 2025493932409880576, 4050987864819761152, 8101975729639522304, 16203951459279044608, 13889101250810609664,
	216172782113783808, 504403158265495552, 1008806316530991104, 2017612633061982208, 4035225266123964416, 8070450532247928832, 16140901064495857664, 13835058055282163712,
	0, 0, 0, 0, 0, 0, 0, 0}
};
static constexpr U64 neighbour_pawn_masks[8] = { 565157600297472,1412894000743680,2825788001487360,5651576002974720,11303152005949440,22606304011898880,45212608023797760,18085043209519104 };
static constexpr U64 doubled_pawn_masks[64] = {
	72340172838076672, 144680345676153344, 289360691352306688, 578721382704613376, 1157442765409226752, 2314885530818453504, 4629771061636907008, 9259542123273814016,
	72340172838076416, 144680345676152832, 289360691352305664, 578721382704611328, 1157442765409222656, 2314885530818445312, 4629771061636890624, 9259542123273781248,
	72340172838010880, 144680345676021760, 289360691352043520, 578721382704087040, 1157442765408174080, 2314885530816348160, 4629771061632696320, 9259542123265392640,
	72340172821233664, 144680345642467328, 289360691284934656, 578721382569869312, 1157442765139738624, 2314885530279477248, 4629771060558954496, 9259542121117908992,
	72340168526266368, 144680337052532736, 289360674105065472, 578721348210130944, 1157442696420261888, 2314885392840523776, 4629770785681047552, 9259541571362095104,
	72339069014638592, 144678138029277184, 289356276058554368, 578712552117108736, 1157425104234217472, 2314850208468434944, 4629700416936869888, 9259400833873739776,
	72057594037927936, 144115188075855872, 288230376151711744, 576460752303423488, 1152921504606846976, 2305843009213693952, 4611686018427387904, 9223372036854775808,
	0, 0, 0, 0, 0, 0, 0, 0
};
static constexpr U64 doubled_pawn_reset_masks[64] = {
	72340172838076673, 144680345676153346, 289360691352306692, 578721382704613384, 1157442765409226768, 2314885530818453536, 4629771061636907072, 9259542123273814144,
	72340172838076672, 144680345676153344, 289360691352306688, 578721382704613376, 1157442765409226752, 2314885530818453504, 4629771061636907008, 9259542123273814016,
	72340172838076416, 144680345676152832, 289360691352305664, 578721382704611328, 1157442765409222656, 2314885530818445312, 4629771061636890624, 9259542123273781248,
	72340172838010880, 144680345676021760, 289360691352043520, 578721382704087040, 1157442765408174080, 2314885530816348160, 4629771061632696320, 9259542123265392640,
	72340172821233664, 144680345642467328, 289360691284934656, 578721382569869312, 1157442765139738624, 2314885530279477248, 4629771060558954496, 9259542121117908992,
	72340168526266368, 144680337052532736, 289360674105065472, 578721348210130944, 1157442696420261888, 2314885392840523776, 4629770785681047552, 9259541571362095104,
	72339069014638592, 144678138029277184, 289356276058554368, 578712552117108736, 1157425104234217472, 2314850208468434944, 4629700416936869888, 9259400833873739776,
	72057594037927936, 144115188075855872, 288230376151711744, 576460752303423488, 1152921504606846976, 2305843009213693952, 4611686018427387904, 9223372036854775808
};

static constexpr U64 aFile= 0x101010101010101;
static constexpr U64 bFile= 0x202020202020202;
static constexpr U64 cFile= 0x404040404040404;
static constexpr U64 dFile= 0x808080808080808;
static constexpr U64 eFile= 0x1010101010101010;
static constexpr U64 fFile= 0x2020202020202020;
static constexpr U64 gFile= 0x4040404040404040;
static constexpr U64 hFile= 0x8080808080808080;
static constexpr U64 centralSquares= 0x1818000000;
static constexpr U64 notEdges= 0x7E7E7E7E7E7E00;



struct TableEntrie {
	int eval = 0;
	int flag = 0;
	int move = 0;
	int depth = -9999;
	TableEntrie() {};
	TableEntrie(const int t_eval, const int t_flag, const int t_move,const int t_depth) {
		eval = t_eval;
		flag = t_flag;
		move = t_move;
		depth = t_depth;
	}
};
struct KillerTable {
	int table[10000][3];
	void push_move(const int move, const int depth) {
		table[depth][2] = table[depth][1];
		table[depth][1] = table[depth][0];
		table[depth][0] = move;
	}
	bool find(const int move, const int depth) {
		return (table[depth][0] == move) || (table[depth][1] == move) || (table[depth][2] == move);
	}
};
struct PVTable {
	int current_ind = 0;
	std::array<int, PV_TABLE_SIZE> table{};
	inline void delete_upper(const int ind) {
		for (int i = ind; i < PV_TABLE_SIZE; i++) {
			table[i] = 0;
		}
	}
};

struct Rosy {
	const bool DEBUG = false;
	Position pos;
	unsigned long long nodes = 0;
	int maxDepth;
	int current_desired_depth;
	int current_eval;
	int number_of_calls;
	std::unordered_map<U64, TableEntrie> hash_map{};
	KillerTable killer_table{};
	int pv_length[PV_TABLE_SIZE];
	int pv_table[PV_TABLE_SIZE][PV_TABLE_SIZE];
	U64 last_search_time = 1000;
	U64 time_at_begining_of_game = 10000;
	int last_search_depth = 7;
	U64 max_search_time = 10000;
	U64 max_search_nodes = 1000000;
	int potential_ponder_move = 0;
	U64 max_HashTableSize = 10000000;
	U64 move_overhead = 1000;
	bool use_opening_book = true;
	bool set_hashtable_size = false;
	PVTable pvTable{};
	std::atomic<bool> pondering;
	int weights[14] = { //83 , 302 , 351 , 550 , 949 , 6 , 2 , 7 , 4 , 19 , 10 , 24 , 6 , 13
	100,305,333,563,950,2,2,3,5,20,10,20,10,15
	};
	Rosy();
	Rosy(const Rosy& other);

	void uci_loop();
	inline int parse_move(std::string moveString) {
		while ((moveString.back() == *" ") || (moveString.back() == *"\n")) {
			moveString.pop_back();
		}
		std::vector<int> moves = {};
		pos.get_legal_moves(&moves);
		int move = 0;
		for (int i = 0; i < moves.size(); i++) {
			if (uci(moves[i]) == moveString) {
				return moves[i];
			}
		}
		return 0;
	}
	inline void parse_position(char* command) {
		command += 9;
		char* current_char = command;
		if (strncmp(command, "startpos", 8) == 0) {
			pos.parse_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		}
		else {
			current_char = strstr(command, "fen");
			if (current_char == nullptr) {
				pos.parse_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
			}
			else {
				current_char += 4;
				std::string fen = std::string(current_char);
				pos.parse_FEN(fen);
			}
		}
		current_char = strstr(command, "moves");
		if (current_char != nullptr) {
			current_char += 6;
			while (*current_char) {
				std::string move_str = "";
				while ((*current_char != *" ") && (*current_char)) {
					//std::cout << *current_char << "\n";
					move_str += *current_char;
					current_char++;
				}
				current_char++;
				pos.make_move(parse_move(move_str));
			}
		}
		print_position(pos);
		if (DEBUG) {
		}
	}
	inline void parse_go(char* command, std::atomic<bool>* run);

	inline TableEntrie lookup(const U64 key) {
		auto yield = hash_map.find(key);
		if (yield != hash_map.end()) {
			return yield->second;
		}
		return TableEntrie{};
	}

	int get_best_move(const int depth);
	void best_move(std::atomic<bool>* run);
	void infinite_search(std::atomic<bool>* run);
	void ponder_search(std::atomic<bool>* run);
	void movetime_search(std::atomic<bool>* run);
	void max_nodes_search(std::atomic<bool>* run);
	void find_mate(std::atomic<bool>* run);

	int quiet_root_call(const int depth, int alpha, int beta);
	int root_call(const int depth, int alpha, int beta, std::atomic<bool>* run);
	int pv_search(const int depth, int alpha, int beta);
	int quiescence(int alpha, int beta);

	int infinite_root_call(const int depth, int alpha, int beta, std::atomic<bool>* run);
	int infinite_pv_search(const int depth, int alpha, int beta, std::atomic<bool>* run);
	int nodes_root_call(const int depth, int alpha, int beta, std::atomic<bool>* run);
	int nodes_pv_search(const int depth, int alpha, int beta, std::atomic<bool>* run);
	int movetime_root_call(const int depth, int alpha, int beta, std::atomic<bool>* run, std::chrono::steady_clock::time_point& search_begin);
	int movetime_pv_search(const int depth, int alpha, int beta, std::atomic<bool>* run, std::chrono::steady_clock::time_point& begin);

	TableEntrie lookUp();
	std::vector<int> order(std::vector<int>& moves, TableEntrie& entry);

	inline int evaluate(const bool is_draw, const bool is_lost);
	inline int pawn_eval() {
		return pawn_chains() + passed_pawns() + doubledPawns() + isolated_pawns();
	}
	inline int get_phase();
	inline int raw_material(const bool is_endgame);
	inline int king_attack_zones();
	inline int king_pawn_shield();
	inline int pawn_chains();
	inline int passed_pawns();
	inline int isolated_pawns();
	inline int doubledPawns();
	inline int get_white_mobility_conectivity(Position& pos) {
		const U64 enemy_attacks = pos.get_attacks_by(black);
		U64 enemy_pieces = pos.occupancies[black];
		U64 valid_targets = (~pos.occupancies[both]) | enemy_pieces;
		U64 tempRooks = pos.bitboards[R];
		int ret = 0, move = 0;
		int bonus[6] = {1,4,3,4,2,1};
		while (tempRooks) {
			U64 isolated = tempRooks & twos_complement(tempRooks);
			const int sq = bitscan(isolated);
			U64 attacks = get_rook_attacks(pos.occupancies[both], sq) & valid_targets;
			ret += count_bits(attacks) * bonus[R];
			ret += count_bits(attacks & pos.occupancies[white]);
			tempRooks = tempRooks & ones_decrement(tempRooks);
		}
		U64 tempBishops = pos.bitboards[B];
		while (tempBishops) {
			U64 isolated = tempBishops & twos_complement(tempBishops);
			const int sq = bitscan(isolated);
			U64 attacks = get_bishop_attacks(pos.occupancies[both], sq) & valid_targets;
			ret += count_bits(attacks) * bonus[B];
			ret += count_bits(attacks & pos.occupancies[white]);
			tempBishops = tempBishops & ones_decrement(tempBishops);
		}
		U64 tempQueens = pos.bitboards[Q];
		while (tempQueens) {
			U64 isolated = tempQueens & twos_complement(tempQueens);
			const int sq = bitscan(isolated);
			U64 attacks = get_queen_attacks(pos.occupancies[both], sq) & valid_targets;
			ret += count_bits(attacks) * bonus[Q];
			ret += count_bits(attacks & pos.occupancies[white]);
			tempQueens = tempQueens & ones_decrement(tempQueens);
		}
		U64 tempKnights = pos.bitboards[N];
		while (tempKnights) {
			U64 isolated = tempKnights & twos_complement(tempKnights);
			const int sq = bitscan(isolated);
			U64 attacks = knight_attacks[sq] & valid_targets;
			ret += count_bits(attacks) * bonus[N];
			ret += count_bits(attacks & pos.occupancies[white]);
			tempKnights = tempKnights & ones_decrement(tempKnights);
		}
		const int kingpos = bitscan(pos.bitboards[K]);
		U64 attacks = king_attacks[kingpos] & (~(pos.occupancies[white] | enemy_attacks));
		ret += count_bits(attacks) * bonus[K];
		ret += count_bits(attacks & pos.occupancies[white]);

		valid_targets = ~pos.occupancies[both];
		U64 pushes = (pos.bitboards[P] >> 8) & valid_targets;
		U64 doublePushes = ((pushes >> 8) & 0xFF00000000ULL) & valid_targets;
		ret += count_bits(pushes | doublePushes) * bonus[P];
		U64 enpassant = 0ULL;
		set_bit(enpassant, pos.enpassant_square);
		const bool is_valid_enpassant_square = (pos.enpassant_square != a8);
		const U64 targets = (pos.occupancies[black] | ((is_valid_enpassant_square)*enpassant));
		U64 captures = (pos.bitboards[0] >> 7) & notAFile & targets;
		captures |= (pos.bitboards[0] >> 9) & notHFile & targets;
		ret += count_bits(captures) * bonus[P];
		return ret;
	}
	inline int get_black_mobility_conectivity(Position& pos) {
		const U64 enemy_attacks = pos.get_attacks_by(white);
		U64 enemy_pieces = pos.occupancies[white];
		U64 valid_targets = (~pos.occupancies[both]) | enemy_pieces;
		int ret = 0, move = 0;
		int bonus[6] = { 1,4,3,4,2,1 };

		U64 tempRooks = pos.bitboards[r];
		while (tempRooks) {
			U64 isolated = tempRooks & twos_complement(tempRooks);
			const int sq = bitscan(isolated);
			U64 attacks = get_rook_attacks(pos.occupancies[both], sq) & valid_targets;
			ret += count_bits(attacks) * bonus[R];
			ret += count_bits(attacks & pos.occupancies[black]);
			tempRooks = tempRooks & ones_decrement(tempRooks);
		}
		U64 tempBishops = pos.bitboards[b];
		while (tempBishops) {
			U64 isolated = tempBishops & twos_complement(tempBishops);
			const int sq = bitscan(isolated);
			U64 attacks = get_bishop_attacks(pos.occupancies[both], sq) & valid_targets;
			ret += count_bits(attacks) * bonus[B];
			ret += count_bits(attacks & pos.occupancies[black]);
			tempBishops = tempBishops & ones_decrement(tempBishops);
		}
		U64 tempQueens = pos.bitboards[q];
		while (tempQueens) {
			U64 isolated = tempQueens & twos_complement(tempQueens);
			const int sq = bitscan(isolated);
			U64 attacks = get_queen_attacks(pos.occupancies[both], sq) & valid_targets;
			ret += count_bits(attacks) * bonus[Q];
			ret += count_bits(attacks & pos.occupancies[black]);
			tempQueens = tempQueens & ones_decrement(tempQueens);
		}
		U64 tempKnights = pos.bitboards[n];
		while (tempKnights) {
			U64 isolated = tempKnights & twos_complement(tempKnights);
			const int sq = bitscan(isolated);
			U64 attacks = knight_attacks[sq] & valid_targets;
			ret += count_bits(attacks) * bonus[N];
			ret += count_bits(attacks & pos.occupancies[black]);
			tempKnights = tempKnights & ones_decrement(tempKnights);
		}
		const int kingpos = bitscan(pos.bitboards[k]);
		U64 attacks = king_attacks[kingpos] & (~(pos.occupancies[black] | enemy_attacks));
		ret += count_bits(attacks) * bonus[K];
		ret += count_bits(attacks & pos.occupancies[black]);

		valid_targets = ~pos.occupancies[both];
		U64 pushes = (pos.bitboards[p] << 8) & valid_targets;
		U64 doublePushes = ((pushes << 8) & 0xFF000000ULL) & valid_targets;
		ret += count_bits(pushes | doublePushes) * bonus[P];
		U64 enpassant = 0ULL;
		set_bit(enpassant, pos.enpassant_square);
		const U64 targets = (pos.occupancies[white] | enpassant);
		U64 captures = (pos.bitboards[p] << 7) & notHFile & targets;
		captures |= (pos.bitboards[p] << 9) & notAFile & targets;
		ret += count_bits(captures) * bonus[P];
		return ret;
	}
};

constexpr int int_power(int base, int exp)
{
	int result = 1;
	for (;;)
	{
		if (exp & 1)
			result *= base;
		exp >>= 1;
		if (!exp)
			break;
		base *= base;
	}

	return result;
}