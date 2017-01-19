#include "zhash.h"
#include <functional>

// Return 64 bit unsigned integer in between [0, 2^64 - 1]
u8 ranval( ranctx *x ) {
    u8 e = x->a - rot(x->b, 7);
    x->a = x->b ^ rot(x->c, 13);
    x->b = x->c + rot(x->d, 37);
    x->c = x->d + e;
    x->d = e + x->a;
    return x->d;
}

// init seed
void raninit( ranctx *x, u8 seed ) {
    u8 i;
    x->a = 0xf1ea5eed, x->b = x->c = x->d = seed;
    for (i=0; i<20; ++i) {
        // scramble a few rounds
        ranval(x);
    }
}

zHashTable::zHashTable()
{
    mask = 0xfffff;
    zHash_table_size = sizeof(kvPair) * (mask+1);
    m_table = new kvPair[mask+1];
    memset(m_table, 0, zHash_table_size);

    /*for (int i = 0; i < 22; ++i) {
        for (int j = 0; j < 16; ++j) {
            m_hash_table[i][j] = ranval(&m_rand);
        }
    }

    for (int i = 0; i < 5; ++i) {
        m_depth_hash_table[i] = ranval(&m_rand);
    }*/

}

zHashTable::~zHashTable()
{
    delete [] m_table;
}

void zHashTable::clear()
{
    memset(m_table, 0, zHash_table_size);
}

u8 zHashTable::getIndex(MyGameBoard board, int depth)
{
    /*u8 index = m_depth_hash_table[depth];
    for (int i = 0; i < 4; ++i) {
        index = index ^ (m_hash_table[int(((board.getRow(i) >> 15) && 0x1f))][4*(3-i)] ^ 
        m_hash_table[int(((board.getRow(i) >> 10) && 0x1f))][4*(3-i)+1] ^ 
        m_hash_table[int(((board.getRow(i) >> 5) && 0x1f))][4*(3-i)+2] ^ 
        m_hash_table[int(((board.getRow(i)) && 0x1f))][4*(3-i)+3]);
    }
    return index;*/
    std::hash<unsigned long> hash_int;

    u8 result = (hash_int(board.getLine(0))
            ^ hash_int(board.getLine(1))
            ^ hash_int(board.getLine(2))
            ^ hash_int(board.getLine(3))
            ^ hash_int(depth));
    return result & mask;
}

double zHashTable::retrieve(MyGameBoard board, int depth)
{
    u8 index = getIndex(board, depth);
    return m_table[index].value;
}

short zHashTable::getBestMove(MyGameBoard board, int depth)
{
    u8 index = getIndex(board, depth);
    return m_table[index].bestMove;
}

bool zHashTable::exist(MyGameBoard board, int depth)
{
    u8 index = getIndex(board, depth);
    return (m_table[index].key == index &&
            m_table[index].real_board == board &&
            m_table[index].remainDepth == depth);
}

bool zHashTable::exist_dir(MyGameBoard board, int depth)
{
    u8 index = getIndex(board, depth);
    return (m_table[index].key == index &&
            m_table[index].real_board == board && 
            m_table[index].remainDepth == depth);
}

void zHashTable::insert(MyGameBoard board, double score, int depth, int best_move)
{
    u8 index = getIndex(board, depth);
    m_table[index].key = index;
    m_table[index].value = score;
    m_table[index].remainDepth = (short)depth;
    m_table[index].bestMove = best_move;
    m_table[index].real_board = board;
}

