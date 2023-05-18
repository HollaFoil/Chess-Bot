#pragma once
#include "Constants.h"
#include "BitUtils.h"
#include <stdint.h>
#include <bit>
#include <iostream>
#include <bitset>
#include <array>
#include <vector>

#define square std::array<std::array<uint8_t, 8>, 8>
#define squareULL std::array<std::array<ULL, 8>, 8>


class State {
public:
	int ID;
	ULL blockers;
	ULL whiteBlockers;
	ULL blackBlockers;
	ULL pinned;
	ULL whiteKing;
	ULL blackKing;
	ULL enPassant;
	ULL doubleAttackTable;
	ULL attackTable;

	ULL blockersCopy;
	ULL whiteBlockersCopy;
	ULL blackBlockersCopy;
	ULL pinnedCopy;
	ULL whiteKingCopy;
	ULL blackKingCopy;
	ULL attackTableCopy;
	squareULL legalMoves;

	bool canWhiteCastleQueen = true;
	bool canWhiteCastleKing = true;
	bool canBlackCastleQueen = true;
	bool canBlackCastleKing = true;
	bool prevCapture = false;
	ULL slidingPiecesWhite;
	ULL slidingPiecesBlack;

	square board;
	ULL hash;
	int eval = 0;
	bool whiteToMove;
	bool inCheck;

	State() {
		ID = id++;
	}

	void GenerateDefault();
	void LoadFromFen(std::string fen);
	void Eval();
	int GetPieceValue(uint8_t piece, uint8_t index, uint8_t color);
	void CalculateHash();
	ULL GenerateAttackTable(uint8_t color);
	ULL GenerateAttackTableSliding(uint8_t color);
	ULL GetPieceMoves(uint8_t row, uint8_t coll);
	ULL GetBishopMoves(uint8_t index);
	ULL GetRookMoves(uint8_t index);
	ULL GetQueenMoves(uint8_t index);
	ULL GetPawnMoves(uint8_t row, uint8_t coll, uint8_t index);
	ULL GetPawnCaptures(uint8_t row, uint8_t coll, uint8_t index);
	ULL GetPawnPush(uint8_t row, uint8_t coll, uint8_t index);
	ULL GetKnightMoves(uint8_t index);
	ULL GetKingMoves(uint8_t index);
	ULL GetKingCastling(uint8_t index);
	std::pair<ULL, ULL> GetPinnedPiece(ULL king, ULL slidingPiece);
	void GetLegalMoves();
	bool IsMoveLegal(ULL from, ULL to, bool wasInCheck);
	State MutateCopy(ULL from, ULL to);
	bool isInCheck();
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