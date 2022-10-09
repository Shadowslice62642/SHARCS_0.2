#pragma once

//define general orientation moves
template<char length, char ori, char oriented>
class ori_move
{
public:
	char len = length;
	char _ori = ori;

	ori_move() : _permuting_array({0, 3, 2, 1}), _orienting_array({0, 0, 0, 0}) {} //defaults to U corner
	ori_move(std::array<char, oriented> permuting_array, std::array<char, oriented> orienting_array) : _permuting_array(permuting_array), _orienting_array(orienting_array) {}
	void operator()(std::array<char, length> &ori_array) {
		char x;
		x = (ori_array[_permuting_array[0]] + _orienting_array[0]) % _ori;
        for (int i = 0; i < oriented - 1; i++) {
            ori_array[_permuting_array[i]] = (ori_array[_permuting_array[i + 1]] + _orienting_array[i + 1]) % _ori;
        }
		ori_array[_permuting_array[oriented - 1]] = x;
	}

private:
    std::array<char, oriented> _permuting_array;
    std::array<char, oriented> _orienting_array;
};


//define general permutation moves
template<char length, char permuted>
class perm_move
{
public:
	char len = length;
    char perm = permuted;

	perm_move() {} //: _permuting_array({0,3,2,1}) {} //defaults to U
	perm_move(std::array<char, permuted> permuting_array) : _permuting_array(permuting_array) {}
	void operator()(std::array<char, length> &perm_array) {
		char x;
        x = perm_array[_permuting_array[0]];
        for (int i = 0; i < perm - 1; i++) {
            perm_array[_permuting_array[i]] = perm_array[_permuting_array[i + 1]];
        }
		perm_array[_permuting_array[perm - 1]] = x;
	}

private:
	std::array<char, permuted> _permuting_array;
};

void loadStandardMoves(std::array<perm_move<8, 4>, 6> &cp_moves, std::array<perm_move<12, 4>, 9> &ep_moves, std::array<ori_move<8, 3, 4>, 6> &co_moves, std::array<ori_move<12, 2, 4>, 9> &eo_moves) {
    
    //cp_moves
    cp_moves[0] = perm_move<8, 4>({0, 3, 2, 1}); // U
    cp_moves[1] = perm_move<8, 4>({4, 7, 6, 5}); // D
    cp_moves[2] = perm_move<8, 4>({2, 3, 6, 7}); // R
    cp_moves[3] = perm_move<8, 4>({0, 1, 4, 5}); // L
    cp_moves[4] = perm_move<8, 4>({0, 5, 6, 3}); // F
    cp_moves[5] = perm_move<8, 4>({1, 2, 7, 4}); // B
    //ep_moves
    ep_moves[0] = perm_move<12, 4>({0, 3, 2, 1}); // U
    ep_moves[1] = perm_move<12, 4>({8, 11, 10, 9}); // D
    ep_moves[2] = perm_move<12, 4>({3, 6, 11, 7}); // R
    ep_moves[3] = perm_move<12, 4>({1, 4, 9, 5}); // L
    ep_moves[4] = perm_move<12, 4>({0, 5, 10, 6}); // F
    ep_moves[5] = perm_move<12, 4>({2, 7, 8, 4}); // B
    ep_moves[6] = perm_move<12, 4>({0, 10, 8, 2}); // M
    ep_moves[7] = perm_move<12, 4>({1, 9, 11, 3}); // S
    ep_moves[8] = perm_move<12, 4>({4, 5, 6, 7}); // E
    //co_moves
    co_moves[0] = ori_move<8, 3, 4>({0, 3, 2, 1}, {0, 0, 0, 0}); // U
    co_moves[1] = ori_move<8, 3, 4>({4, 7, 6, 5}, {0, 0, 0, 0}); // D
    co_moves[2] = ori_move<8, 3, 4>({2, 3, 6, 7}, {1, 2, 1, 2}); // R
    co_moves[3] = ori_move<8, 3, 4>({0, 1, 4, 5}, {1, 2, 1, 2}); // L
    co_moves[4] = ori_move<8, 3, 4>({0, 5, 6, 3}, {2, 1, 2, 1}); // F
    co_moves[5] = ori_move<8, 3, 4>({1, 2, 7, 4}, {1, 2, 1, 2}); // B
    //eo_moves
    eo_moves[0] = ori_move<12, 2, 4>({0, 3, 2, 1}, {0, 0, 0, 0}); // U
    eo_moves[1] = ori_move<12, 2, 4>({8, 11, 10, 9}, {0, 0, 0, 0}); // D
    eo_moves[2] = ori_move<12, 2, 4>({3, 6, 11, 7}, {0, 0, 0, 0}); // R
    eo_moves[3] = ori_move<12, 2, 4>({1, 4, 9, 5}, {0, 0, 0, 0}); // L
    eo_moves[4] = ori_move<12, 2, 4>({0, 5, 10, 6}, {1, 1, 1, 1}); // F
    eo_moves[5] = ori_move<12, 2, 4>({2, 7, 8, 4}, {1, 1, 1, 1}); // B
    eo_moves[6] = ori_move<12, 2, 4>({0, 10, 8, 2}, {1, 1, 1, 1}); // M
    eo_moves[7] = ori_move<12, 2, 4>({1, 9, 11, 3}, {1, 1, 1, 1}); // S
    eo_moves[8] = ori_move<12, 2, 4>({4, 5, 6, 7}, {1, 1, 1, 1}); // E
}