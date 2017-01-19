#include <iostream>
#include <cstdlib>
//#include "Fib2584/GameBoard.h"
#include "../MyGameBoard.h"
#include "../Fib2584/Typedefs.h"
#include "../Fib2584/Statistic.h"
#include "../Fib2584Ai.h"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc == 1) {
        cerr << "usage: play_game rounds [other arguments which your AI needs]" << endl;
        return 1;
    }
    int iPlayRounds = atoi(argv[1]);
    // create and initialize AI
    Fib2584Ai ai;
    ai.initialize(argc, argv);
    // initialize statistic data
    Statistic statistic;
    statistic.setStartTime();
    long score_per_1000 = 0;
    // play each round
    for(int i = 0;i < iPlayRounds;i++) {
        MyGameBoard gameBoard;
        gameBoard.initialize();
        int iScore = 0;
        int arrayBoard[4][4];
        int moveIndex = 0;
        // First random
        gameBoard.getArrayBoard(arrayBoard);
        gameBoard.addTile(ai.generateEvilMove(arrayBoard), moveIndex);
        //gameBoard.showBoard();
        // Second random
        moveIndex++;
        gameBoard.getArrayBoard(arrayBoard);
        gameBoard.addTile(ai.generateEvilMove(arrayBoard), moveIndex);
        //gameBoard.showBoard();

        while(!gameBoard.terminated()) {
            gameBoard.getArrayBoard(arrayBoard);
            MoveDirection moveDirection = ai.generateMove(arrayBoard);
            //cout << "move_direction: " << (int) moveDirection << endl;

            MyGameBoard originalBoard = gameBoard;
            iScore += gameBoard.move(moveDirection);
            //gameBoard.showBoard();
            //cout << endl;
            if(originalBoard == gameBoard)
            {
                cout << "last board: " << endl;
                originalBoard.showBoard();
                cout << endl;
                cout << "move_direction: " << (int) moveDirection << endl;
                cout << "Repeated\n" << endl;
                exit(1);
            }
            statistic.increaseOneMove();

            moveIndex++;
            gameBoard.getArrayBoard(arrayBoard);
            //cout << "Before add: " << endl;
            //gameBoard.showBoard();
            //int evil_pos = ai.generateEvilMove(arrayBoard);
            //gameBoard.showBoard();
            /*cout << "In main file Add tile: " <<
                evil_pos << endl;*/
            //gameBoard.addTile(evil_pos, moveIndex);
            gameBoard.addTile(ai.generateEvilMove(arrayBoard), moveIndex);
            //cout << "Add at: " << evil_pos << endl;
            //cout << "After add: " << endl;
            //gameBoard.showBoard();
            //cout << endl;
        }
        gameBoard.getArrayBoard(arrayBoard);
        cout << "round " << i+1 << ": " << iScore << endl;
        ai.gameOver(arrayBoard, iScore);
        statistic.increaseOneGame();
        statistic.updateScore(iScore);
        statistic.updateMaxTile(gameBoard.getMaxTile());
        score_per_1000 += iScore;
        if ((i+1) % 1000 == 0) {
            //cout << "Average score for round " << i + 1 << " : " << score_per_1000 / 1000 << endl;
            score_per_1000 = 0;
        }
    }
    cout << endl;
    statistic.setFinishTime();
    statistic.show();

    return 0;
}
