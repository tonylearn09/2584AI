#include "FeatureBoard.h"

FeatureBoard::FeatureBoard()
    :merge_score(0)
{
}

FeatureBoard::FeatureBoard(MyGameBoard board, int reward)
    : merge_score(reward), my_board(board)
{
    my_board.getSameBoard(same_boards);
}

FeatureBoard::FeatureBoard(const FeatureBoard &feature)
    :merge_score(feature.merge_score), my_board(feature.my_board)
{
    for (int i = 0; i < 8; ++i) 
        same_boards[i] = feature.same_boards[i];
}

double FeatureBoard::get_feature_val(double *inner_row, double *outer_row,
        double *square_tuple1, double *square_tuple2, double *axes_tuple1,
        double *axes_tuple2, double *axes_tuple3, double *empty_tile_feature)
{
    double total = 0.0;
    for (int i = 0; i < 8; ++i) {
        total += inner_row[inner_row_ind(same_boards[i])];
        total += outer_row[outer_row_ind(same_boards[i])];
        total += square_tuple1[square_tuple1_ind(same_boards[i])];
        total += square_tuple2[square_tuple2_ind(same_boards[i])];
        total += axes_tuple1[axes_tuple1_ind(same_boards[i])];
        total += axes_tuple2[axes_tuple2_ind(same_boards[i])];
        total += axes_tuple3[axes_tuple3_ind(same_boards[i])];
    }
    total += empty_tile_feature[empty_val_ind(my_board)];
    return total;
}

void FeatureBoard::updateWeight(double *inner_row, double *outer_row, double
        *square_tuple1, double *square_tuple2, double *axes_tuple1,
        double *axes_tuple2, double *axes_tuple3, double *empty_tile_feature,
        double delta)
{

    for (int i = 0; i < 8; ++i) {
        inner_row[inner_row_ind(same_boards[i])] += delta;
        outer_row[outer_row_ind(same_boards[i])] += delta;
        square_tuple1[square_tuple1_ind(same_boards[i])] += delta;
        square_tuple2[square_tuple2_ind(same_boards[i])] += delta;
        axes_tuple1[axes_tuple1_ind(same_boards[i])] += delta;
        axes_tuple2[axes_tuple2_ind(same_boards[i])] += delta;
        axes_tuple3[axes_tuple3_ind(same_boards[i])] += delta;
    }
    empty_tile_feature[empty_val_ind(my_board)] += delta;
}

unsigned long FeatureBoard::outer_row_ind(MyGameBoard my_board)
{
    //unsigned long long ind = (my_board.get_right()) & 0xfffff;
    //return ind;
    //return my_board.getRow(0).get_right();
    return my_board.getLine(3);
}

unsigned long FeatureBoard::inner_row_ind(MyGameBoard my_board)
{
    //unsigned long long ind = (((my_board.get_right()) & 0xfffff00000) >> 20);
    //return ind;
    //return my_board.getRow(1).get_right();
    return my_board.getLine(2);
}

unsigned long FeatureBoard::square_tuple1_ind(MyGameBoard my_board)
{
    /*unsigned long long ind = ((((my_board.get_right()) & 0x07fff00000) >> 5) |
        ((my_board.get_right()) & 0x07fff));
    return ind;*/
    /*return ((((my_board.getRow(1).get_right()) & 0x7fff) << 15) | 
        ((my_board.getRow(0).get_right()) & 0x7fff));*/
    /*return my_board.getFeatureIndex((((my_board.getRow(1).get_right()) & 0x7fff) << 15) | 
        ((my_board.getRow(0).get_right()) & 0x7fff));*/
    /*return my_board.getFeatureIndex((((my_board.getRow(1).get_right()) & 0x7fff) << 15) | 
        ((my_board.getRow(0).get_right()) & 0x7fff));*/
    return my_board.getFeatureIndex(((my_board.getLine(2) & 0x7fff) << 15) | 
        (my_board.getLine(3) & 0x7fff));
}

unsigned long FeatureBoard::square_tuple2_ind(MyGameBoard my_board)
{
    /*return my_board.getFeatureIndex((((my_board.getRow(2).get_right()) & 0x7fff) << 15) | 
        ((my_board.getRow(1).get_right()) & 0x7fff));*/
    return my_board.getFeatureIndex(((my_board.getLine(3) & 0x7fff) << 15) | 
        (my_board.getLine(2) & 0x7fff));
}

unsigned long FeatureBoard::axes_tuple1_ind(MyGameBoard my_board)
{
    /*return my_board.getFeatureIndex((((my_board.getRow(1).get_right()) & 0x3ff) << 20) | 
        (my_board.getRow(0).get_right()));*/

    return my_board.getFeatureIndex(((my_board.getLine(2) & 0x3ff) << 20) | 
        (my_board.getLine(3)));
}

unsigned long FeatureBoard::axes_tuple2_ind(MyGameBoard my_board)
{
    /*return my_board.getFeatureIndex((((my_board.getRow(2).get_right()) & 0x3ff) << 20) | 
        (my_board.getRow(1).get_right()));*/
    return my_board.getFeatureIndex(((my_board.getLine(1) & 0x3ff) << 20) | 
        (my_board.getLine(2)));
}

unsigned long FeatureBoard::axes_tuple3_ind(MyGameBoard my_board)
{
    /*return my_board.getFeatureIndex((((my_board.getRow(3).get_right()) & 0x3ff) << 20) | 
        (my_board.getRow(2).get_right()));*/
    return my_board.getFeatureIndex(((my_board.getLine(0) & 0x3ff) << 20) | 
        (my_board.getLine(1)));
}

int FeatureBoard::empty_val_ind(MyGameBoard my_board)
{
    return my_board.countEmptyTile();
}

int FeatureBoard::getMergeScore()
{
    return merge_score;
}


