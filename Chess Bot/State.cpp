#include "Constants.h"
#include "BitUtils.h"
#include "State.h"
#include <bit>
#include <bitset>
#include <array>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <map>

#define square std::array<std::array<int, 8>, 8>

ULL State::GetBishopMoves(int index) {
	ULL mask = (BitBoards::BishopMask[index] & blockers) * BitBoards::BishopMagics[index];
	ULL id = mask >> (64 - BitBoards::BishopRellevantBits[index]);
	ULL moves = BitBoards::BishopTable[index][id];
	return moves;
}
ULL State::GetRookMoves(int index) {
	ULL mask = (BitBoards::RookMask[index] & blockers) * BitBoards::RookMagics[index];
	ULL id = mask >> (64 - BitBoards::RookRellevantBits[index]);
	ULL moves = BitBoards::RookTable[index][id];
	return moves;
}
ULL State::GetQueenMoves(int index) {
	ULL moves = GetBishopMoves(index) | GetRookMoves(index);
	return moves;
}
ULL State::GetPawnPush(int row, int coll, int index) {
	if (1ULL << (index + 8) & blockers) return 0ULL;
	ULL pawnPushMask = BitBoards::PawnTableMoves[(board[row][coll] & WHITE) ? 0 : 1][index];
	ULL pawnPushMoves = pawnPushMask & (pawnPushMask ^ blockers);
	return pawnPushMoves;
}
ULL State::GetPawnCaptures(int row, int coll, int index) {
	ULL pawnCaptureMask = BitBoards::PawnTableCaptures[(board[row][coll] & WHITE ? 0 : 1)][index];
	
	return pawnCaptureMask;
}
ULL State::GetPawnMoves(int row, int coll, int index) {
	ULL pawnCaptureMask = GetPawnCaptures(row, coll, index);
	ULL pawnCaptureMoves = pawnCaptureMask & blockers;
	return  pawnCaptureMoves | GetPawnPush(row, coll, index);
}
ULL State::GetKnightMoves(int index) {
	return BitBoards::KnightTable[index];
}
ULL State::GetKingMoves(int index) {
	return BitBoards::KingTable[index];
}

void State::Save() {
	blockersCopy = blockers;
	whiteBlockersCopy = whiteBlockers;
	blackBlockersCopy = blackBlockers;
	pinnedCopy = pinned;
	whiteKingCopy = whiteKing;
	blackKingCopy = blackKing;
}
void State::Restore() {
	blockers = blockersCopy;
	whiteBlockers = whiteBlockersCopy;
	blackBlockers = blackBlockersCopy;
	pinned = pinnedCopy;
	whiteKing = whiteKingCopy;
	blackKing = blackKingCopy;
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
	Save();
}

int GetPieceID(int piece) {
	int id = 0;
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
	std::vector<std::string> parts;
	size_t pos = 0;
	std::unordered_map<char, int> pieces = { {'p', PAWN | BLACK},
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
	while ((pos = fen.find(" ")) != std::string::npos) {
		parts.push_back(fen.substr(0, pos));
		fen.erase(0, pos + 1);
	}
	int row = 0;
	int coll = 0;
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
			int index = 63 - (i * 8 + j);
			blockers |= (1ULL << index);
			if (board[i][j] & WHITE) whiteBlockers |= (1ULL << index);
			else blackBlockers |= (1ULL << index);
			if (board[i][j] == (WHITE | KING)) whiteKing = 1ULL << index;
			else if (board[i][j] == (BLACK | KING)) blackKing = 1ULL << index;
		}
	}
	blockers = whiteBlockers | blackBlockers;
	whiteToMove = parts[1] == "w" ? true : false;
	Save();
}

void State::CalculateHash() {
	hash = whiteToMove ? 0 : BitBoards::ZobristBlackToMove;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == 0) continue;
			int piece = board[i][j];
			int id = GetPieceID(piece);
			hash ^= BitBoards::ZobristNumbers[id][i * 8 + j];
		}
	}
}


void State::SetPiece(ULL from, ULL to) {
	int fromIndex = 63 - getFirstSetBitPos(from);
	int toIndex = 63 - getFirstSetBitPos(to);
	int toRow = toIndex / 8, toColl = toIndex % 8;
	int fromRow = fromIndex / 8, fromColl = fromIndex % 8;
	int pieceTo = board[toRow][toColl];
	int pieceFrom = board[fromRow][fromColl];
	if (pieceTo & BLACK) {
		blackBlockers ^= to;
		hash ^= BitBoards::ZobristNumbers[GetPieceID(pieceTo)][toIndex];
		board[toRow][toColl] = pieceFrom;
	}
	else if (pieceTo & WHITE) {
		whiteBlockers ^= to;
		hash ^= BitBoards::ZobristNumbers[GetPieceID(pieceTo)][toIndex];
		board[toRow][toColl] = pieceFrom;
	}
	board[fromRow][fromColl] = EMPTY;
	if (pieceFrom & BLACK) {
		blackBlockers ^= from;
		blackBlockers ^= to;
	}
	else if (pieceFrom & WHITE) {
		whiteBlockers ^= from;
		whiteBlockers ^= to;
	}
	hash ^= BitBoards::ZobristNumbers[GetPieceID(pieceFrom)][fromIndex];
	hash ^= BitBoards::ZobristNumbers[GetPieceID(pieceFrom)][toIndex];
	hash ^= BitBoards::ZobristBlackToMove;
	blockers = blackBlockers | whiteBlockers;
	if (pieceFrom == (KING | WHITE)) whiteKing = to;
	else if (pieceFrom == (KING | BLACK)) blackKing = to;
}

State State::MutateCopy(ULL from, ULL to) {
	State next;
	next.whiteToMove = !whiteToMove;;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			next.board[i][j] = board[i][j];
		}
	}
	next.whiteKing = whiteKing;
	next.blackKing = blackKing;
	next.blackBlockers = blackBlockers;
	next.whiteBlockers = whiteBlockers;
	next.blockers = blockers;
	next.hash = hash;
	next.SetPiece(from, to);
	next.Save();
	return next;
}
void State::MutateSaveState(ULL from, ULL to) {
	int fromIndex = 63 - getFirstSetBitPos(from);
	int toIndex = 63 - getFirstSetBitPos(to);
	blockers = (blockers ^ from) | to;
	if (whiteToMove) {
		blackBlockers = (blackBlockers ^ to) & blackBlockers;
		whiteBlockers = whiteBlockers ^ from ^ to;
	}
	else {
		whiteBlockers = (whiteBlockers ^ to) & whiteBlockers;
		blackBlockers = blackBlockers ^ from ^ to;
	}
	//next.board[toIndex / 8][toIndex % 8] = next.board[fromIndex / 8][fromIndex % 8];
	//next.board[fromIndex / 8][fromIndex % 8] = EMPTY;
	if (!(board[fromIndex / 8][fromIndex % 8] & KING)) return;
	if (whiteToMove) whiteKing = to;
	else blackKing = to;
	return;
}

ULL State::GetPieceMoves(int row, int coll) {
	int piece = board[row][coll];
	int index = 63 - (row * 8 + coll);
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
		moves = GetKingMoves(index);
	}
	return moves;
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

ULL State::GenerateAttackTable(int color) {
	ULL attackTable = 0ULL;
	//std::cout << "--------------- BLOCKERS -------------------" << '\n';
	//print(whiteBlockers);
	//print(blackBlockers);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!(board[i][j] & color)) continue;
			if (color == BLACK && (whiteBlockers & (1ULL << (63 - (8 * i + j))))) continue;
			if (color == WHITE && (blackBlockers & (1ULL << (63 - (8 * i + j))))) continue;
			ULL moves;
			if (board[i][j] & PAWN) moves = GetPawnCaptures(i, j, 63 - (8 * i + j));
			else moves = GetPieceMoves(i, j);
			attackTable |= moves;
			//std::cout << "i: " << i << " j: " << j << " index: " << '\n';
			//print((1ULL << (63 - (8 * i + j))));
			//print(moves);
			//print(attackTable);
		}
	}
	//std::cout << std::flush << std::endl;
	return attackTable;
}

bool State::IsMoveLegal(ULL from, ULL to) {
	if (from == to) return false;
	if (whiteToMove && (whiteBlockers & to)) return false;
	if (!whiteToMove && (blackBlockers & to)) return false;
	MutateSaveState(from, to);
	ULL opponentAttacks = GenerateAttackTable(whiteToMove ? BLACK : WHITE);
	bool legal = true;
	if (whiteToMove && (whiteKing & opponentAttacks)) legal = false;
	if (!whiteToMove && (blackKing & opponentAttacks)) legal = false;
	Restore();
	return legal;
}

void State::Eval() {
	eval = 0;
	int color = whiteToMove ? WHITE : BLACK;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int value = 0;
			int piece = (board[i][j] ^ WHITE ^ BLACK) & board[i][j];
			int index = board[i][j] & BLACK ? i * 8 + j : 63 - (i * 8 + j);

			switch (piece) {
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
			
			int scalar = board[i][j] & color ? 1 : -1;
			eval += scalar * value;
		}
	}
}

bool operator==(const State& lhs, const State& rhs)
{
	if (lhs.blockers != rhs.blockers) return false;
	else return lhs.board == rhs.board && lhs.whiteToMove == rhs.whiteToMove;
}

