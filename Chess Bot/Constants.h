#pragma once
#define ULL unsigned long long

class BitBoards {
public:
	const static ULL RookMask[];
	const static ULL BishopMask[];
	const static ULL RookMagics[];
	const static ULL BishopMagics[];
	const static int RookRellevantBits[64];
	const static int BishopRellevantBits[64];
	static ULL RookTable[64][4096];
	static ULL BishopTable[64][4096];
	static ULL KnightTable[64];
	static ULL KingTable[64];
	static ULL PawnTableCaptures[2][64];
	static ULL PawnTableMoves[2][64];
	static int ZobristNumbers[12][64];
	static int ZobristBlackToMove;
	static void InitTables();
private:
	static ULL GetBlockersFromIndex(ULL mask, int index);
	static void GenerateBishopMoves(int square, int index);
	static void GenerateRookMoves(int square, int index);
	static void GenerateKnightMoves(int square);
	static void GenerateKingMoves(int square);
	static void GeneratePawnMoves(int square);
	static void GenerateZobristNumbers();
};

static int id = 0;

const ULL ROWS = 0xFF000000000000FFULL;
const ULL COLLUMNS = 0x8181818181818181ULL;

const ULL RANK_1 = 0xffull;
const ULL RANK_2 = 0xff00ull;
const ULL RANK_3 = 0xff0000ull;
const ULL RANK_4 = 0xff000000ull;
const ULL RANK_5 = 0xff00000000ull;
const ULL RANK_6 = 0xff0000000000ull;
const ULL RANK_7 = 0xff000000000000ull;
const ULL RANK_8 = 0xff00000000000000ull;

const ULL FILE_H = 0x8080808080808080ull;
const ULL FILE_G = 0x4040404040404040ull;
const ULL FILE_F = 0x2020202020202020ull;
const ULL FILE_E = 0x1010101010101010ull;
const ULL FILE_D = 0x808080808080808ull;
const ULL FILE_C = 0x404040404040404ull;
const ULL FILE_B = 0x202020202020202ull;
const ULL FILE_A = 0x101010101010101ull;

const ULL CASTLE_KING_ATTACK_MASK_WHITE  = 0x000000000000000eULL;
const ULL CASTLE_QUEEN_ATTACK_MASK_WHITE = 0x0000000000000038ULL;
const ULL CASTLE_KING_ATTACK_MASK_BLACK  = 0x0e00000000000000ULL;
const ULL CASTLE_QUEEN_ATTACK_MASK_BLACK = 0x3800000000000000ULL;

const ULL CASTLE_KING_AFFECTED_MASK_WHITE  = 0x000000000000000fULL;
const ULL CASTLE_QUEEN_AFFECTED_MASK_WHITE = 0x00000000000000f8ULL;
const ULL CASTLE_KING_AFFECTED_MASK_BLACK  = 0x0f00000000000000ULL;
const ULL CASTLE_QUEEN_AFFECTED_MASK_BLACK = 0xf800000000000000ULL;

const ULL WHITE_ROOK_KING  = 0x0000000000000001ULL;
const ULL WHITE_ROOK_QUEEN = 0x0000000000000080ULL;
const ULL BLACK_ROOK_KING  = 0x0100000000000000ULL;
const ULL BLACK_ROOK_QUEEN = 0x8000000000000000ULL;

const int EMPTY  = 0x0;
const int KING   = 0x1;
const int QUEEN  = 0x2;
const int KNIGHT = 0x4;
const int BISHOP = 0x8;
const int ROOK   = 0x10;
const int PAWN   = 0x20;
const int WHITE  = 0x40;
const int BLACK  = 0x80;

const int PAWNSCORE   = 100;
const int KNIGHTSCORE = 305;
const int BISHOPSCORE = 333;
const int ROOKSCORE   = 563;
const int QUEENSCORE  = 950;
const int KINGSCORE   = 100000;

const int pawnEval[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        5, 10, 10, -20, -20, 10, 10, 5,
        5, -5, -10, 0, 0, -10, -5, 5,
        0, 0, 0, 20, 20, 0, 0, 0,
        5, 5, 10, 25, 25, 10, 5, 5,
        10, 10, 20, 30, 30, 20, 10, 10,
        50, 50, 50, 50, 50, 50, 50, 50,
        0, 0, 0, 0, 0, 0, 0, 0
};

const int knightEval[64] = {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20, 0, 0, 0, 0, -20, -40,
        -30, 0, 10, 15, 15, 10, 0, -30,
        -30, 5, 15, 20, 20, 15, 5, -30,
        -30, 0, 15, 20, 20, 15, 0, -30,
        -30, 5, 10, 15, 15, 10, 5, -30,
        -40, -20, 0, 5, 5, 0, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50
};

const int bishopEval[64] = {
        -20, -10, -10, -10, -10, -10, -10, -20,
        -10, 5, 0, 0, 0, 0, 5, -10,
        -10, 10, 10, 10, 10, 10, 10, -10,
        -10, 0, 10, 10, 10, 10, 0, -10,
        -10, 5, 5, 10, 10, 5, 5, -10,
        -10, 0, 5, 10, 10, 5, 0, -10,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -20, -10, -10, -10, -10, -10, -10, -20
};

const int rookEval[64] = {
        0, 0, 0, 5, 5, 0, 0, 0,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        5, 10, 10, 10, 10, 10, 10, 5,
        0, 0, 0, 0, 0, 0, 0, 0
};

const int queenEval[64] = {
        -20, -10, -10, -5, -5, -10, -10, -20,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -10, 0, 5, 5, 5, 5, 0, -10,
        -5, 0, 5, 5, 5, 5, 0, -5,
        0, 0, 5, 5, 5, 5, 0, -5,
        -10, 5, 5, 5, 5, 5, 0, -10,
        -10, 0, 5, 0, 0, 0, 0, -10,
        -20, -10, -10, -5, -5, -10, -10, -20
};

const int kingEval[64] = {
        20, 30, 10, 0, 0, 10, 30, 20,
        20, 20, 0, 0, 0, 0, 20, 20,
        -10, -20, -20, -20, -20, -20, -20, -10,
        20, -30, -30, -40, -40, -30, -30, -20,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30
};

const int kingEvalEndGame[64] = {
        -50, -30, -30, -30, -30, -30, -30, -50,
        -30, -30, 0, 0, 0, 0, -30, -30,
        -30, -10, 20, 30, 30, 20, -10, -30,
        -30, -10, 30, 40, 40, 30, -10, -30,
        -30, -10, 30, 40, 40, 30, -10, -30,
        -30, -10, 20, 30, 30, 20, -10, -30,
        -30, -20, -10, 0, 0, -10, -20, -30,
};