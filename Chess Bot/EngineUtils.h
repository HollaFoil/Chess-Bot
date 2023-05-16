#pragma once
#include "State.h"
#include <string>
#define ULL unsigned long long
class Engine {
public:
	Engine(bool PlayingAsWhite);
	void LoadFen(std::string fen);
	std::string GetMove();
private:
	bool playingAsWhite;
	int color;
	State current;
	ULL IDTracker;
};