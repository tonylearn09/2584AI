#ifndef __MYMyGameBoard_H__
#define __MYMyGameBoard_H__

#include <iostream>
#include "Fib2584/Typedefs.h"
#include "Fib2584/MoveTable.h"
//#include "Fib2584/BitBoard.h"
#include "BitBoardAI.h"
#include "Fib2584/Random.h"
#include <string>

using namespace std;

class MyGameBoard
{
public:
	static const int fibonacci_[32];
private:
	static Random random_;
public:
	MyGameBoard();
    MyGameBoard(const MyGameBoard &source); // Added
    MyGameBoard(const int board[4][4]); // Added
    MyGameBoard(BitBoardAI b_board); // Added
	void initialize();
	int move(MoveDirection moveDirection);
	void addTile(int pos, unsigned long long move_count);
	bool terminated();
	void getArrayBoard(int board[4][4]);
	int getMaxTile();
	void showBoard();
	bool operator==(MyGameBoard myGameBoard);
    void getSameBoard(MyGameBoard same_board[8]); // Added
	int getTile(int row, int column);
private:
	int invFib(const int n); // Added
	BitBoardAI getColumn(int column);
	BitBoardAI restoreRow(BitBoardAI rowBits, int row);
	BitBoardAI restoreColumn(BitBoardAI columnBits, int column);
	int getFibonacci(int index);
public: // test, Added
	int countEmptyTile(); // Move it to pulbic
    unsigned long getFeatureIndex(unsigned long long feature_bits);
    MyGameBoard flip_board();
    MyGameBoard mirror_board();
    MyGameBoard flip_diagonal_ld_to_ru_board();
    MyGameBoard flip_diagonal_lu_to_rd_board();
    BitBoardAI getBitBoardAI();
    unsigned long long getString();
	BitBoardAI getRow(int row); // Added
	unsigned long getLine( int num );
private:
	BitBoardAI board_;
};

#endif
