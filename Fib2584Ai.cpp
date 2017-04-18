#include "Fib2584Ai.h"
#include <iostream>
#include <string>

using namespace std;

Fib2584Ai::Fib2584Ai()
    //: move_count_evil(0), td(false, "weight_new.dat"), td_evil(false, "weight_new.dat"), ab(td_evil)
    : move_count_evil(0), td(false), ab(td)
{
}

void Fib2584Ai::initialize(int argc, char* argv[])
{
	srand(time(NULL));
	if (argc > 2 && strcmp(argv[2], "-t") == 0)
        td.setTraining(true);
}


MoveDirection Fib2584Ai::generateMove(int board[4][4])
{
	MoveDirection dir = td.generateMove(board);
    return dir;
    //return ab.generateMove(board, move_count_evil);
}

int Fib2584Ai::generateEvilMove(int board[4][4])
{
    //return rand() % 16;
    if (move_count_evil == 0) {
        ++move_count_evil;
        //return rand() % 16;
        return 9;
    } else {
        return ab.generateEvilMove(board, ++move_count_evil);
    }
}

void Fib2584Ai::gameOver(int board[4][4], int iScore)
{
	if (td.isTraining()) {
        // this board is not a real board, just for gameover's 
        // after state, use current board to represent
        int max_score = 60010;
        if (iScore < max_score) {
            for (int i = 0; i < 5; ++i) {
                State s;
                s.game_over_state = true;
                td.addRecord(s);
            }
            //FeatureBoard fea_board(MyGameBoard(board), 0);
            //td.addRecord(fea_board);
            td.updateWeight();
        } else {
            td.write_to_file();
            exit(0);
        }
    }
    move_count_evil = 0;
	return;
}

/**********************************
You can implement any additional functions
you may need.
**********************************/
