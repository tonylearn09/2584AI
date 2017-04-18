#include "MyGameBoard.h"
#include <iostream>
#include <string>
#include <stdlib.h>

const int MyGameBoard::fibonacci_[32] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309};
Random MyGameBoard::random_;

const unsigned int tile_list_hard[3] = {0x1, 0x1, 0x3};
const unsigned int tile_list[4] = {0x1, 0x1, 0x1, 0x3};

MyGameBoard::MyGameBoard():
board_(0)
{
}

MyGameBoard::MyGameBoard(const MyGameBoard &source)
    : board_(source.board_)
{
}

MyGameBoard::MyGameBoard(BitBoardAI b_board)
    : board_(b_board)
{
}

int MyGameBoard::invFib(const int n)
{
    for (int i = 0; i < 32; ++i) {
        if (fibonacci_[i] == n)
            return i;
    }
    return -1;
}

MyGameBoard::MyGameBoard(const int board[4][4])
: board_(0)
{
    for (int i = 0; i < 16; ++i) {
        int inv_fib = invFib(board[i/4][i%4]);
        board_ = (board_ << 5) | BitBoardAI(inv_fib & 0x1f);
    }
}

/* For getFeatureIndex */
int getInt(unsigned long long n)
{
    int result = 0;
    unsigned long mask = 0x10;
    for (int i = 0; i < 5; ++i) {
        //cout << ((n & mask) >> (4-i)) << endl;
        result = (result << 1) | ((n & mask) >> (4-i));
        mask = mask >> 1;
    }
    //cout << "result: " << result << endl;
    return result;
}

unsigned long MyGameBoard::getFeatureIndex(unsigned long long feature_bits)
{
    unsigned long res = 0;
	/*for(int count_shift = 5; count_shift > -1; count_shift--) {
        //cout << "count_shift: " << count_shift << endl;
        //cout << feature_bits << endl;
        //cout << "shift : " << (feature_bits >> (5 * count_shift)) << endl;
		//res  = 22 * res + getInt(feature_bits >> (5 * count_shift));
		res  = 22 * res + ((feature_bits >> (5 * count_shift)) & 0x1f);
	}*/
    res  = 22 * res + ((feature_bits >> 25) & 0x1f);
    res  = 22 * res + ((feature_bits >> 20) & 0x1f);
    res  = 22 * res + ((feature_bits >> 15) & 0x1f);
    res  = 22 * res + ((feature_bits >> 10) & 0x1f);
    res  = 22 * res + ((feature_bits >> 5) & 0x1f);
    res  = 22 * res + (feature_bits & 0x1f);
    return res;
}

void MyGameBoard::initialize()
{
	board_ = 0;
}

int MyGameBoard::move(MoveDirection moveDirection)
{
	BitBoardAI newBoard = 0;
	int score = 0;
	if(moveDirection == MOVE_UP || moveDirection == MOVE_DOWN) {
		for(int i = 0;i < 4;i++) {
			BitBoardAI tempColumn = 0;
			tempColumn = getColumn(i);
			newBoard |= restoreColumn(MoveTable::move_table.row_move_table_[moveDirection == MOVE_DOWN][tempColumn], i);
			score += MoveTable::move_table.row_move_score_table_[moveDirection == MOVE_DOWN][tempColumn];
		}
	}
	else {
		for(int i = 0;i < 4;i++) {
			BitBoardAI tempRow = 0;
			tempRow = getRow(i);
			newBoard |= restoreRow(MoveTable::move_table.row_move_table_[moveDirection == MOVE_RIGHT][tempRow], i);
			score += MoveTable::move_table.row_move_score_table_[moveDirection == MOVE_RIGHT][tempRow];
		}
	}
	board_ = newBoard;
	return score;
}

void MyGameBoard::addTile(int pos, unsigned long long move_count)
{
    if (getTile(3-pos/4, 3-pos%4) != 0) {
        showBoard();
        cout << "pos: " << pos << " is " << getTile(3-pos/4, 3-pos%4) << endl;
        exit(1);
    }
    // The sequence specified
    
    BitBoardAI add_tile = tile_list[move_count % 4];
    add_tile <<= (5 * (15-pos));

    board_ |= add_tile; // Add the tile
    
    /*
    BitBoardAI add_tile = tile_list_hard[move_count % 3];
    add_tile <<= (5 * (15-pos));
    board_ |= add_tile; // Add the tile
    */
}

int MyGameBoard::countEmptyTile()
{
	int count = 0;
	BitBoardAI tileMask = 0x1f;
	for(;tileMask != 0;tileMask <<= 5) {
		if((board_ & tileMask) == 0)
			count++;
	}
	return count;
}

bool MyGameBoard::terminated()
{
	bool movable = false;
	BitBoardAI tempColumn;
	BitBoardAI tempRow;
	for(int i = 0;i < 4;i++) {
		tempColumn = getColumn(i);
		movable |= MoveTable::move_table.row_can_move_[0][tempColumn];
		movable |= MoveTable::move_table.row_can_move_[1][tempColumn];
		tempRow = getRow(i);
		movable |= MoveTable::move_table.row_can_move_[0][tempRow];
		movable |= MoveTable::move_table.row_can_move_[1][tempRow];
		if(movable)
			break;
	}
	return !movable;
}

void MyGameBoard::getArrayBoard(int board[4][4])
{
	BitBoardAI tempBoard = board_;
	for(int i = 0;i < 16;i++) {
		board[3 - (i / 4)][3 - (i % 4)] = fibonacci_[(int)(tempBoard & 0x1f)];
		tempBoard = tempBoard >> 5;
	}
}

int MyGameBoard::getMaxTile()
{
	BitBoardAI countBoard = board_;
	int max_tile = 0;
	for(int i = 0;i < 16;i++) {
		if((countBoard & 0x1f) > max_tile)
			max_tile = countBoard & 0x1f;
		countBoard >>= 5;
	}
	return fibonacci_[max_tile];
}

BitBoardAI MyGameBoard::getRow( int row )
{
	return board_ >> (row * 20) & 0xfffff;
}

BitBoardAI MyGameBoard::getColumn( int column )
{
	BitBoardAI tempBoard = board_ >> (column * 5) & BitBoardAI(0x0001, 0xf0001f0001f0001f);
	return (tempBoard & BitBoardAI(0x0001, 0xf000000000000000)) >> 45 |
      	   (tempBoard & BitBoardAI(0x0000, 0x00001f0000000000)) >> 30 |
      	   (tempBoard & BitBoardAI(0x0000, 0x0000000001f00000)) >> 15 |
       	   (tempBoard & BitBoardAI(0x0000, 0x000000000000001f));
}

BitBoardAI MyGameBoard::restoreRow( BitBoardAI rowBits, int row )
{
  	return rowBits << (row * 20);
}

BitBoardAI MyGameBoard::restoreColumn( BitBoardAI columnBits, int column )
{
  	return ((columnBits & BitBoardAI(0xf8000)) << 45 |
            (columnBits & BitBoardAI(0x07c00)) << 30 |
            (columnBits & BitBoardAI(0x003e0)) << 15 |
            (columnBits & BitBoardAI(0x0001f)) ) << (column * 5);
}

int MyGameBoard::getTile( int row, int column )
{
	int tile = ((board_ >> (row * 20) & 0xfffff) >> (column * 5)) & 0x1f;
	return fibonacci_[tile];
}

void MyGameBoard::showBoard()
{
  	for(int row = 3;row >= 0;row--) {
   		for(int column = 3;column >= 0;column--)
      		cout << dec << getTile(row, column) << "\t";
    	cout << "\n";
  	}
}

bool MyGameBoard::operator==(MyGameBoard myGameBoard)
{
	return board_ == myGameBoard.board_;
}

void MyGameBoard::getSameBoard(MyGameBoard same_board[8])
{
    MyGameBoard up_down = flip_board();
    MyGameBoard left_right = mirror_board();
    MyGameBoard flip_diagonal_ld_to_ru = flip_diagonal_ld_to_ru_board();
    MyGameBoard flip_diagonal_lu_to_rd= flip_diagonal_lu_to_rd_board();
    same_board[0] = *this;
    same_board[1] = left_right;
    same_board[2] = flip_diagonal_ld_to_ru;
    same_board[3] = flip_diagonal_lu_to_rd;
    same_board[4] = up_down;
    same_board[5] = left_right.flip_board();
    same_board[6] = flip_diagonal_ld_to_ru.flip_board();
    same_board[7] = flip_diagonal_lu_to_rd.flip_board();
}

MyGameBoard MyGameBoard::flip_board()
{
    /*board_.show();
    cout << endl;
    (board_ & BitBoard(0xffff, 0xf000000000000000)).show();
    cout << endl;
    ((board_ & BitBoard(0xffff, 0xf000000000000000)) >> 60).show();
    cout << endl;
    (board_ & BitBoard(0x0000,0x0fffff0000000000)).show();
    cout << endl;
    ((board_ & BitBoard(0x0000,0x0fffff0000000000)) >> 20).show();
    cout << endl;
    (board_ & BitBoard(0x0000, 0x000000fffff00000)).show();
    cout << endl;
    ((board_ & BitBoard(0x0000, 0x000000fffff00000)) << 20).show();
    cout << endl;
    (board_ & BitBoard(0x0000, 0x00000000000fffff)).show();
    cout << endl;
    ((board_ & BitBoard(0x0000, 0x00000000000fffff)) << 60).show();
    cout << endl; */


    return MyGameBoard(((board_ & BitBoardAI(0xffff, 0xf000000000000000)) >> 60) | 
        ((board_ & BitBoardAI(0x0000,0x0fffff0000000000)) >> 20) |
        ((board_ & BitBoardAI(0x0000, 0x000000fffff00000)) << 20) |
        ((board_ & BitBoardAI(0x0000, 0x00000000000fffff)) << 60));
}

MyGameBoard MyGameBoard::mirror_board()
{
    return MyGameBoard(((board_ & BitBoardAI(0xf800, 0x0f8000f8000f8000)) >> 15) | 
        ((board_ & BitBoardAI(0x07c0, 0x007c0007c0007c00)) >> 5) |
        ((board_ & BitBoardAI(0x003e, 0x0003e0003e0003e0)) << 5) |
        ((board_ & BitBoardAI(0x0001, 0xf0001f0001f0001f)) << 15));
}

MyGameBoard MyGameBoard::flip_diagonal_ld_to_ru_board()
{
    return MyGameBoard((board_ & BitBoardAI(0x0001, 0xf003e007c00f8000)) |
        (board_ & BitBoardAI(0x003e, 0x007c00f800000000)) >> 25 |
        (board_ & BitBoardAI(0x0000, 0x00001f003e007c00)) << 25 | 
        (board_ & BitBoardAI(0x07c0, 0x0f80000000000000)) >> 50 |
        (board_ & BitBoardAI(0x0000, 0x0000000001f003e0)) << 50 |
        (board_ & BitBoardAI(0xf800, 0x0000000000000000)) >> 75 |
        (board_ & BitBoardAI(0x0000, 0x000000000000001f)) << 75);
}
MyGameBoard MyGameBoard::flip_diagonal_lu_to_rd_board()
{
    return MyGameBoard((board_ & BitBoardAI(0xf800, 0x007c00003e00001f)) |
        (board_ & BitBoardAI(0x07c0, 0x0003e00001f00000)) >> 15 |
        (board_ & BitBoardAI(0x0000, 0x0f800007c00003e0)) << 15 |
        (board_ & BitBoardAI(0x003e, 0x00001f0000000000)) >> 30 |
        (board_ & BitBoardAI(0x0000, 0x000000f800007c00)) << 30 |
        (board_ & BitBoardAI(0x0001, 0xf000000000000000)) >> 45 |
        (board_ & BitBoardAI(0x0000, 0x00000000000f8000)) << 45);
}

BitBoardAI MyGameBoard::getBitBoardAI()
{
    return board_;
}

unsigned long long MyGameBoard::getString()
{
    unsigned long long board_left_bits = int(board_ >> 64);
    unsigned long long board_right_bits =  (int)board_;
    return 0.5 * (board_left_bits + board_right_bits) * 
        (board_left_bits + board_right_bits + 1) + board_right_bits;
}

unsigned long MyGameBoard::getLine( int num )
{
	unsigned long result = 0;
	unsigned long long boardRight = board_.get_right();
    switch (num) {
        case 0:
            result = (board_.get_left() << 4) | (boardRight >> 60 & 0xf);
            break;
        case 1:
            result = boardRight >> 40 & 0xfffff;
            break;
        case 2:
            result = boardRight >> 20 & 0xfffff;
            break;
        case 3:
            result = boardRight & 0xfffff;
            break;
        default:
            break;
    }
	return result;
}
