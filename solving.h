#pragma once

template<char corner_count, char corner_ori, char corner_orientation, char corner_moves, char edge_count, char edge_ori, char edge_orientation, char edge_moves>
bool solve(array<c_move<corner_count, corner_ori, corner_orientation>, corner_moves> &cc_moves, array<c_move<edge_count, edge_ori, edge_orientation>, edge_moves> &ce_moves, array<char, corner_count> &corners, array<char, edge_count> &edges, char move_order, char depth, char max_depth) {
    bool solved = false;
    char i = 0;
    char move_count = (corner_moves < edge_moves) ? corner_moves : edge_moves;
    while ( !solved && i < move_count ) {
        char j = 1;
        while (j < move_order) {
            cc_moves[i](corners);
            ce_moves[i](edges);
            solved = ((corners[0] >> 3) == 0) && ((corners[0] & 7) == 0);
            for (int k = 1; k < corner_count; k++) {
                solved &= ((corners[k] >> 3) == k) && ((corners[k] & 7) == 0);
            }
            for (int k = 0; k < edge_count; k++) {
                solved &= ((edges[k] >> 3) == k) && ((edges[k] & 7) == 0);
            }
            if (solved) {
                cout << cc_moves[i].name;
                switch (j) {
                    case 1:
                        cout << " ";
                        break;
                    case 2:
                        cout << "2 ";
                        break;
                    case 3:
                        cout << "\' ";
                }
                return true;
            } else if (depth < max_depth) {
                solved = solve<corner_count, corner_ori, corner_orientation, corner_moves, edge_count, edge_ori, edge_orientation, edge_moves>(cc_moves, ce_moves, corners, edges, move_order, depth + 1, max_depth);
                if (solved) {
                    cout << cc_moves[i].name;
                    switch (j) {
                        case 1:
                            cout << " ";
                        break;
                        case 2:
                            cout << "2 ";
                        break;
                        case 3:
                            cout << "\' ";
                    }
                    return true;
                }
            }
            j++;
        }
        cc_moves[i](corners);
        ce_moves[i](edges);
        i++;
    }
    return false;
};