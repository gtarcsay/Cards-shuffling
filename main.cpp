/* rotate algorithm
 move element to left (higher one): rotate(target place for the card, vector no. of the card, no. +1 (upper boundary))
 move element to right (lower one): rotate(where to move, which card to move)
 in case of reverse iteration: cards.rbegin()+size is the real begin of the vector:
 std::rotate(cards.begin()+3, cards.begin() + 8, cards.begin()+9);   H5 <- H10  
 std::rotate(cards.rbegin()+size-11, cards.rbegin() + size-8, cards.rbegin()+size-7);  H8 -> HQ
 
*/
#include <iostream>
#include "cards.h"
#include <numeric>

void demo()
{
    Deck d = Deck();
    std::cout << "Konstrualt pakli szinsorrendben: \n";
    std::cout << d;
    d.my_shuffle(1,1);
    std::cout <<"Egy idealis osszefesules utan: \n";
    std::cout <<d;

}

void sequence(int n, bool mine)
{
    
    std::vector<double> avg(12);
    for(int i = 1; i<n+1; i++){

        Deck cards = Deck();    //új pakli
        if(mine == true){
        cards.my_shuffle(10, 1); //keverés
        cards.lift();   //emelés
        std::vector<int> seq = analyze(cards);  //szekvencia keresés
        for(int j= 0; j< seq.size(); j++)
        {
        avg[j] = (i-1)/double(i)*double(avg[j])+1/double(i)*double(seq[j]);
        }
     

    }
       
}
}


int main(int, char**) {


        demo();


 
    }



