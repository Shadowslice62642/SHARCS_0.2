#pragma once

template<char corner_count, char corner_ori, char corner_orientation, char corner_moves, char edge_count, char edge_ori, char edge_orientation, char edge_moves>
bool solve(array<c_move<corner_count, corner_ori, corner_orientation>, corner_moves> &cc_moves, array<c_move<edge_count, edge_ori, edge_orientation>, edge_moves> &ce_moves, //moves
            array<char, corner_count> &corners, array<char, edge_count> &edges, //pieces
            char max_depth, stack<string> &solution, char last_move ) { //misc
    bool solved = false;
    char i = 0, j = 1, move_count = (corner_moves < edge_moves) ? corner_moves : edge_moves;
    while ( !solved && i < move_count ) {
        if (i == last_move) { i++; continue; }
        j = 1;
        while (j < cc_moves[i].move_order) {
            cc_moves[i](corners);
            ce_moves[i](edges);
            switch (j) {
                case 1:
                    solution.push(cc_moves[i].name + " ");
                    break;
                case 2:
                    solution.pop();
                    solution.push(cc_moves[i].name + "2 ");
                    break;
                case 3:
                    solution.pop();
                    solution.push(cc_moves[i].name + "\' ");
            }
            if (cc_moves[i].solved(corners) && ce_moves[i].solved(edges)) {
                return true;
            } else if (solution.size() < max_depth) {
                solved = solve<corner_count, corner_ori, corner_orientation, corner_moves, edge_count, edge_ori, edge_orientation, edge_moves>(cc_moves, ce_moves, corners, edges, max_depth, solution, i);
                if (solved) { return true; }
            }
            j++;
        }
        cc_moves[i](corners);
        ce_moves[i](edges);
        solution.pop();
        i++;
    }
    return false;
};

template<char corner_count, char corner_ori, char corner_orientation, char corner_moves, char edge_count, char edge_ori, char edge_orientation, char edge_moves>
void solve(array<c_move<corner_count, corner_ori, corner_orientation>, corner_moves> cc_moves, array<c_move<edge_count, edge_ori, edge_orientation>, edge_moves> ce_moves, array<char, corner_count> &corners, array<char, edge_count> &edges, char max_depth) {
    bool solved = false;
    stack<string> solution;
    string actual_solution = "";
    int i = 0;

    auto start = high_resolution_clock::now();
    auto start1 = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    float seconds_duration = duration.count();
    while (i < max_depth & !solved) {
        i++;
        start1 = high_resolution_clock::now();
        solved = solve<8, 3, 4, 6, 12, 2, 4, 9>(cc_moves, ce_moves, corners, edges, i, solution, 12);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        seconds_duration = duration.count();
        seconds_duration /= 1000000;
        if (!solved) { cout << "No solution at depth " << i << endl << "Searched in " << seconds_duration << " seconds" << endl; }
    }
    stop = high_resolution_clock::now();

    while (solution.size() > 0) {
        actual_solution = solution.top() + actual_solution;
        solution.pop();
    }

    cout << "Solution found at depth " << i << ": " << actual_solution << endl;
    duration = duration_cast<microseconds>(stop - start);
    seconds_duration = duration.count();
    seconds_duration /= 1000000;
    cout << "Solutions found in " << seconds_duration << " seconds"; 
    return;
};