#include "board.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>

using namespace std;

Board::Board() : Board(8)
{ }

Board::Board(int new_size)
{
    size = new_size;
    board = new char*[new_size];
    for(int i = 0; i < new_size ; i++){

        board[i] = new char[new_size];
        memset(board[i], '-', new_size*sizeof('-'));
    }
}


Board::~Board(){
    for(int i = 0; i < size ; i++){
        delete[] board[i];
    }
    delete[] board;
    cout << "board deleted" << endl;
}

int Board::CountSymbol(char symbol){
    int res = 0;
    for(int i = 0 ; i < size ; i++){
        for(int j = 0 ; j < size ; j++){
            if(board[i][j] == symbol)res++;
        }
    }
    return res;
}


std::string Board::PlayTurn(Player* player, Player* opponent){
    int ligne,colonne, temp,index;
    int ligne_attaque,colonne_attaque,ligne_defense,colonne_defense,ligne_safe,colonne_safe = 0;
    string log;
    bool res = false;
    do{
        if(player->GetType() == 'h'){
            cin >> ligne >> colonne;
            res = AddSymbol(player->GetSymbol( ), ligne , colonne , log);
            if(!res) cout<< "coordonnes invalides" << endl;
        }
        else{

            vector < tuple <int,int> > choix_attaque;
            int compteur_attaque = 0;
            for(int i = 0 ; i < size && !res ; i++){
                for(int j = 0 ; j < size ; j++){
                    if(board[i][j] == '-'){
                        temp = ComputeModifications(player->GetSymbol( ),i,j);
                        if(temp >= compteur_attaque){
                            if(temp > compteur_attaque)choix_attaque.clear();
                            compteur_attaque = temp;
                            ligne_attaque = i;
                            colonne_attaque = j;


                            choix_attaque.push_back(make_tuple(ligne_attaque,colonne_attaque));
                      }
                    }
                }
            }


            vector < tuple <int,int> > choix_defense;
            int compteur_defense = 0;
            for(int i = 0 ; i < size && !res ; i++){
                for(int j = 0 ; j < size ; j++){
                    if(board[i][j] == '-'){
                        temp = ComputeModifications(opponent->GetSymbol( ),i,j);
                        if(temp >= compteur_defense){
                            if(temp > compteur_defense)choix_defense.clear();
                            compteur_defense = temp;
                            ligne_defense = i;
                            colonne_defense = j;
                            choix_defense.push_back(make_tuple(ligne_defense,colonne_defense));
                      }
                    }
                }
            }

            vector < tuple <int,int> > choix_safe;
            int compteur_safe = 0;
            for(int i = 0 ; i < size && !res ; i++){
                for(int j = 0 ; j < size ; j++){
                    if(board[i][j] == '-'){
                        temp = ComputeSafeSpot(player->GetSymbol( ),i,j);
                        if(temp >= compteur_safe){
                            if(temp > compteur_safe)choix_safe.clear();
                            compteur_safe = temp;
                            ligne_safe = i;
                            colonne_safe = j;
                            choix_safe.push_back(make_tuple(ligne_safe,colonne_safe));
                      }
                    }
                }
            }


            if((compteur_attaque == 0 && compteur_defense == 0) || ((compteur_defense <= 2 * compteur_attaque + 1) && (compteur_attaque == 0) ) ){
                if(compteur_safe > 0 + 3){
                    cout<< "choix safe "<< endl;
                    index = rand()%choix_safe.size();
                    ligne = get<0>(choix_safe[index]);
                    colonne = get<1>(choix_safe[index]);

                } else {
                    cout<< "choix au hasard "<< endl;
                    ligne = rand()%size;
                    colonne = rand()%size;
                }
            } else {
                if(compteur_defense > 2 * compteur_attaque + 1) {

                    cout<< "choix protection" << endl;
                    index = rand()%choix_defense.size();
                    ligne = get<0>(choix_defense[index]);
                    colonne = get<1>(choix_defense[index]);

                }else {
                    cout<< "choix attaque" << endl;

                    index = rand()%choix_attaque.size();
                    ligne = get<0>(choix_attaque[index]);
                    colonne = get<1>(choix_attaque[index]);

                }
            }
            //
            //int compteur_safe = ComputeSafeSpot(player->GetSymbol( ),ligne,colonne);
            //cout<< "modifications en attaque en " << ligne << "," << colonne << " " << compteur_attaque << endl;
            //cout<< "modifications en defense en " << compteur_defense << endl;
            //cout<< "angles safe " << compteur_safe << endl;
            cout<< "joué en (" << ligne << "," << colonne << ")"  << endl;
            log.append( to_string(player->GetNumber()));
            res = AddSymbol(player->GetSymbol( ), ligne , colonne , log);
            if(!res)log = log.substr(0, log.size()-1);
        }
    }
    while( !res );
    return log;
}

bool Board::AddSymbol(char symbol , int ligne , int colonne , std::string& log){
    if( ligne >= size || ligne < 0 || colonne < 0 || colonne >= size || board[ligne][colonne] != '-') return 0 ;
    else{
        std::ostringstream oss;
        oss  << " " << ligne << " " << colonne << endl ;
        log.append(oss.str());
        log.append(getBoardString());
         board[ligne][colonne] = symbol;

         //Ligne gauche
         int indice = 1;
         for(int j = colonne - 1 ; j >= 0 && board[ligne][j] != '-' && board[ligne][j] != symbol ; j-- , indice++);
         if(colonne - indice >= 0 && board[ligne ][colonne - indice] == symbol)
            for(int temp = 0 ; temp <= indice ; temp++)board[ligne][colonne - temp] = symbol;

         //Ligne droite
         indice = 1;
         for(int j = colonne + 1 ; j < size && board[ligne][j] != '-' && board[ligne][j] != symbol ; j++ , indice++);
         if(colonne + indice < size && board[ligne ][colonne + indice] == symbol)
            for(int temp = 0 ; temp <= indice ; temp++)board[ligne][colonne + temp] = symbol;

         //colonne haut
         indice = 1;
         for(int i = ligne -1 ; i >= 0 && board[i][colonne] != '-' && board[i][colonne] != symbol ; i-- , indice++);
         if(ligne - indice >= 0 && board[ligne -indice ][colonne] == symbol)
            for(int temp = 0 ; temp <= indice ; temp++) board[ligne-temp ][colonne] = symbol;

         //colonne bas
         indice = 1;
         for(int i = ligne + 1 ; i < size && board[i][colonne] != '-' && board[i][colonne] != symbol ; i++ , indice++);
         if(ligne + indice < size  && board[ligne + indice ][colonne] == symbol)
            for(int temp = 0 ; temp <= indice ; temp++) board[ligne+temp ][colonne] = symbol;

         //diagonale haut droit
         indice = 1;
         for(int i = ligne - 1 ,  j = colonne + 1 ; i >= 0 && j < size && board[i][j] != '-' && board[i][j] != symbol ; i--, j++ , indice++);
         if(ligne - indice >= 0 && colonne + indice < size && board[ligne - indice][colonne + indice] == symbol)
            for(int temp = 0 ; temp < indice ; temp++)board[ligne - temp][colonne + temp] = symbol;

         //diagonale haut gauche
         indice = 1;
         for(int i = ligne - 1 ,  j = colonne - 1 ; i >= 0 && j >= 0 && board[i][j] != '-' && board[i][j] != symbol ; i--, j-- , indice++);
         if(ligne - indice >= 0 && colonne - indice >= 0 && board[ligne - indice][colonne - indice] == symbol)
            for(int temp = 0 ; temp < indice ; temp++)board[ligne - temp][colonne - temp] = symbol;

         //diagonale bas gauche
         indice = 1;
         for(int i = ligne + 1 ,  j = colonne - 1 ; i < size && j >= 0 && board[i][j] != '-' && board[i][j] != symbol ; i++, j-- , indice++);
         if(ligne + indice < size && colonne - indice >= 0 && board[ligne + indice][colonne - indice] == symbol)
            for(int temp = 0 ; temp < indice ; temp++)board[ligne + temp][colonne - temp] = symbol;

         //diagonale bas droit
         indice = 1;
         for(int i = ligne + 1 ,  j = colonne + 1 ; i < size && j < size && board[i][j] != '-' && board[i][j] != symbol ; i++, j++ , indice++);
         if(ligne + indice < size && colonne + indice < size && board[ligne + indice][colonne + indice] == symbol)
            for(int temp = 0 ; temp < indice ; temp++)board[ligne + temp][colonne + temp] = symbol;

         return 1;
    }
}

int Board::ComputeModifications(char symbol , int ligne , int colonne){
    int res = 0;
    if( ligne >= size || ligne < 0 || colonne < 0 || colonne >= size ) return 0 ;
    else{


        //Ligne gauche
        int indice = 1;
        for(int j = colonne - 1 ; j >= 0 && board[ligne][j] != '-' && board[ligne][j] != symbol ; j-- , indice++);
        if(colonne - indice >= 0 && board[ligne ][colonne - indice] == symbol)
            res += indice -1;

        //Ligne droite
        indice = 1;
        for(int j = colonne + 1 ; j < size && board[ligne][j] != '-' && board[ligne][j] != symbol ; j++ , indice++);
        if(colonne + indice < size && board[ligne ][colonne + indice] == symbol)
           res += indice -1;

        //colonne haut
        indice = 1;
        for(int i = ligne -1 ; i >= 0 && board[i][colonne] != '-' && board[i][colonne] != symbol ; i-- , indice++);
        if(ligne - indice >= 0 && board[ligne -indice ][colonne] == symbol)
           res += indice-1;

        //colonne bas
        indice = 1;
        for(int i = ligne + 1 ; i < size && board[i][colonne] != '-' && board[i][colonne] != symbol ; i++ , indice++);
        if(ligne + indice < size  && board[ligne + indice ][colonne] == symbol)
           res += indice-1;

        //diagonale haut droit
        indice = 1;
        for(int i = ligne - 1 ,  j = colonne + 1 ; i >= 0 && j < size && board[i][j] != '-' && board[i][j] != symbol ; i--, j++ , indice++);
        if(ligne - indice >= 0 && colonne + indice < size && board[ligne - indice][colonne + indice] == symbol)
           res += indice-1;

        //diagonale haut gauche
        indice = 1;
        for(int i = ligne - 1 ,  j = colonne - 1 ; i >= 0 && j >= 0 && board[i][j] != '-' && board[i][j] != symbol ; i--, j-- , indice++);
        if(ligne - indice >= 0 && colonne - indice >= 0 && board[ligne - indice][colonne - indice] == symbol)
           res += indice-1;

        //diagonale bas gauche
        indice = 1;
        for(int i = ligne + 1 ,  j = colonne - 1 ; i < size && j >= 0 && board[i][j] != '-' && board[i][j] != symbol ; i++, j-- , indice++);
        if(ligne + indice < size && colonne - indice >= 0 && board[ligne + indice][colonne - indice] == symbol)
           res += indice-1;
        //diagonale bas droit
        indice = 1;
        for(int i = ligne + 1 ,  j = colonne + 1 ; i < size && j < size && board[i][j] != '-' && board[i][j] != symbol ; i++, j++ , indice++);
        if(ligne + indice < size && colonne + indice < size && board[ligne + indice][colonne + indice] == symbol)
           res += indice-1;

         return res;
    }
}

int Board::ComputeSafeSpot(char symbol , int ligne , int colonne){
    int res = 0;
    if( ligne >= size || ligne < 0 || colonne < 0 || colonne >= size ) return 0 ;
    else{

        //Ligne gauche
        int indice = 1;
        for(int j = colonne - 1 ; j >= 0 && board[ligne][j] == symbol ; j-- , indice++);
        if(colonne - indice < 0 || board[ligne ][colonne - indice + 1 ] == symbol)
            res ++;

        //Ligne droite
        indice = 1;
        for(int j = colonne + 1 ; j < size  && board[ligne][j] == symbol ; j++ , indice++);
        if(colonne + indice >= size || board[ligne ][colonne + indice] == symbol)
           res ++;

        //colonne haut
        indice = 1;
        for(int i = ligne -1 ; i >= 0  && board[i][colonne] == symbol ; i-- , indice++);
        if(ligne - indice < 0 || board[ligne -indice ][colonne] == symbol)
           res ++;

        //colonne bas
        indice = 1;
        for(int i = ligne + 1 ; i < size && board[i][colonne] == symbol ; i++ , indice++);
        if(ligne + indice >= size  || board[ligne + indice ][colonne] == symbol)
           res ++;

        //diagonale haut droit
        indice = 1;
        for(int i = ligne - 1 ,  j = colonne + 1 ; i >= 0  && board[i][j] == symbol ; i--, j++ , indice++);
        if(ligne - indice < 0 || colonne + indice >= size || board[ligne - indice][colonne + indice] == symbol)
           res ++;

        //diagonale haut gauche
        indice = 1;
        for(int i = ligne - 1 ,  j = colonne - 1 ; i >= 0  && board[i][j] == symbol ; i--, j-- , indice++);
        if(ligne - indice < 0 || colonne - indice < 0 || board[ligne - indice][colonne - indice] == symbol)
           res ++;

        //diagonale bas gauche
        indice = 1;
        for(int i = ligne + 1 ,  j = colonne - 1 ; i < size && j >= 0  && board[i][j] == symbol ; i++, j-- , indice++);
        if(ligne + indice >= size || colonne - indice < 0 || board[ligne + indice][colonne - indice] == symbol)
           res ++;

        //diagonale bas droit
        indice = 1;
        for(int i = ligne + 1 ,  j = colonne + 1 ; i < size && j < size  && board[i][j] == symbol ; i++, j++ , indice++);
        if(ligne + indice >= size || colonne + indice >= size || board[ligne + indice][colonne + indice] == symbol)
           res ++;

         return res;
    }
}

bool Board::GameOVer(){
    return CountSymbol('-') == 0;

}

void Board::Print(){
    cout << "  ";
    for(int i = 0 ; i < size ; cout << i << " ",i++);
    cout << endl;
    for(int i = 0 ; i < size ; i++){
        cout << i << " ";
        for(int j = 0 ; j < size ; j++){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

std::string Board::getBoardString(){
    string result;
    std::ostringstream oss;

    for(int i = 0 ; i < size ; i++){
        for(int j = 0 ; j < size ; j++){
            oss << board[i][j] << " ";
        }
        oss << endl;
    }
    oss << endl;
    result = (oss.str());

    return result;

}
