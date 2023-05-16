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

inline int getFirstSetBitPos(ULL n)
{
	if (n == 0) return -1;
	int pow = 0;
	while (n != 1) {
		++pow;
		n = n >> 1;
	}
	return pow;
}
#endif