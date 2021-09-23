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
void makePermTable(uint64_t *pruningTable, std::array<perm_move<pos_count>, move_count> moveset, perm_encoding<pos_count> encode) {
	char bit_count = log2(encode.max_coord) + 1;
	char can_fit = 64 / bit_count;
	uint64_t entry_count = can_fit * encode.max_coord;
	pruningTable = new uint64_t[entry_count];
	std::array<char, pos_count> curr_perm;
	std::array<char, pos_count> steady_curr_perm;
	std::iota(curr_perm.begin(), curr_perm.end(), 0);
	std::iota(steady_curr_perm.begin(), steady_curr_perm.end(), 0);
	for (uint64_t i = 0; i < entry_count; i += can_fit) {
		for (int j = 0; j < move_count; j++) {
			std::copy(steady_curr_perm.begin(), steady_curr_perm.end(), curr_perm.begin());
			moveset[j](curr_perm);
			uint64_t curr_pos = i + (j / can_fit);
			pruningTable[curr_pos] <<= bit_count;
			pruningTable[curr_pos] += encode(curr_perm);
		}
		std::next_permutation(steady_curr_perm.begin(), steady_curr_perm.end());
	}
}

template<char pos_count, char move_count>
void makePermTable(uint64_t* pruningTable, std::array<perm_move<pos_count>, move_count> moveset, perm_encoding<pos_count> encode, split_perm_encoding<pos_count>split_encode) {
	char bit_count = log2(encode.max_coord) + 1;
	char can_fit = 64 / bit_count + 1;
	uint64_t entry_count = can_fit * encode.max_coord;
	pruningTable = new uint64_t[entry_count];
	std::array<char, pos_count> curr_perm;
	std::array<char, pos_count> steady_curr_perm;
	std::iota(curr_perm.begin(), curr_perm.end(), 0);
	std::iota(steady_curr_perm.begin(), steady_curr_perm.end(), 0);
	for (uint64_t i = 0; i < entry_count; i += can_fit) {
		pruningTable[i] = split_encode(curr_perm);
		for (int j = 0; j < move_count; j++) {
			std::copy(steady_curr_perm.begin(), steady_curr_perm.end(), curr_perm.begin());
			moveset[j](curr_perm);
			uint64_t curr_pos = i + 1 + (j / can_fit);
			pruningTable[curr_pos] <<= bit_count;
			pruningTable[curr_pos] += encode(curr_perm);
		}
		std::next_permutation(steady_curr_perm.begin(), steady_curr_perm.end());
	}
}

template<char pos_count, char move_count, char ori_count>
void makeOriTable(uint64_t* pruningTable, std::array<ori_move<pos_count, ori_count>, move_count> moveset, ori_encoding<pos_count, ori_count> encode) {
	char bit_count = log2(encode.max) + 1;
	char can_fit = 64 / bit_count;
	uint64_t entry_count = can_fit * encode.max;
	pruningTable = new uint64_t[entry_count];
	ori_decoding<pos_count, ori_count> decode;
	std::array<char, pos_count> curr_array;
	for (uint64_t i = 0; i < entry_count; i += can_fit) {
		char sub_entry = 0;
		for (uint64_t j = 0; j < move_count; j++) {
			uint64_t curr_pos = i + (sub_entry / can_fit);
			pruningTable[curr_pos] << bit_count;
			curr_array = decode(i);
			moveset[j](curr_array);
			pruningTable[curr_pos] += encode(curr_array);
			sub_entry++;
		}
	}
}

template<char N, char M>
void makePruningTables(std::array<perm_move<12>, N> epMoves,
							std::array<perm_move<8>, M> cpMoves, 
								std::array<ori_move<12, 2>, N> eoMoves, 
									std::array<ori_move<8, 3>, M> coMoves) {
	perm_encoding<12> epEncode = perm_encoding<12>(6);
	split_perm_encoding<12> splitEpEncode = split_perm_encoding<12>(6, 12);
	makePermTable<12, N>(epTable, epMoves, epEncode, splitEpEncode);
	perm_encoding<8> cpEncode;
	makePermTable<8, M>(cpTable, cpMoves, cpEncode);
	ori_encoding<12, 2> eoEncode;
	makeOriTable<12, N, 2>(eoTable, eoMoves, eoEncode);
	ori_encoding<8, 3> coEncode;
	makeOriTable<8, M, 3>(coTable, coMoves, coEncode);

}