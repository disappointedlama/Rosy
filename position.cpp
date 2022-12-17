#include "position.h"
U64 PINNED = 0ULL;
U64 IN_CHECK_VALID = 0ULL;
U64 CHECKERS = 0ULL;
BoardWithPieceType::BoardWithPieceType() {
	board = 0ULL;
	type = -1;
}
BoardWithPieceType::BoardWithPieceType(const int typ, U64 brd) {
	type = typ;
	board = brd;
}

Position::Position() {
	parse_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}
Position::Position(const std::string FEN) {
	parse_FEN(FEN);
}
void Position::parse_FEN(std::string FEN) {
	unsigned int i, j;
	int sq;
	char letter;
	int aRank, aFile;
	std::vector<std::string> strList;
	const std::string delimiter = " ";
	strList.push_back(FEN.substr(0, FEN.find(delimiter)));
	FEN = FEN.substr(FEN.find(delimiter));
	FEN = FEN.substr(1);
	// Empty the board quares
	for (int i = 0; i < 12; i++) {
		bitboards[i] = 0ULL;
	}
	castle = 0;
	moves_without_pawns = 0;
	// read the board - translate each loop idx into a square
	j = 1; i = 0;
	while ((j <= 64) && (i <= strList[0].length()))
	{
		letter = strList[0].at(i);
		i++;
		aFile = 1 + ((j - 1) % 8);
		aRank = 1 + ((j - 1) / 8);
		sq = (int)(((aRank - 1) * 8) + (aFile - 1));
		switch (letter)
		{
		case '/': j--; break;
		case '1': break;
		case '2': j++; break;
		case '3': j += 2; break;
		case '4': j += 3; break;
		case '5': j += 4; break;
		case '6': j += 5; break;
		case '7': j += 6; break;
		case '8': j += 7; break;
		default: set_bit(bitboards[char_pieces(letter)], sq);
		}
		j++;
	}

	strList.push_back(FEN.substr(0, FEN.find(delimiter)));
	FEN = FEN.substr(FEN.find(delimiter));
	FEN = FEN.substr(1);

	// set the turn; default = White
	int sideToMove = white;
	if (strList.size() >= 2)
	{
		if (strList[1] == "w") side = white; else
			if (strList[1] == "b") side = black;
	}

	strList.push_back(FEN.substr(0, FEN.find(delimiter)));
	FEN = FEN.substr(FEN.find(delimiter));
	FEN = FEN.substr(1);

	if (strList[2].find('K') != std::string::npos) {
		castle |= wk;
	}
	if (strList[2].find('Q') != std::string::npos) {
		castle |= wq;
	}
	if (strList[2].find('k') != std::string::npos) {
		castle |= bk;
	}
	if (strList[2].find('q') != std::string::npos) {
		castle |= bq;
	}

	strList.push_back(FEN.substr(0, FEN.find(delimiter)));
	FEN = FEN.substr(FEN.find(delimiter));
	FEN = FEN.substr(1);

	if (FEN[0] == '-') {
		enpassant_square = a8;
	}
	else {
		enpassant_square = (int)(std::find(square_coordinates, square_coordinates + sizeof(square_coordinates) / sizeof(square_coordinates[0]), strList[3]) - square_coordinates);
		enpassant_square = (enpassant_square == 64) * a8 + (enpassant_square != 64) * enpassant_square;
	}

	strList.push_back(FEN.substr(0, FEN.find(delimiter)));
	FEN = FEN.substr(FEN.find(delimiter));
	FEN = FEN.substr(1);

	move_stack.clear();
	pawn_stack.clear();
	castle_stack.clear();
	enpassant_stack.clear();
	hash_stack.clear();

	move_stack.reserve(1000);
	pawn_stack.reserve(1000);
	castle_stack.reserve(1000);
	enpassant_stack.reserve(1000);
	hash_stack.reserve(1000);

	moves_without_pawns = stoi(strList[4]);
	turn = stoi(FEN);
	occupancies[0] = bitboards[0] | bitboards[1] | bitboards[2] | bitboards[3] | bitboards[4] | bitboards[5];
	occupancies[1] = bitboards[6] | bitboards[7] | bitboards[8] | bitboards[9] | bitboards[10] | bitboards[11];
	occupancies[2] = occupancies[0] | occupancies[1];
	pawn_stack.push_back(moves_without_pawns);
	castle_stack.push_back(0);
	castle_stack.push_back(castle);
	enpassant_stack.push_back(0);
	enpassant_stack.push_back(enpassant_square);
	current_hash = get_hash(*this);
	hash_stack.push_back(current_hash);
}
std::string Position::FEN() {
	std::string ret = "";
	const int arr[8] = { 7,6,5,4,3,2,1,0 };
	for (int i = 0; i < 8; i++) {
		int empty_spaces = 0;
		for (int j = 0; j < 8; j++) {
			const int ind = 8 * i + j;
			int piece = get_piece_type_on(ind);
			if (piece == 15) {
				side = !side;
				piece = get_piece_type_on(ind);
				side = !side;
			}
			if ((piece == 15)||(ind==enpassant_square)) {
				empty_spaces++;
			}
			else {
				if (empty_spaces) {
					ret += std::to_string(empty_spaces);
				}
				empty_spaces = 0;
				ret += ascii_pieces[piece];
			}
		}
		if (empty_spaces) {
			ret += std::to_string(empty_spaces);
		}
		if (i < 7) {
			ret += "/";
		}
	}
	ret += " ";
	if (side) {
		ret += "b";
	}
	else {
		ret += "w";
	}
	ret += " ";
	if (get_bit(castle, 0)) {
		ret += "K";
	}
	if (get_bit(castle, 1)) {
		ret += "Q";
	}
	if (get_bit(castle, 2)) {
		ret += "k";
	}
	if (get_bit(castle, 3)) {
		ret += "q";
	}
	if (!(get_bit(castle, 0) || get_bit(castle, 1) || get_bit(castle, 2) || get_bit(castle, 3))) {
		ret += "-";
	}
	ret += " ";
	if ((enpassant_square == a8)||(enpassant_square==64)) {
		ret += "-";
	}
	else{
		ret += square_coordinates[enpassant_square];
	}
	ret += " ";
	ret += std::to_string(moves_without_pawns);
	ret += " ";
	if (turn % 2) {
		ret += std::to_string((turn+1) / 2);
	}
	else {
		ret += std::to_string(turn / 2 + 1);
	}
	return ret;
}

constexpr bool Position::is_attacked_by(const int sq, const bool color) {
	if (color) {
		return ((get_rook_attacks(occupancies[both], sq) & (bitboards[9] | bitboards[10])) | (get_bishop_attacks(occupancies[both], sq) & (bitboards[8] | bitboards[10])) | (pawn_attacks[!color][sq] & bitboards[6]) | (king_attacks[sq] & bitboards[11]) | (knight_attacks[sq] & bitboards[7]));
	}
	return (get_rook_attacks(occupancies[both], sq) & (bitboards[3] | bitboards[4])) | (get_bishop_attacks(occupancies[both], sq) & (bitboards[2] | bitboards[4])) | (pawn_attacks[!color][sq] & bitboards[0]) | (king_attacks[sq] & bitboards[5]) | (knight_attacks[sq] & bitboards[1]);
}

void Position::unmake_move() {
	pawn_stack.pop_back();
	moves_without_pawns = pawn_stack.back();
	castle_stack.pop_back();
	castle = castle_stack.back();
	enpassant_stack.pop_back();
	enpassant_square = enpassant_stack.back();
	current_hash = hash_stack.back();
	hash_stack.pop_back();

	const int move = move_stack.back();
	move_stack.pop_back();

	const int piece_type = get_piece_type(move);
	const int from_square = get_from_square(move);
	const int to_square = get_to_square(move);
	const int captured_type = get_captured_type(move);
	const int promoted_type = get_promotion_type(move);

	const bool double_pawn_push = get_double_push_flag(move);
	const bool capture = get_capture_flag(move);
	const bool is_enpassant = get_enpassant_flag(move);
	const bool is_castle = get_castling_flag(move);

	set_bit(bitboards[piece_type], from_square);
	pop_bit(bitboards[piece_type], to_square);

	const bool is_promotion = promoted_type != 15;
	bitboards[(is_promotion) * promoted_type] &= ~(((is_promotion) * 1ULL) << (to_square));
	//branchlessly pop the piece that was promoted. if move was not a promotion the white pawns are and'ed with a bitboard of ones, thus it would not change

	bitboards[(capture)*captured_type] |= (capture) * (1ULL << (to_square + 8 * (is_enpassant) * ((side)-(!side))));
	//branchlessly set the captured piece. if there was no captured piece the white pawn board is or'ed with 0 (not changed)
	const int square_offset = 56 * (side);
	const bool is_kingside = to_square > from_square;
	bitboards[(9 - 6 * (side))] |= (((is_castle) * 1ULL) << ((is_kingside) * (h8 + square_offset) + (!(is_kingside) * (a8 + square_offset))));
	bitboards[(9 - 6 * (side))] &= ~(((is_castle) * 1ULL) << ((is_kingside) * (f8 + square_offset) + (!(is_kingside)) * (d8 + square_offset)));
	//branchlessly set and pop rook bits if move was castling

	occupancies[0] = bitboards[0] | bitboards[1] | bitboards[2] | bitboards[3] | bitboards[4] | bitboards[5];
	occupancies[1] = bitboards[6] | bitboards[7] | bitboards[8] | bitboards[9] | bitboards[10] | bitboards[11];
	occupancies[2] = occupancies[0] | occupancies[1];
	side = !side;
	turn--;
}
void Position::make_move(const int move) {
	const int piece_type = get_piece_type(move);
	const int from_square = get_from_square(move);
	const int to_square = get_to_square(move);
	const int captured_type = get_captured_type(move);
	const int promoted_type = get_promotion_type(move);

	const bool double_pawn_push = get_double_push_flag(move);
	const bool capture = get_capture_flag(move);
	const bool is_enpassant = get_enpassant_flag(move);
	const bool is_castle = get_castling_flag(move);

	const bool is_white_pawn = (piece_type == P);
	const bool is_black_pawn = (piece_type == p);
	moves_without_pawns = (!(is_white_pawn || is_black_pawn)) * (moves_without_pawns + 1);
	//branchlessly increment the counter if move was not a pawn move
	turn++;

	enpassant_square = (double_pawn_push) * ((is_black_pawn) * (to_square - 8) + (is_white_pawn) * (to_square + 8));
	//branchlessly set enpassant square

	const int offset = 6 * (side);

	pop_bit(bitboards[(capture)*captured_type], (capture) * (!is_enpassant) * to_square + (is_enpassant) * (to_square + 8 * ((!side) - (side))));
	//pop bit on captured square. if move was not a capture the a8 bit of the white pawn is poped. however white pawns should never be on the 8th rank anyway
	const bool is_promotion = promoted_type != 15;
	const int true_piece_type = (!(is_promotion)) * piece_type + (is_promotion) * (promoted_type);
	pop_bit(bitboards[piece_type], from_square);
	set_bit(bitboards[true_piece_type], to_square);
	//make move of piece
	const bool bK = (piece_type == k);
	const bool bR = (piece_type == r);
	pop_bit(castle, 4 - (bK || (bR && (from_square == a8)) || (to_square == a8)));
	pop_bit(castle, 4 - 2 * (bK || (bR && (from_square == h8)) || (to_square == h8)));
	const bool wK = (piece_type == K);
	const bool wR = (piece_type == R);
	pop_bit(castle, 4 - 3 * (wK || (wR && (from_square == a1)) || (to_square == a1)));
	pop_bit(castle, 4 - 4 * (wK || (wR && (from_square == h1)) || (to_square == h1)));
	//branchlessly pop castle right bits

	const int square_offset = 56 * (side);
	const bool is_kingside = to_square > from_square;
	const int rook_source = (is_kingside) * (h1 - square_offset) + (!is_kingside) * (a1 - square_offset);
	const int rook_target = from_square + (to_square == g1 - square_offset) - (to_square == c1 - square_offset);
	pop_bit(bitboards[(is_castle) * (piece_type - 2)], (is_castle)*rook_source);
	//pop rooks if move is castling
	bitboards[(is_castle) * (piece_type - 2)] |= ((is_castle) * (1ULL) << (rook_target));
	//set rook if move is castling
	castle &= ~(((is_castle) * 1ULL) << (2 * (piece_type == k)));
	castle &= ~(((is_castle) * 1ULL) << (1 + 2 * (piece_type == k)));
	//pop castle right bits if move is castling

	side = !side;
	occupancies[0] = (bitboards[0] | bitboards[1]) | (bitboards[2] | bitboards[3]) | (bitboards[4] | bitboards[5]);
	occupancies[1] = bitboards[6] | bitboards[7] | bitboards[8] | bitboards[9] | bitboards[10] | bitboards[11];
	occupancies[2] = occupancies[0] | occupancies[1];
	pawn_stack.push_back(moves_without_pawns);
	move_stack.push_back(move);
	castle_stack.push_back(castle);
	enpassant_stack.push_back(enpassant_square);
	hash_stack.push_back(current_hash);
	update_hash(move);
}

void Position::get_legal_moves(std::vector<int>* ret){
	ret->reserve(256);
	const int kingpos = bitscan(bitboards[K + (side) * 6]);
	const bool in_check = is_attacked_by(kingpos, !side);
	const U64 kings_queen_scope = get_queen_attacks(occupancies[both], kingpos);
	const U64 enemy_attacks = get_attacks_by(!side);

	if (in_check) {
		legal_in_check_move_generator(ret, kingpos, kings_queen_scope, enemy_attacks);
		return;
	}
	legal_move_generator(ret, kingpos, kings_queen_scope, enemy_attacks);
}
inline void Position::legal_move_generator(std::vector<int>* ret, const int kingpos, const U64 kings_queen_scope, const U64 enemy_attacks) {
	get_castles(ret);
	const U64 pinned = get_moves_for_pinned_pieces(ret, kingpos, enemy_attacks);
	U64 enemy_pieces = occupancies[(!side)];
	U64 valid_targets = (~occupancies[both]) | enemy_pieces;
	const int offset = (side) * 6;
	int type = R + offset;
	U64 tempRooks = bitboards[type] & (~pinned);
	int move;
	while (tempRooks) {
		U64 isolated = tempRooks & twos_complement(tempRooks);
		const int sq = bitscan(isolated);
		U64 attacks = get_rook_attacks(occupancies[both], sq) & valid_targets;

		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, (bool)(get_bit(enemy_pieces, to)), false, false, false);
			ret->push_back(move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempRooks = tempRooks & ones_decrement(tempRooks);
	}

	type = B + offset;
	U64 tempBishops = bitboards[type] & (~pinned);

	while (tempBishops) {
		U64 isolated = tempBishops & twos_complement(tempBishops);
		const int sq = bitscan(isolated);
		U64 attacks = get_bishop_attacks(occupancies[both], sq) & valid_targets;
		;
		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, (bool)(get_bit(enemy_pieces, to)), false, false, false);
			ret->push_back(move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempBishops = tempBishops & ones_decrement(tempBishops);
	}

	type = Q + offset;
	U64 tempQueens = bitboards[type] & (~pinned);

	while (tempQueens) {
		U64 isolated = tempQueens & twos_complement(tempQueens);
		const int sq = bitscan(isolated);
		U64 attacks = get_queen_attacks(occupancies[both], sq) & valid_targets;

		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, (bool)(get_bit(enemy_pieces, to)), false, false, false);
			ret->push_back(move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempQueens = tempQueens & ones_decrement(tempQueens);
	}

	type = N + offset;
	U64 tempKnights = bitboards[type] & (~pinned);

	while (tempKnights) {
		U64 isolated = tempKnights & twos_complement(tempKnights);
		const int sq = bitscan(isolated);
		U64 attacks = knight_attacks[sq] & valid_targets;

		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, (bool)(get_bit(enemy_pieces, to)), false, false, false);
			ret->push_back(move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempKnights = tempKnights & ones_decrement(tempKnights);
	}

	type = K + offset;
	U64 attacks = king_attacks[kingpos] & (~(occupancies[(side)] | enemy_attacks));
	while (attacks) {

		U64 isolated = attacks & twos_complement(attacks);

		const int to = bitscan(isolated);
		move = encode_move(kingpos, to, type, get_piece_type_on(to), 15, (bool)(get_bit(enemy_pieces, to)), false, false, false);
		ret->push_back(move);

		attacks = attacks & ones_decrement(attacks);
	}
	PINNED = pinned;
	get_legal_pawn_moves(ret, kings_queen_scope, enemy_attacks);
}
inline void Position::legal_in_check_move_generator(std::vector<int>* ret, const int kingpos, const U64 kings_queen_scope, const U64 enemy_attacks) {
	const U64 pinned = get_pinned_pieces(kingpos, enemy_attacks);
	PINNED = pinned;
	const U64 checkers = get_checkers(kingpos);
	const bool not_double_check = count_bits(checkers) < 2;
	CHECKERS = (not_double_check) * checkers;
	U64 valid_targets = (not_double_check) * (get_checking_rays(kingpos) | checkers);
	U64 test = (~occupancies[both]) | occupancies[(!side)];
	const int offset = (side) * 6;
	int type = R + offset;
	U64 tempRooks = bitboards[type] & ((~pinned) * (not_double_check));
	int move;
	while (tempRooks) {
		U64 isolated = tempRooks & twos_complement(tempRooks);
		const int sq = bitscan(isolated);
		U64 attacks = get_rook_attacks(occupancies[both], sq) & valid_targets;

		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, (bool)(get_bit(occupancies[(!side)], to)), false, false, false);
			ret->push_back(move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempRooks = tempRooks & ones_decrement(tempRooks);
	}

	type = B + offset;
	U64 tempBishops = bitboards[type] & ((~pinned) * (not_double_check));

	while (tempBishops) {
		U64 isolated = tempBishops & twos_complement(tempBishops);
		const int sq = bitscan(isolated);
		U64 attacks = get_bishop_attacks(occupancies[both], sq) & valid_targets;

		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, (bool)(get_bit(occupancies[(!side)], to)), false, false, false);
			ret->push_back(move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempBishops = tempBishops & ones_decrement(tempBishops);
	}

	type = Q + offset;
	U64 tempQueens = bitboards[type] & ((~pinned) * (not_double_check));

	while (tempQueens) {
		U64 isolated = tempQueens & twos_complement(tempQueens);
		const int sq = bitscan(isolated);
		U64 attacks = get_queen_attacks(occupancies[both], sq) & valid_targets;

		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, (bool)(get_bit(occupancies[(!side)], to)), false, false, false);
			ret->push_back(move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempQueens = tempQueens & ones_decrement(tempQueens);
	}

	type = N + offset;
	U64 tempKnights = bitboards[type] & ((~pinned) * (not_double_check));

	while (tempKnights) {
		U64 isolated = tempKnights & twos_complement(tempKnights);
		const int sq = bitscan(isolated);
		U64 attacks = knight_attacks[sq] & valid_targets;

		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, (bool)(get_bit(occupancies[(!side)], to)), false, false, false);
			ret->push_back(move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempKnights = tempKnights & ones_decrement(tempKnights);
	}

	IN_CHECK_VALID = (not_double_check)*valid_targets;
	type = K + offset;
	pop_bit(occupancies[both], kingpos);
	const U64 enemy_attacks_without_king = get_attacks_by(!side);
	set_bit(occupancies[both], kingpos);
	U64 attacks = king_attacks[kingpos] & (~(enemy_attacks_without_king | occupancies[(side)]));
	while (attacks) {

		U64 isolated = attacks & twos_complement(attacks);

		const int to = bitscan(isolated);
		move = encode_move(kingpos, to, type, get_piece_type_on(to), 15, (bool)(get_bit(occupancies[(!side)], to)), false, false, false);
		ret->push_back(move);

		attacks = attacks & ones_decrement(attacks);
	}
	in_check_get_legal_pawn_moves(ret, kings_queen_scope | knight_attacks[kingpos], enemy_attacks);
}

constexpr void Position::get_legal_pawn_moves(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks) {
	if (side) {
		legal_bpawn_pushes(ret, kings_queen_scope, enemy_attacks);
		legal_bpawn_captures(ret, kings_queen_scope, enemy_attacks);
		return;
	}
	legal_wpawn_pushes(ret, kings_queen_scope, enemy_attacks);
	legal_wpawn_captures(ret, kings_queen_scope, enemy_attacks);
}
inline void Position::legal_bpawn_pushes(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks) {
	U64 valid_targets = ~occupancies[both];
	U64 promoters = bitboards[6] & (~PINNED) & secondRank;
	U64 push_promotions = (promoters << 8) & valid_targets;
	U64 pushes = ((bitboards[6]&~(PINNED|promoters)) << 8) & valid_targets;
	U64 doublePushes = ((pushes << 8) & 0xFF000000ULL) & valid_targets;
	int move;
	while (push_promotions) {
		const U64 isolated = push_promotions & twos_complement(push_promotions);
		int ind = bitscan(isolated);
		move = encode_move(ind - 8, ind, p, 15, n, false, false, false, true);
		ret->push_back(move);
		set_promotion_type(move, b);
		ret->push_back(move);
		set_promotion_type(move, r);
		ret->push_back(move);
		set_promotion_type(move, q);
		ret->push_back(move);
		push_promotions = push_promotions & ones_decrement(push_promotions);
	}
	while (pushes) {
		U64 isolated = pushes & twos_complement(pushes);
		int ind = bitscan(isolated);
		move = encode_move(ind - 8, ind, p, 15, 15, false, false, false, false);
		ret->push_back(move);
		pushes = pushes & ones_decrement(pushes);
	}
	while (doublePushes) {
		U64 isolated = doublePushes & twos_complement(doublePushes);
		int ind = bitscan(isolated);
		move = encode_move(ind - 16, ind, p, 15, 15, false, true, false, false);
		ret->push_back(move);
		doublePushes = doublePushes & ones_decrement(doublePushes);
	}
}
inline void Position::legal_wpawn_pushes(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks) {
	U64 valid_targets = ~occupancies[both];
	U64 promoters = bitboards[0] & (~PINNED) & seventhRank;
	U64 push_promotions = (promoters >> 8) & valid_targets;
	U64 pushes = ((bitboards[0] & ~(PINNED | promoters)) >> 8) & valid_targets;
	U64 doublePushes = ((pushes >> 8) & 0xFF00000000ULL) & valid_targets;
	int move;
	while (push_promotions) {
		const U64 isolated = push_promotions & twos_complement(push_promotions);
		int ind = bitscan(isolated);
		move = encode_move(ind + 8, ind, P, 15, N, false, false, false, true);
		ret->push_back(move);
		set_promotion_type(move, B);
		ret->push_back(move);
		set_promotion_type(move, R);
		ret->push_back(move);
		set_promotion_type(move, Q);
		ret->push_back(move);
		push_promotions = push_promotions & ones_decrement(push_promotions);
	}
	while (pushes) {
		U64 isolated = pushes & twos_complement(pushes);
		int ind = bitscan(isolated);

		move = encode_move(ind + 8, ind, P, 15, 15, false, false, false, false);
		ret->push_back(move);
		pushes = pushes & ones_decrement(pushes);
	}
	while (doublePushes) {
		U64 isolated = doublePushes & twos_complement(doublePushes);
		int ind = bitscan(isolated);
		move = encode_move(ind + 16, ind, P, 15, 15, false, true, false, false);
		ret->push_back(move);
		doublePushes = doublePushes & ones_decrement(doublePushes);
	}
}
inline void Position::legal_bpawn_captures(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks) {
	const U64 targets = occupancies[white];
	U64 promoters = bitboards[6] & (~PINNED) & secondRank;
	U64 captures = ((bitboards[6] & ~(PINNED | promoters)) << 7) & notHFile & targets;

	U64 enpassant = 0ULL;
	set_bit(enpassant, enpassant_square);
	const bool left_enpassant = ((enpassant >> 7) & notAFile) & (bitboards[6] & ~(PINNED | promoters));
	const bool right_enpassant = ((enpassant >> 9) & notHFile) & (bitboards[6] & ~(PINNED | promoters));
	if (left_enpassant) {
		int move = encode_move(enpassant_square - 7, enpassant_square, p, P, 15, true, false, false, true);
		try_out_move(ret, move);
	}
	if (right_enpassant) {
		int move = encode_move(enpassant_square - 9, enpassant_square, p, P, 15, true, false, false, true);
		try_out_move(ret, move);
	}

	while (captures) {
		U64 isolated = captures & twos_complement(captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind - 7, ind, p, get_piece_type_on(ind), 15, true, false, false, false);
		ret->push_back(move);
		captures = captures & ones_decrement(captures);
	}
	captures = ((bitboards[6] & ~(PINNED | promoters)) << 9) & notAFile & targets;
	while (captures) {
		U64 isolated = captures & twos_complement(captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind - 9, ind, p, get_piece_type_on(ind), 15, true, false, false, false);
		ret->push_back(move);
		captures = captures & ones_decrement(captures);
	}

	U64 promotion_captures = ((promoters) << 7) & notHFile & targets;
	while (promotion_captures) {
		U64 isolated = promotion_captures & twos_complement(promotion_captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind - 7, ind, p, get_piece_type_on(ind), n, true, false, false, false);
		ret->push_back(move);
		set_promotion_type(move, b);
		ret->push_back(move);
		set_promotion_type(move, r);
		ret->push_back(move);
		set_promotion_type(move, q);
		ret->push_back(move);
		promotion_captures = promotion_captures & ones_decrement(promotion_captures);
	}
	promotion_captures = ((promoters) << 9) & notAFile & targets;
	while (promotion_captures) {
		U64 isolated = promotion_captures & twos_complement(promotion_captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind - 9, ind, p, get_piece_type_on(ind), n, true, false, false, false);
		ret->push_back(move);
		set_promotion_type(move, b);
		ret->push_back(move);
		set_promotion_type(move, r);
		ret->push_back(move);
		set_promotion_type(move, q);
		ret->push_back(move);
		promotion_captures = promotion_captures & ones_decrement(promotion_captures);
	}
}
inline void Position::legal_wpawn_captures(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks) {
	U64 promoters = bitboards[0] & (~PINNED) & seventhRank;
	const U64 targets = occupancies[black];
	U64 captures = ((bitboards[0] & ~(PINNED | promoters)) >> 7) & notAFile & targets;


	U64 enpassant = 0ULL;
	enpassant = (enpassant_square != a8) * (1ULL << enpassant_square);
	const bool left_enpassant = ((enpassant << 7) & notHFile) & (bitboards[0] & ~(PINNED | promoters));
	const bool right_enpassant = ((enpassant << 9) & notAFile) & (bitboards[0] & ~(PINNED | promoters));
	if (left_enpassant) {
		int move = encode_move(enpassant_square + 7, enpassant_square, P, p, 15, true, false, false, true);
		try_out_move(ret, move);
	}
	if (right_enpassant) {
		int move = encode_move(enpassant_square + 9, enpassant_square, P, p, 15, true, false, false, true);
		try_out_move(ret, move);
	}

	while (captures) {
		U64 isolated = captures & twos_complement(captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind + 7, ind, P, get_piece_type_on(ind), 15, true, false, false, false);
		ret->push_back(move);
		captures = captures & ones_decrement(captures);
	}
	captures = ((bitboards[0] & ~(PINNED | promoters)) >> 9) & notHFile & targets;
	while (captures) {
		U64 isolated = captures & twos_complement(captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind + 9, ind, P, get_piece_type_on(ind), 15, true, false, false, false);
		ret->push_back(move);
		captures = captures & ones_decrement(captures);
	}
	U64 promotion_captures = ((promoters) >> 7) & notAFile & targets;
	while (promotion_captures) {
		U64 isolated = promotion_captures & twos_complement(promotion_captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind + 7, ind, P, get_piece_type_on(ind), N, true, false, false, false);
		ret->push_back(move);
		set_promotion_type(move, B);
		ret->push_back(move);
		set_promotion_type(move, R);
		ret->push_back(move);
		set_promotion_type(move, Q);
		ret->push_back(move);
		promotion_captures = promotion_captures & ones_decrement(promotion_captures);
	}
	promotion_captures = ((promoters) >> 9) & notHFile & targets;
	while (promotion_captures) {
		U64 isolated = promotion_captures & twos_complement(promotion_captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind + 9, ind, P, get_piece_type_on(ind), N, true, false, false, false);
		ret->push_back(move);
		set_promotion_type(move, B);
		ret->push_back(move);
		set_promotion_type(move, R);
		ret->push_back(move);
		set_promotion_type(move, Q);
		ret->push_back(move);
		promotion_captures = promotion_captures & ones_decrement(promotion_captures);
	}
}

constexpr void Position::in_check_get_legal_pawn_moves(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks) {
	if (side) {
		in_check_legal_bpawn_pushes(ret, kings_queen_scope, enemy_attacks);
		in_check_legal_bpawn_captures(ret, kings_queen_scope, enemy_attacks);
		return;
	}
	in_check_legal_wpawn_pushes(ret, kings_queen_scope, enemy_attacks);
	in_check_legal_wpawn_captures(ret, kings_queen_scope, enemy_attacks);
}
inline void Position::in_check_legal_bpawn_pushes(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks) {
	const U64 valid_targets = ~occupancies[both];
	U64 promoters = bitboards[6] & (~PINNED) & secondRank;
	U64 push_promotions = (promoters << 8) & IN_CHECK_VALID & valid_targets;
	U64 pushes = ((bitboards[6] & ~(PINNED | promoters)) << 8) & valid_targets;
	U64 doublePushes = ((pushes << 8) & 0xFF000000ULL) & IN_CHECK_VALID & valid_targets;
	pushes = pushes & IN_CHECK_VALID & valid_targets;
	int move;
	while (push_promotions) {
		const U64 isolated = push_promotions & twos_complement(push_promotions);
		int ind = bitscan(isolated);
		move = encode_move(ind - 8, ind, p, 15, n, false, false, false, false);
		ret->push_back(move);
		set_promotion_type(move, b);
		ret->push_back(move);
		set_promotion_type(move, r);
		ret->push_back(move);
		set_promotion_type(move, q);
		ret->push_back(move);
		push_promotions = push_promotions & ones_decrement(push_promotions);
	}
	while (pushes) {
		U64 isolated = pushes & twos_complement(pushes);
		int ind = bitscan(isolated);
		move = encode_move(ind - 8, ind, p, 15, 15, false, false, false, false);
		ret->push_back(move);
		pushes = pushes & ones_decrement(pushes);
	}
	while (doublePushes) {
		U64 isolated = doublePushes & twos_complement(doublePushes);
		int ind = bitscan(isolated);
		move = encode_move(ind - 16, ind, p, 15, 15, false, true, false, false);
		ret->push_back(move);
		doublePushes = doublePushes & ones_decrement(doublePushes);
	}
}
inline void Position::in_check_legal_wpawn_pushes(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks) {
	U64 valid_targets = ~occupancies[both];
	U64 promoters = bitboards[0] & (~PINNED) & seventhRank;
	U64 push_promotions = (promoters >> 8) & IN_CHECK_VALID & valid_targets;
	U64 pushes = ((bitboards[0] & ~(PINNED | promoters)) >> 8) & valid_targets;
	U64 doublePushes = ((pushes >> 8) & 0xFF00000000ULL) & IN_CHECK_VALID & valid_targets;
	pushes = pushes & IN_CHECK_VALID & valid_targets;
	int move;
	while (push_promotions) {
		const U64 isolated = push_promotions & twos_complement(push_promotions);
		int ind = bitscan(isolated);
		move = encode_move(ind + 8, ind, P, 15, N, false, false, false, false);
		ret->push_back(move);
		set_promotion_type(move, B);
		ret->push_back(move);
		set_promotion_type(move, R);
		ret->push_back(move);
		set_promotion_type(move, Q);
		ret->push_back(move);
		push_promotions = push_promotions & ones_decrement(push_promotions);
	}
	while (pushes) {
		U64 isolated = pushes & twos_complement(pushes);
		int ind = bitscan(isolated);

		move = encode_move(ind + 8, ind, P, 15, 15, false, false, false, false);
		ret->push_back(move);
		pushes = pushes & ones_decrement(pushes);
	}
	while (doublePushes) {
		U64 isolated = doublePushes & twos_complement(doublePushes);
		int ind = bitscan(isolated);
		move = encode_move(ind + 16, ind, P, 15, 15, false, true, false, false);
		ret->push_back(move);
		doublePushes = doublePushes & ones_decrement(doublePushes);
	}
}
inline void Position::in_check_legal_bpawn_captures(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks) {
	const U64 targets = CHECKERS;
	U64 promoters = bitboards[6] & (~PINNED) & secondRank;
	U64 captures = ((bitboards[6] & (~(PINNED|promoters))) << 7) & notHFile & targets;

	U64 enpassant = 0ULL;
	set_bit(enpassant, enpassant_square);
	const bool left_enpassant = ((enpassant >> 7) & notAFile) & (bitboards[6] & ~(PINNED | promoters));
	const bool right_enpassant = ((enpassant >> 9) & notHFile) & (bitboards[6] & ~(PINNED | promoters));
	if (left_enpassant) {
		int move = encode_move(enpassant_square - 7, enpassant_square, p, P, 15, true, false, false, true);
		try_out_move(ret, move);
	}
	if (right_enpassant) {
		int move = encode_move(enpassant_square - 9, enpassant_square, p, P, 15, true, false, false, true);
		try_out_move(ret, move);
	}

	while (captures) {
		U64 isolated = captures & twos_complement(captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind - 7, ind, p, get_piece_type_on(ind), 15, true, false, false, false);
		ret->push_back(move);
		captures = captures & ones_decrement(captures);
	}
	captures = ((bitboards[6] & ~(PINNED | promoters)) << 9) & notAFile & targets;
	while (captures) {
		U64 isolated = captures & twos_complement(captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind - 9, ind, p, get_piece_type_on(ind), 15, true, false, false, false);
		ret->push_back(move);
		captures = captures & ones_decrement(captures);
	}

	U64 promotion_captures = ((promoters) << 7) & notHFile & targets;
	while (promotion_captures) {
		U64 isolated = promotion_captures & twos_complement(promotion_captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind - 7, ind, p, get_piece_type_on(ind), n, true, false, false, false);
		ret->push_back(move);
		set_promotion_type(move, b);
		ret->push_back(move);
		set_promotion_type(move, r);
		ret->push_back(move);
		set_promotion_type(move, q);
		ret->push_back(move);
		promotion_captures = promotion_captures & ones_decrement(promotion_captures);
	}
	promotion_captures = ((promoters) << 9) & notAFile & targets;
	while (promotion_captures) {
		U64 isolated = promotion_captures & twos_complement(promotion_captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind - 9, ind, p, get_piece_type_on(ind), n, true, false, false, false);
		ret->push_back(move);
		set_promotion_type(move, b);
		ret->push_back(move);
		set_promotion_type(move, r);
		ret->push_back(move);
		set_promotion_type(move, q);
		ret->push_back(move);
		promotion_captures = promotion_captures & ones_decrement(promotion_captures);
	}
}
inline void Position::in_check_legal_wpawn_captures(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks) {
	const U64 targets = CHECKERS;
	U64 promoters = bitboards[0] & (~PINNED) & seventhRank;
	U64 captures = ((bitboards[0] & (~(PINNED | promoters))) >> 7) & notAFile & targets;

	U64 enpassant = (enpassant_square != a8) * (1ULL << enpassant_square);
	const bool left_enpassant = ((enpassant << 7) & notHFile) & (bitboards[0] & ~(PINNED | promoters));
	const bool right_enpassant = ((enpassant << 9) & notAFile) & (bitboards[0] & ~(PINNED | promoters));
	if (left_enpassant) {
		int move = encode_move(enpassant_square + 7, enpassant_square, P, p, 15, true, false, false, true);
		try_out_move(ret, move);
	}
	if (right_enpassant) {
		int move = encode_move(enpassant_square + 9, enpassant_square, P, p, 15, true, false, false, true);
		try_out_move(ret, move);
	}

	while (captures) {
		U64 isolated = captures & twos_complement(captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind + 7, ind, P, get_piece_type_on(ind), 15, true, false, false, false);
		ret->push_back(move);
		captures = captures & ones_decrement(captures);
	}
	captures = ((bitboards[0] & ~(PINNED | promoters)) >> 9) & notHFile & targets;
	while (captures) {
		U64 isolated = captures & twos_complement(captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind + 9, ind, P, get_piece_type_on(ind), 15, true, false, false, false);
		ret->push_back(move);
		captures = captures & ones_decrement(captures);
	}
	U64 promotion_captures = ((promoters) >> 7) & notAFile & targets;
	while (promotion_captures) {
		U64 isolated = promotion_captures & twos_complement(promotion_captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind + 7, ind, P, get_piece_type_on(ind), N, true, false, false, false);
		ret->push_back(move);
		set_promotion_type(move, B);
		ret->push_back(move);
		set_promotion_type(move, R);
		ret->push_back(move);
		set_promotion_type(move, Q);
		ret->push_back(move);
		promotion_captures = promotion_captures & ones_decrement(promotion_captures);
	}
	promotion_captures = ((promoters) >> 9) & notHFile & targets;
	while (promotion_captures) {
		U64 isolated = promotion_captures & twos_complement(promotion_captures);
		const int ind = bitscan(isolated);
		int move = encode_move(ind + 9, ind, P, get_piece_type_on(ind), N, true, false, false, false);
		ret->push_back(move);
		set_promotion_type(move, B);
		ret->push_back(move);
		set_promotion_type(move, R);
		ret->push_back(move);
		set_promotion_type(move, Q);
		ret->push_back(move);
		promotion_captures = promotion_captures & ones_decrement(promotion_captures);
	}
}

inline void Position::try_out_move(std::vector<int>* ret, int& move){
	make_move(move);
	if (!is_attacked_by(bitscan(bitboards[11 - (side) * 6]), side)) ret->push_back(move);
	//append move if the king is not attacked after playing the move
	unmake_move();
}

void Position::get_legal_captures(std::vector<int>* ret) {
	ret->reserve(256);
	const int kingpos = bitscan(bitboards[K + (side) * 6]);
	const bool in_check = is_attacked_by(kingpos, !side);
	const U64 kings_queen_scope = get_queen_attacks(occupancies[both], kingpos);
	const U64 enemy_attacks = get_attacks_by(!side);
	if (in_check) {
		legal_in_check_capture_gen(ret, kings_queen_scope, enemy_attacks);
		return;
	}
	legal_capture_gen(ret, kings_queen_scope, enemy_attacks);
}
inline void Position::legal_capture_gen(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks) {
	const int offset = (side) * 6;
	const int kingpos = bitscan(bitboards[K + offset]);
	const U64 pinned = get_captures_for_pinned_pieces(ret, kingpos, enemy_attacks);
	PINNED = pinned;
	const U64 enemy_pieces = occupancies[(!side)];
	int type = R + offset;
	U64 tempRooks = bitboards[type] & (~pinned);
	int move;
	while (tempRooks) {
		U64 isolated = tempRooks & twos_complement(tempRooks);
		const int sq = bitscan(isolated);
		U64 attacks = get_rook_attacks(occupancies[both], sq) & enemy_pieces;

		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, true, false, false, false);
			ret->push_back(move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempRooks = tempRooks & ones_decrement(tempRooks);
	}

	type = B + offset;
	U64 tempBishops = bitboards[type] & (~pinned);

	while (tempBishops) {
		U64 isolated = tempBishops & twos_complement(tempBishops);
		const int sq = bitscan(isolated);
		U64 attacks = get_bishop_attacks(occupancies[both], sq) & enemy_pieces;
		;
		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, true, false, false, false);
			ret->push_back(move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempBishops = tempBishops & ones_decrement(tempBishops);
	}

	type = Q + offset;
	U64 tempQueens = bitboards[type] & (~pinned);

	while (tempQueens) {
		U64 isolated = tempQueens & twos_complement(tempQueens);
		const int sq = bitscan(isolated);
		U64 attacks = get_queen_attacks(occupancies[both], sq) & enemy_pieces;

		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, true, false, false, false);
			ret->push_back(move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempQueens = tempQueens & ones_decrement(tempQueens);
	}

	type = N + offset;
	U64 tempKnights = bitboards[type] & (~pinned);

	while (tempKnights) {
		U64 isolated = tempKnights & twos_complement(tempKnights);
		const int sq = bitscan(isolated);
		U64 attacks = knight_attacks[sq] & enemy_pieces;

		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, true, false, false, false);
			ret->push_back(move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempKnights = tempKnights & ones_decrement(tempKnights);
	}
	type = K + offset;
	U64 tempKing = bitboards[type];

	U64 attacks = king_attacks[kingpos] & (enemy_pieces & (~enemy_attacks));

	while (attacks) {

		U64 isolated2 = attacks & twos_complement(attacks);

		const int to = bitscan(isolated2);
		move = encode_move(kingpos, to, type, get_piece_type_on(to), 15, true, false, false, false);
		ret->push_back(move);

		attacks = attacks & ones_decrement(attacks);
	}
	get_pawn_captures(ret, kings_queen_scope, enemy_attacks);
}
inline void Position::legal_in_check_capture_gen(std::vector<int>* ret, const U64 kings_queen_scope, const U64 enemy_attacks) {
	const int offset = (side) * 6;
	const int kingpos = bitscan(bitboards[K + offset]);
	const U64 pinned = get_pinned_pieces(kingpos, enemy_attacks);
	const U64 checkers = get_checkers(kingpos);
	const bool not_double_check = count_bits(checkers) < 2;
	PINNED = pinned;
	CHECKERS = (not_double_check)*checkers;

	int type = R + offset;
	U64 tempRooks = bitboards[type] & ((~pinned) * (not_double_check));
	int move;
	while (tempRooks) {
		U64 isolated = tempRooks & twos_complement(tempRooks);
		const int sq = bitscan(isolated);
		U64 attacks = get_rook_attacks(occupancies[both], sq) & checkers;

		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, true, false, false, false);
			try_out_move(ret, move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempRooks = tempRooks & ones_decrement(tempRooks);
	}

	type = B + offset;
	U64 tempBishops = bitboards[type] & ((~pinned) * (not_double_check));

	while (tempBishops) {
		U64 isolated = tempBishops & twos_complement(tempBishops);
		const int sq = bitscan(isolated);
		U64 attacks = get_bishop_attacks(occupancies[both], sq) & checkers;
		;
		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, true, false, false, false);
			try_out_move(ret, move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempBishops = tempBishops & ones_decrement(tempBishops);
	}

	type = Q + offset;
	U64 tempQueens = bitboards[type] & ((~pinned) * (not_double_check));

	while (tempQueens) {
		U64 isolated = tempQueens & twos_complement(tempQueens);
		const int sq = bitscan(isolated);
		U64 attacks = get_queen_attacks(occupancies[both], sq) & checkers;

		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, true, false, false, false);
			try_out_move(ret, move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempQueens = tempQueens & ones_decrement(tempQueens);
	}

	type = N + offset;
	U64 tempKnights = bitboards[type] & ((~pinned) * (not_double_check));

	while (tempKnights) {
		U64 isolated = tempKnights & twos_complement(tempKnights);
		const int sq = bitscan(isolated);
		U64 attacks = knight_attacks[sq] & checkers;

		while (attacks) {

			U64 isolated2 = attacks & twos_complement(attacks);

			const int to = bitscan(isolated2);
			move = encode_move(sq, to, type, get_piece_type_on(to), 15, true, false, false, false);
			try_out_move(ret, move);

			attacks = attacks & ones_decrement(attacks);
		}
		tempKnights = tempKnights & ones_decrement(tempKnights);
	}
	type = K + offset;
	U64 tempKing = bitboards[type];

	IN_CHECK_VALID = (not_double_check) * (get_checking_rays(kingpos) | checkers);
	type = K + offset;
	pop_bit(occupancies[both], kingpos);
	const U64 enemy_attacks_without_king = get_attacks_by(!side);
	set_bit(occupancies[both], kingpos);
	U64 attacks = (king_attacks[kingpos] & occupancies[(!side)]) & (~(enemy_attacks_without_king | occupancies[(side)]));
	while (attacks) {

		U64 isolated = attacks & twos_complement(attacks);

		const int to = bitscan(isolated);
		move = encode_move(kingpos, to, type, get_piece_type_on(to), 15, true, false, false, false);
		ret->push_back(move);

		attacks = attacks & ones_decrement(attacks);
	}
	in_check_get_pawn_captures(ret, kings_queen_scope, enemy_attacks);
}
inline U64 Position::get_attacks_by(const bool color) {
	int offset = (color) * 6;
	U64 attacks = ((bool)(bitboards[K+offset])) * king_attacks[bitscan(bitboards[K + offset])];
	int type = N + offset;
	U64 tempKnights = bitboards[type];

	while (tempKnights) {
		U64 isolated = tempKnights & twos_complement(tempKnights);
		attacks |= knight_attacks[bitscan(isolated)];
		tempKnights = tempKnights & ones_decrement(tempKnights);
	}
	//knight attacks individually

	attacks |= (!color) * (((bitboards[0] >> 7) & notAFile) | ((bitboards[0] >> 9) & notHFile)) + (color) * (((bitboards[6] << 7) & notHFile) | ((bitboards[6] << 9) & notAFile));
	//setwise pawn attacks

	const U64 rooks_w_queens = bitboards[R + offset] | bitboards[Q + offset];
	U64 rooks = rooks_w_queens;
	U64 flood = rooks_w_queens;
	U64 empty = ~occupancies[both];
	flood |= rooks = (rooks >> 8) & empty;
	flood |= rooks = (rooks >> 8) & empty;
	flood |= rooks = (rooks >> 8) & empty;
	flood |= rooks = (rooks >> 8) & empty;
	flood |= rooks = (rooks >> 8) & empty;
	flood |= (rooks >> 8) & empty;
	attacks |= flood >> 8;

	rooks = rooks_w_queens;
	flood = rooks_w_queens;
	flood |= rooks = (rooks << 8) & empty;
	flood |= rooks = (rooks << 8) & empty;
	flood |= rooks = (rooks << 8) & empty;
	flood |= rooks = (rooks << 8) & empty;
	flood |= rooks = (rooks << 8) & empty;
	flood |= (rooks << 8) & empty;
	attacks |= flood << 8;

	rooks = rooks_w_queens;
	flood = rooks_w_queens;
	empty &= notAFile;
	flood |= rooks = (rooks << 1) & empty;
	flood |= rooks = (rooks << 1) & empty;
	flood |= rooks = (rooks << 1) & empty;
	flood |= rooks = (rooks << 1) & empty;
	flood |= rooks = (rooks << 1) & empty;
	flood |= (rooks << 1) & empty;
	attacks |= (flood << 1) & notAFile;

	const U64 bishops_w_queens = bitboards[B + offset] | bitboards[Q + offset];
	U64 bishops = bishops_w_queens;
	flood = bishops_w_queens;
	flood |= bishops = (bishops >> 7) & empty;
	flood |= bishops = (bishops >> 7) & empty;
	flood |= bishops = (bishops >> 7) & empty;
	flood |= bishops = (bishops >> 7) & empty;
	flood |= bishops = (bishops >> 7) & empty;
	flood |= (bishops >> 7) & empty;
	attacks |= (flood >> 7) & notAFile;

	bishops = bishops_w_queens;
	flood = bishops_w_queens;
	flood |= bishops = (bishops << 9) & empty;
	flood |= bishops = (bishops << 9) & empty;
	flood |= bishops = (bishops << 9) & empty;
	flood |= bishops = (bishops << 9) & empty;
	flood |= bishops = (bishops << 9) & empty;
	flood |= (bishops << 9) & empty;
	attacks |= (flood << 9) & notAFile;


	rooks = rooks_w_queens;
	flood = rooks_w_queens;
	empty = ~occupancies[both] & notHFile;
	flood |= rooks = (rooks >> 1) & empty;
	flood |= rooks = (rooks >> 1) & empty;
	flood |= rooks = (rooks >> 1) & empty;
	flood |= rooks = (rooks >> 1) & empty;
	flood |= rooks = (rooks >> 1) & empty;
	flood |= (rooks >> 1) & empty;
	attacks |= (flood >> 1) & notHFile;

	bishops = bishops_w_queens;
	flood = bishops_w_queens;
	flood |= bishops = (bishops >> 9) & empty;
    flood |= bishops = (bishops >> 9) & empty;
    flood |= bishops = (bishops >> 9) & empty;
    flood |= bishops = (bishops >> 9) & empty;
    flood |= bishops = (bishops >> 9) & empty;
    flood |= (bishops >> 9) & empty;
	attacks |= (flood >> 9) & notHFile;

	bishops = bishops_w_queens;
	flood = bishops_w_queens;
	flood |= bishops = (bishops << 7) & empty;
	flood |= bishops = (bishops << 7) & empty;
	flood |= bishops = (bishops << 7) & empty;
	flood |= bishops = (bishops << 7) & empty;
	flood |= bishops = (bishops << 7) & empty;
	flood |= (bishops << 7) & empty;
	attacks |= (flood << 7) & notHFile;

	return attacks;
}

inline void Position::get_pawn_captures(std::vector<int>* ptr, const U64 kings_queen_scope, const U64 enemy_attacks) {
	if (side) {
		legal_bpawn_captures(ptr, kings_queen_scope, enemy_attacks);
		return;
	}
	legal_wpawn_captures(ptr, kings_queen_scope, enemy_attacks);
}
inline void Position::in_check_get_pawn_captures(std::vector<int>* ptr, const U64 kings_queen_scope, const U64 enemy_attacks) {
	if (side) {
		in_check_legal_bpawn_captures(ptr, kings_queen_scope, enemy_attacks);
		return;
	}
	in_check_legal_wpawn_captures(ptr, kings_queen_scope, enemy_attacks);
}

int char_pieces(const char piece) {
	switch (piece) {
	case 'P':return P;
	case 'N':return N;
	case 'B':return B;
	case 'R':return R;
	case 'Q':return Q;
	case 'K':return K;
	case 'p':return p;
	case 'n':return n;
	case 'b':return b;
	case 'r':return r;
	case 'q':return q;
	case 'k':return k;
	default:return -1;
	}
};
void print_position(Position& pos) {
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
					if (get_bit(pos.bitboards[piece_on_square], square)) {
						piece = piece_on_square;
						break;
					}
				}
				printf(" %c", (piece == -1) ? ' .' : ascii_pieces[piece]);
			}
			printf("\n");
		}
		printf("\n    a b c d e f g h \n\n");
		printf("    To move: %s\n", (pos.side) ? "black" : "white");
		printf("    enpassant square: %s\n", (pos.enpassant_square != a8) ? square_coordinates[pos.enpassant_square].c_str() : "none");

		printf("    castling rights: %c%c%c%c\n", (get_bit(pos.castle, 0)) ? 'K' : '-', (get_bit(pos.castle, 1)) ? 'Q' : '-', (get_bit(pos.castle, 2)) ? 'k' : '-', (get_bit(pos.castle, 3)) ? 'q' : '-');
		//print castling rights

		printf("    halfmoves since last pawn move: %d\n", pos.moves_without_pawns);
		printf("    current halfclock turn: %d\n", pos.turn);
		std::cout << "    current hash: " << pos.current_hash << "\n";
}