
#include <iostream>
#include <chrono>
#include <array>
#include <stack>
#include <string>

using namespace std;
using namespace std::chrono;

#include "moves.h"
#include "solving.h"

//standard moves
auto cp_moves = array<perm_move<8, 4>, 6>{};
auto ep_moves = array<perm_move<12, 4>, 9>{};
auto co_moves = array<ori_move<8, 3, 4>, 6>{};
auto eo_moves = array<ori_move<12, 2, 4>, 9>{};
auto cc_moves = array<c_move<8, 3, 4>, 6>{};
auto ce_moves = array<c_move<12, 2, 4>, 9>{};

int main()
{
    array<char, 8> cp = {0, 1, 2, 3, 4, 5, 6, 7};
    array<char, 8> co = {0, 0, 0, 0, 0, 0, 0, 0};
    array<char, 8> combined_corners = {0, 1, 2, 3, 4, 5, 6, 7};
    for (int i = 0; i < 8; i++) { combined_corners[i] <<= 3;};
    array<char, 12> combined_edges = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    for (int i = 0; i < 12; i++) { combined_edges[i] <<= 3;};
    loadStandardMoves(cp_moves, ep_moves, co_moves, eo_moves);
    loadStandardMoves(cc_moves, ce_moves);
    cp_moves[4](cp);
    co_moves[4](co);

    /*cout << "Permutation: ";
    for (int i = 0; i < 8; i++) {
        cout << (int)cp[i] << " ";
    }
    cout << endl << "Orientation: ";
    for (int i = 0; i < 8; i++) {
        cout << (int)co[i] << " ";
    }
    cout << endl;*/

    cc_moves[4](combined_corners);
    ce_moves[4](combined_edges);
    cc_moves[4](combined_corners);
    ce_moves[4](combined_edges);

    cc_moves[1](combined_corners);
    ce_moves[1](combined_edges);
    cc_moves[1](combined_corners);
    ce_moves[1](combined_edges);

    cc_moves[4](combined_corners);
    ce_moves[4](combined_edges);

    cc_moves[3](combined_corners);
    ce_moves[3](combined_edges);

    cc_moves[5](combined_corners);
    ce_moves[5](combined_edges);

    cc_moves[0](combined_corners);
    ce_moves[0](combined_edges);

    cc_moves[1](combined_corners);
    ce_moves[1](combined_edges);
    cc_moves[1](combined_corners);
    ce_moves[1](combined_edges);
    cc_moves[1](combined_corners);
    ce_moves[1](combined_edges);

    cc_moves[2](combined_corners);
    ce_moves[2](combined_edges);
    cc_moves[2](combined_corners);
    ce_moves[2](combined_edges);
    cc_moves[2](combined_corners);
    ce_moves[2](combined_edges);

    auto start = high_resolution_clock::now();
    solve<8, 3, 4, 6, 12, 2, 4, 9>(cc_moves, ce_moves, combined_corners, combined_edges, 12);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    float seconds_duration = duration.count();
    seconds_duration /= 1000000;
    cout << "Solutions found in " << seconds_duration << " seconds"; 

}

