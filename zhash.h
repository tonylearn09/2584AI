#ifndef __ZHASH_H
#define __ZHASH_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "MyGameBoard.h"

using namespace std;

typedef unsigned long long u8;
// random controllers
typedef struct ranctx { u8 a; u8 b; u8 c; u8 d; } ranctx;

// rotate
#define rot(x, k) (((x) << (k)) | ((x) >> (64 - (k))))
#define zHashSeed 0xD4E12C77

class kvPair{
    public:
        u8 key;
        double value;
        short remainDepth;
        short bestMove;
        MyGameBoard real_board;
        short dir;
        short evil_location;
};

class zHashTable{
    public:
        zHashTable();
        ~zHashTable();
        void clear();
        u8 getIndex(MyGameBoard board, int depth);
        double retrieve(MyGameBoard board, int depth);
        short getBestMove(MyGameBoard board, int depth);
        bool exist(MyGameBoard board, int depth);
        bool exist_dir(MyGameBoard board, int depth);
        void insert(MyGameBoard board, double score, int depth, int best_move);
    private:
        ranctx m_rand;
        u8 m_seed;
        u8 m_hash_table[22][16];
        u8 m_depth_hash_table[5];
        u8 mask;
        kvPair *m_table;
        int zHash_table_size;
};

#endif
