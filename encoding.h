#pragma once
#include <iostream>
#include <math.h>

//TODO rewrite with template args properly

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
	const char pos = pos_count;
	const char piece;
	const uint64_t max;

	split_perm_encoding() : piece(11), br(5) { max = factorial(pos_count);  } //default to 6/6 edges
	split_perm_encoding(int piece_count, int break_point) : piece(piece_count - 1), br(break_point) { max = factorial(pos_count)/factorial(piece_count); }
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
		coord <<= 32;
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
	char br;
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
	ori_encoding() { max = pow(ori_count, pos_count); } //default to corners;
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