#include "Constants.h"
#include "BitUtils.h"
#include <bit>
#include <bitset>
#include <random>

const unsigned long long BitBoards::RookMagics[64] = {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x2802200800400ULL,
    0x2200800100020080ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x4010040010010802ULL,
    0x20008806104ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x21200680100081ULL,
    0x20100080080080ULL,
    0x2000a00200410ULL,
    0x20080800400ULL,
    0x80088400100102ULL,
    0x80004600042881ULL,
    0x4040008040800020ULL,
    0x440003000200801ULL,
    0x4200011004500ULL,
    0x188020010100100ULL,
    0x14800401802800ULL,
    0x2080040080800200ULL,
    0x124080204001001ULL,
    0x200046502000484ULL,
    0x480400080088020ULL,
    0x1000422010034000ULL,
    0x30200100110040ULL,
    0x100021010009ULL,
    0x2002080100110004ULL,
    0x202008004008002ULL,
    0x20020004010100ULL,
    0x2048440040820001ULL,
    0x101002200408200ULL,
    0x40802000401080ULL,
    0x4008142004410100ULL,
    0x2060820c0120200ULL,
    0x1001004080100ULL,
    0x20c020080040080ULL,
    0x2935610830022400ULL,
    0x44440041009200ULL,
    0x280001040802101ULL,
    0x2100190040002085ULL,
    0x80c0084100102001ULL,
    0x4024081001000421ULL,
    0x20030a0244872ULL,
    0x12001008414402ULL,
    0x2006104900a0804ULL,
    0x1004081002402ULL,
};
const unsigned long long BitBoards::BishopMagics[64] = {
    0x40040822862081ULL,
    0x40810a4108000ULL,
    0x2008008400920040ULL,
    0x61050104000008ULL,
    0x8282021010016100ULL,
    0x41008210400a0001ULL,
    0x3004202104050c0ULL,
    0x22010108410402ULL,
    0x60400862888605ULL,
    0x6311401040228ULL,
    0x80801082000ULL,
    0x802a082080240100ULL,
    0x1860061210016800ULL,
    0x401016010a810ULL,
    0x1000060545201005ULL,
    0x21000c2098280819ULL,
    0x2020004242020200ULL,
    0x4102100490040101ULL,
    0x114012208001500ULL,
    0x108000682004460ULL,
    0x7809000490401000ULL,
    0x420b001601052912ULL,
    0x408c8206100300ULL,
    0x2231001041180110ULL,
    0x8010102008a02100ULL,
    0x204201004080084ULL,
    0x410500058008811ULL,
    0x480a040008010820ULL,
    0x2194082044002002ULL,
    0x2008a20001004200ULL,
    0x40908041041004ULL,
    0x881002200540404ULL,
    0x4001082002082101ULL,
    0x8110408880880ULL,
    0x8000404040080200ULL,
    0x200020082180080ULL,
    0x1184440400114100ULL,
    0xc220008020110412ULL,
    0x4088084040090100ULL,
    0x8822104100121080ULL,
    0x100111884008200aULL,
    0x2844040288820200ULL,
    0x90901088003010ULL,
    0x1000a218000400ULL,
    0x1102010420204ULL,
    0x8414a3483000200ULL,
    0x6410849901420400ULL,
    0x201080200901040ULL,
    0x204880808050002ULL,
    0x1001008201210000ULL,
    0x16a6300a890040aULL,
    0x8049000441108600ULL,
    0x2212002060410044ULL,
    0x100086308020020ULL,
    0x484241408020421ULL,
    0x105084028429c085ULL,
    0x4282480801080cULL,
    0x81c098488088240ULL,
    0x1400000090480820ULL,
    0x4444000030208810ULL,
    0x1020142010820200ULL,
    0x2234802004018200ULL,
    0xc2040450820a00ULL,
    0x2101021090020ULL,
};
const unsigned long long BitBoards::RookMask[64]{
    0x101010101017eULL,
    0x202020202027cULL,
    0x404040404047aULL,
    0x8080808080876ULL,
    0x1010101010106eULL,
    0x2020202020205eULL,
    0x4040404040403eULL,
    0x8080808080807eULL,
    0x1010101017e00ULL,
    0x2020202027c00ULL,
    0x4040404047a00ULL,
    0x8080808087600ULL,
    0x10101010106e00ULL,
    0x20202020205e00ULL,
    0x40404040403e00ULL,
    0x80808080807e00ULL,
    0x10101017e0100ULL,
    0x20202027c0200ULL,
    0x40404047a0400ULL,
    0x8080808760800ULL,
    0x101010106e1000ULL,
    0x202020205e2000ULL,
    0x404040403e4000ULL,
    0x808080807e8000ULL,
    0x101017e010100ULL,
    0x202027c020200ULL,
    0x404047a040400ULL,
    0x8080876080800ULL,
    0x1010106e101000ULL,
    0x2020205e202000ULL,
    0x4040403e404000ULL,
    0x8080807e808000ULL,
    0x1017e01010100ULL,
    0x2027c02020200ULL,
    0x4047a04040400ULL,
    0x8087608080800ULL,
    0x10106e10101000ULL,
    0x20205e20202000ULL,
    0x40403e40404000ULL,
    0x80807e80808000ULL,
    0x17e0101010100ULL,
    0x27c0202020200ULL,
    0x47a0404040400ULL,
    0x8760808080800ULL,
    0x106e1010101000ULL,
    0x205e2020202000ULL,
    0x403e4040404000ULL,
    0x807e8080808000ULL,
    0x7e010101010100ULL,
    0x7c020202020200ULL,
    0x7a040404040400ULL,
    0x76080808080800ULL,
    0x6e101010101000ULL,
    0x5e202020202000ULL,
    0x3e404040404000ULL,
    0x7e808080808000ULL,
    0x7e01010101010100ULL,
    0x7c02020202020200ULL,
    0x7a04040404040400ULL,
    0x7608080808080800ULL,
    0x6e10101010101000ULL,
    0x5e20202020202000ULL,
    0x3e40404040404000ULL,
    0x7e80808080808000ULL
};
const unsigned long long BitBoards::BishopMask[64]{
    0x40201008040200ULL,
    0x402010080400ULL,
    0x4020100a00ULL,
    0x40221400ULL,
    0x2442800ULL,
    0x204085000ULL,
    0x20408102000ULL,
    0x2040810204000ULL,
    0x20100804020000ULL,
    0x40201008040000ULL,
    0x4020100a0000ULL,
    0x4022140000ULL,
    0x244280000ULL,
    0x20408500000ULL,
    0x2040810200000ULL,
    0x4081020400000ULL,
    0x10080402000200ULL,
    0x20100804000400ULL,
    0x4020100a000a00ULL,
    0x402214001400ULL,
    0x24428002800ULL,
    0x2040850005000ULL,
    0x4081020002000ULL,
    0x8102040004000ULL,
    0x8040200020400ULL,
    0x10080400040800ULL,
    0x20100a000a1000ULL,
    0x40221400142200ULL,
    0x2442800284400ULL,
    0x4085000500800ULL,
    0x8102000201000ULL,
    0x10204000402000ULL,
    0x4020002040800ULL,
    0x8040004081000ULL,
    0x100a000a102000ULL,
    0x22140014224000ULL,
    0x44280028440200ULL,
    0x8500050080400ULL,
    0x10200020100800ULL,
    0x20400040201000ULL,
    0x2000204081000ULL,
    0x4000408102000ULL,
    0xa000a10204000ULL,
    0x14001422400000ULL,
    0x28002844020000ULL,
    0x50005008040200ULL,
    0x20002010080400ULL,
    0x40004020100800ULL,
    0x20408102000ULL,
    0x40810204000ULL,
    0xa1020400000ULL,
    0x142240000000ULL,
    0x284402000000ULL,
    0x500804020000ULL,
    0x201008040200ULL,
    0x402010080400ULL,
    0x2040810204000ULL,
    0x4081020400000ULL,
    0xa102040000000ULL,
    0x14224000000000ULL,
    0x28440200000000ULL,
    0x50080402000000ULL,
    0x20100804020000ULL,
    0x40201008040200ULL
};
unsigned long long BitBoards::BishopTable[64][4096];
unsigned long long BitBoards::RookTable[64][4096];
unsigned long long BitBoards::KnightTable[64];
unsigned long long BitBoards::KingTable[64];
unsigned long long BitBoards::PawnTableCaptures[2][64];
unsigned long long BitBoards::PawnTableMoves[2][64];
ULL BitBoards::ZobristNumbers[12][64];
ULL BitBoards::ZobristBlackToMove;

const int BitBoards::RookRellevantBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};
const int BitBoards::BishopRellevantBits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};
ULL BitBoards::GetBlockersFromIndex(ULL mask, int index) {
    ULL maskCopy = mask;
    ULL blockers = 0ULL;
    int bits = std::popcount(maskCopy);
    for (int i = 0; i < bits; i++) {
        ULL bitMask = _popLsb(maskCopy);
        maskCopy ^= bitMask;
        if (index & (1 << i)) {
            blockers |= (bitMask);
        }
    }
    return blockers;
}

void BitBoards::InitTables() {
    GenerateZobristNumbers();
    for (int square = 0; square < 64; square++) {
        GenerateKingMoves(square);
        GenerateKnightMoves(square);
        GeneratePawnMoves(square);
        for (int i = 0; i < 4096; i++) {
            RookTable[square][i] = 0;
            BishopTable[square][i] = 0;
        }
    }
    for (int square = 0; square < 64; square++) {
        int variationsBishop = 1 << BishopRellevantBits[square];
        int variationsRook = 1 << RookRellevantBits[square];
        for (int i = 0; i < 4096; i++) {
            if (i < variationsBishop) GenerateBishopMoves(square, i);
            if (i < variationsRook) GenerateRookMoves(square, i);
        }
    }
}

void BitBoards::GeneratePawnMoves(int square) {
    ULL start = 1ULL << square;

    ULL whiteAttackPawn = ((start << 9) & ~FILE_A) | ((start << 7) & ~FILE_H);
    ULL blackAttackPawn = ((start >> 9) & ~FILE_H) | ((start >> 7) & ~FILE_A);

    BitBoards::PawnTableCaptures[0][square] = whiteAttackPawn;
    BitBoards::PawnTableCaptures[1][square] = blackAttackPawn;

    ULL whiteMovePawn = ((start << 8)) | ((start & RANK_2) ? (start << 16) : 0ULL);
    ULL blackMovePawn = ((start >> 8)) | ((start & RANK_7) ? (start >> 16) : 0ULL);

    BitBoards::PawnTableMoves[0][square] = whiteMovePawn;
    BitBoards::PawnTableMoves[1][square] = blackMovePawn;
}

void BitBoards::GenerateKnightMoves(int square) {
    ULL start = 1ULL << square;

    ULL attackKnight = (((start << 15) | (start >> 17)) & ~FILE_H) | 
        (((start >> 15) | (start << 17)) & ~FILE_A) | 
        (((start << 6) | (start >> 10)) & ~(FILE_G | FILE_H)) | 
        (((start >> 6) | (start << 10)) & ~(FILE_A | FILE_B)); 

    BitBoards::KnightTable[square] = attackKnight;
}

void BitBoards::GenerateKingMoves(int square) {
        ULL start = 1ULL << square;

        ULL attackKing = (((start << 7) | (start >> 9) | (start >> 1)) & (~FILE_H)) |
            (((start << 9) | (start >> 7) | (start << 1)) & (~FILE_A)) |
            ((start >> 8) | (start << 8));

        BitBoards::KingTable[square] = attackKing;
    }

void BitBoards::GenerateBishopMoves(int square, int index) {
    ULL mask = BitBoards::BishopMask[square];
    ULL blockers = BitBoards::GetBlockersFromIndex(mask, index);
    ULL moves = 0LL;
    int dirs[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
    for (auto dir : dirs) {
        int pos = square;
        while (true) {
            pos += dir[0] * 1 + dir[1] * 8;
            if (pos < 0 || pos > 63) break;
            if (abs(square / 8 - pos / 8) != abs(square % 8 - pos % 8)) break;
            moves |= (1ULL << pos);
            if (dir[0] != 0 && (blockers | COLLUMNS) & (1ULL << pos)) break;
            else if (dir[1] != 0 && (blockers | ROWS) & (1ULL << pos)) break;
        }
    }
    ULL magicsMultiplied = ((blockers & mask) * BitBoards::BishopMagics[square]);
    ULL hash = (magicsMultiplied >> (64 - BitBoards::BishopRellevantBits[square]));
    if (BitBoards::BishopTable[square][hash] != 0) {
        return;
    }
    BitBoards::BishopTable[square][hash] = moves;
}

void BitBoards::GenerateZobristNumbers() {
    const ULL range_to = 0xFFFFFFFFFFFFFFFFULL;
    const ULL range_from = 0ULL;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<ULL>  distr(range_from, range_to);
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 64; j++) {
            ZobristNumbers[i][j] = distr(generator);
        }
    }
    ZobristBlackToMove = distr(generator);
}

void BitBoards::GenerateRookMoves(int square, int index) {

    ULL mask = BitBoards::RookMask[square];
    ULL blockers = BitBoards::GetBlockersFromIndex(mask, index);
    ULL moves = 0LL;
    int dirs[4][2] = { {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
    for (auto dir : dirs) {
        int pos = square;
        while (true) {
            pos += dir[0] * 1 + dir[1] * 8;
            if (pos < 0 || pos > 63) break;
            if (dir[0] != 0 && square / 8 != pos / 8) break;
            if (dir[1] != 0 && square % 8 != pos % 8) break;
            moves |= (1ULL << pos);
            if (dir[0] != 0 && (blockers | COLLUMNS) & (1ULL << pos)) break;
            else if (dir[1] != 0 && (blockers | ROWS) & (1ULL << pos)) break;
        }
    }
    ULL magicsMultiplied = ((blockers & mask) * BitBoards::RookMagics[square]);
    ULL hash = (magicsMultiplied >> (64 - BitBoards::RookRellevantBits[square]));
    if (BitBoards::RookTable[square][hash] != 0) {
        return;
    }
    BitBoards::RookTable[square][hash] = moves;
}