/*************************************

    Gus Oberdick
    CS 2014 - Checkers Project
    Phase 3

    *NOTE NOTE NOTE
    This isn't working perfectly, and
    evaluation is really easy. The computer
    does make moves and jumps but there are
    some occations where a invalid move
    takes place. I can't explain these,
    but I wanted to submit what I have,
    and will be redoing a lot of it and
    resubmitting when I have time with finals.

*************************************/

#include <iostream>
#include "checkers.h"

using namespace std;
using namespace main_savitch_14;

int main()
{
    Checkers chk;
    game::who winner;
    winner = chk.play();
    return 0;
}
