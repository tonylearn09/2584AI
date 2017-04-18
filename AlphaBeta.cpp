#include "AlphaBeta.h"
#include "MyGameBoard.h"
#include <stdlib.h>
#include <sys/time.h>
#include <float.h>

#define TT_SIZE 1048576

AlphaBeta::AlphaBeta(TDLearningNew &td)
    : td(td)
{
}

AlphaBeta::~AlphaBeta()
{
}

MoveDirection AlphaBeta::generateMove(int board[4][4], unsigned long long move_count)
{
    MyGameBoard next_board(board);
    MoveDirection dir = (MoveDirection)-1;

    int depth = 1; // At first, depth = 0
    max_node(next_board, dir, -DBL_MAX, DBL_MAX, depth, move_count);
    return dir;
}

int random_place(int board[4][4])
{
    MyGameBoard new_board(board);
	int emptyTileNum = new_board.countEmptyTile();
	int randomTileLocation = rand() % emptyTileNum;
	int count = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board[i][j] == 0) {
                if (count == randomTileLocation)
                    return i * 4 + j;
            } else {
                continue;
            }
            count++;
        }
    }
    return -1;
}

int AlphaBeta::generateEvilMove(int board[4][4], unsigned long long move_count)
{
    MyGameBoard added_board(board);
    int try_evil_pos = 0;

    int depth = 0; // first depth = 0


    //if (td.isTraining()) {
        //if (rand() % 1024 == 0)
            //return random_place(board);
    //}
    //return random_place(board);

    min_node(added_board, try_evil_pos, -DBL_MAX, DBL_MAX, depth, move_count);
    return try_evil_pos;
}

double AlphaBeta::min_node(MyGameBoard &board, int &evil_pos, double alpha, 
        double beta, int depth, unsigned long long evil_move_count)
{
    /*if (trans_table_evil.exist_dir(board, depth)) {
        evil_pos =
            trans_table_evil.getBestMove(board, depth);
        return trans_table_evil.retrieve(board, depth);
    }*/

    //double m = beta;
    double m = DBL_MAX;
    int worst_pos = -1;
    int temp = -1; // To memorize empty place
    double worst_score = DBL_MAX;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board.getTile(3-i, 3-j) == 0) {
                temp = i*4 + j;
                MyGameBoard next_board(board);
                next_board.addTile(temp, evil_move_count);

                MoveDirection dir = (MoveDirection)0; // Dummy one
                /*double score = max_node(next_board, dir, alpha, m,
                        depth, evil_move_count);*/
                /*double score = max_node(next_board, dir, min(beta,m) - 1, min(beta,m),
                        depth, evil_move_count);*/
                double score = max_node(next_board, dir, alpha,
                        (m < beta ? m : beta), depth, evil_move_count);
                if (score < worst_score) {
                    worst_score = score;
                    worst_pos = temp;
                }
                if (score < m) {
                    m = score; // update beta
                } 
                if (m <= alpha) {
                    evil_pos = worst_pos;
                    //trans_table_evil.insert(board, m, depth, evil_pos);
                    return m; // cutoff
                }

            }
        }
    }
    if (worst_pos == -1) {
        // game over
        //if (temp != -1)
        evil_pos = temp;
        //trans_table_evil.insert(board, -DBL_MAX, depth, evil_pos);
        return -DBL_MAX; // For maxNode to know
    } else {
        evil_pos = worst_pos;
        //trans_table_evil.insert(board, m, depth, evil_pos);
        return m;
    }
}

double AlphaBeta::max_node(MyGameBoard &board, MoveDirection &dir, double alpha, double beta, 
        int depth, unsigned long long move_count)
{
    if (depth == 0) {
        //double m = alpha;
        int best_dir = -1;
        double best_score = -DBL_MAX;
        for (int i = 0; i < 4; ++i) {
            MyGameBoard next_board(board);
            int reward = next_board.move((MoveDirection) i);
            if (!(next_board == board)) {
                double score = reward + td.evaluate(next_board);
                if (score > best_score) {
                    best_score = score;
                    best_dir = i;
                }
            }
        }
        dir = (MoveDirection) best_dir;
        if (dir == -1) {
            // game over
            return DBL_MAX; // For minNode to know
        }
        return best_score;
    } else {
        int temp_dir;
        //double m = alpha;
        /*if (trans_table_move.exist(board, depth)) {
            dir = (MoveDirection) (trans_table_move.getBestMove(board, depth));
            return trans_table_move.retrieve(board,depth); // return score
        }*/
        double m = -DBL_MAX;
        int best_dir = -1;
        double best_score = -DBL_MAX;
        for (int i = 0; i < 4; ++i) {
            MyGameBoard next_board(board);
            int reward = next_board.move((MoveDirection) i);
            if (!(next_board == board)) {
                int try_evil_pos = 0;
                temp_dir = i;
                /*double score = reward + min_node(next_board, try_evil_pos,
                        m, beta, depth - 1, move_count + 1);*/
                double score = reward + min_node(next_board, try_evil_pos,
                        (alpha > m ? alpha : m), beta, depth - 1, move_count + 1);
                if (score > best_score) {
                    best_score = score;
                    best_dir = i;
                }
                if (score > m) {
                    //dir = (MoveDirection) i;
                    m = score; // update alpha
                } 
                if (m >= beta) {
                    dir = (MoveDirection)best_dir;
                    //trans_table_move.insert(board, m, depth, dir);
                    return m; // cutoff
                }
            }
        }
        dir = (MoveDirection) best_dir;

        if (best_dir == -1)
            dir = (MoveDirection) temp_dir;

        if (dir == -1) {
            //trans_table_move.insert(board, DBL_MAX, depth, temp_dir);
            return DBL_MAX; // For min to know
        } else {
            //trans_table_move.insert(board, m, depth, best_dir);
            return m;
        }
    }
}
