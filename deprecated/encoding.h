#pragma once
#include <iostream>
#include <math.h>
#include <algorithm>

//Written with __popcnt so should be compiled with mvsc/ For gcc, replace with __builtin_popcount

template<char pos_count>
class perm_encoding
{
public:
	char pos = pos_count;
	char piece;
	uint64_t max_coord;

	perm_encoding() : piece(pos_count - 1) { max_coord = factorial(pos_count); } //default to corners
	perm_encoding(int piece_count) : piece(piece_count - 1) { max_coord = factorial(pos_count) / factorial(piece_count);  }
	uint64_t operator()(std::array<char, pos_count> perm_array) {
		//preprocessing
		for (int i = 0; i < pos_count; i++) {
			temp_array[perm_array[i]] = i;
		}
		//calculation
		seen = 0;
		coord = 0;
		for (int i = 0; i < piece; i++) {
			temp = pos_count - temp_array[i] - 1;
			coord += temp_array[i] - __popcnt(seen >> temp);
			coord *= pos_count - i - 1;
			seen += (1 << temp);
		}
		temp = pos_count - temp_array[piece] - 1;
		coord += temp_array[piece] - __popcnt(seen >> temp);
		return coord;
	}

	uint64_t factorial(uint64_t n)
	{
		return n <= 1 ? 1 : n * factorial(n - 1);
	}

private:
	std::array<char, pos_count> temp_array;
	int seen = 0;
	uint64_t coord = 0;
	uint64_t temp = 0;
};

template<char pos_count>
class split_perm_encoding
{
public:
	char pos = pos_count;
	char piece;
	uint64_t max;
	char br;

	split_perm_encoding() : piece(11), br(5) { max = factorial(pos_count);  } //default to 6/6 edges
	split_perm_encoding(int piece_count, int break_point) : piece(piece_count - 1), br(break_point) { max = factorial(pos_count)/factorial(break_point); }
	uint64_t operator()(std::array<char, pos_count> perm_array) {
		//preprocessing
		for (int i = 0; i < pos_count; i++) {
			temp_array[perm_array[i]] = i;
		}
		//calculation
		seen = 0;
		coord = 0;
		coord_temp = 0;
		for (int i = 0; i < br; i++) {
			temp = pos_count - temp_array[i] - 1;
			coord += temp_array[i] - __popcnt(seen >> temp);
			coord *= pos_count - i - 1;
			seen += (1 << temp);
		}
		coord <<= (int)log2(max)+1;
		std::transform(perm_array.begin(), perm_array.end(), perm_array.begin(), [break_point = br](char x) -> char { return (x - break_point + pos_count) % pos_count; });
		for (int i = 0; i < pos_count; i++) {
			temp_array[perm_array[i]] = i;
		}
		for (int i = 0; i < br; i++) {
			seen += (1 << (pos_count - temp_array[i] - 1));
		}
		for (int i = br; i < piece; i++) {
			temp = pos_count - temp_array[i] - 1;
			coord_temp += temp_array[i] - __popcnt(seen >> temp);
			coord_temp*= pos_count - i - 1;
			seen += (1 << temp);
		}
		temp = pos_count - temp_array[piece] - 1;
		coord_temp += temp_array[piece] - __popcnt(seen >> temp);
		coord += coord_temp;
		return coord;
	}

	uint64_t factorial(uint64_t n)
	{
		return n <= 1 ? 1 : n * factorial(n - 1);
	}

private:
	std::array<char, pos_count> temp_array;
	int seen = 0;
	uint64_t coord = 0;
	uint64_t temp = 0;
	uint64_t coord_temp = 0;
};

template<char pos_count, char ori_count>
class ori_encoding {
public:
	char pos = pos_count;
	uint64_t max;
	char ori = ori_count;
	ori_encoding() { max = pow(ori_count, pos_count-1); } //default to corners;
	uint64_t operator()(std::array<char, pos_count> ori_array) {
		coord = 0;
		for (int i = 0; i < pos_count; i++) {
			coord *= ori_count;
			coord += ori_array[i];
		}
		return coord;
	}
private:
	uint64_t coord = 0;
};

template<char pos_count, char ori_count>
class ori_decoding {
public:
	ori_decoding() { ori_array.fill(0);  }
	std::array<char, pos_count> operator()(uint64_t coord) {
		for (int i = 0; i < pos_count; i++) {
			ori_array[pos_count - i - 1] = coord % ori_count;
			coord /= ori_count;
		}
		return ori_array;
	}
	
private:
	std::array<char, pos_count> ori_array;
};