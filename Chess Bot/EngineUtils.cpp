#pragma once
#include "Constants.h"
#include "BitUtils.h"
#include "EngineUtils.h"
#include "State.h"
#include <bit>
#include <bitset>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>

void print2(ULL num) {
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

Engine::Engine(bool PlayingAsWhite) {
	IDTracker = 0;
	current.GenerateDefault();
	playingAsWhite = PlayingAsWhite;
	color = playingAsWhite ? WHITE : BLACK;
}
void Engine::LoadFen(std::string fen) {
	current.LoadFromFen(fen);
}

std::unordered_map<State, int, State::HashFunction> visited;

std::pair<int, std::pair<ULL, ULL>> GetStateEval(State &state, int maxDepth = 9, int depth = 1, int alpha = -1000000, int beta = 1000000) {
	int color = state.whiteToMove ? WHITE : BLACK;
	int evalScalar = state.whiteToMove ? 1 : -1;
	if (depth == maxDepth) {
		state.Eval();
		return std::make_pair(state.eval * evalScalar, std::make_pair(0, 0));
	}

	if (visited.contains(state)) {
		return std::make_pair(visited[state], std::make_pair(0,0));
	}
	ULL bestFrom = -1;
	ULL bestTo = -1;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!(state.board[i][j] & color)) continue;
			ULL moves = state.GetPieceMoves(i, j);
			while (moves != 0ULL) {
				int index = getFirstSetBitPos(moves);
				ULL from = 1ULL << (63 - (i * 8 + j));
				ULL to = 1ULL << index;
				moves ^= 1ULL << index;
				if (!state.IsMoveLegal(from, to)) continue;
				State next = state.MutateCopy(from, to);
				int eval = -GetStateEval(next, maxDepth, depth + 1, -beta, -alpha).first;

				if (eval >= beta) return std::make_pair(beta, std::make_pair(0, 0));
				if (eval > alpha) {
					bestFrom = from;
					bestTo = to;
					alpha = eval;
				}
			}
		}
	}
	visited.insert(std::make_pair(state, alpha));
	return std::make_pair(alpha, std::make_pair(bestFrom, bestTo));
}

std::string Engine::GetMove() {
	std::string move = "";
	current.Eval();
	std::cout << "Eval depth 0:" << current.eval << std::endl;
	visited.clear();
	auto result = GetStateEval(current);
	std::cout << "Eval depth 1: " << result.first << std::endl;
	ULL from = result.second.first;
	ULL to = result.second.second;
	int indexFrom = 63 - getFirstSetBitPos(from);
	int indexTo = 63 - getFirstSetBitPos(to);
	move += 'a' + indexFrom % 8;
	move += '0' + (8 - indexFrom / 8);
	move += 'a' + (indexTo % 8);
	move += '0' + (8 - indexTo / 8);
	return move;
}
