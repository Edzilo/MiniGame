#include <iostream>
#include "board.h"
#include "player.h"
#include "matrix.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

using namespace std;
const int size = 8;

int main()
{
    srand (time(NULL));
    std::string log;
    std::string report;

    Player* joueur1 = new Player(1,'x','c');
    Player* joueur2 = new Player(2,'o','c');
    Player* nextplayer = joueur1;
    Player* opponent = joueur2;
    Board* tab = new Board(size);
    tab->Print();

    while(!tab->GameOVer()){
        log.append(tab->PlayTurn(nextplayer,opponent));
        tab->Print();
        if(nextplayer->GetNumber() == joueur1->GetNumber()){
            nextplayer = joueur2;
            opponent = joueur1;
        }
        else {
            nextplayer = joueur1;
            opponent = joueur2;
        }
    }
    //outputFile.close();
    int score1 = tab->CountSymbol(joueur1->GetSymbol());
    int score2 = tab->CountSymbol(joueur2->GetSymbol());
    cout << "score joueur 1 " << joueur1->GetSymbol() << " : " << score1 << endl;
    cout << "score joueur 2 " << joueur2->GetSymbol() << " : " << score2 << endl;

    joueur1->SetWon(score1 >= score2);
    joueur2->SetWon(score2 >= score1);

    int num_winner = joueur1->GetWon() ? joueur1->GetNumber() : joueur2->GetNumber();
    report.append(std::to_string(size));
    report.append("\n");
    report.append(std::to_string(num_winner));
    report.append("\n");
    report.append(log);
    std::ofstream out("/home/gcerba/Minigame/output.txt");
    out << report;
    out.close();

    delete joueur1;
    delete joueur2;
    delete tab;

    return 0;
}
