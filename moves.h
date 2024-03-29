#pragma once

template<
        template<char, char, char> class corner_move, char corner_length, char corner_ori, char corner_oriented, 
        template<char, char, char> class edge_move, char edge_length, char edge_ori, char edge_oriented,
        uint64_t movecount>
class movepair
{
    corner_move<corner_length, corner_ori, corner_oriented> _corners;
    edge_move<edge_length, edge_ori, edge_oriented> _edges;

    movepair(corner_move<corner_length, corner_ori, corner_oriented> cmoves, edge_move<edge_length, edge_ori, edge_oriented> emoves) : _corners(cmoves), _edges(emoves) {};
    void operator() (array<char, corner_length> &corners, array<char, edge_length> &edges) {
        //cmoves
    }
};

template<
        template<char, char, char> class corner_move, char corner_length, char corner_ori, char corner_oriented, 
        template<char, char, char> class edge_move, char edge_length, char edge_ori, char edge_oriented,
        uint64_t movecount>
class puzzle
{
public:
    array<corner_move<corner_length, corner_ori, corner_oriented>, movecount> _corner_moves;
    array<edge_move<edge_length, edge_ori, edge_oriented>, movecount> _edge_moves;
    array<char, movecount> move_orders;
    array<string, movecount> move_names;

    puzzle(array<corner_move<corner_length, corner_ori, corner_oriented>, movecount> corner_moves, array<edge_move<edge_length, edge_ori, edge_oriented>, movecount> edge_moves):
                _corner_moves(corner_moves), _edge_moves(edge_moves) { calculate_orders(); GetNames(); };

    void operator[] (int i) {
        return _corner_moves[i];
    }

    void calculate_orders() {
        char mo;
        bool identity;
        array<char, corner_length> corner_array;
        array<char, edge_length> edge_array;
        for(char move = 0; move < movecount; move++){
            mo = 0;
            identity = false;
            
            for (char i = 0; i < corner_length; i++) { corner_array[i] = (i << 3); }
            for (char i = 0; i < edge_length; i++) { edge_array[i] = (i << 3); }
            
            while (!identity) {
                _corner_moves[move](corner_array);
                _edge_moves[move](edge_array);
                identity = solved(corner_array, edge_array);
                mo++;
            }

            move_orders[move] = mo;

        }
    }

    bool solved(array<char, corner_length> corner_array, array<char, edge_length> edge_array) {
        bool solved = ((corner_array[0] >> 3) == 0) && ((corner_array[0] & 0b00000111) == 0);
        for (char i = 1; i < corner_length; i++) {
            solved &= ((corner_array[i] >> 3) == i) && ((corner_array[i] & 0b00000111) == 0);
        }
        for (char i = 0; i < edge_length; i++) {
            solved &= ((edge_array[i] >> 3) == i) && ((edge_array[i] & 0b00000111) == 0);
        }
        return solved;
    }

    void GetNames() {
        for(int i = 0; i < movecount; i++) {
            move_names[i] = _corner_moves[i].name;
        }
    }

};


//define general moves on merged array
template<char length, char ori, char oriented>
class c_move
{
public:
	
	char len = length;
    string name;
    char move_order;

	c_move() : _permuting_array({0, 3, 2, 1}), _orienting_array({0, 0, 0, 0}) {} //defaults to U corner
    c_move(array<char, oriented> permuting_array, array<char, oriented> orienting_array) : _permuting_array(permuting_array), _orienting_array(orienting_array) { calculate_order(); } //take move definition and find move order
    c_move(array<char, oriented> permuting_array, array<char, oriented> orienting_array, string name) : _permuting_array(permuting_array), _orienting_array(orienting_array), name(name) { calculate_order(); } //take move definition and name and find move order
    
	void operator()(array<char, length> &piece_array) {
		char x;
		x = ((piece_array[_permuting_array[0]] & 7) + _orienting_array[0]) % _ori;
        x += ((piece_array[_permuting_array[0]] >> 3) << 3);
        for (int i = 0; i < oriented - 1; i++) {
            piece_array[_permuting_array[i]] = ((piece_array[_permuting_array[i + 1]] & 0b00000111) + _orienting_array[i + 1]) % _ori;
            piece_array[_permuting_array[i]] += (piece_array[_permuting_array[i + 1]] & 0b11111000);
        }
		piece_array[_permuting_array[oriented - 1]] = x;
	}

    void calculate_order() {
        char mo = 0;
        bool identity = false;
        array<char,length> piece_array;
        for (int i = 0; i < length; i++) { piece_array[i] = (i << 3); }
        
        while (!identity) {
            (*this)(piece_array);
            identity = solved(piece_array);
            mo++;
        }

        move_order = mo;
    }

    bool solved(array<char, length> piece_array) {
        bool solved = ((piece_array[0] >> 3) == 0) && ((piece_array[0] & 7) == 0);
        for (int i = 1; i < piece_array.size(); i++) {
            solved &= ((piece_array[i] >> 3) == i) && ((piece_array[i] & 7) == 0);
        }
        return solved;
    }

    //print the state of given array
    void print(array<char, length> piece_array) {
        cout << "Permutation: ";
        for (int i = 0; i < len; i++) {
            cout << ((int)piece_array[i] >> 3) << " ";
        }
        cout << endl << "Orientation: ";
        for (int i = 0; i < len; i++) {
            cout << ((piece_array[i] & 7) % _ori) << " ";
        }
        cout << endl;
    }

private:
    array<char, oriented> _permuting_array;
    array<char, oriented> _orienting_array;
    array<char, oriented> _solved_state;
    char _ori = ori;
};


//define general orientation moves
template<char length, char ori, char oriented>
class ori_move
{
public:
	char len = length;
	char _ori = ori;

	ori_move() : _permuting_array({0, 3, 2, 1}), _orienting_array({0, 0, 0, 0}) {} //defaults to U corner
	ori_move(array<char, oriented> permuting_array, array<char, oriented> orienting_array) : _permuting_array(permuting_array), _orienting_array(orienting_array) {}
	void operator()(array<char, length> &ori_array) {
		char x;
		x = (ori_array[_permuting_array[0]] + _orienting_array[0]) % _ori;
        for (int i = 0; i < oriented - 1; i++) {
            ori_array[_permuting_array[i]] = (ori_array[_permuting_array[i + 1]] + _orienting_array[i + 1]) % _ori;
        }
		ori_array[_permuting_array[oriented - 1]] = x;
	}

    //print the state of given array
    void print(array<char, length> piece_array) {
        cout << endl << "Orientation: ";

        for (int i = 0; i < len; i++) {
            cout << piece_array[i] << " ";
        }

        cout << endl;
    }

private:
    array<char, oriented> _permuting_array;
    array<char, oriented> _orienting_array;
};


//define general permutation moves
template<char length, char permuted>
class perm_move
{
public:
	char len = length;
    char perm = permuted;

	perm_move() {} //: _permuting_array({0,3,2,1}) {} //defaults to U
	perm_move(array<char, permuted> permuting_array) : _permuting_array(permuting_array) {}
	void operator()(array<char, length> &perm_array) {
		char x;
        x = perm_array[_permuting_array[0]];
        for (int i = 0; i < perm - 1; i++) {
            perm_array[_permuting_array[i]] = perm_array[_permuting_array[i + 1]];
        }
		perm_array[_permuting_array[perm - 1]] = x;
	}

private:
	array<char, permuted> _permuting_array;
};

void loadStandardMoves(array<perm_move<8, 4>, 6> &cp_moves, array<perm_move<12, 4>, 9> &ep_moves, array<ori_move<8, 3, 4>, 6> &co_moves, array<ori_move<12, 2, 4>, 9> &eo_moves) {
    
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

void loadStandardMoves(array<c_move<8, 3, 4>, 6> &cc_moves, array<c_move<12, 2, 4>, 6> &ce_moves) {

    //combined corner_moves
    cc_moves[0] = c_move<8, 3, 4>({0, 3, 2, 1}, {0, 0, 0, 0}, "U"); // U
    cc_moves[1] = c_move<8, 3, 4>({4, 7, 6, 5}, {0, 0, 0, 0}, "D"); // D
    cc_moves[2] = c_move<8, 3, 4>({2, 3, 6, 7}, {1, 2, 1, 2}, "R"); // R
    cc_moves[3] = c_move<8, 3, 4>({0, 1, 4, 5}, {1, 2, 1, 2}, "L"); // L
    cc_moves[4] = c_move<8, 3, 4>({0, 5, 6, 3}, {2, 1, 2, 1}, "F"); // F
    cc_moves[5] = c_move<8, 3, 4>({1, 2, 7, 4}, {1, 2, 1, 2}, "B"); // B

    //combined edge_moves
    ce_moves[0] = c_move<12, 2, 4>({0, 3, 2, 1}, {0, 0, 0, 0}, "U"); // U
    ce_moves[1] = c_move<12, 2, 4>({8, 11, 10, 9}, {0, 0, 0, 0}, "D"); // D
    ce_moves[2] = c_move<12, 2, 4>({3, 6, 11, 7}, {0, 0, 0, 0}, "R"); // R
    ce_moves[3] = c_move<12, 2, 4>({1, 4, 9, 5}, {0, 0, 0, 0}, "L"); // L
    ce_moves[4] = c_move<12, 2, 4>({0, 5, 10, 6}, {1, 1, 1, 1}, "F"); // F
    ce_moves[5] = c_move<12, 2, 4>({2, 7, 8, 4}, {1, 1, 1, 1}, "B"); // B
    //ce_moves[6] = c_move<12, 2, 4>({0, 10, 8, 2}, {1, 1, 1, 1}, "M"); // M
    //ce_moves[7] = c_move<12, 2, 4>({1, 9, 11, 3}, {1, 1, 1, 1}, "S"); // S
    //ce_moves[8] = c_move<12, 2, 4>({4, 5, 6, 7}, {1, 1, 1, 1}, "E"); // E
}