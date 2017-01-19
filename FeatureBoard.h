#ifndef __FEATUREBOARD_H
#define __FEATUREBOARD_H

#include "MyGameBoard.h"

class FeatureBoard {
    public:
        FeatureBoard();
        FeatureBoard(MyGameBoard board, int reward);
        FeatureBoard(const FeatureBoard &feature);

        double get_feature_val(double *inner_row, double *outer_row,
                double *square_tuple1, double *square_tuple2,
                double *axes_tuple1, double *axes_tuple2,
                double *axes_tuple3, double *empty_tile_feature);
        /*double get_feature_val(double *inner_row, double *outer_row,
                double *square_tuple1, double *square_tuple2,
                double *empty_tile_feature);*/
        void updateWeight(double *inner_row, double *outer_row,
                double *square_tuple1, double *square_tuple2,
                double *axes_tuple1, double *axes_tuple2, double *axes_tuple3, 
                double *empty_tile_feature, double delta);
        /*void updateWeight(double *inner_row, double *outer_row,
                double *square_tuple1, double *square_tuple2,
                double *empty_tile_feature, double delta);*/
        int getMergeScore();
    private:
        int merge_score;
        MyGameBoard my_board;
        MyGameBoard same_boards[8];
        int empty_val_ind(MyGameBoard my_board);
        unsigned long square_tuple2_ind(MyGameBoard my_board);
        unsigned long square_tuple1_ind(MyGameBoard my_board);
        unsigned long inner_row_ind(MyGameBoard my_board);
        unsigned long outer_row_ind(MyGameBoard my_board);
        unsigned long axes_tuple1_ind(MyGameBoard my_board);
        unsigned long axes_tuple2_ind(MyGameBoard my_board);
        unsigned long axes_tuple3_ind(MyGameBoard my_board);
};


#endif
