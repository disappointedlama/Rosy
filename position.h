#pragma once
#include <vector>
#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include "bitboard.h"
#include "pawnAttacks.h"
#include "knightAttacks.h"
#include "bishopAttacks.h"
#include "rookAttacks.h"
#include "kingAttacks.h"
#include "move.h"

#include <emmintrin.h>
//sides to move
enum { white, black, both };
//castling bits
enum { wk=1, wq=2, bk=4, bq=8 };
//pieces
enum { P, N, B, R, Q, K, p, n, b, r, q, k };
//ASCII pieces
constexpr char ascii_pieces[] = "PNBRQKpnbrqk";
//convert ascii char pieces to encoded constants

constexpr size_t keys[781] = {
	1955905755,2917724850,528713011,4002650133,3904912825,2290164951,2173758225,3397570462,2364008929,3343129098,
	96101477,3298280309,3156334464,600493636,2317104146,3042143777,541895771,1232816638,2382696038,965792888,
	1173567957,2227649077,609105840,3384498373,1041708359,2823415959,3669028195,3007469191,1995600893,4002920470,
	3865022930,2682734334,4091902832,1148463252,160795174,2603429182,962636092,813517554,2028278692,3246545587,
	3388916790,2117649516,1279631167,2491445784,4058266796,4009572536,5941948,3876542818,674288968,2858334872,
	1775043690,1464912850,94868864,509921329,557586544,777926077,1511695308,2935025869,3580901127,903016734,
	617790539,316555458,1913904517,3464237150,3906854381,1389734550,3850062568,1458825168,2842181944,796100708,
	2115096716,2138401147,1248082691,2204907488,3184153667,1589025691,2418707984,274029591,1640602558,4211619756,
	3840273244,2369293953,970303623,3433539689,3753981470,1712658535,3080119355,1887754049,2852260519,1095510505,
	2091859635,2830319155,4130900824,3641568606,2833434866,757472647,51622369,1421066222,3567402886,2488348906,
	2765570790,4017547798,2002212702,2341142564,3639837672,88121664,1248042021,1313743923,559866507,1307084890,
	740380937,321877084,2080248487,458959306,2337925297,1564945723,1357440095,3327144934,186514269,1691744091,
	4240648399,1299025251,2023595303,3459590765,430452712,2765233828,2563888428,2654803638,767572737,1919751290,
	3052261786,3451026004,297412616,1219597257,1679596971,1972726459,3539308174,3868869863,2048383730,2102738788,
	3690809490,431199809,3153411493,208414087,3052044272,3307052916,2862601426,3369934117,4210205979,1092332685,
	2048505636,1983910633,2430668956,3834923104,1218449856,4004628289,2445082272,1945186983,846706666,688972451,
	4020385334,205392895,4249242477,2674275547,2180918129,2760533981,4201289198,2410785291,3392164573,1761416201,
	3905831751,1144701889,645776660,3676120914,1084831367,2567705195,2373583161,118281162,1212497487,3037324531,
	524066887,2832936672,2882540321,3197112472,665591536,3172424406,3842131295,2419862068,3829468130,2378279280,
	242323136,877528507,1006563988,1766111089,1133923825,1225297327,4124529501,1652089896,1871729004,414850200,
	1985129463,1975158453,2238376033,3913913132,2905905756,2236213341,3843192797,3930705681,1447712088,1127807609,
	647535070,2519721741,1231321684,4246762042,395827597,903740239,3721242739,1519415891,3887793494,3868840930,
	2534586481,2781620064,2013441793,2622827363,3355093852,3008425016,1991709080,2218742768,1422814864,2560047615,
	3807578853,2063333163,2567394209,1657173616,3188339662,3858570800,3311969537,2355015608,2977655152,1594218343,
	598349278,825597578,754823982,2492530765,4126704967,4125735886,3742022810,715027332,677263121,491408442,
	2096820455,2101581593,2705151763,3721385230,799908292,2522788910,2012347454,272052770,2479590006,3775260357,
	328986182,2880303695,3188735046,2952945385,365778524,836155025,1791310530,3163699528,3637828205,674665652,
	3917951589,3925237058,3540601326,3097769688,3876739610,654959539,1752758065,3361988085,3393252973,4054270635,
	3775737227,1177247763,2563074907,3485705405,216188492,3806671163,2791197929,2713284304,2429320202,2541824127,
	2315159515,2717886382,2733360657,3658702676,3837246845,2438437377,1442166263,1870351457,1720374596,2395058646,
	2886481406,114793783,672893474,3131400981,4287819630,493677646,2333056992,686878449,1134310442,2115817071,
	3820638389,4276511772,1460830667,3177611317,3939585596,3373975724,3082926546,3403575094,2295499079,1625272978,
	1846650363,3490476732,3797567179,4171879058,388158259,147176527,813407346,1020828306,432203015,3366199553,
	1765218274,3875004435,1918774427,114817070,3510896416,1335803362,727841086,136152948,1696834291,2342815743,
	1067637236,211983234,2617371690,87036209,3882999700,2108528132,2064650728,1559026896,282054078,262611296,
	2876615810,3480336891,3390476320,969275102,1420810478,965586581,2608203335,3737457060,2159065683,3735118904,
	3155625235,522635058,3730546841,4280006036,2643716265,3514817192,1908236072,4089900888,2301033040,2943887929,
	2471583744,3316925711,819106431,4149598228,1573267444,2851334633,1105253178,3912588245,3820372003,3147285059,
	1336594287,2786756299,2485113413,3277439264,3069662783,3951128889,4184791628,1705590188,703384585,924006401,
	669749649,2484206731,228922075,3185260820,108708298,1647027395,1779148025,2967983292,2053895987,1027424591,
	916683123,3176823893,1724331507,2665784041,3790731569,883517954,3152625657,1463237520,3582538329,1993872156,
	3480183664,1497204846,1678278782,272942438,747709320,2331110813,3088344755,1454316302,2998120184,3798168034,
	1192213607,2417309080,1528162678,2775504984,4095485235,1597405214,2958996648,3566963615,4086411978,3706370024,
	736635185,2320818615,3300598700,1636082280,3412263067,2812176958,582828307,629354204,1252198866,2491053108,
	96195559,2186805624,2938809647,3282517920,2665223056,2944537957,2574640986,3560294527,3873033674,3423179340,
	2167555318,2546954231,4040027301,1594094968,2623152444,2707260150,820802838,1649910961,1423603237,1771730853,
	337370390,1135102055,3015251343,3447365788,2902668577,133615799,922506083,1014138820,3672583076,3567731228,
	1395480688,866319919,4187799888,205115400,1453105837,591755256,2296792007,1816261884,2785630559,1253625830,
	3199997291,314738513,2232439218,2318932716,1956927132,2666262106,2452465849,1038501157,2233591944,4185759137,
	613678207,1580467405,584170677,1242977799,1753401050,1677039748,504722658,3339216858,3770587149,2861196418,
	1372688634,1977573311,3694955693,603721486,2277268213,2785076391,3884971851,764982197,4030507641,3380828507,
	1157751030,2148298000,3028787717,723427115,305805413,4266786994,1812342933,1262417057,548245634,2873681473,
	1901150410,19952985,856639169,131985465,985699293,3820308324,2816154139,2377232646,1249916445,2035345967,
	194192633,1825001920,668121081,3849783486,2256032414,1929394983,3346685540,101798374,57408303,457086798,
	289436484,714374894,1914852521,4012055835,1762207523,3996359136,4079558399,2572270496,691745833,51210099,
	3840756439,3347508245,377580604,4245438661,1395040574,4009375364,3120588412,207184529,700310580,818460336,
	2708496932,242340370,2764592601,4210931336,2427522129,3470630398,3659126481,2131771055,116022405,3058490957,
	3625062142,3646275177,158026136,339209693,3583043526,150249777,1833237477,395275193,1912813040,3509662725,
	3375968005,1837026632,3002270512,1125976294,2476072150,3802580262,3052748497,1597401624,996813831,3619377093,
	187458914,2427956280,1725777481,2229640500,766006506,4038454310,240997324,2395980594,153449208,4018211813,
	3324276949,3094336115,2455602432,2130876067,1820197949,1706935560,2994413151,999525148,1515672727,3185288706,
	2518618896,105348754,3835956232,440134741,2153906062,3510021244,2312644718,3846815903,307753268,3746243185,
	2959224942,1148986073,3566009348,4011469129,2122057167,3931407117,3529381179,1788148154,1893527286,3855021044,
	2750668008,1975501120,2863787128,920170238,2769315936,2848328074,2602074368,1441272097,3053698905,1923872831,
	3746294705,2092475895,2792362098,1068288223,2238601621,2978989450,174496901,768522126,333092308,3060495084,
	2373865859,6867010,942240001,1812231015,2128329316,2611790983,3655381094,3238959699,417377521,44950955,
	2426252087,677562309,166336562,1438157834,1288916853,740726264,804947643,3883778745,1968218781,3568963587,
	990948403,2489768215,4176080564,1171498917,1856960307,55773632,2804983043,661154741,3606757602,442526575,
	3041606004,4266296233,3310369529,3582569912,426358109,373878455,3417903835,1399004322,1576191987,4129968121,
	275621469,2659958782,2515151945,2741907632,3238813433,2459707923,2114961596,82410306,2606744968,2941508485,
	2908852135,258659608,772060472,564186055,531061684,1452473168,1617779692,1161923594,1988301787,2247064215,
	1620348372,3023397248,2451308920,1439983640,4148907222,975510009,765340157,3462975381,2439125679,67464123,
	2113215073,3395947748,1518974565,3200714730,3248054724,258848944,3279264270,1391532445,1197260834,2110597756,
	2988136439,1554156922,2108272979,1561958105,3078538877,75543109,1041904254,608297846,1669640806,1271669185,
	3513126169,1546242385,2968099268,2390555559,1329625284,2931958158,4104519878,3891359420,2049454828,2382748619,
	1323245958,1138491864,1933685515,1850768133,943109348,4180330693,3834027367,3326653056,2545413649,2048959233,
	58688142,2548515341,2286600671,3339908861,1721277320,317942544,3855889576,435107956,2467573238,2499868359,
	3616336295,3078887817,3558141797,77014429,2081071138,2424235833,3105052571,838439136,1898026920,1413721270,
	3747078345
};
constexpr int base_piece_values[15] = {
	//83 , 302 , 351 , 550 , 949 , 0 , 83 , 302 , 351 , 550 , 949 , 0 , 0 , 0 , 0
	100 , 305 , 333 , 563 , 950 , 0 , 100 , 305 , 333 , 563 , 950 , 0 , 0 , 0 , 0
};
struct BoardWithPieceType {
	int type;
	U64 board;
	BoardWithPieceType();
	BoardWithPieceType(const int typ, U64 brd);
};
int char_pieces(const char piece);

static constexpr U64 get_queen_attacks(U64& occ, const int sq) {
	return get_bishop_attacks(occ, sq) | get_rook_attacks(occ, sq);
};
struct Position {
	//define piece bitboard {wP,wN,wB,wR,wQ,wK,bP,bN,bB,bR,bQ,bK}
	std::array<U64,12> bitboards= { 71776119061217280ULL, 4755801206503243776ULL, 2594073385365405696ULL, 9295429630892703744ULL, 576460752303423488ULL, 1152921504606846976ULL,
	65280ULL, 66ULL, 36ULL, 129ULL, 8ULL, 16ULL };
	//define occupancies {white Pieces, black Pieces, allPieces}
	std::array<U64, 3> occupancies = { 18446462598732840960ULL, 65535ULL, 18446462598732906495ULL};
	bool side=white;
	int enpassant_square=a8;
	int castle=15;//wk,wq,bk,bq
	int turn=0;
	int moves_without_pawns=0;
	U64 current_hash=0;
	std::vector<int> pawn_stack{};
	std::vector<int> move_stack{};
	std::vector<int> castle_stack{};
	std::vector<int> enpassant_stack{};
	std::vector<size_t> hash_stack{};
	Position();
	Position(const std::string FEN);
	void parse_FEN(std::string FEN);
	std::string FEN();
	void make_move(const int move);
	void unmake_move();
	inline void make_nullmove() {
		hash_stack.push_back(current_hash);
		moves_without_pawns++;
		turn++;
		current_hash ^= (enpassant_square!=a8)*keys[773+(enpassant_square%8)];
		current_hash ^= keys[772];
		enpassant_square = a8;
		side = !side;
		move_stack.push_back(0);
		enpassant_stack.push_back(enpassant_square);
		castle_stack.push_back(castle);
		pawn_stack.push_back(moves_without_pawns);
	};
	inline void unmake_nullmove() {
		//moves_without_pawns--;
		pawn_stack.pop_back();
		moves_without_pawns = pawn_stack.back();
		enpassant_stack.pop_back();
		enpassant_square = enpassant_stack.back();
		castle_stack.pop_back();
		castle = castle_stack.back();
		current_hash = hash_stack.back();
		hash_stack.pop_back();
		move_stack.pop_back();
		turn--;
		side = !side;
	}

	constexpr bool is_attacked_by(const int sq, const bool color);
	inline int get_piece_type_on(const int sq) {
		const int offset = (!side) * 6;
		bool found_piece = false;
		int piece_type = 0;
		bool bit;
		for (int ind = offset; ind < offset + 6; ind++) {
			bit= get_bit(bitboards[ind], sq);
			if (bit) {
				found_piece = bit;
				piece_type = ind;
				break;
			}
		}
		const bool is_enpassant = (sq == enpassant_square);
		return (!found_piece) * ((is_enpassant)*offset + 15 * (!is_enpassant)) + (found_piece)*piece_type;
		/*
		int type = (!side) * 6;
		int piece_type = 0;
		bool bit = get_bit(bitboards[type], sq);
		bool found_piece = bit;
		piece_type += (bit)*type;
		type++;
		found_piece |= (bit = get_bit(bitboards[type], sq));
		piece_type += (bit)*type;
		type++;
		found_piece |= (bit = get_bit(bitboards[type], sq));
		piece_type += (bit)*type;
		type++;
		found_piece |= (bit = get_bit(bitboards[type], sq));
		piece_type += (bit)*type;
		type++;
		found_piece |= (bit = get_bit(bitboards[type], sq));
		piece_type += (bit)*type;
		type++;
		found_piece |= (bit = get_bit(bitboards[type], sq));
		piece_type += (bit)*type;
		const bool is_enpassant = (sq == enpassant_square);
		return (found_piece)*piece_type + (!found_piece) * ((is_enpassant) * ((!side) * 6) + 15 * (!is_enpassant));
		*/
	}
	inline int get_piece_type_on(const int sq, const bool color) {
		const int offset = (!color) * 6;
		bool found_piece = false;
		int piece_type = 0;
		bool bit;
		for (int ind = offset; ind < offset + 6; ind++) {
			bit = get_bit(bitboards[ind], sq);
			if (bit) {
				found_piece = bit;
				piece_type = (bit)*ind;
				break;
			}
		}
		const bool is_enpassant = (sq == enpassant_square);
		return (!found_piece) * ((is_enpassant)*offset + 15 * (!is_enpassant)) + (found_piece)*piece_type;
	}
	inline int get_kind_of_piece_on(const int sq) {
		bool found_piece;
		int piece_type;
		bool bit;
		for (int ind = 0; ind < 12; ind++) {
			bit = get_bit(bitboards[ind], sq);
			found_piece = bit;
			piece_type = (bit)*ind;
			if (bit) { break; }
		}
		const bool is_enpassant = (sq == enpassant_square);
		return 15 * (!(found_piece || is_enpassant)) + (found_piece)*piece_type;
	}

	void get_legal_moves(std::vector<int>* ret);

	inline void legal_move_generator(std::vector<int>* ret, const int kingpos, const U64 kings_queen_scope, const U64 enemy_attacks);
	inline void legal_in_check_move_generator(std::vector<int>* ret, const int kingpos, const U64 kings_queen_scope, const U64 enemy_attacks);

	inline void try_out_move(std::vector<int>* ret, int& move);
	constexpr void dont_push_move_to_vector(std::vector<int>* ret, int& move) { return; };

	constexpr void get_legal_pawn_moves(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks);
	inline void legal_bpawn_pushes(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks);
	inline void legal_wpawn_pushes(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks);
	inline void legal_bpawn_captures(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks);
	inline void legal_wpawn_captures(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks);

	constexpr  void in_check_get_legal_pawn_moves(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks);
	inline void in_check_legal_bpawn_pushes(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks);
	inline void in_check_legal_wpawn_pushes(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks);
	inline void in_check_legal_bpawn_captures(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks);
	inline void in_check_legal_wpawn_captures(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks);

	void get_legal_captures(std::vector<int>* ret);
	inline void legal_capture_gen(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks);
	inline void legal_in_check_capture_gen(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks);

	inline U64 get_pinned_pieces(const int kingpos, const U64 enemy_attacks) {
		U64 potentially_pinned_by_bishops = enemy_attacks & get_bishop_attacks(occupancies[both], kingpos) & occupancies[(side)];
		U64 potentially_pinned_by_rooks = enemy_attacks & get_rook_attacks(occupancies[both], kingpos) & occupancies[(side)];
		U64 pinned_pieces = 0ULL;
		const int offset = 6 * (!side);
		while (potentially_pinned_by_bishops) {
			const U64 isolated = potentially_pinned_by_bishops & twos_complement(potentially_pinned_by_bishops);

			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			U64 pot_pinner = get_bishop_attacks(occupancies[both], kingpos) & (bitboards[B + offset] | bitboards[Q + offset]);
			occupancies[both] |= isolated;//reset bit of piece from occupancies
			pinned_pieces |= (((bool)(pot_pinner)) & ((bool)(isolated & get_bishop_attacks(occupancies[both], static_cast<unsigned long long>(bitscan(pot_pinner)))))) * isolated;
			potentially_pinned_by_bishops = potentially_pinned_by_bishops & ones_decrement(potentially_pinned_by_bishops);
		}
		while (potentially_pinned_by_rooks) {
			const U64 isolated = potentially_pinned_by_rooks & twos_complement(potentially_pinned_by_rooks);
			occupancies[both] &= ~isolated;//pop bit of piece from occupancies
			U64 pot_pinner = get_rook_attacks(occupancies[both], kingpos) & (bitboards[R + offset] | bitboards[Q + offset]);
			occupancies[both] |= isolated;//reset bit of piece from occupancies
			pinned_pieces |= (((bool)(pot_pinner)) & ((bool)(isolated & get_rook_attacks(occupancies[both], static_cast<unsigned long long>(bitscan(pot_pinner)))))) * isolated;

			potentially_pinned_by_rooks = potentially_pinned_by_rooks & ones_decrement(potentially_pinned_by_rooks);
		}
		return pinned_pieces;
	}
	inline U64 get_moves_for_pinned_pieces(std::vector<int>* ret, const int kingpos, const U64 enemy_attacks) {
		const int piece_offset = 6 * (side);
		U64 bishops_pot_pinned_by_bishops = enemy_attacks & get_bishop_attacks(occupancies[both], kingpos) & bitboards[B + piece_offset];
		U64 bishops_pot_pinned_by_rooks = enemy_attacks & get_rook_attacks(occupancies[both], kingpos) & bitboards[B + piece_offset];

		U64 queens_pot_pinned_by_bishops = enemy_attacks & get_bishop_attacks(occupancies[both], kingpos) & bitboards[Q + piece_offset];
		U64 queens_pot_pinned_by_rooks = enemy_attacks & get_rook_attacks(occupancies[both], kingpos) & bitboards[Q + piece_offset];

		U64 rooks_pot_pinned_by_bishops = enemy_attacks & get_bishop_attacks(occupancies[both], kingpos) & bitboards[R + piece_offset];
		U64 rooks_pot_pinned_by_rooks = enemy_attacks & get_rook_attacks(occupancies[both], kingpos) & bitboards[R + piece_offset];

		U64 pawns_pot_pinned_by_bishops = enemy_attacks & get_bishop_attacks(occupancies[both], kingpos) & bitboards[P + piece_offset];
		U64 pawns_pot_pinned_by_rooks = enemy_attacks & get_rook_attacks(occupancies[both], kingpos) & bitboards[P + piece_offset];

		U64 knights_pot_pinned_by_bishops = enemy_attacks & get_bishop_attacks(occupancies[both], kingpos) & bitboards[N + piece_offset];
		U64 knights_pot_pinned_by_rooks = enemy_attacks & get_rook_attacks(occupancies[both], kingpos) & bitboards[N + piece_offset];

		U64 pinned_pieces = 0ULL;

		const int offset = 6 * (!side);
		int type = B + piece_offset;
		while (bishops_pot_pinned_by_bishops) {
			const U64 isolated = bishops_pot_pinned_by_bishops & twos_complement(bishops_pot_pinned_by_bishops);

			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			U64 pot_attacks = get_bishop_attacks(occupancies[both], kingpos);
			const U64 pinner = pot_attacks & (bitboards[B + offset] | bitboards[Q + offset]);
			pinned_pieces |= ((bool)(pinner)) * isolated;
			if (pinner) {
				const int from = bitscan(isolated);
				const int to = bitscan(pinner);
				int move = encode_move(from, to, type, get_piece_type_on(to), 15, true, false, false, false);
				ret->push_back(move);
				U64 attacks = (pot_attacks & get_bishop_attacks(occupancies[both], to)) & (~isolated);
				while (attacks) {
					const U64 isolated2 = attacks & twos_complement(attacks);
					move = encode_move(from, bitscan(isolated2), type, 15, 15, false, false, false, false);
					ret->push_back(move);
					attacks = attacks & ones_decrement(attacks);
				}
			}
			occupancies[both] |= isolated;//reset bit of piece from occupancies
			bishops_pot_pinned_by_bishops = bishops_pot_pinned_by_bishops & ones_decrement(bishops_pot_pinned_by_bishops);
		}
		while (bishops_pot_pinned_by_rooks) {
			const U64 isolated = bishops_pot_pinned_by_rooks & twos_complement(bishops_pot_pinned_by_rooks);
			occupancies[both] &= ~isolated;//pop bit of piece from occupancies
			pinned_pieces |= ((bool)(get_rook_attacks(occupancies[both], kingpos) & (bitboards[R + offset] | bitboards[Q + offset]))) * isolated;
			occupancies[both] |= isolated;//reset bit of piece from occupancies
			bishops_pot_pinned_by_rooks = bishops_pot_pinned_by_rooks & ones_decrement(bishops_pot_pinned_by_rooks);
		}

		type = Q + piece_offset;
		while (queens_pot_pinned_by_bishops) {
			const U64 isolated = queens_pot_pinned_by_bishops & twos_complement(queens_pot_pinned_by_bishops);

			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			U64 pot_attacks = get_bishop_attacks(occupancies[both], kingpos);
			const U64 pinner = pot_attacks & (bitboards[B + offset] | bitboards[Q + offset]);
			pinned_pieces |= ((bool)(pinner)) * isolated;
			if (pinner) {
				const int from = bitscan(isolated);
				const int to = bitscan(pinner);
				int move = encode_move(from, to, type, get_piece_type_on(to), 15, true, false, false, false);
				ret->push_back(move);
				U64 attacks = (pot_attacks & get_bishop_attacks(occupancies[both], to)) & (~isolated);
				while (attacks) {
					const U64 isolated2 = attacks & twos_complement(attacks);
					move = encode_move(from, bitscan(isolated2), type, 15, 15, false, false, false, false);
					ret->push_back(move);
					attacks = attacks & ones_decrement(attacks);
				}
			}
			occupancies[both] |= isolated;//reset bit of piece from occupancies
			queens_pot_pinned_by_bishops = queens_pot_pinned_by_bishops & ones_decrement(queens_pot_pinned_by_bishops);
		}
		while (queens_pot_pinned_by_rooks) {
			const U64 isolated = queens_pot_pinned_by_rooks & twos_complement(queens_pot_pinned_by_rooks);

			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			U64 pot_attacks = get_rook_attacks(occupancies[both], kingpos);
			const U64 pinner = pot_attacks & (bitboards[R + offset] | bitboards[Q + offset]);
			pinned_pieces |= ((bool)(pinner)) * isolated;
			if (pinner) {
				const int from = bitscan(isolated);
				const int to = bitscan(pinner);
				int move = encode_move(from, to, type, get_piece_type_on(to), 15, true, false, false, false);
				ret->push_back(move);
				U64 attacks = (pot_attacks & get_rook_attacks(occupancies[both], to)) & (~isolated);
				while (attacks) {
					const U64 isolated2 = attacks & twos_complement(attacks);
					move = encode_move(from, bitscan(isolated2), type, 15, 15, false, false, false, false);
					ret->push_back(move);
					attacks = attacks & ones_decrement(attacks);
				}
			}
			occupancies[both] |= isolated;//reset bit of piece from occupancies
			queens_pot_pinned_by_rooks = queens_pot_pinned_by_rooks & ones_decrement(queens_pot_pinned_by_rooks);
		}
		type = R + piece_offset;
		while (rooks_pot_pinned_by_rooks) {
			const U64 isolated = rooks_pot_pinned_by_rooks & twos_complement(rooks_pot_pinned_by_rooks);

			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			U64 pot_attacks = get_rook_attacks(occupancies[both], kingpos);
			const U64 pinner = pot_attacks & (bitboards[R + offset] | bitboards[Q + offset]);
			pinned_pieces |= ((bool)(pinner)) * isolated;
			if (pinner) {
				const int from = bitscan(isolated);
				const int to = bitscan(pinner);
				int move = encode_move(from, to, type, get_piece_type_on(to), 15, true, false, false, false);
				ret->push_back(move);
				U64 attacks = (pot_attacks & get_rook_attacks(occupancies[both], to)) & (~isolated);
				while (attacks) {
					const U64 isolated2 = attacks & twos_complement(attacks);
					move = encode_move(from, bitscan(isolated2), type, 15, 15, false, false, false, false);
					ret->push_back(move);
					attacks = attacks & ones_decrement(attacks);
				}
			}
			occupancies[both] |= isolated;//reset bit of piece from occupancies
			rooks_pot_pinned_by_rooks = rooks_pot_pinned_by_rooks & ones_decrement(rooks_pot_pinned_by_rooks);
		}
		while (rooks_pot_pinned_by_bishops) {
			const U64 isolated = rooks_pot_pinned_by_bishops & twos_complement(rooks_pot_pinned_by_bishops);
			occupancies[both] &= ~isolated;//pop bit of piece from occupancies
			pinned_pieces |= ((bool)(get_bishop_attacks(occupancies[both], kingpos) & (bitboards[B + offset] | bitboards[Q + offset]))) * isolated;
			occupancies[both] |= isolated;//reset bit of piece from occupancies
			rooks_pot_pinned_by_bishops = rooks_pot_pinned_by_bishops & ones_decrement(rooks_pot_pinned_by_bishops);
		}
		/*
		*/
		type = P + piece_offset;
		U64 enpassant = 0ULL;
		enpassant = (enpassant_square != a8) * (enpassant | (1ULL << enpassant_square));
		while (pawns_pot_pinned_by_bishops) {
			const U64 isolated = pawns_pot_pinned_by_bishops & twos_complement(pawns_pot_pinned_by_bishops);
			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			U64 pot_attacks = get_bishop_attacks(occupancies[both], kingpos);
			const U64 pinner = pot_attacks & (bitboards[B + offset] | bitboards[Q + offset]);
			pinned_pieces |= ((bool)(pinner)) * isolated;
			if (pinner) {
				const int from = bitscan(isolated);
				const int to = bitscan(pinner);
				U64 attacks = pawn_attacks[(side)][from];
				const bool is_enpassant = attacks & enpassant;
				if (is_enpassant) {
					int move = encode_move(from, enpassant_square, type, get_piece_type_on(P + offset), 15, true, false, false, true);
					ret->push_back(move);
				}
				else if (attacks & pinner) {
					int move = encode_move(from, to, type, get_piece_type_on(to), 15, true, false, false, false);
					if ((side && (to < a7)) || ((!side) && (to > h2))) {
						set_promotion_type(move, N + piece_offset);
						ret->push_back(move);
						set_promotion_type(move, B + piece_offset);
						ret->push_back(move);
						set_promotion_type(move, R + piece_offset);
						ret->push_back(move);
						set_promotion_type(move, Q + piece_offset);
					}
					ret->push_back(move);
				}
			}
			occupancies[both] |= isolated;//reset bit of piece from occupancies

			pawns_pot_pinned_by_bishops = pawns_pot_pinned_by_bishops & ones_decrement(pawns_pot_pinned_by_bishops);
		}
		while (pawns_pot_pinned_by_rooks) {
			const U64 isolated = pawns_pot_pinned_by_rooks & twos_complement(pawns_pot_pinned_by_rooks);
			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			U64 pot_attacks = get_rook_attacks(occupancies[both], kingpos);
			const U64 pinner = pot_attacks & (bitboards[R + offset] | bitboards[Q + offset]);
			pinned_pieces |= ((bool)(pinner)) * isolated;
			if (pinner) {
				const int from = bitscan(isolated);
				const int to = bitscan(pinner);
				U64 valid_targets = (~occupancies[both]) & pot_attacks;
				const int push_target = from + 8 * ((side)-(!side));
				const int double_push_target = from + 16 * ((side)-(!side));
				if (get_bit(valid_targets, push_target)) {
					int move = encode_move(from, push_target, type, 15, 15, false, false, false, false);
					if ((push_target < a7) || (push_target > h2)) {
						set_promotion_type(move, N + piece_offset);
						ret->push_back(move);
						set_promotion_type(move, B + piece_offset);
						ret->push_back(move);
						set_promotion_type(move, R + piece_offset);
						ret->push_back(move);
						set_promotion_type(move, Q + piece_offset);
					}
					ret->push_back(move);
				}
				if ((double_push_target < a3) && (double_push_target > h6)) {
					if (get_bit(valid_targets & (((!side) * 0xFF00000000ULL) | ((side) * 0xFF000000ULL)), double_push_target)) {
						int move = encode_move(from, double_push_target, type, 15, 15, false, true, false, false);
						ret->push_back(move);
					}
				}
			}
			occupancies[both] |= isolated;//reset bit of piece from occupancies

			pawns_pot_pinned_by_rooks = pawns_pot_pinned_by_rooks & ones_decrement(pawns_pot_pinned_by_rooks);
		}


		while (knights_pot_pinned_by_bishops) {
			const U64 isolated = knights_pot_pinned_by_bishops & twos_complement(knights_pot_pinned_by_bishops);

			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			pinned_pieces |= ((bool)(get_bishop_attacks(occupancies[both], kingpos) & (bitboards[B + offset] | bitboards[Q + offset]))) * isolated;
			occupancies[both] |= isolated;//reset bit of piece from occupancies

			knights_pot_pinned_by_bishops = knights_pot_pinned_by_bishops & ones_decrement(knights_pot_pinned_by_bishops);
		}
		while (knights_pot_pinned_by_rooks) {
			const U64 isolated = knights_pot_pinned_by_rooks & twos_complement(knights_pot_pinned_by_rooks);

			occupancies[both] &= ~isolated;//pop bit of piece from occupancies
			pinned_pieces |= ((bool)(get_rook_attacks(occupancies[both], kingpos) & (bitboards[R + offset] | bitboards[Q + offset]))) * isolated;
			occupancies[both] |= isolated;//reset bit of piece from occupancies

			knights_pot_pinned_by_rooks = knights_pot_pinned_by_rooks & ones_decrement(knights_pot_pinned_by_rooks);
		}
		return pinned_pieces;
	}
	inline U64 get_captures_for_pinned_pieces(std::vector<int>* ret, const int kingpos, const U64 enemy_attacks) {
		const int piece_offset = 6 * (side);
		U64 bishops_pot_pinned_by_bishops = enemy_attacks & get_bishop_attacks(occupancies[both], kingpos) & bitboards[B + piece_offset];
		U64 bishops_pot_pinned_by_rooks = enemy_attacks & get_rook_attacks(occupancies[both], kingpos) & bitboards[B + piece_offset];

		U64 queens_pot_pinned_by_bishops = enemy_attacks & get_bishop_attacks(occupancies[both], kingpos) & bitboards[Q + piece_offset];
		U64 queens_pot_pinned_by_rooks = enemy_attacks & get_rook_attacks(occupancies[both], kingpos) & bitboards[Q + piece_offset];

		U64 rooks_pot_pinned_by_bishops = enemy_attacks & get_bishop_attacks(occupancies[both], kingpos) & bitboards[R + piece_offset];
		U64 rooks_pot_pinned_by_rooks = enemy_attacks & get_rook_attacks(occupancies[both], kingpos) & bitboards[R + piece_offset];

		U64 pawns_pot_pinned_by_bishops = enemy_attacks & get_bishop_attacks(occupancies[both], kingpos) & bitboards[P + piece_offset];
		U64 pawns_pot_pinned_by_rooks = enemy_attacks & get_rook_attacks(occupancies[both], kingpos) & bitboards[P + piece_offset];

		U64 knights_pot_pinned_by_bishops = enemy_attacks & get_bishop_attacks(occupancies[both], kingpos) & bitboards[N + piece_offset];
		U64 knights_pot_pinned_by_rooks = enemy_attacks & get_rook_attacks(occupancies[both], kingpos) & bitboards[N + piece_offset];

		U64 pinned_pieces = 0ULL;

		const int offset = 6 * (!side);
		int type = B + piece_offset;
		while (bishops_pot_pinned_by_bishops) {
			const U64 isolated = bishops_pot_pinned_by_bishops & twos_complement(bishops_pot_pinned_by_bishops);

			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			U64 pot_attacks = get_bishop_attacks(occupancies[both], kingpos);
			const U64 pinner = pot_attacks & (bitboards[B + offset] | bitboards[Q + offset]);
			pinned_pieces |= ((bool)(pinner)) * isolated;
			if (pinner) {
				const int from = bitscan(isolated);
				const int to = bitscan(pinner);
				int move = encode_move(from, to, type, get_piece_type_on(to), 15, true, false, false, false);
				ret->push_back(move);
			}
			occupancies[both] |= isolated;//reset bit of piece from occupancies
			bishops_pot_pinned_by_bishops = bishops_pot_pinned_by_bishops & ones_decrement(bishops_pot_pinned_by_bishops);
		}
		while (bishops_pot_pinned_by_rooks) {
			const U64 isolated = bishops_pot_pinned_by_rooks & twos_complement(bishops_pot_pinned_by_rooks);
			occupancies[both] &= ~isolated;//pop bit of piece from occupancies
			pinned_pieces |= ((bool)(get_rook_attacks(occupancies[both], kingpos) & (bitboards[R + offset] | bitboards[Q + offset]))) * isolated;
			occupancies[both] |= isolated;//reset bit of piece from occupancies
			bishops_pot_pinned_by_rooks = bishops_pot_pinned_by_rooks & ones_decrement(bishops_pot_pinned_by_rooks);
		}

		type = Q + piece_offset;
		while (queens_pot_pinned_by_bishops) {
			const U64 isolated = queens_pot_pinned_by_bishops & twos_complement(queens_pot_pinned_by_bishops);

			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			U64 pot_attacks = get_bishop_attacks(occupancies[both], kingpos);
			const U64 pinner = pot_attacks & (bitboards[B + offset] | bitboards[Q + offset]);
			pinned_pieces |= ((bool)(pinner)) * isolated;
			if (pinner) {
				const int from = bitscan(isolated);
				const int to = bitscan(pinner);
				int move = encode_move(from, to, type, get_piece_type_on(to), 15, true, false, false, false);
				ret->push_back(move);
			}
			occupancies[both] |= isolated;//reset bit of piece from occupancies
			queens_pot_pinned_by_bishops = queens_pot_pinned_by_bishops & ones_decrement(queens_pot_pinned_by_bishops);
		}
		while (queens_pot_pinned_by_rooks) {
			const U64 isolated = queens_pot_pinned_by_rooks & twos_complement(queens_pot_pinned_by_rooks);

			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			U64 pot_attacks = get_rook_attacks(occupancies[both], kingpos);
			const U64 pinner = pot_attacks & (bitboards[R + offset] | bitboards[Q + offset]);
			pinned_pieces |= ((bool)(pinner)) * isolated;
			if (pinner) {
				const int from = bitscan(isolated);
				const int to = bitscan(pinner);
				int move = encode_move(from, to, type, get_piece_type_on(to), 15, true, false, false, false);
				ret->push_back(move);
			}
			occupancies[both] |= isolated;//reset bit of piece from occupancies
			queens_pot_pinned_by_rooks = queens_pot_pinned_by_rooks & ones_decrement(queens_pot_pinned_by_rooks);
		}
		type = R + piece_offset;
		while (rooks_pot_pinned_by_rooks) {
			const U64 isolated = rooks_pot_pinned_by_rooks & twos_complement(rooks_pot_pinned_by_rooks);

			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			U64 pot_attacks = get_rook_attacks(occupancies[both], kingpos);
			const U64 pinner = pot_attacks & (bitboards[R + offset] | bitboards[Q + offset]);
			pinned_pieces |= ((bool)(pinner)) * isolated;
			if (pinner) {
				const int from = bitscan(isolated);
				const int to = bitscan(pinner);
				int move = encode_move(from, to, type, get_piece_type_on(to), 15, true, false, false, false);
				ret->push_back(move);
			}
			occupancies[both] |= isolated;//reset bit of piece from occupancies
			rooks_pot_pinned_by_rooks = rooks_pot_pinned_by_rooks & ones_decrement(rooks_pot_pinned_by_rooks);
		}
		while (rooks_pot_pinned_by_bishops) {
			const U64 isolated = rooks_pot_pinned_by_bishops & twos_complement(rooks_pot_pinned_by_bishops);
			occupancies[both] &= ~isolated;//pop bit of piece from occupancies
			pinned_pieces |= ((bool)(get_bishop_attacks(occupancies[both], kingpos) & (bitboards[B + offset] | bitboards[Q + offset]))) * isolated;
			occupancies[both] |= isolated;//reset bit of piece from occupancies
			rooks_pot_pinned_by_bishops = rooks_pot_pinned_by_bishops & ones_decrement(rooks_pot_pinned_by_bishops);
		}
		/*
		*/
		type = P + piece_offset;
		U64 enpassant = 0ULL;
		enpassant = (enpassant_square != a8) * (enpassant | (1ULL << enpassant_square));
		while (pawns_pot_pinned_by_bishops) {
			const U64 isolated = pawns_pot_pinned_by_bishops & twos_complement(pawns_pot_pinned_by_bishops);
			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			U64 pot_attacks = get_bishop_attacks(occupancies[both], kingpos);
			const U64 pinner = pot_attacks & (bitboards[B + offset] | bitboards[Q + offset]);
			pinned_pieces |= ((bool)(pinner)) * isolated;
			if (pinner) {
				const int from = bitscan(isolated);
				const int to = bitscan(pinner);
				U64 attacks = pawn_attacks[(side)][from];
				const bool is_enpassant = attacks & enpassant;
				if (is_enpassant) {
					int move = encode_move(from, enpassant_square, type, get_piece_type_on(P + offset), 15, true, false, false, true);
					ret->push_back(move);
				}
				else if (attacks & pinner) {
					int move = encode_move(from, to, type, get_piece_type_on(to), 15, true, false, false, false);
					if ((side && (to < a7)) || ((!side) && (to > h2))) {
						set_promotion_type(move, N + piece_offset);
						ret->push_back(move);
						set_promotion_type(move, B + piece_offset);
						ret->push_back(move);
						set_promotion_type(move, R + piece_offset);
						ret->push_back(move);
						set_promotion_type(move, Q + piece_offset);
					}
					ret->push_back(move);
				}
			}
			occupancies[both] |= isolated;//reset bit of piece from occupancies

			pawns_pot_pinned_by_bishops = pawns_pot_pinned_by_bishops & ones_decrement(pawns_pot_pinned_by_bishops);
		}
		while (pawns_pot_pinned_by_rooks) {
			const U64 isolated = pawns_pot_pinned_by_rooks & twos_complement(pawns_pot_pinned_by_rooks);
			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			U64 pot_attacks = get_rook_attacks(occupancies[both], kingpos);
			const U64 pinner = pot_attacks & (bitboards[R + offset] | bitboards[Q + offset]);
			pinned_pieces |= ((bool)(pinner)) * isolated;
			occupancies[both] |= isolated;//reset bit of piece from occupancies

			pawns_pot_pinned_by_rooks = pawns_pot_pinned_by_rooks & ones_decrement(pawns_pot_pinned_by_rooks);
		}


		while (knights_pot_pinned_by_bishops) {
			const U64 isolated = knights_pot_pinned_by_bishops & twos_complement(knights_pot_pinned_by_bishops);

			occupancies[both] &= ~isolated;//pop bit of piece from occupancie
			pinned_pieces |= ((bool)(get_bishop_attacks(occupancies[both], kingpos) & (bitboards[B + offset] | bitboards[Q + offset]))) * isolated;
			occupancies[both] |= isolated;//reset bit of piece from occupancies

			knights_pot_pinned_by_bishops = knights_pot_pinned_by_bishops & ones_decrement(knights_pot_pinned_by_bishops);
		}
		while (knights_pot_pinned_by_rooks) {
			const U64 isolated = knights_pot_pinned_by_rooks & twos_complement(knights_pot_pinned_by_rooks);

			occupancies[both] &= ~isolated;//pop bit of piece from occupancies
			pinned_pieces |= ((bool)(get_rook_attacks(occupancies[both], kingpos) & (bitboards[R + offset] | bitboards[Q + offset]))) * isolated;
			occupancies[both] |= isolated;//reset bit of piece from occupancies

			knights_pot_pinned_by_rooks = knights_pot_pinned_by_rooks & ones_decrement(knights_pot_pinned_by_rooks);
		}
		return pinned_pieces;
	}

	inline U64 get_checkers(const int kingpos) {
		const int offset = 6 * (!side);
		return (get_bishop_attacks(occupancies[both], kingpos) & (bitboards[B + offset] | bitboards[Q + offset])) | (get_rook_attacks(occupancies[both], kingpos) & (bitboards[R + offset] | bitboards[Q + offset])) | (knight_attacks[kingpos] & bitboards[N + offset]) | (pawn_attacks[(side)][kingpos] & bitboards[P + offset]);
	}
	inline U64 get_checking_rays(const int kingpos) {
		const int offset = 6 * (!side);
		const U64 kings_rook_scope = get_rook_attacks(occupancies[both], kingpos);
		const U64 kings_bishop_scope = get_bishop_attacks(occupancies[both], kingpos);
		const U64 checking_rooks = kings_rook_scope & (bitboards[R + offset] | bitboards[Q + offset]);
		const U64 checking_bishops = kings_bishop_scope & (bitboards[B + offset] | bitboards[Q + offset]);

		return (((bool)(checking_rooks)) * (checking_rooks | (get_rook_attacks(occupancies[both], bitscan(checking_rooks)) & kings_rook_scope))) | (((bool)(checking_bishops)) * (checking_bishops | (get_bishop_attacks(occupancies[both], bitscan(checking_bishops)) & kings_bishop_scope)));
	}

	inline int get_smallest_attack(const int sq, const bool color) {
		if (get_bit(occupancies[both], sq)) {
			int move = 0;
			set_promotion_type(move, 15);
			set_to_square(move, sq);
			set_captured_type(move, get_kind_of_piece_on(sq));
			set_capture_flag(move, true);
			const int offset = 6 * (color);
			U64 pot_pawns = pawn_attacks[(color)][sq] & bitboards[offset];
			if (pot_pawns) {
				set_piece_type(move, P + offset);
				set_from_square(move, bitscan(pot_pawns));
				return move;
			}
			U64 pot_knights = knight_attacks[sq] & bitboards[N + offset];
			if (pot_knights) {
				set_piece_type(move, N + offset);
				set_from_square(move, bitscan(pot_knights));
				return move;
			}
			const U64 bishop_attacks = get_bishop_attacks(occupancies[both], sq);
			U64 pot_bishops = bishop_attacks & bitboards[B + offset];
			if (pot_bishops) {
				set_piece_type(move, B + offset);
				set_from_square(move, bitscan(pot_bishops));
				return move;
			}
			const U64 rook_attacks = get_rook_attacks(occupancies[both], sq);
			U64 pot_rooks = rook_attacks & bitboards[R + offset];
			if (pot_rooks) {
				set_piece_type(move, R + offset);
				set_from_square(move, bitscan(pot_rooks));
				return move;
			}
			U64 pot_queens = (rook_attacks | bishop_attacks) & bitboards[Q + offset];
			if (pot_queens) {
				set_piece_type(move, Q + offset);
				set_from_square(move, bitscan(pot_queens));
				return move;
			}
		}
		return 0;
	}
	inline int see(const int square, const bool color)
	{
		int value = 0;
		int move = get_smallest_attack(square, color);
		/* skip if the square isn't attacked anymore by this side */
		if (move)
		{
			make_move(move);
			/* Do not consider captures if they lose material, therefor max zero */
			value = std::max(-1000000, base_piece_values[get_captured_type(move)] - see(square, !color));
			unmake_move();
		}
		return value;
	}

	inline U64 get_attacks_by(const bool color);
	inline void get_castles(std::vector<int>* const ptr) {
		const U64 empty = ~(occupancies[both]);

		const int king_index = 5 + (side) * 6;
		const int kingpos = bitscan(bitboards[king_index]);
		const bool king_on_valid_square = ((side) && (kingpos == e8)) || ((!side) && (kingpos == e1));

		const int enemy_color = (side)*white + (!side) * black;

		const bool king_in_valid_state = king_on_valid_square && (!is_attacked_by(kingpos, enemy_color));
		bool queenside = king_in_valid_state && (get_bit(castle, 1 + 2 * (side)));
		bool kingside = king_in_valid_state && (get_bit(castle, 2 * (side)));

		const int square_offset = 56 * (side);

		queenside &= (((bitboards[king_index]) >> 1) & empty) && (!is_attacked_by(d1 - square_offset, enemy_color));
		queenside &= (((bitboards[king_index]) >> 2) & empty) && (!is_attacked_by(c1 - square_offset, enemy_color));
		queenside &= (bool)(((bitboards[king_index]) >> 3) & empty);

		int move = encode_move(kingpos, kingpos - 2, king_index, 15, 15, false, false, true, false);

		if (queenside) {
			ptr->push_back(move);
		}

		kingside &= (((bitboards[king_index]) << 1) & empty) && (!is_attacked_by(f1 - square_offset, enemy_color));
		kingside &= (((bitboards[king_index]) << 2) & empty) && (!is_attacked_by(g1 - square_offset, enemy_color));
		set_to_square(move, kingpos + 2);
		if (kingside) {
			ptr->push_back(move);
		}
	}
	inline void get_pawn_captures(std::vector<int>* const ptr, const U64 kings_queen_scope, const U64 enemy_attacks);
	inline void in_check_get_pawn_captures(std::vector<int>* ptr, const U64 kings_queen_scope, const U64 enemy_attacks);
	inline void update_hash(const int move) {
		const int piece_type = get_piece_type(move);
		const int from_square = get_from_square(move);
		const int to_square = get_to_square(move);
		const int captured_type = get_captured_type(move);
		const int promoted_type = get_promotion_type(move);

		const bool capture = get_capture_flag(move);
		const bool is_enpassant = get_enpassant_flag(move);
		const bool is_castle = get_castling_flag(move);
		const bool is_double_push = get_double_push_flag(move);

		const int offset = piece_type * 64;

		assert(offset + from_square < 781);
		assert(offset + from_square >= 0);

		current_hash ^= keys[offset + from_square];
		current_hash ^= (capture) * ((!is_enpassant) * keys[captured_type * 64 + to_square] + (is_enpassant) * keys[to_square + (side) * (6 * 64 + 8) + (!side) * (-8)]);
		current_hash ^= keys[offset + to_square];

		const int rook_source = (is_castle) * ((piece_type == k) * ((to_square == g8) * h8 + (to_square == c8) * a8) + (piece_type == K) * ((to_square == g1) * h1 + (to_square == c1) * a1));
		const int rook_target = (is_castle) * (from_square + ((to_square == g8) | (to_square == g1)) - ((to_square == c8) | (to_square == c1)));
		current_hash ^= (is_castle) * keys[offset - 128 + rook_source];
		current_hash ^= (is_castle) * keys[offset - 128 + rook_target];

		const auto size = static_cast<std::vector<int, std::allocator<int>>::size_type>(enpassant_stack.size() - 2);
		assert(castle_stack.size() == enpassant_stack.size());
		assert(castle_stack.size() >= 3);
		const int different_rights = castle ^ castle_stack[size];
		current_hash ^= (get_bit(different_rights, 0)) * keys[12 * 64];
		current_hash ^= (get_bit(different_rights, 1)) * keys[12 * 64 + 1];
		current_hash ^= (get_bit(different_rights, 2)) * keys[12 * 64 + 2];
		current_hash ^= (get_bit(different_rights, 3)) * keys[12 * 64 + 3];
		current_hash ^= keys[772];

		assert(enpassant_stack.size() >= 3);
		const int old_enpassant_square = enpassant_stack[size];
		current_hash ^= ((old_enpassant_square != a8) && (old_enpassant_square != 64))*keys[773 + (old_enpassant_square%8)];
		//undo old enpassant key
		current_hash ^= (is_double_push)*keys[773 + (from_square % 8)];

		current_hash = current_hash % 4294967296;
	};
	bool operator== (const Position& other) {
		return (bitboards == other.bitboards);
	}
	void print() {
		printf("\n");
		for (int rank = 0; rank < 8; rank++) {
			for (int file = 0; file < 8; file++) {
				//loop over board ranks and files

				int square = rank * 8 + file;
				//convert to square index

				if (!file) {
					printf(" %d ", 8 - rank);
				}//print rank on the left side

				int piece = -1;

				for (int piece_on_square = P; piece_on_square <= k; piece_on_square++) {
					if (get_bit(bitboards[piece_on_square], square)) {
						piece = piece_on_square;
						break;
					}
				}
				printf(" %c", (piece == -1) ? ' .' : ascii_pieces[piece]);
			}
			printf("\n");
		}
		printf("\n    a b c d e f g h \n\n");
		printf("    To move: %s\n", (side) ? "black" : "white");
		printf("    enpassant square: %s\n", (enpassant_square != a8) ? square_coordinates[enpassant_square].c_str() : "none");

		printf("    castling rights: %c%c%c%c\n", (get_bit(castle, 0)) ? 'K' : '-', (get_bit(castle, 1)) ? 'Q' : '-', (get_bit(castle, 2)) ? 'k' : '-', (get_bit(castle, 3)) ? 'q' : '-');
		//print castling rights

		printf("    halfmoves since last pawn move: %d\n", moves_without_pawns);
		printf("    current halfclock turn: %d\n", turn);
		std::cout << "    current hash: " << current_hash << "\n";
	}
};
void print_position(Position& pos);
inline U64 get_hash(Position pos) {

	size_t ret = 0;
	for (int i = 0; i < 12; i++) {
		U64 board = pos.bitboards[i];
		while (board) {
			U64 isolated = board & twos_complement(board);
			ret ^= keys[bitscan(isolated) + i * 64];
			board = board & ones_decrement(board);
		}
	}

	ret ^= (get_bit(pos.castle, 0)) * keys[12 * 64];
	ret ^= (get_bit(pos.castle, 1)) * keys[12 * 64 + 1];
	ret ^= (get_bit(pos.castle, 2)) * keys[12 * 64 + 2];
	ret ^= (get_bit(pos.castle, 3)) * keys[12 * 64 + 3];

	ret ^= pos.side * keys[772];

	ret ^= ((pos.enpassant_square != a8)&&(pos.enpassant_square!=64)) * keys[773 + (pos.enpassant_square % 8)];
	return ret % 4294967296;
}