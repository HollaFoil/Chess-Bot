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
#include <cmath>
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
std::unordered_map<State, std::pair<ULL, ULL>, State::HashFunction> bestMoves;
std::unordered_map<State, std::pair<ULL, ULL>, State::HashFunction> bestMovesCopy;

int Quiescence(State& state, int alpha, int beta, int depth = 1) {
	int color = state.whiteToMove ? WHITE : BLACK;
	//if (depth == 1 && !state.prevCapture) return state.eval;
	if (state.eval >= beta) return beta;
	if (alpha < state.eval) alpha = state.eval;
	if (depth == 8) {
		return state.eval;
	}
	if (visited.contains(state)) {
		return visited[state];
	}
	bool isInCheck = state.isInCheck();
	bool foundLegal = false;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!(state.board[i][j] & color)) continue;
			ULL moves = state.GetPieceMoves(i, j);
			ULL blockers = color == WHITE ? state.blackBlockers : state.whiteBlockers;
			ULL maskedmoves = moves & blockers;
			if (isInCheck) maskedmoves = moves;
			while (maskedmoves != 0ULL) {
				uint8_t index = getFirstSetBitPos(maskedmoves);
				ULL from = 1ULL << (63 - (i * 8 + j));
				ULL to = 1ULL << index;
				maskedmoves ^= 1ULL << index;
				if (!state.IsMoveLegal(from, to, isInCheck)) continue;
				bool foundLegal = true;
				State next = state.MutateCopy(from, to);
				int eval = -Quiescence(next, -beta, -alpha, depth + 1);
				if (eval >= beta) beta;
				if (eval > alpha) {
					alpha = eval;
				}
			}
		}
	}
	if (isInCheck && !foundLegal) {
		return std::max(beta, -90000 + 5000 * depth);
	}
	if (!foundLegal) {
		//state.Eval();
		return std::min(beta, state.eval);
	}
	return std::max(alpha, -999999);
}

std::pair<int, std::pair<ULL, ULL>> GetStateEval(State &state, int maxDepth = 7, int depth = 1, int alpha = -1000000, int beta = 1000000) {
	int color = state.whiteToMove ? WHITE : BLACK;
	if (depth == maxDepth) {
		//int result = Quiescence(state, alpha, beta);
		int result = state.eval;
		return std::make_pair(result, std::make_pair(0, 0));
	}

	if (visited.contains(state)) {
		return std::make_pair(visited[state], std::make_pair(0,0));
	}
	bool isInCheck = state.isInCheck();
	ULL bestFrom = -1;
	ULL bestTo = -1;
	bool foundLegal = false;
	if (bestMoves.contains(state)) {
		std::pair<ULL, ULL> &p = bestMoves[state];
		ULL from = p.first;
		ULL to = p.second;
		State next = state.MutateCopy(from, to);
		foundLegal = true;
		int eval = -GetStateEval(next, maxDepth, depth + 1, -beta, -alpha).first;
		if (eval >= beta) return std::make_pair(std::min(beta, 999999), std::make_pair(0, 0));
		if (eval > alpha) {
			bestMovesCopy[state] = std::make_pair(from, to);
			bestFrom = from;
			bestTo = to;
			alpha = eval;
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!(state.board[i][j] & color)) continue;
			ULL moves = state.GetPieceMoves(i, j);
			while (moves != 0ULL) {
				uint8_t index = getFirstSetBitPos(moves);
				ULL from = 1ULL << (63 - (i * 8 + j));
				ULL to = 1ULL << index;
				moves ^= 1ULL << index;
				if (!state.IsMoveLegal(from, to, isInCheck)) continue;
				foundLegal = true;
				State next = state.MutateCopy(from, to);
				int eval = -GetStateEval(next, maxDepth, depth + 1, -beta, -alpha).first;
				if (eval >= beta) return std::make_pair(std::min(beta, 999999), std::make_pair(0, 0));
				if (eval > alpha) {
					bestMovesCopy[state] = std::make_pair(from, to);
					bestFrom = from;
					bestTo = to;
					alpha = eval;
				}
			}
		}
	}
	if (isInCheck && !foundLegal) {
		if (depth == 3) {
			std::cout << "yeah idk";
		}
		return std::make_pair(- 90000 + 5000 * depth, std::make_pair(0, 0));
	}
	if (!foundLegal) {
		return std::make_pair(0, std::make_pair(0, 0));
	}
	visited.insert(std::make_pair(state, alpha));
	return std::make_pair(std::max(alpha, -999999), std::make_pair(bestFrom, bestTo));
	//if (bestFrom == -1)
	
}

std::string Engine::GetMove() {
	std::string move = "";
	visited.reserve(pow(2, 24));
	current.Eval();
	std::cout << "Eval depth 0:" << current.eval << std::endl;
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
	bestMoves.swap(bestMovesCopy);
	bestMovesCopy.clear();
	visited.clear();
	return move;
}


struct info {
	int move = 0;
	uint8_t bestMove;
};