#ifndef __ALPHABETA_H
#define __ALPHABETA_H

#include "MyGameBoard.h"
#include "TDLearningNew.h"
//#include "zhash.h"

#define TRANS_TABLE_MAX_SIZE 234256 // 22^4, just a number not too big

struct TableEle {
    short evil_location;
    double value;
};

class AlphaBeta {
    public:
        AlphaBeta(TDLearningNew &td);
        ~AlphaBeta();
        int generateEvilMove(int board[4][4], unsigned long long move_count);
        MoveDirection generateMove(int board[4][4], unsigned long long move_count);
    private:
        int max_time;
        TDLearningNew &td;
        double min_node(MyGameBoard &board, int &evil_pos, double alpha,
                double beta, int depth, unsigned long long evil_move_count); 
        double max_node(MyGameBoard &board, MoveDirection &dir, 
                double alpha, double beta, int depth, unsigned long long move_count);
        //zHashTable trans_table_move;
        //zHashTable trans_table_evil;

};


#endif
