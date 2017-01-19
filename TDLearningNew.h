#ifndef __TDLEARNINGNEW_H
#define __TDLEARNINGNEW_H
#include "MyGameBoard.h"
#include "Fib2584/Typedefs.h"
#include "FeatureBoard.h"
#include <string>
#include <stack>
#include <vector>

typedef struct State {
    int best_move_dir;
    int merge_score; // reward
    bool game_over_state;
    FeatureBoard next_feature_board;
} State;

class TDLearningNew {
    public:
        TDLearningNew(bool want_train = false);
        //TDLearningNew(bool want_train = false, std::string name = "weight.dat");
        ~TDLearningNew();
        void write_to_file();
        void init();
        MoveDirection generateMove(int board[4][4]);
        void updateWeight();
        //void addRecord(FeatureBoard &fb);
        void addRecord(State &s);
        void setTraining(bool is_train);
        bool isTraining();
        double evaluate(MyGameBoard board);
        int generateEvilMove(int board[4][4], unsigned long long move_count);
    private:
        std::string file_name;
        bool training;
        double *outer_row;
        double *inner_row;
        double *square_tuple1;
        double *square_tuple2;
        double *empty_tile_feature;
        double *axis_tuple1;
        double *axis_tuple2;
        double *axis_tuple3;
        //std::stack<FeatureBoard> record;
        std::vector<State> record;
        double learning_rate;
};


#endif
