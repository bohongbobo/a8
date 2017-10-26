/**************************************************
    Gus Oberdick
    CS 2014 - Checkers Project
    Phase 3

    This is the Declaration file for the class
    Checkers. This is class is hold functions that
    stores a board of spaces (Space class), and
    functions that manipulate and store the rules
    for a checkers game
***************************************************/
#ifndef CHECKERS
#define CHECKERS

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <vector>
#include "space.h"
#include "game.h"

/****************

    Jumps: can create a jump check function

*****************/

using namespace main_savitch_14;

class Checkers: public game
{

public:
    Checkers();
    ~Checkers();

    void get_board_info()const;

    // ACCESSOR
    int get_side(){return board_size;}
    int get_moves(){return move_number;}

    // turn stuff
    game::who last_mover()const;

    // BUILD CHECKER BOARD
    void restart();
    void build_board();
    void set_board();
    virtual game *clone()const{return new Checkers(*this);}
//    Checkers *clone()const{return new Checkers(*this);}
    virtual Checkers *clone_me()const {return new Checkers(*this);}
    void compute_moves(std::queue<std::string>& moves) const;
    int evaluate()const;

    /*****************
        Test boards
    *****************/
    void set_jump_board();
    void set_king_board();
    bool is_game_over()const;

    // MOVE and MANIPULATION FUNCTIONS
    std::string get_turn()const;
    Space get_space(int x, int y)const;
    std::string construct_move(int x1, int y1, int x2, int y2)const;
    void construct_mult_jumps(std::vector<std::string> jumps, std::queue<std::string>& moves)const;
    bool king_ready(int x, int y)const;

    // JUMP FUNCTIONS
    void remove_piece(int x, int y);
    std::vector<std::string> check_open_neighbor(int x, int y)const;
    void print_jumps(int x,int y)const;
    bool is_single_jump(std::string move, int x, int y)const;
    bool is_jump()const;
    bool is_mult_jump(int x, int y)const;
    void jump(const std::string& move);
    void mult_jump(const std::string& move);
    void make_move(const std::string& move);
    bool is_legal(const std::string& move) const; // look at back of text

    // DISPLAY FUNCTIONS
    std::string get_user_move()const;
    void display_status()const;

private:
    int move_number, black_pieces, white_pieces;
    const int static board_size=10;
    Space board[board_size][board_size];
    std::string player[2];
};

/********************************************************

    Board configuration
    ==========
    |# # # # |
    | # # # #|
    |# # # # |
    | # # # #|
    |# # # # |
    | # # # #|
    |# # # # |
    | # # # #|
    ==========

********************************************************/

#endif // CHECKERS_H
