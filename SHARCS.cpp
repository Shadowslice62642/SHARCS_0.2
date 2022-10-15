// SHARCS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <array>
#include "moves.h"

//standard moves
auto cp_moves = std::array<perm_move<8, 4>, 6>{};
auto ep_moves = std::array<perm_move<12, 4>, 9>{};
auto co_moves = std::array<ori_move<8, 3, 4>, 6>{};
auto eo_moves = std::array<ori_move<12, 2, 4>, 9>{};
auto cc_moves = std::array<move<8, 3, 4>, 6>{};

int main()
{
    std::array<char, 8> cp = {0, 1, 2, 3, 4, 5, 6, 7};
    std::array<char, 8> co = {0, 0, 0, 0, 0, 0, 0, 0};
    std::array<char, 8> combined_corners = {0, 1, 2, 3, 4, 5, 6, 7};
    for (int i = 0; i < 8; i++) { combined_corners[i] <<= 4; std::cout << (int)combined_corners[i] << " ";};
    std::cout << std::endl;
    loadStandardMoves(cp_moves, ep_moves, co_moves, eo_moves, cc_moves);
    cc_moves[5].print(combined_corners);

    cp_moves[5](cp);

    std::cout << "Permutation: ";
    for (int i = 0; i < 8; i++) {
        std::cout << (int)cp[i] << " ";
    }
    std::cout << std::endl;

    co_moves[5](co);
    
    std::cout << "Orientation: ";
    for (int i = 0; i < 8; i++) {
        std::cout << (int)co[i] << " ";
    }
    std::cout << std::endl;

    cc_moves[5](combined_corners);

    cc_moves[5].print(combined_corners);
}

