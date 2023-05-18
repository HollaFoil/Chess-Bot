#include "Constants.h"
#include "BitUtils.h"
#include "State.h"
#include <stdint.h>
#include <bit>
#include <bitset>
#include <array>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <map>

#define square std::array<std::array<uint8_t, 8>, 8>
 #define squareULL std::array<std::array<ULL, 8>, 8>


ULL State::GetBishopMoves(uint8_t index) {
	ULL mask = (BitBoards::BishopMask[index] & blockers) * BitBoards::BishopMagics[index];
	ULL id = mask >> (64 - BitBoards::BishopRellevantBits[index]);
	ULL moves = BitBoards::BishopTable[index][id];
	return moves;
}
ULL State::GetRookMoves(uint8_t index) {
	ULL mask = (BitBoards::RookMask[index] & blockers) * BitBoards::RookMagics[index];
	ULL id = mask >> (64 - BitBoards::RookRellevantBits[index]);
	ULL moves = BitBoards::RookTable[index][id];
	return moves;
}
ULL State::GetQueenMoves(uint8_t index) {
	ULL moves = GetBishopMoves(index) | GetRookMoves(index);
	return moves;
}
ULL State::GetPawnPush(uint8_t row, uint8_t coll, uint8_t index) {
	//if (1ULL << (63index + 8) & blockers) return 0ULL;
	/// BAD !!!! ------------------------------------------
	ULL pawnPushMask = BitBoards::PawnTableMoves[(board[row][coll] & WHITE) ? 0 : 1][index];
	ULL pawnPushMoves = pawnPushMask & (pawnPushMask ^ blockers);
	return pawnPushMoves;
}
ULL State::GetPawnCaptures(uint8_t row, uint8_t coll, uint8_t index) {
	ULL pawnCaptureMask = BitBoards::PawnTableCaptures[(board[row][coll] & WHITE ? 0 : 1)][index];
	
	return pawnCaptureMask;
}
ULL State::GetPawnMoves(uint8_t row, uint8_t coll, uint8_t index) {
	ULL pawnCaptureMask = GetPawnCaptures(row, coll, index);
	ULL pawnCaptureMoves = pawnCaptureMask & (blockers); //| enPassant);
	return  pawnCaptureMoves | GetPawnPush(row, coll, index);
}
ULL State::GetKnightMoves(uint8_t index) {
	return BitBoards::KnightTable[index];
}
ULL State::GetKingCastling(uint8_t index) {
	ULL castling = 0ULL;
	int color = (whiteBlockers & (1ULL << 63 - index)) ? WHITE : BLACK;
	if (color == WHITE && canWhiteCastleKing && (blockers & CASTLE_KING_AFFECTED_MASK_WHITE) == (whiteKing | WHITE_ROOK_KING)) castling |= whiteKing >> 2;
	if (color == WHITE && canWhiteCastleQueen && (blockers & CASTLE_QUEEN_AFFECTED_MASK_WHITE) == (whiteKing | WHITE_ROOK_QUEEN)) castling |= whiteKing << 2;
	if (color == BLACK && canBlackCastleKing && (blockers & CASTLE_KING_AFFECTED_MASK_BLACK) == (blackKing | BLACK_ROOK_KING)) castling |= blackKing >> 2;
	if (color == BLACK && canBlackCastleQueen && (blockers & CASTLE_QUEEN_AFFECTED_MASK_BLACK) == (blackKing | BLACK_ROOK_QUEEN)) castling |= blackKing << 2;
	return castling;
}
ULL State::GetKingMoves(uint8_t index) {
	
	return BitBoards::KingTable[index];
}

void State::Save() {
	GenerateAttackTable(whiteToMove ? BLACK : WHITE);
	blockersCopy = blockers;
	whiteBlockersCopy = whiteBlockers;
	blackBlockersCopy = blackBlockers;
	pinnedCopy = pinned;
	whiteKingCopy = whiteKing;
	blackKingCopy = blackKing;
	attackTableCopy = attackTable;

	slidingPiecesWhite = 0ULL;
	slidingPiecesBlack = 0ULL;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((board[i][j] & (ROOK|QUEEN|BISHOP)) == 0) continue;
			if (board[i][j] & WHITE) slidingPiecesWhite |= 1ULL << (63 - (i<<3) - j);
			else slidingPiecesBlack |= 1ULL << (63 - (i << 3) - j);
		}
	}
}
void State::Restore() {
	blockers = blockersCopy;
	whiteBlockers = whiteBlockersCopy;
	blackBlockers = blackBlockersCopy;
	pinned = pinnedCopy;
	whiteKing = whiteKingCopy;
	blackKing = blackKingCopy;
	attackTable = attackTableCopy;
}

void State::GenerateDefault() {
	board = { { {ROOK | WHITE, BISHOP | WHITE, KNIGHT | WHITE, KING | WHITE, QUEEN | WHITE, KNIGHT | WHITE, BISHOP | WHITE, ROOK | WHITE},
				{PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE, PAWN | WHITE},
				{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
				{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
				{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
				{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
				{PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK, PAWN | BLACK},
				{ROOK | BLACK, BISHOP | BLACK, KNIGHT | BLACK, KING | BLACK, QUEEN | BLACK, KNIGHT | BLACK, BISHOP | BLACK, ROOK | BLACK}} };
	whiteToMove = true;
	inCheck = false;
	blockers = 0xffff00000000ffffULL;
	whiteBlockers = 0x000000000000ffffULL;
	blackBlockers = 0xffff000000000000ULL;
}

void State::GetLegalMoves() {
	ULL opponentSlidingPieces = whiteToMove ? slidingPiecesBlack : slidingPiecesWhite;
	while (opponentSlidingPieces != 0ULL) {

	}
}

int GetPieceID(uint8_t piece) {
	uint8_t id = 0;
	if (piece & PAWN) {
		id = 0;
	}
	else if (piece & BISHOP) {
		id = 1;
	}
	else if (piece & ROOK) {
		id = 2;
	}
	else if (piece & QUEEN) {
		id = 3;
	}
	else if (piece & KNIGHT) {
		id = 4;
	}
	else if (piece & KING) {
		id = 5;
	}
	if (piece & BLACK) piece += 6;
	return id;
}


void State::LoadFromFen(std::string fen) {
	size_t pos = 0;
	std::unordered_map<char, uint8_t> pieces = { {'p', PAWN | BLACK},
											 {'b', BISHOP | BLACK},
											 {'k', KING | BLACK},
											 {'n', KNIGHT | BLACK},
											 {'q', QUEEN | BLACK},
											 {'r', ROOK | BLACK},
											 {'P', PAWN | WHITE},
											 {'B', BISHOP | WHITE},
											 {'K', KING | WHITE},
											 {'N', KNIGHT | WHITE},
											 {'Q', QUEEN | WHITE},
											 {'R', ROOK | WHITE}, };
	std::vector<std::string> parts;
	while ((pos = fen.find(" ")) != std::string::npos) {
		parts.push_back(fen.substr(0, pos));
		fen.erase(0, pos + 1);
	}
	uint8_t row = 0;
	uint8_t coll = 0;
	for (int i = 0; i < parts[0].size(); i++) {
		char c = parts[0][i];
		if (c == '/') {
			coll = 0;
			row++;
			continue;
		}
		if (pieces.contains(c)) {
			board[row][coll] = pieces[c];
			coll++;
			continue;
		}
		for (int j = 0; j < c - '0'; j++) {
			board[row][coll] = EMPTY;
			coll++;
		}
	}
	whiteBlockers = 0;
	blackBlockers = 0;
	blockers = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == EMPTY) continue;
			uint8_t index = 63 - (i * 8 + j);
			blockers |= (1ULL << index);
			if (board[i][j] & WHITE) whiteBlockers |= (1ULL << index);
			else blackBlockers |= (1ULL << index);
			if (board[i][j] == (WHITE | KING)) whiteKing = 1ULL << index;
			else if (board[i][j] == (BLACK | KING)) blackKing = 1ULL << index;
		}
	}
	blockers = whiteBlockers | blackBlockers;
	whiteToMove = parts[1] == "w" ? true : false;
	canWhiteCastleKing = false;
	canWhiteCastleQueen = false;
	canBlackCastleKing = false;
	canBlackCastleQueen = false;
	if (parts[2].find('K') != std::string::npos) canWhiteCastleKing = true;
	if (parts[2].find('Q') != std::string::npos) canWhiteCastleQueen = true;
	if (parts[2].find('k') != std::string::npos) canBlackCastleKing = true;
	if (parts[2].find('q') != std::string::npos) canBlackCastleQueen = true;
	if (parts[3] != "-") {
		int enpassantIndex = parts[3][0] - 'a' + 8 * (8 - (parts[3][1] - '0'));
		enPassant = 1ULL << (63 - enpassantIndex);
	}
	else enPassant = 0ULL;
	Eval();
	Save();
}

void State::CalculateHash() {
	hash = whiteToMove ? 0 : BitBoards::ZobristBlackToMove;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == 0) continue;
			uint8_t piece = board[i][j];
			uint8_t id = GetPieceID(piece);
			hash ^= BitBoards::ZobristNumbers[id][i * 8 + j];
		}
	}
}


void State::SetPiece(ULL from, ULL to) {
	uint8_t color = whiteToMove ? WHITE : BLACK;
	uint8_t fromIndex = 63 - getFirstSetBitPos(from);
	uint8_t toIndex = 63 - getFirstSetBitPos(to);
	uint8_t toRow = toIndex >> 3, toColl = toIndex & 0b111;
	uint8_t fromRow = fromIndex >> 3, fromColl = fromIndex & 0b111;
	uint8_t pieceTo = board[toRow][toColl];
	uint8_t pieceFrom = board[fromRow][fromColl];
	uint8_t newPiece = pieceFrom;
	if (pieceFrom == (WHITE | PAWN) && (to & RANK_8)) newPiece = WHITE | QUEEN;
	else if (pieceFrom == (BLACK | PAWN) && (to & RANK_1)) newPiece = BLACK | QUEEN;
	if (pieceTo & BLACK) {
		blackBlockers ^= to;
		hash ^= BitBoards::ZobristNumbers[GetPieceID(pieceTo)][toIndex];
	}
	else if (pieceTo & WHITE) {
		whiteBlockers ^= to;
		hash ^= BitBoards::ZobristNumbers[GetPieceID(pieceTo)][toIndex];
	}
	if (pieceFrom == (WHITE | PAWN) && to == enPassant) {
		blackBlockers ^= (to << 8);
		hash ^= BitBoards::ZobristNumbers[GetPieceID(BLACK | PAWN)][toIndex + 8];
		board[toRow + 1][toColl] = EMPTY;
		eval -= GetPieceValue(BLACK | PAWN, toIndex + 8, WHITE);
	}
	else if (pieceFrom == (BLACK | PAWN) && to == enPassant) {
		whiteBlockers ^= (to >> 8);
		hash ^= BitBoards::ZobristNumbers[GetPieceID(WHITE | PAWN)][toIndex - 8];
		board[toRow - 1][toColl] = EMPTY;
		eval -= GetPieceValue(WHITE | PAWN, toIndex - 8, BLACK);
	}
	if (pieceFrom & BLACK) {
		blackBlockers ^= from;
		blackBlockers ^= to;
	}
	else if (pieceFrom & WHITE) {
		whiteBlockers ^= from;
		whiteBlockers ^= to;
	}
	eval -= GetPieceValue(pieceFrom, fromIndex, color);
	eval -= GetPieceValue(pieceTo, toIndex, color);
	eval += GetPieceValue(newPiece, toIndex, color);
	board[toRow][toColl] = newPiece;
	board[fromRow][fromColl] = EMPTY;
	hash ^= BitBoards::ZobristNumbers[GetPieceID(newPiece)][fromIndex];
	hash ^= BitBoards::ZobristNumbers[GetPieceID(newPiece)][toIndex];
	hash ^= BitBoards::ZobristBlackToMove;
	blockers = blackBlockers | whiteBlockers;
	if (pieceFrom == (KING | WHITE)) {
		whiteKing = to;
		canWhiteCastleKing = false;
		canWhiteCastleQueen = false;
	}
	else if (pieceFrom == (KING | BLACK)) {
		blackKing = to;
		canBlackCastleKing = false;
		canBlackCastleQueen = false;
	}
	if ((from | to) & WHITE_ROOK_KING) canWhiteCastleKing = false;
	if ((from | to) & WHITE_ROOK_QUEEN) canWhiteCastleQueen = false;
	if ((from | to) & BLACK_ROOK_KING) canBlackCastleKing = false;
	if ((from | to) & BLACK_ROOK_QUEEN) canBlackCastleQueen = false;
	if (pieceFrom != EMPTY) prevCapture = true;
}

void printsquare(square& num) {
	ULL mask = 1ULL << 63;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int p = num[i][j];
			if (p == EMPTY) std::cout << '.' << " ";
			if (p == (KING | WHITE)) std::cout << 'K' << " ";
			if (p == (KING | BLACK)) std::cout << 'k' << " ";
			if (p == (QUEEN | WHITE)) std::cout << 'Q' << " ";
			if (p == (QUEEN | BLACK)) std::cout << 'q' << " ";
			if (p == (PAWN | WHITE)) std::cout << 'P' << " ";
			if (p == (PAWN | BLACK)) std::cout << 'p' << " ";
			if (p == (KNIGHT | WHITE)) std::cout << 'N' << " ";
			if (p == (KNIGHT | BLACK)) std::cout << 'n' << " ";
			if (p == (BISHOP | WHITE)) std::cout << 'B' << " ";
			if (p == (BISHOP | BLACK)) std::cout << 'b' << " ";
			if (p == (ROOK | WHITE)) std::cout << 'R' << " ";
			if (p == (ROOK | BLACK)) std::cout << 'r' << " ";
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

std::pair<ULL, ULL> State::GetPinnedPiece(ULL king, ULL slidingPiece) {
	uint8_t kpos = 63 - getFirstSetBitPos(king);
	uint8_t ppos = 63 - getFirstSetBitPos(slidingPiece);
	uint8_t krow = kpos >> 3, kcoll = kpos & 0b111;
	uint8_t prow = ppos >> 3, pcoll = ppos & 0b111;
	ULL RookMovesPiece = State::GetRookMoves(ppos);
	ULL BishopMovesPiece = State::GetBishopMoves(ppos);
	ULL RookMovesKing = State::GetRookMoves(kpos);
	ULL BishopMovesKing = State::GetBishopMoves(kpos);

	ULL pinBlockers = king == whiteKing ? whiteBlockers : blackBlockers;

	ULL pinnedPiece;
	if (krow == prow || kcoll == pcoll) {
		ULL mask = RookMovesPiece & RookMovesKing & blockers;
		if (!(board[prow][pcoll]) & (ROOK|QUEEN)) return std::make_pair(0, 0);
		if (!(blockers & pinBlockers)) return std::make_pair(0, 0);
		if (!isPowOfTwo(mask)) return std::make_pair(0, 0);
		ULL pieceMoves = 0;
		uint8_t pieceIndex = 63 - getFirstSetBitPos(mask);
		if (board[pieceIndex >> 3][pieceIndex & 0b111] & (ROOK | QUEEN)) pieceMoves = (RookMovesPiece & RookMovesKing) | slidingPiece;
		return std::make_pair(mask, pieceMoves);
	}
	uint8_t diffRow = krow > prow ? krow - prow : prow - krow;
	uint8_t diffColl = kcoll > pcoll ? kcoll - pcoll : pcoll - kcoll;
	if (diffRow == diffColl) {
		ULL mask = BishopMovesPiece & BishopMovesKing & blockers;
		if (!(board[prow][pcoll]) & (BISHOP | QUEEN)) return std::make_pair(0, 0);
		if (!(blockers & pinBlockers)) return std::make_pair(0, 0);
		if (!isPowOfTwo(mask)) return std::make_pair(0, 0);
		ULL pieceMoves = 0;
		uint8_t pieceIndex = 63 - getFirstSetBitPos(mask);
		if (board[pieceIndex >> 3][pieceIndex & 0b111] & (BISHOP | QUEEN)) pieceMoves = (BishopMovesPiece & BishopMovesKing) | slidingPiece;
		return std::make_pair(mask, pieceMoves);
	}
	return std::make_pair(0, 0);
}

State State::MutateCopy(ULL from, ULL to) {
	int evalbefore = eval;
	State next;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			next.board[i][j] = board[i][j];
		}
	}
	next.canBlackCastleKing = canBlackCastleKing;
	next.canBlackCastleQueen = canBlackCastleQueen;
	next.canWhiteCastleKing = canWhiteCastleKing;
	next.canWhiteCastleQueen = canWhiteCastleQueen;
	next.whiteKing = whiteKing;
	next.blackKing = blackKing;
	next.blackBlockers = blackBlockers;
	next.whiteBlockers = whiteBlockers;
	next.blockers = blockers;
	next.hash = hash;
	next.eval = eval;
	next.whiteToMove = whiteToMove;
	next.SetPiece(from, to);
	if (to == whiteKing && (from >> 2) == to) next.SetPiece(WHITE_ROOK_KING, whiteKing << 1);
	else if (to == whiteKing && (from << 2) == to) next.SetPiece(WHITE_ROOK_QUEEN, whiteKing >> 1);
	else if (to == blackKing && (from >> 2) == to) next.SetPiece(BLACK_ROOK_KING, blackKing << 1);
	else if (to == blackKing && (from << 2) == to) next.SetPiece(BLACK_ROOK_QUEEN, blackKing >> 1);
	next.eval = -next.eval;
	next.whiteToMove = !whiteToMove;
	next.Save();
	return next;
}
void State::MutateSaveState(ULL from, ULL to) {
	uint8_t fromIndex = 63 - getFirstSetBitPos(from);
	uint8_t toIndex = 63 - getFirstSetBitPos(to);
	blockers = (blockers ^ from) | to;
	if (whiteToMove) {
		blackBlockers = (blackBlockers ^ to) & blackBlockers;
		whiteBlockers = whiteBlockers ^ from ^ to;
	}
	else {
		whiteBlockers = (whiteBlockers ^ to) & whiteBlockers;
		blackBlockers = blackBlockers ^ from ^ to;
	}
	if (board[toIndex >> 3][toIndex & 0b111] & (PAWN|KNIGHT)) {
		ULL moves = GetPieceMoves(toIndex >> 3, toIndex & 0b111);
		attackTable = (attackTable ^ moves) | doubleAttackTable;
	}
	

	if (!(board[fromIndex >> 3][fromIndex & 0b111] & KING)) return;
	if (whiteToMove) whiteKing = to;
	else blackKing = to;
	return;
}

ULL State::GetPieceMoves(uint8_t row, uint8_t coll) {
	uint8_t piece = board[row][coll];
	uint8_t index = 63 - (row * 8 + coll);
	ULL moves = 0;
	if (piece & PAWN) {
		moves = GetPawnMoves(row, coll, index);
	}
	else if (piece & BISHOP) {
		moves = GetBishopMoves(index);
	}
	else if (piece & ROOK) {
		moves = GetRookMoves(index);
	}
	else if (piece & QUEEN) {
		moves = GetQueenMoves(index);
	}
	else if (piece & KNIGHT) {
		moves = GetKnightMoves(index);
	}
	else if (piece & KING) {
		moves = GetKingMoves(index) | GetKingCastling(index);
	}
	return moves;
}

bool State::isInCheck() {
	uint8_t opponentColor = whiteToMove ? BLACK : WHITE;
	ULL king = whiteToMove ? whiteKing : blackKing;
	ULL attacks = GenerateAttackTableSliding(opponentColor)|attackTable;
	return (attacks & king) > 0;
}

void print(ULL num) {
	ULL mask = 1ULL << 63;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((num & mask)) std::cout << 1;
			else std::cout << 0;
			mask = mask >> 1;
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

ULL State::GenerateAttackTable(uint8_t color) {
	attackTable = 0ULL;
	doubleAttackTable = 0ULL;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!(board[i][j] & color)) continue;
			if (board[i][j] & (QUEEN | ROOK | BISHOP)) continue;
			if (color == BLACK && (whiteBlockers & (1ULL << (63 - (8 * i + j))))) continue;
			if (color == WHITE && (blackBlockers & (1ULL << (63 - (8 * i + j))))) continue;
			ULL moves;
			if (board[i][j] & PAWN) moves = GetPawnCaptures(i, j, 63 - (8 * i + j));
			else if (board[i][j] & KING) moves = GetKingMoves(63 - (8 * i + j));
			else moves = GetPieceMoves(i, j);
			doubleAttackTable |= (moves & attackTable);
			attackTable |= moves;
		}
	}
	return attackTable;
}

ULL State::GenerateAttackTableSliding(uint8_t color) {
	ULL attackTable = 0ULL;
	ULL pieces = color == WHITE ? slidingPiecesWhite : slidingPiecesBlack;
	while (pieces != 0ULL) {
		uint8_t pos = getFirstSetBitPos(pieces);
		uint8_t i = (63 - pos) >> 3, j = (63 - pos) & 0b111;
		pieces ^= 1ULL << pos;
		if (color == BLACK && (whiteBlockers & (1ULL << pos))) continue;
		if (color == WHITE && (blackBlockers & (1ULL << pos))) continue;
		ULL moves = GetPieceMoves(i, j);
		attackTable |= moves;
	}
	return attackTable;
}

bool State::IsMoveLegal(ULL from, ULL to, bool wasInCheck) {
	if (from == to) return false;
	else if (whiteToMove && (whiteBlockers & to)) return false;
	else if (!whiteToMove && (blackBlockers & to)) return false;
	MutateSaveState(from, to);
	ULL opponentAttacks = GenerateAttackTableSliding(whiteToMove ? BLACK : WHITE)|attackTable;
	bool legal = true;
	if (whiteToMove && (whiteKing & opponentAttacks)) legal = false;
	else if (!whiteToMove && (blackKing & opponentAttacks)) legal = false;

	if (whiteToMove && (to == whiteKing) && ((from >> 2) == to) && (CASTLE_KING_ATTACK_MASK_WHITE & opponentAttacks)) legal = false;
	else if (whiteToMove && (to == whiteKing) && ((from << 2) == to) && (CASTLE_QUEEN_ATTACK_MASK_WHITE & opponentAttacks)) legal = false;
	else if (!whiteToMove && (to == blackKing) && ((from >> 2) == to) && (CASTLE_KING_ATTACK_MASK_BLACK & opponentAttacks)) legal = false;
	else if (!whiteToMove && (to == blackKing) && ((from << 2) == to) && (CASTLE_QUEEN_ATTACK_MASK_BLACK & opponentAttacks)) legal = false;

	Restore();
	return legal;
}

int State::GetPieceValue(uint8_t piece, uint8_t index, uint8_t color) {
	int value = 0;
	uint8_t type = (piece ^ WHITE ^ BLACK) & piece;
	index = piece & BLACK ? index : 63 - index;
	switch (type) {
	case EMPTY:
		return 0;
	case PAWN:
		value = PAWNSCORE + pawnEval[index];
		break;
	case BISHOP:
		value = BISHOPSCORE + bishopEval[index];
		break;
	case KNIGHT:
		value = KNIGHTSCORE + knightEval[index];
		break;
	case ROOK:
		value = ROOKSCORE + rookEval[index];
		break;
	case QUEEN:
		value = QUEENSCORE + queenEval[index];
		break;
	case KING:
		value = KINGSCORE + kingEval[index];
		break;
	}
	int scalar = piece & color ? 1 : -1;
	return value*scalar;
}

void State::Eval() {
	eval = 0;
	int color = whiteToMove ? WHITE : BLACK;
	uint8_t count = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int value = 0;
			value = GetPieceValue(board[i][j], count, color);
			count++;
			eval += value;
		}
	}
}

bool operator==(const State& lhs, const State& rhs)
{
	if (lhs.blockers != rhs.blockers) return false;
	else return lhs.board == rhs.board && lhs.whiteToMove == rhs.whiteToMove;
}

