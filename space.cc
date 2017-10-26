
/**************************************************
    Gus Oberdick
    CS 2014 - Checkers Project
    Phase 3

    This is the implementation class for the Space
    class
***************************************************/
#include "space.h"

/*********************
    CONSTRUCTOR
*********************/
Space::Space()
{
    occupied=king=playable=false;
    b_color="";
    piece="";
}

Space::Space(bool space_playable, bool space_occupied, bool space_king, std::string space_color)
{
    playable = space_playable;
    occupied = space_occupied;
    king = space_king;
    b_color = space_color;
    // set spot[array][]
    for(int i=0; i<space_size; ++i)
    {
        for(int j=0; j<space_size; ++j)
        {
            spot[j][i]=b_color+"  ";
        }
    }
}

/*********************
      MUTATORS
*********************/
void Space::set_char(std::string piece_color, char character)
{
    spot[1][1]=b_color+piece_color+character+" ";
    occupied = false;
    playable = false;

}

void Space::set_piece(std::string piece_color)
{
    spot[1][1]=b_color+piece_color+"OO";
    occupied = true;
    piece = piece_color;
}

void Space::set_king(std::string piece_color)
{
    spot[1][1]=b_color+piece_color+"MM";
    king = true;
    occupied = true;
    piece = piece_color;
}

/*********************
      DISPLAY
*********************/
// print the space
void Space::print_line(int line)const
{
    for(int i=0; i<space_size; ++i)
    {
        std::cout << spot[i][line];
        std::cout << RESET;
    }
}




