#ifndef PLAYER_H
#define PLAYER_H


class Player
{
public:
    Player();
    Player(int num , char sym , char type);
    char GetSymbol( ) {return symbol;}
    int GetNumber( ) {return number;}
    char GetType ( ) {return type;}
    bool GetWon ( ) {return won;}
    void SetWon (bool victoire ) {won = victoire;}

protected:
    int number;
    char symbol;
    char type;
    bool won;
};

#endif // PLAYER_H
