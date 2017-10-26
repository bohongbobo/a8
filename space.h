/**************************************************
    Gus Oberdick
    CS 2014 - Checkers Project
    Phase 3

    This is the declaration file for the class
    Space. The class Space stores all of individual
    spaces. This class also acts as a "piece" class.
***************************************************/

#ifndef SPACE
#define SPACE

#include <iostream>
#include "colors.h"

class Space
{
public:
    // Constructors
    Space();
    Space(bool space_playable, bool space_occupied, bool space_king, std::string space_color);

    // accessors
    bool is_occupied()const{return occupied;}
    bool is_king()const{return king;}
    bool is_playable()const{return playable;}
    std::string get_color()const{return b_color;}
    std::string get_piece()const{return piece;}
    int get_size()const{return space_size;}

    // mutators
    void set_char(std::string piece_color, char character);
    void set_piece(std::string piece_color);
    void set_king(std::string piece_color);

    // display space
    void print_line(int line)const;

private:
    const static int space_size=3;
    bool occupied, king, playable;
    std::string b_color, spot[space_size][space_size];
    std::string piece;
};

#endif // SPACE_H
