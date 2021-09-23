// SHARCS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <array>
#include "perm_move.h"
#include "ori_move.h"
#include "coord_move.h"

//standard moves
auto cp_moves = std::array<perm_move<8>, 18>{};
auto ep_moves = std::array<perm_move<12>, 18>{};
auto co_moves = std::array<ori_move<8, 3>, 18>{};
auto eo_moves = std::array<ori_move<12, 2>, 18>{};

void loadStandardMoves() {
    //cp_moves
    cp_moves[0] = perm_move<8>(0, 3, 2, 1); // U
    cp_moves[1] = perm_move<8>(4, 7, 6, 5); // D
    cp_moves[2] = perm_move<8>(2, 3, 6, 7); // R
    cp_moves[3] = perm_move<8>(0, 1, 4, 5); // L
    cp_moves[4] = perm_move<8>(0, 5, 6, 3); // F
    cp_moves[5] = perm_move<8>(1, 2, 7, 4); // B
    //ep_moves
    ep_moves[0] = perm_move<12>(0, 3, 2, 1); // U
    ep_moves[1] = perm_move<12>(8, 11, 10, 9); // D
    ep_moves[2] = perm_move<12>(3, 6, 11, 7); // R
    ep_moves[3] = perm_move<12>(1, 4, 9, 5); // L
    ep_moves[4] = perm_move<12>(0, 5, 10, 6); // F
    ep_moves[5] = perm_move<12>(2, 7, 8, 4); // B
    ep_moves[6] = perm_move<12>(0, 10, 8, 2); // M
    ep_moves[7] = perm_move<12>(1, 9, 11, 3); // S
    ep_moves[8] = perm_move<12>(4, 5, 6, 7); // E
    //co_moves
    co_moves[0] = ori_move<8, 3>(0, 3, 2, 1, 0, 0, 0, 0); // U
    co_moves[1] = ori_move<8, 3>(4, 7, 6, 5, 0, 0, 0, 0); // D
    co_moves[2] = ori_move<8, 3>(2, 3, 6, 7, 1, 2, 1, 2); // R
    co_moves[3] = ori_move<8, 3>(0, 1, 4, 5, 1, 2, 1, 2); // L
    co_moves[4] = ori_move<8, 3>(0, 5, 6, 3, 2, 1, 2, 1); // F
    co_moves[5] = ori_move<8, 3>(1, 2, 7, 4, 1, 2, 1, 2); // B
    //eo_moves
    eo_moves[0] = ori_move<12, 2>(0, 3, 2, 1, 0, 0, 0, 0); // U
    eo_moves[1] = ori_move<12, 2>(8, 11, 10, 9, 0, 0, 0, 0); // D
    eo_moves[2] = ori_move<12, 2>(3, 6, 11, 7, 0, 0, 0, 0); // R
    eo_moves[3] = ori_move<12, 2>(1, 4, 9, 5, 0, 0, 0, 0); // L
    eo_moves[4] = ori_move<12, 2>(0, 5, 10, 6, 1, 1, 1, 1); // F
    eo_moves[5] = ori_move<12, 2>(2, 7, 8, 4, 1, 1, 1, 1); // B
    eo_moves[6] = ori_move<12, 2>(0, 10, 8, 2, 1, 1, 1, 1); // M
    eo_moves[7] = ori_move<12, 2>(1, 9, 11, 3, 1, 1, 1, 1); // S
    eo_moves[8] = ori_move<12, 2>(4, 5, 6, 7, 1, 1, 1, 1); // E
}

int main()
{
    loadStandardMoves();
    /*std::array<char, 8> perm;
    std::iota(perm.begin(), perm.end(), 0);
    for (int i = 0; i < 1234567890; i++) {
        cp_moves[i % 6](perm);
    }
    for (int i = 0; i < 8; i++) {
        std::cout << (int)perm[i] << " ";
    }*/
    makePruningTables(ep_moves, cp_moves, eo_moves, co_moves);
}
