#include "TDLearningNew.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <fstream>
#include <iostream>
#include <math.h>

using namespace std;

#define ROW_SIZE  1048576
#define SQUARE_TUPLE_SIZE 113379904
//const double discount_for_reward[] = {1, 0.95, pow(0.95, 2), pow(0.95, 3), pow(0.95, 4), pow(0.95, 5)};

TDLearningNew::TDLearningNew(bool want_train)
{
    learning_rate = 0.000025;
    //training = false;
    training = want_train;
    //file_name = name;
    //file_name = std::string("weight_lamda_old.dat");
    file_name = std::string("weight_lamda.dat");
    inner_row = new double[ROW_SIZE];
    if (!inner_row) {
        printf("No space for inner_row\n");
        exit(1);
    }
    outer_row = new double[ROW_SIZE];
    if (!outer_row) {
        printf("No space for outer_row\n");
        exit(1);
    }
    square_tuple1 = new double[SQUARE_TUPLE_SIZE];
    if (!square_tuple1) {
        printf("No space for square_tuple1\n");
        exit(1);
    }
    square_tuple2 = new double[SQUARE_TUPLE_SIZE];
    if (!square_tuple2) {
        printf("No space for square_tuple2\n");
        exit(1);
    }
    axis_tuple1 = new double[SQUARE_TUPLE_SIZE];
    if (!axis_tuple1) {
        printf("No space for axis_tuple1\n");
        exit(1);
    }
    axis_tuple2 = new double[SQUARE_TUPLE_SIZE];
    if (!axis_tuple2) {
        printf("No space for axis_tuple2\n");
        exit(1);
    }
    axis_tuple3 = new double[SQUARE_TUPLE_SIZE];
    if (!axis_tuple3) {
        printf("No space for axis_tuple3\n");
        exit(1);
    }
    empty_tile_feature = new double[16];
    if (!empty_tile_feature) {
        printf("No space for empty\n");
        exit(1);
    }
    init();
}

void TDLearningNew::write_to_file()
{
        ofstream fout(file_name.c_str(), ifstream::out | ifstream::binary);
        fout.write((char *)inner_row, ROW_SIZE * sizeof(double));
        fout.write((char *)outer_row, sizeof(double) * ROW_SIZE);
        fout.write((char *)square_tuple1, sizeof(double) * SQUARE_TUPLE_SIZE);
        fout.write((char *)square_tuple2, sizeof(double) * SQUARE_TUPLE_SIZE);
        fout.write((char *)axis_tuple1, sizeof(double) * SQUARE_TUPLE_SIZE);
        fout.write((char *)axis_tuple2, sizeof(double) * SQUARE_TUPLE_SIZE);
        fout.write((char *)axis_tuple3, sizeof(double) * SQUARE_TUPLE_SIZE); 
        fout.write((char *)empty_tile_feature, sizeof(double) * 16);
        fout.close();

        delete [] inner_row;
        delete [] outer_row;
        delete [] square_tuple1;
        delete [] square_tuple2;
        delete [] empty_tile_feature;
        delete [] axis_tuple1;
        delete [] axis_tuple2;
        delete [] axis_tuple3;
}

TDLearningNew::~TDLearningNew()
{
    if (training)
        printf("I am training\n");
    if (training) {
        ofstream fout(file_name.c_str(), ifstream::out | ifstream::binary);
        fout.write((char *)inner_row, ROW_SIZE * sizeof(double));
        fout.write((char *)outer_row, sizeof(double) * ROW_SIZE);
        fout.write((char *)square_tuple1, sizeof(double) * SQUARE_TUPLE_SIZE);
        fout.write((char *)square_tuple2, sizeof(double) * SQUARE_TUPLE_SIZE);
        fout.write((char *)axis_tuple1, sizeof(double) * SQUARE_TUPLE_SIZE);
        fout.write((char *)axis_tuple2, sizeof(double) * SQUARE_TUPLE_SIZE);
        fout.write((char *)axis_tuple3, sizeof(double) * SQUARE_TUPLE_SIZE); 
        fout.write((char *)empty_tile_feature, sizeof(double) * 16);
        fout.close();
    }
    delete [] inner_row;
    delete [] outer_row;
    delete [] square_tuple1;
    delete [] square_tuple2;
    delete [] empty_tile_feature;
    delete [] axis_tuple1;
    delete [] axis_tuple2;
    delete [] axis_tuple3;
}

void TDLearningNew::init()
{

    ifstream fin(file_name.c_str(), ios::in | ios::binary);
    if (!fin) {
        memset(inner_row, 0, sizeof(double) * ROW_SIZE);
        memset(outer_row, 0, sizeof(double) * ROW_SIZE);
        memset(square_tuple1, 0, sizeof(double) * SQUARE_TUPLE_SIZE);
        memset(square_tuple2, 0, sizeof(double) * SQUARE_TUPLE_SIZE);
        memset(axis_tuple1, 0, sizeof(double) * SQUARE_TUPLE_SIZE);
        memset(axis_tuple2, 0, sizeof(double) * SQUARE_TUPLE_SIZE);
        memset(axis_tuple3, 0, sizeof(double) * SQUARE_TUPLE_SIZE);
        memset(empty_tile_feature, 0, sizeof(double) * 16);
    } else {
        fin.read((char *)inner_row, sizeof(double) * ROW_SIZE);
        fin.read((char *)outer_row, sizeof(double) * ROW_SIZE);
        fin.read((char *)square_tuple1, sizeof(double) * SQUARE_TUPLE_SIZE);
        fin.read((char *)square_tuple2, sizeof(double) * SQUARE_TUPLE_SIZE);
	cout << "finish square_tuple\n" << endl;
        fin.read((char *)axis_tuple1, sizeof(double) * SQUARE_TUPLE_SIZE);
        fin.read((char *)axis_tuple2, sizeof(double) * SQUARE_TUPLE_SIZE);
        fin.read((char *)axis_tuple3, sizeof(double) * SQUARE_TUPLE_SIZE);
	cout << "finish axes_tuple\n" << endl;
        fin.read((char *)empty_tile_feature, sizeof(double) * 16);
        fin.close();
    }
}

MoveDirection TDLearningNew::generateMove(int board[4][4])
{
    MyGameBoard start_board(board);
    /*start_board.showBoard();
    printf("\n");*/
    MoveDirection best_dir;
    double best_val = -DBL_MAX;
    FeatureBoard best_feature;
    State s;

    for (int dir = 0; dir < 4; ++dir) {
        MyGameBoard new_board(start_board);
        int reward = new_board.move((MoveDirection) dir);
        if (!(new_board == start_board)) {
            FeatureBoard new_feature(new_board, reward);
            double val = new_feature.get_feature_val(inner_row, outer_row,
                    square_tuple1, square_tuple2, axis_tuple1, axis_tuple2,
                    axis_tuple3, empty_tile_feature) + reward;
            /*double val = new_feature.get_feature_val(inner_row, outer_row,
              square_tuple1, square_tuple2, empty_tile_feature) + reward;*/
            if (val > best_val) {
                best_dir = (MoveDirection) dir;
                best_val = val;
                //best_feature = new_feature;
                // State
                s.best_move_dir = dir;
                s.merge_score = reward;
                s.next_feature_board = new_feature;
                s.game_over_state = false;
            }
        }
    }

    if (training) {
        //record.push(best_feature);
        record.push_back(s);
    }
    return best_dir;
}



void TDLearningNew::updateWeight()
{
    double lambda = 0.5;
    int record_size = record.size();
    for (int i = record_size - 6; i > -1; --i) {
        double delta = 0;
        int size = 5; // update using 5 state
        double score = 0;
        int total_reward = 0;
        for (int j = 0; j < size; ++j) {
            double weight = 0;
            if (!record[i+j+1].game_over_state) {
                if (j != size - 1) {
                    weight = (1 - lambda) * pow(lambda, j);
                } else {
                    weight = pow(lambda, j);
                }
                total_reward = record[i+j+1].merge_score + total_reward;
                score +=  weight * (total_reward + 
                         record[i+j+1].next_feature_board.get_feature_val(inner_row,
                            outer_row, square_tuple1, square_tuple2, axis_tuple1, 
                            axis_tuple2, axis_tuple3, empty_tile_feature));
            } else {
                ;
            }
        }
        delta = learning_rate * (score - 
                record[i].next_feature_board.get_feature_val(inner_row, outer_row, square_tuple1,
                    square_tuple2, axis_tuple1, axis_tuple2, axis_tuple3, empty_tile_feature));
        record[i].next_feature_board.updateWeight(inner_row, outer_row, square_tuple1,
                square_tuple2, axis_tuple1, axis_tuple2, axis_tuple3, 
                empty_tile_feature, delta);
    }
    record.clear();
}


/*
void TDLearningNew::updateWeight()
{
    bool is_last = true;
    double recent_score = 0;
    double delta = 0;
    while (record.size()) {
        FeatureBoard fea = record.top();
        record.pop();
        if (is_last) {
            recent_score = 0.0;
            is_last = false;
        } else {
            delta = recent_score - fea.get_feature_val(inner_row, outer_row,
                    square_tuple1, square_tuple2, axis_tuple1, axis_tuple2,
                    axis_tuple3, empty_tile_feature);
            delta *= learning_rate;
            fea.updateWeight(inner_row, outer_row, square_tuple1,
                    square_tuple2, axis_tuple1, axis_tuple2,
                    axis_tuple3,  empty_tile_feature, delta);
            recent_score = fea.get_feature_val(inner_row, outer_row,
                    square_tuple1, square_tuple2, axis_tuple1, axis_tuple2,
                    axis_tuple3, empty_tile_feature) + fea.getMergeScore();
        }
    }
}
*/

/*
void TDLearningNew::addRecord(FeatureBoard &fb)
{
    record.push(fb);
}
*/

void TDLearningNew::addRecord(State &s)
{
    record.push_back(s);
}


void TDLearningNew::setTraining(bool is_train)
{
    training = is_train;
}

bool TDLearningNew::isTraining()
{
    return training;
}

double TDLearningNew::evaluate(MyGameBoard board)
{
    FeatureBoard fb(board, 0);
    return fb.get_feature_val(inner_row, outer_row, square_tuple1,
            square_tuple2, axis_tuple1, axis_tuple2, axis_tuple3,
            empty_tile_feature);
}
