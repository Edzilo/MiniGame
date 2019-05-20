#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include<bits/stdc++.h>
#include "player.h"

class Board
{
public:
    Board();
    Board(int size);
    void Print();
    std::string getBoardString();
    ~Board();
    int CountSymbol(char symbol);
    bool AddSymbol(char symbol , int i , int j , std::string& log);
    int ComputeModifications(char symbol , int i , int j);
    int ComputeSafeSpot(char symbol , int i , int j);
    std::string PlayTurn(Player* player , Player* opponent);
    bool GameOVer();


protected:
    int size;
    char** board;
};

#endif // BOARD_H
