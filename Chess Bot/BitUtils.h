#ifndef BITUTILS_H
#define BITUTILS_H
#define ULL unsigned long long
#include <bit>

inline ULL _popLsb(ULL board) {
	ULL lsb = std::bit_floor(board);
	return lsb;
}

inline int _popCount(ULL board) {
	return std::popcount(board);
}

inline uint8_t getFirstSetBitPos(ULL n)
{
	unsigned long pos;
	_BitScanForward64(&pos, n);
	return pos;
}
inline uint8_t getLastSetBitPos(ULL n) {
	unsigned long pos;
	_BitScanReverse64(&pos, n);
	return pos;
}

bool isPowOfTwo(ULL bits)
{
	return bits && !(bits & (bits - 1));
}

#endif