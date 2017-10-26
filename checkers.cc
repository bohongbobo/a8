
/**************************************************
    Gus Oberdick
    CS 2014 - Checkers Project
    Phase 3

    This is the implementation class for the Checkers
    class
***************************************************/
#include "checkers.h"

Checkers::Checkers()
{
    player[0]=WHITE;
    player[1]=BLACK;
    move_number=0;
    white_pieces=black_pieces=0;
//    black_pieces=12;
//    white_pieces=12;
}

Checkers::~Checkers()
{
}

void Checkers::get_board_info()const
{
    for(int y=0;y<board_size;++y)
    {
        for(int x=0;x<board_size;++x)
        {
            Space tmp = get_space(x,y);
            std::cout << "Piece: " << x << " " << y << " ";
            std::cout << tmp.is_king() << " " <<  tmp.is_occupied() << " " << tmp.is_playable() << std::endl;
        }
    }
}

game::who Checkers::last_mover()const
{
    return (move_number % 2 == 1 ? HUMAN : COMPUTER);
}

/*********************
    BOARD HANDLING
*********************/
void Checkers::restart()
{
    build_board();
    set_board();
//    set_jump_board();
//    move_number=1;
//    set_king_board();

    for(int y=0;y<board_size;++y)
    {
        for(int x=0;x<board_size;++x)
        {
            if(board[x][y].is_playable()&&board[x][y].is_occupied()&&board[x][y].get_piece()==player[0])
            {
                white_pieces+=1;
            }
            else if(board[x][y].is_playable()&&board[x][y].is_occupied()&&board[x][y].get_piece()==player[1])
            {
                black_pieces+=1;
            }
        }
    }
    std::cout << "WHITE: " << white_pieces << " BLACK: " << black_pieces << std::endl;
}

void Checkers::build_board()
{
    int i,j;
    i=j=0;
    ifstream fin;
    fin.open("checkerboard.txt");
    char tmp;
    while(!fin.eof())
    {
        if(fin.peek()=='\n')
            fin.ignore();
        fin >> tmp;
        if(tmp=='B')
        {
            board[i][j]=Space(false,false,false,B_WHITE);
        }
        else if(tmp=='I')
        {
            board[i][j]=Space(false,false,false,B_BLACK);
        }
        else if(tmp=='X')
        {
            board[i][j]=Space(true,false,false,B_RED);
        }
        ++i;
        // increment i and j
        if(i>=board_size)
        {
            i=0;
            ++j;
        }
    }
    fin.close();
}

void Checkers::set_board()
{
    // letters and numbers for board
    char letter='A';
    char num = '1';
    for(int i=0;i<board_size;++i)
    {
        for(int j=0;j<board_size;++j)
        {
            // check if spot is playable and not in rows
            // and if the pieces aren't in rows 3 & 4

            // WHITE PIECES
            if((board[j][i].is_playable()==true) && (i==6 || i==7 || i==8))
            {
                board[j][i].set_piece(player[0]);
            }
            // BLACK PIECES
            else if((board[j][i].is_playable()==true) && (i==1 || i==2 || i==3))
            {
                board[j][i].set_piece(player[1]);
            }
            // BORDER LETTERS
            else if((!board[j][i].is_playable())&&i==0&&(j!=0&&j!=9))
            {
                board[j][i].set_char(BLACK, letter);
                letter += 1;
            }
            // BORDER NUMBERS
            else if((!board[j][i].is_playable())&&j==0&&(i!=0&&i!=9))
            {
                board[j][i].set_char(BLACK, num);
                num +=1;
            }
        }
    }
}

void Checkers::compute_moves(std::queue<std::string>& moves) const
{
    // Get color
    std::string c_player, move;
    c_player=get_turn();
//    if(c_player==BLACK)
//    {
//        std::cout << "BLACK" << std::endl;
//    }
//    else
//    {
//        std::cout << "WHite" << std::endl;
//    }
    for(int y=1;y<board_size;++y)
    {
        for(int x=1;x<board_size;++x)
        {
            if(board[x][y].is_playable()&&board[x][y].is_occupied())
            {
                /*********************
                    Get Simple Moves
                **********************/
//                std::cout << "Board[" << x << "][" << y << "]" << std::endl;
                if(is_jump()||is_mult_jump(x, y))
                {
                    std::vector<std::string> single_moves=check_open_neighbor(x,y), mult_moves;
                    for(std::size_t i=0;i<single_moves.size();++i)
                    {
                        moves.push(single_moves[i]);
                    }
                    /*********************************
                        check for multiple jumps
                    *********************************/
                    if(moves.size()!=0&&is_mult_jump(x,y))
                    {
                        construct_mult_jumps(single_moves, moves);
                    }
                }
                if(board[x][y].get_piece()==c_player||board[x][y].is_king())
                {
                    if(board[x-1][y-1].is_playable()&&!board[x-1][y-1].is_occupied())
                    {
                        move = construct_move(x,y,x-1,y-1);
                        moves.push(move);
                    }
                    if(board[x+1][y-1].is_playable()&&!board[x+1][y-1].is_occupied())
                    {
                        move = construct_move(x,y,x+1,y-1);
                        moves.push(move);
                    }
                }
                if(board[x][y].get_piece()==c_player||board[x][y].is_king())
                {
                    if(board[x-1][y+1].is_playable()&&!board[x-1][y+1].is_occupied())
                    {
                        move = construct_move(x,y,x-1,y+1);
                        moves.push(move);
                    }
                    if(board[x+1][y+1].is_playable()&&!board[x+1][y+1].is_occupied())
                    {
                        move = construct_move(x,y,x+1,y+1);
                        moves.push(move);
                    }
                }
            }
        }
    }
//    std::cout << "Size: " << moves.size() << std::endl;
//    for(std::queue<std::string> i=moves;!i.empty();i.pop())
//        std::cout << i.front() << std::endl;
//    exit(EXIT_FAILURE);
}

int Checkers::evaluate()const
{
    // TOTAL POINTS
    int points=0;

    // determine whose who
    // human--> WHITE, computer--> BLACK
    std::string human = WHITE, comp = BLACK;

    // Count who has more players
    points = black_pieces-white_pieces;

    for(int y=0; y<board_size;++y)
    {
        for(int x=0; x<board_size;++x)
        {

            // Count who has more kings
            if(board[x][y].is_king())
            {
                if(board[x][y].get_piece()==WHITE)
                {
                    points-=3;
                }
                else
                {
                    points+=3;
                }
            }

        }
    }

    return points;
}

void Checkers::set_jump_board()
{
    // letters and numbers for board
    char letter='A';
    char num = '1';
    for(int i=0;i<board_size;++i)
    {
        for(int j=0;j<board_size;++j)
        {
            // check if spot is playable and not in rows
            // and if the pieces aren't in rows 3 & 4

            // WHITE PIECES
            if((board[j][i].is_playable()==true) && (i==5&&j==6))
            {
//                board[j][i].set_piece(player[1]);
                board[j][i].set_king(player[0]);
            }
            // BLACK PIECES
            else if((board[j][i].is_playable()==true) && (i==2||i==4||i==6)&&(j==3||j==5||j==7))
            {
                board[j][i].set_piece(player[1]);
            }
            // BORDER LETTERS
            else if((!board[j][i].is_playable())&&i==0&&(j!=0&&j!=9))
            {
                board[j][i].set_char(BLACK, letter);
                letter += 1;
            }
            // BORDER NUMBERS
            else if((!board[j][i].is_playable())&&j==0&&(i!=0&&i!=9))
            {
                board[j][i].set_char(BLACK, num);
                num +=1;
            }
        }
    }
}

// TEST BOARDS
void Checkers::set_king_board()
{
    // letters and numbers for board
    char letter='A';
    char num = '1';
    for(int i=0;i<board_size;++i)
    {
        for(int j=0;j<board_size;++j)
        {
            // check if spot is playable and not in rows
            // and if the pieces aren't in rows 3 & 4

            // WHITE PIECES
            if((board[j][i].is_playable()==true) && (i==3&&j==6))
//            if((board[j][i].is_playable()==true) && (i==1&&j==6))
            {
                board[j][i].set_king(player[0]);
            }
            // BLACK PIECES
            else if((board[j][i].is_playable()==true) && (i==2 || i==4) && (j==5||j==7))
            {
                board[j][i].set_piece(player[1]);
            }
            // BORDER LETTERS
            else if((!board[j][i].is_playable())&&i==0&&(j!=0&&j!=9))
            {
                board[j][i].set_char(BLACK, letter);
                letter += 1;
            }
            // BORDER NUMBERS
            else if((!board[j][i].is_playable())&&j==0&&(i!=0&&i!=9))
            {
                board[j][i].set_char(BLACK, num);
                num +=1;
            }
        }
    }
}

bool Checkers::is_game_over()const
{
    if(black_pieces==0)
    {
        std::cout << "WHITE WINS!!!" << std::endl;
        return true;
    }
    else if(white_pieces==0)
    {
        std::cout << "BLACK WINS!!!" << std::endl;
        return true;
    }
    return false;
}

/**********************
    MOVE FUNCTIONS
**********************/
std::string Checkers::get_turn()const
{
    if(move_number%2==0)
        return player[0];
    else
        return player[1];
}

Space Checkers::get_space(int x, int y)const
{
    return board[x][y];
}

std::string Checkers::construct_move(int x1, int y1, int x2, int y2)const
{
    char move[]={char(x1)+'a'-char(1),'0' + char(y1), char(x2)+'a'-char(1), '0' + char(y2)};
    std::string new_move="";
    for(int i=0;i<4;++i)
    {
        new_move+=move[i];
    }
    return new_move;
}

void Checkers::construct_mult_jumps(std::vector<std::string> jumps, std::queue<std::string>& moves)const
{
    std::vector<std::string> bin;

    std::string move=jumps[0], player=get_turn(), final_move, tmp;

    int x1,y1,x2,y2;
    x1=y1=x2=y2=0;
    x1 = int(toupper(move[0])-'A')+1;
    y1 = int(move[1]-'1')+1;

    //
    for(int i=0;i<jumps.size();++i)
    {
        Checkers *chk=clone_me();
        final_move="";
        move = jumps[i];
//        std::cout << "move: " << move << std::endl;

        // get current move
        int x1,y1,x2,y2;
        x1=y1=x2=y2=0;
        x1 = int(toupper(move[0])-'A')+1;
        y1 = int(move[1]-'1')+1;
        x2 = int(toupper(move[2])-'A')+1;
        y2 = int(move[3]-'1')+1;

        // $$$$$$$$$$$$$$$$$$$$$$$$$$$
        final_move += move[0];
        final_move += move[1];

        // set "virtual" new spot and test again
        chk->jump(chk->construct_move(x1,y1,x2,y2));
        if(chk->board[x1][y1].is_king())
        {
            chk->board[x2][y2].set_king(player);
        }
        else
        {
            chk->board[x2][y2].set_piece(player);
        }
        chk->board[x1][y1] = Space(true, false, false,RED);
        std::vector<std::string> new_jump=chk->check_open_neighbor(x2,y2);
        if(new_jump.size()!=0)
        {
            for(int a=0;a<new_jump.size();++a)
            {
                // $$$$$$$$$$$$$$$$$$$$$$$$$$$
                tmp = final_move;
                tmp += new_jump[a];
                bin.push_back(tmp);
                                // one more depth
                Checkers *chk=clone_me();

                move = bin.back();
                x1 = int(toupper(move[0])-'A')+1;
                y1 = int(move[1]-'1')+1;
                x2 = int(toupper(move[4])-'A')+1;
                y2 = int(move[5]-'1')+1;

                // depth move
//                std::cout << "Depth: " << move << std::endl;

                // set "virtual" new spot and test again
                chk->mult_jump(move);
                if(chk->board[x1][y1].is_king())
                {
                    chk->board[x2][y2].set_king(player);
                }
                else
                {
                    chk->board[x2][y2].set_piece(player);
                }
                chk->board[x1][y1] = Space(true, false, false,RED);
//                chk->display_status();

                std::vector<std::string> new2_jump=chk->check_open_neighbor(x2,y2);
                if(new2_jump.size()!=0)
                {
                    for(int b=0;b<new2_jump.size();++b)
                    {
                        tmp = move;
//                        std::cout << "new2: " << new2_jump[b] << std::endl;
                        tmp += new2_jump[b][2];
                        tmp += new2_jump[b][3];
                        bin.push_back(tmp);
                    }
                }
            }
        }
    }

    /****************************************
        Combine moves and insert them into
        MOVES
    ****************************************/
    for(int j=0;j<bin.size();++j)
    {
        moves.push(bin[j]);
    }
}

bool Checkers::king_ready(int x, int y)const
{
    Space tmp = board[x][y];
    std::string c_player=get_turn();
    if(!tmp.is_king()&&c_player==player[0]&&y==1)
    {
        return true;
    }
    else if(!tmp.is_king()&&c_player==player[1]&&y==8)
    {
        return true;
    }
    return false;
}

/************************
    Jump functions
************************/
std::vector<std::string> Checkers::check_open_neighbor(int x, int y)const
{
    /***************************************
    Use move_list to pass in potential moves
    ***************************************/
    std::vector<std::string> move_list;
    std::string move;
    // Get the current player
    std::string player = board[x][y].get_piece();

    // Store potential moves
    int x_moves[4] = {x-1, x+1, x-2, x+2};
    int y_moves[4] = {y-1, y+1, y-2, y+2};

    // index values
    int i,j;
    j=0;

    while(j<2)
    {
        for(int i=0;i<2;++i)
        {
//            std::cout << "i: " << i << " j: " << j << std::endl;
            if(board[x_moves[i]][y_moves[j]].is_occupied()&&board[x_moves[i]][y_moves[j]].get_piece()!=player)
            {
//                std::cout << "i: " << i << " j: " << j << std::endl;
                j+=2;
                i+=2;
                if(board[x_moves[i]][y_moves[j]].is_playable()&&!board[x_moves[i]][y_moves[j]].is_occupied())
                {
                    if(board[x][y].is_king())
                    {
                        move=construct_move(x,y,x_moves[i],y_moves[j]);
                        move_list.push_back(move);
                    }
                    else if(board[x][y].get_piece()==WHITE&&!board[x][y].is_king()&&y_moves[j]==y-2)
                    {
                        move=construct_move(x,y,x_moves[i],y_moves[j]);
                        move_list.push_back(move);
                    }
                    else if(board[x][y].get_piece()==BLACK&&!board[x][y].is_king()&&y_moves[j]==y+2)
                    {
                        move=construct_move(x,y,x_moves[i],y_moves[j]);
                        move_list.push_back(move);
                    }
//                    std::cout << "MOVE " << move << std::endl;
                }
                i-=2;
                j-=2;
            }
        }
        ++j;
    }
    return move_list;
}

void Checkers::print_jumps(int x, int y)const
{
    std::vector<std::string> moves=check_open_neighbor(x,y);
    std::cout << "--Available Jumps--" << std::endl;
    for(int i=0;i<moves.size();++i)
    {
        std::cout << moves[i] << std::endl;
    }
}

bool Checkers::is_single_jump(std::string move, int x, int y)const
{
    int x1,y1,x2,y2;
    x1=y1=x2=y2=0;
    x1 = int(toupper(move[0])-'A')+1;
    y1 = int(move[1]-'1')+1;
    x2 = int(toupper(move[2])-'A')+1;
    y2 = int(move[3]-'1')+1;

    std::string c_player = get_turn();

//    if(c_player==WHITE)
//    {
//        std::cout << "WHITE" << std::endl;
//    }
//    else if(c_player==BLACK)
//    {
//        std::cout << "BLACK" << std::endl;
//    }

    if(board[x][y].is_king())
    {
        if(board[x+1][y-1].get_piece()!=get_turn()&&!board[x+2][y-2].is_occupied()&&board[x+2][y-2].is_playable())
        {
            return true;
        }
        else if(board[x-1][y-1].get_piece()!=get_turn()&&!board[x-2][y-2].is_occupied()&&board[x-2][y-2].is_playable())
        {
            return true;
        }
        else if(board[x+1][y+1].get_piece()!=get_turn()&&!board[x+2][y+2].is_occupied()&&board[x+2][y+2].is_playable())
        {
            return true;
        }
        else if(board[x-1][y+1].get_piece()!=get_turn()&&!board[x-2][y+2].is_occupied()&&board[x-2][y+2].is_playable())
        {
            return true;
        }
    }
    // CHECK WHITE
    else if(c_player==WHITE&&(board[x1-1][y1-1].get_piece()!=c_player||board[x1+1][y1-1].get_piece()!=c_player))
    {
        if((x2==x1+2||x2==x1-2)&&y2==y1-2)
        {
            return true;
        }
    }
    // CHECK BLACK
    else if(c_player==BLACK&&(board[x1-1][y1+1].get_piece()!=c_player||board[x1+1][y1+1].get_piece()!=c_player))
    {
        if((x2==x1+2||x2==x1-2)&&y2==y1+2)
        {
            return true;
        }
    }
    return false;
}

bool Checkers::is_jump()const
{
    for(int y=0;y<board_size;++y)
    {
        for(int x=0;x<board_size;++x)
        {
            Space tmp=get_space(x,y);
            if(tmp.is_playable())
            {
                /*************************
                    White can jump
                *************************/
                if((tmp.get_piece()==player[0]&&get_turn()==player[0])||tmp.is_king())
                {
        //            std::cout << x << " " << y <<std::endl;
                    if(board[x+1][y-1].get_piece()==player[1]&&!board[x+2][y-2].is_occupied()&&board[x+2][y-2].is_playable())
                    {
//                        std::cout << "WHITE CAN JUMP RIGHT" << std::endl;
                        return true;
                    }
                    else if(board[x-1][y-1].get_piece()==player[1]&&!board[x-2][y-2].is_occupied()&&board[x-2][y-2].is_playable())
                    {
//                        std::cout << "WHITE CAN JUMP LEFT" << std::endl;
                        return true;
                    }
                }
                /*************************
                    Black can jump
                *************************/
                else if((tmp.get_piece()==player[1]&&get_turn()==player[1])||tmp.is_king())
                {
        //                    std::cout << x << " " << y <<std::endl;
                    if(board[x+1][y+1].get_piece()==player[0]&&!board[x+2][y+2].is_occupied()&&board[x+2][y+2].is_playable())
                    {
//                        std::cout << "BLACK CAN JUMP RIGHT" << std::endl;
                        return true;
                    }
                    else if(board[x-1][y+1].get_piece()==player[0]&&!board[x-2][y+2].is_occupied()&&board[x-2][y+2].is_playable())
                    {
//                        std::cout << "BLACK CAN JUMP LEFT" << std::endl;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Checkers::is_mult_jump(int x, int y)const
{
    // set current player
    std::string player = board[x][y].get_piece();

    // compute if there is more jumps available
    bool space_open = true;

    // get current available moves
    std::vector<std::string> moves=check_open_neighbor(x,y);
//    print_jumps(x,y);

    int jump_count=0,count = 0;

    if(moves.size()==0)
    {
        return false;
    }
    else
    {
        for(int i=0;i<moves.size();++i)
        {
//            std::cout << "new step" << std::endl;
            ++count;
            ++jump_count;
            // Get cloned boards
            Checkers *chk = clone_me();

//            std::cout << "Primary CLONE" << std::endl;
//            chk->display_status();

            space_open = true;
            // get current move
            int x1,y1,x2,y2;
            x1=y1=x2=y2=0;
            x1 = int(toupper((moves[i])[0])-'A')+1;
            y1 = int((moves[i])[1]-'1')+1;
            x2 = int(toupper((moves[i])[2])-'A')+1;
            y2 = int((moves[i])[3]-'1')+1;

            // set "virtual" new spot and test again
            chk->jump(chk->construct_move(x1,y1,x2,y2));
            chk->board[x1][y1] = Space(true, false, false,RED);
            if(board[x1][y1].is_king())
            {
                chk->board[x2][y2].set_king(player);
            }
            else
            {
                chk->board[x2][y2].set_piece(player);
            }
            std::vector<std::string> new_moves=chk->check_open_neighbor(x2,y2);
//            std::cout << "newmoves: " << new_moves.size() << std::endl;
//            chk->print_jumps(x2,y2);
//            std::cout << "Primary CLONE" << std::endl;
//            chk->display_status();

            while(space_open==true)
            {
                if(new_moves.size()==0)
                {
                    space_open=false;
                }
                else
                {
//                    ++jump_count;
                    Checkers *new_chk=chk->clone_me();
                    for(int j=0;j<new_moves.size();++j)
                    {
                        int a1,b1,a2,b2;
                        a1=b1=a2=b2=0;
                        a1 = int(toupper((new_moves[j])[0])-'A')+1;
                        b1 = int((new_moves[j])[1]-'1')+1;
                        a2 = int(toupper((new_moves[j])[2])-'A')+1;
                        b2 = int((new_moves[j])[3]-'1')+1;

//                        std::cout << "++count" << std::endl;
                        ++count;

                        new_chk->jump(new_chk->construct_move(a1,b1,a2,b2));
                        new_chk->board[a1][b1] = Space(true, false, false,RED);
                        if(chk->board[a1][b1].is_king())
                        {
                            new_chk->board[a2][b2].set_king(player);
                        }
                        else
                        {
                            new_chk->board[a2][b2].set_piece(player);
                        }
//                        std::cout << "Stem CLONE" << std::endl;
//                        new_chk->display_status();
                        new_moves=new_chk->check_open_neighbor(a2,b2);
//                        std::cout << "newmoves: " << new_moves.size() << std::endl;
//                        new_chk->print_jumps(a2,b2);
                    }
                    chk = new_chk;
                }
            }
        }
//        std::cout << "Jump count: " << jump_count << std::endl;
//        std::cout << "Count: " << count << std::endl;
        if(count<=jump_count)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

void Checkers::jump(const std::string& move)
{
    int x1,y1,x2,y2;
    x1=y1=x2=y2=0;
    x1 = int(toupper(move[0])-'A')+1;
    y1 = int(move[1]-'1')+1;
    x2 = int(toupper(move[2])-'A')+1;
    y2 = int(move[3]-'1')+1;

    if(y2==y1-2)
    {
        if(x2==x1+2)
        {
            remove_piece(x1+1, y1-1);
        }
        else if(x2==x1-2)
        {
            remove_piece(x1-1, y1-1);
        }
    }
    else if(y2==y1+2)
    {
        if(x2==x1+2)
        {
            remove_piece(x1+1, y1+1);
        }
        else if(x2==x1-2)
        {
            remove_piece(x1-1, y1+1);
        }
    }
    else
    {
        return;
    }
}

void Checkers::mult_jump(const std::string& move)
{
    // Break down move attributes
    int moves = move.size()/2;
    std::string new_move;

    int x1,y1,x2,y2;
    x1=y1=x2=y2=0;

    for(int i=0;i<moves;++i)
    {
        x1 = int(toupper(move[i])-'A')+1;
        y1 = int(move[i+1]-'1')+1;
        x2 = int(toupper(move[i+2])-'A')+1;
        y2 = int(move[i+3]-'1')+1;
//        std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
        new_move = construct_move(x1,y1,x2,y2);
        jump(new_move);
        ++i;
    }
}

void Checkers::remove_piece(int x, int y)
{
    board[x][y] = Space(true, false, false, B_RED);
    if(get_turn()==player[0])
        black_pieces-=1;
    else
        white_pieces-=1;
//        std::cout << "White pieces: " << white_pieces << std::endl;
//        std::cout << "Black pieces: " << black_pieces << std::endl;
}
void Checkers::make_move(const std::string& move)
{
    // Get whose turn it is
    std::string player_color;
    if(move_number%2==0)
    {
        player_color = player[0];
    }
    else if(move_number%2==1)
    {
        player_color = player[1];
    }

    // get move info
    int x1,y1,x2,y2;
    x1=y1=x2=y2=0;
    x1 = int(toupper(move[0])-'A')+1;
    y1 = int(move[1]-'1')+1;
    x2 = int(toupper(move[2])-'A')+1;
    y2 = int(move[3]-'1')+1;

    // make jump (aka remove captured piece
    if(is_mult_jump(x1,y1)==true)
    {
        mult_jump(move);
//        std::cout << "King: " << board[x1][y1].is_king()<<std::endl;
        int dif_size = move.size();
        x2 = int(toupper(move[dif_size-2])-'A')+1;
        y2 = int(move[dif_size-1]-'1')+1;
    }
    else if(is_jump())
    {
        jump(move);
//        std::cout << "King: " << board[x1][y1].is_king()<<std::endl;
    }

    // get old color
    std::string color;
    color = board[x1][y1].get_color();

    // set old piece and new piece
    if(board[x1][y1].is_king())
    {
        board[x2][y2].set_king(player_color);
    }
    else if(king_ready(x2,y2))
    {
        board[x2][y2].set_king(player_color);
//        std::cout << "is king: " << board[x2][y2].is_king() << std::endl;
    }
    else
    {
        board[x2][y2].set_piece(player_color);
    }
    board[x1][y1] = Space(true, false, false, color);

    ++move_number;
//    std::cout << "Move number: " << move_number << std::endl;
}

bool Checkers::is_legal(const std::string& move) const
{
    std::cout << std::endl;

    // Break down move attributes
    int x, y, x1,y1,x2,y2;
    x1=y1=x2=y2=0;
    x1 = x = int(toupper(move[0])-'A')+1;
    y1 = y = int(move[1]-'1')+1;
    x2 = int(toupper(move[2])-'A')+1;
    y2 = int(move[3]-'1')+1;

    //check multijump
    if(is_mult_jump(x1,y1)&&board[x1][y1].get_piece()==get_turn())
    {
        int moves=move.size()/2, count=0;
        if(moves%2==1)
        {
            moves -= 1;
        }
        x1=y1=x2=y2=0;

        std::string new_move;

        for(int i=0;i<=moves;++i)
        {
            std::cout << "MOVE: " << move << std::endl;
            x1 = int(toupper(move[i])-'A')+1;
            y1 = int(move[i+1]-'1')+1;
            x2 = int(toupper(move[i+2])-'A')+1;
            y2 = int(move[i+3]-'1')+1;
            new_move = construct_move(x1,y1,x2,y2);
            std::cout << "move: " << new_move << std::endl;
            if(is_single_jump(new_move, x, y))
            {
                ++count;
            }
            ++i;
        }
        if(count==moves||count==moves-1)
        {
            return true;
        }
    }

    // if turn is EVEN its white's turn
    if(board[x1][y1].is_king())
    {
        // if new spot is playable and not occupied
        if(board[x2][y2].is_playable()&&!board[x2][y2].is_occupied())
        {
            if(is_single_jump(move,x1,y1)&&board[x2][y2].is_playable())
            {
                if(x2==x1+2&&y2==y1-2)
                {
                    return true;
                }
                else if(x2==x1-2&&y2==y1-2)
                {
                    return true;
                }
                else if(x2==x1+2&&y2==y1+2)
                {
                    return true;
                }
                else if(x2==x1-2&&y2==y1+2)
                {
                    return true;
                }
                else
                    return false;
            }
            // move right
            else if(x2==x1+1&&y2==y1-1)
                return true;
            // move left
            else if(x2==x1-1&&y2==y1-1)
                return true;
            // move right
            if(x2==x1+1&&y2==y1+1)
                return true;
            // move left
            else if(x2==x1-1&&y2==y1+1)
                return true;
        }
    }

    // if turn is EVEN its white's turn
    else if((move_number%2==0 && board[x1][y1].get_piece()==player[0]))
    {
        // if new spot is playable and not occupied
        if(board[x2][y2].is_playable()&&!board[x2][y2].is_occupied())
        {
            if(is_jump()&&board[x2][y2].is_playable())
            {
                if(x2==x1+2&&y2==y1-2)
                {
                    return true;
                }
                else if(x2==x1-2&&y2==y1-2)
                {
                    return true;
                }
                else
                    return false;
            }
            // move right
            else if(x2==x1+1&&y2==y1-1)
                return true;
            // move left
            else if(x2==x1-1&&y2==y1-1)
                return true;
        }
    }
    // if turn is ODD its blacks turn
    else if((move_number%2==1 && board[x1][y1].get_piece()==player[1]))
    {
        if(board[x2][y2].is_playable()&&!board[x2][y2].is_occupied())
        {
            // check jumps
            if(is_jump()&&board[x2][y2].is_playable())
            {
                if(x2==x1+2&&y2==y1+2)
                {
                    return true;
                }
                else if(x2==x1-2&&y2==y1+2)
                {
                    return true;
                }
                else
                    return false;
            }
            // move right
            if(x2==x1+1&&y2==y1+1)
                return true;
            // move left
            else if(x2==x1-1&&y2==y1+1)
                return true;
        }
    }
    return false;
}

/**********************
       DISPLAY
**********************/
std::string Checkers::get_user_move()const
{
    std::string answer;
    /***********************
        Get User Move
    ***********************/
    if(move_number%2==0)
    {
        std::cout << "White's move" << std::endl;
    }
    else
    {
        std::cout << "Black's move" << std::endl;
    }

    std::cout << "Move format: original spot --> new spot" << std::endl;
    std::cout << "ex/ a1b2... a1 = original and b2 = new spot" << std::endl;
    std::cout << "NOTE: For Multi-jump enter open spaces in path" << std::endl;
    bool entering = true;
    while(entering)
    {
        // check format of the move
        std::cout << "Enter your move (ex/ a1b2): ";
        std::getline(cin, answer);

        // NEED TO CHECK IF NUMBER AND LETTER ARE IN RANGE
        if(answer[0]>'h' || answer[2]>'h')
        {
            std::cout << "WARNING: not correct format" << std::endl;
        }
        else if(!isalpha(answer[0])||!isalpha(answer[2]))
        {
            std::cout << "WARNING: not correct format" << std::endl;
        }

        else if(!isdigit(answer[1])||!isdigit(answer[3]))
        {
            std::cout << "WARNING: not correct format" << std::endl;
        }

        else
        {
            // Break down move attributes
            int x1,y1;
            x1=y1=0;
            x1 = int(toupper(answer[0])-'A')+1;
            y1 = int(answer[1]-'1')+1;

            if(is_mult_jump(x1,y1))
            {
                std::cout << "MULTI-JUMP AVAILABLE" << std::endl;
                for(int i=0;i<answer.size();++i)
                {
                    if(i%2==0)
                    {
                        if(!isalpha(answer[i]))
                        {
                            std::cout << "WARNING: not correct format" << std::endl;
                        }
                    }
                    else if(i%2==1)
                    {
                        if(!isdigit(answer[i]))
                        {
                            std::cout << "WARNING: not correct format" << std::endl;
                        }
                    }
                }
                if(answer.size()%2==1||answer.size()<=4)
                {
                    std::cout << "WARNING: not correct format" << std::endl;
                }
                else
                {
                    entering = false;
                }
            }
            else if(answer.size()>4)
            {
                std::cout << "WARNING: not correct format" << std::endl;
            }
            else
                entering = false;
        }
    }

    return answer;
}

void Checkers::display_status()const
{
    for(int i=0;i<board_size;++i)
    {
        for(int x=0;x<3;++x)
        {
            for(int j=0;j<board_size;++j)
            {
                board[j][i].print_line(x);
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}
