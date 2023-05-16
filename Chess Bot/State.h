#pragma once
#include "Constants.h"
#include "BitUtils.h"
#include <bit>
#include <iostream>
#include <bitset>
#include <array>

#define square std::array<std::array<int, 8>, 8>

class State {
public:
	ULL blockers;
	ULL whiteBlockers;
	ULL blackBlockers;
	ULL pinned;
	ULL whiteKing;
	ULL blackKing;

	ULL blockersCopy;
	ULL whiteBlockersCopy;
	ULL blackBlockersCopy;
	ULL pinnedCopy;
	ULL whiteKingCopy;
	ULL blackKingCopy;

	square board;
	int hash;
	int eval = 0;
	bool whiteToMove;
	bool inCheck;

	State() {}

	void GenerateDefault();
	void LoadFromFen(std::string fen);
	void Eval();
	void CalculateHash();
	ULL GenerateAttackTable(int color);
	ULL GetPieceMoves(int row, int coll);
	ULL GetBishopMoves(int index);
	ULL GetRookMoves(int index);
	ULL GetQueenMoves(int index);
	ULL GetPawnMoves(int row, int coll, int index);
	ULL GetPawnCaptures(int row, int coll, int index);
	ULL GetPawnPush(int row, int coll, int index);
	ULL GetKnightMoves(int index);
	ULL GetKingMoves(int index);
	bool IsMoveLegal(ULL from, ULL to);
	State MutateCopy(ULL from, ULL to);
	void Save();
	void MutateSaveState(ULL from, ULL to);
	void Restore();
	void SetPiece(ULL from, ULL to);
	friend bool operator==(const State& lhs, const State& rhs);
	struct HashFunction
	{
		size_t operator()(const State& state) const
		{
			return state.hash;
		}
	};
};
bool operator==(const State& lhs, const State& rhs);