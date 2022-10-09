#pragma once
#include <iostream>
#include <math.h>
#include <algorithm>
#include <numeric>
#include "perm_move.h"
#include "ori_move.h"
#include "encoding.h"

uint64_t *epTable;
uint64_t *cpTable;
uint64_t *eoTable;
uint64_t *coTable;

template<char pos_count, char move_count>
uint64_t* makePermTable(std::array<perm_move<pos_count>, move_count> moveset, perm_encoding<pos_count> encode) {
	char bit_count = log2(encode.max_coord) + 1;
	char can_fit = 64 / bit_count;
	char entries_needed = ceil((float)move_count / (float)can_fit);
	uint64_t entry_count = entries_needed * encode.max_coord;
	uint64_t *moveTable = new uint64_t[entry_count];
	memset(moveTable, 0, entry_count);
	std::array<char, pos_count> curr_perm;
	std::array<char, pos_count> steady_curr_perm;
	std::iota(curr_perm.begin(), curr_perm.end(), 0);
	std::iota(steady_curr_perm.begin(), steady_curr_perm.end(), 0);
	for (uint64_t i = 0; i < encode.max_coord; ++i) {
		uint64_t curr_pos = entries_needed * encode(curr_perm);
		for (int j = 0; j < move_count; j++) {
			std::copy(steady_curr_perm.begin(), steady_curr_perm.end(), curr_perm.begin());
			moveset[j](curr_perm);
			curr_pos += (j / can_fit);
			moveTable[curr_pos] <<= bit_count;
			moveTable[curr_pos] += encode(curr_perm);
		}
		std::next_permutation(steady_curr_perm.begin(), steady_curr_perm.end());
	}
	return moveTable;
}

template<char pos_count, char move_count>
uint64_t* makePermTable(std::array<perm_move<pos_count>, move_count> moveset, perm_encoding<pos_count> encode, split_perm_encoding<pos_count>split_encode) {
	char bit_count = log2(encode.max_coord) + 1;
	char can_fit = 64 / bit_count;
	char entries_needed = (ceil((float)move_count / (float)can_fit) + 1);
	uint64_t entry_count = entries_needed * encode.max_coord;
	uint64_t *moveTable = new uint64_t[entry_count];
	memset(moveTable, 0, entry_count);
	std::array<char, pos_count> curr_perm;
	std::array<char, pos_count> steady_curr_perm;
	std::iota(curr_perm.begin(), curr_perm.end(), 0);
	std::iota(steady_curr_perm.begin(), steady_curr_perm.end(), 0);
	can_fit++;
	for (uint64_t i = 0; i < encode.max_coord; ++i) {
		uint64_t curr_pos = entries_needed * encode(curr_perm);
		moveTable[curr_pos] = split_encode(curr_perm);
		curr_pos++;
		for (int j = 0; j < move_count; j++) {
			std::copy(steady_curr_perm.begin(), steady_curr_perm.end(), curr_perm.begin());
			curr_pos += (j / can_fit);
			moveset[j](curr_perm);
			moveTable[curr_pos] <<= bit_count;
			moveTable[curr_pos] += encode(curr_perm);
		}
		std::transform(steady_curr_perm.begin(), steady_curr_perm.end(), steady_curr_perm.begin(), [break_point = split_encode.br](char x) -> char { return (x < break_point) ? x : -1; });
		std::next_permutation(steady_curr_perm.begin(), steady_curr_perm.end());
		char iter = 6;
		for (int i = 0; i < pos_count; i++) {
			if (steady_curr_perm[i] == -1) {
				steady_curr_perm[i] = iter;
				iter++;
			}
		}
	}
	return moveTable;
}

template<char pos_count, char move_count, char ori_count>
uint64_t* makeOriTable(std::array<ori_move<pos_count, ori_count>, move_count> moveset, ori_encoding<pos_count, ori_count> encode) {
	char bit_count = log2(encode.max) + 1;
	char can_fit = 64 / bit_count;
	uint64_t entry_count = ceil((float)move_count / (float)can_fit) * encode.max;
	uint64_t *moveTable = new uint64_t[entry_count];
	memset(moveTable, 0, entry_count);
	ori_decoding<pos_count, ori_count> decode;
	std::array<char, pos_count> curr_array;
	for (uint64_t i = 0; i < encode.max; i++) {
		for (uint64_t j = 0; j < move_count; j++) {
			uint64_t curr_pos = (i / can_fit) + (j / can_fit);
			moveTable[curr_pos] <<= bit_count;
			curr_array = decode(i);
			moveset[j](curr_array);
			moveTable[curr_pos] += encode(curr_array);
		}
	}
	return moveTable;
}

template<char N, char M>
void makeMoveTables(std::array<perm_move<12>, N> epMoves,
							std::array<perm_move<8>, M> cpMoves, 
								std::array<ori_move<12, 2>, N> eoMoves, 
									std::array<ori_move<8, 3>, M> coMoves) {
	perm_encoding<12> epEncode = perm_encoding<12>(6);
	split_perm_encoding<12> splitEpEncode = split_perm_encoding<12>(12, 6);
	epTable = makePermTable<12, N>(epMoves, epEncode, splitEpEncode);
	perm_encoding<8> cpEncode;
	cpTable = makePermTable<8, M>(cpMoves, cpEncode);
	ori_encoding<12, 2> eoEncode;
	eoTable = makeOriTable<12, N, 2>(eoMoves, eoEncode);
	ori_encoding<8, 3> coEncode;
	coTable = makeOriTable<8, M, 3>(coMoves, coEncode);

}