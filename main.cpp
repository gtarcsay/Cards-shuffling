/* rotate algorithm
 move element to left (higher one): rotate(target place for the card, vector no. of the card, no. +1 (upper boundary))
 move element to right (lower one): rotate(where to move, which card to move)
 in case of reverse iteration: cards.rbegin()+size is the real begin of the vector:
 std::rotate(cards.begin()+3, cards.begin() + 8, cards.begin()+9);   H5 <- H10  
 std::rotate(cards.rbegin()+size-11, cards.rbegin() + size-8, cards.rbegin()+size-7);  H8 -> HQ
 
*/
#include <iostream>
#include "cards.h"
#include <fstream>


void demo()
{
    Deck d = Deck();
    std::cout << "Konstrualt pakli szinsorrendben: \n";
    std::cout << d;
    d.my_shuffle(1,1, 3, get_gen());
    std::cout <<"Egy idealis osszefesules utan: \n";
    std::cout <<d;

}

void print_to_file(std::vector<double> to_print)
{
        std::ofstream myfile;
        myfile.open ("2sequence.txt");
        for(int i =0; i< to_print.size(); i++)
        {
            myfile << i+1 << "\t" << to_print[i] << std::endl;
        }
        
        myfile.close();
}



void sequence(int n, int repeat, int row)
{
    std::vector<int> seq(n);
    for(int i = 0; i < n;i++)
        {
            Deck cards = Deck();  
            std::shuffle(cards.begin(), cards.end(), get_gen());
            seq[i] = cards.analyze(row);
        }

    double ideal_average =  accumulate(seq.begin(), seq.end(), 0.0)/n;
    std::cout << "3 same color in a row, average  = "<< ideal_average << std::endl;
    std::vector<double>avg(repeat);

        for(int i = 0 ;i < n; i++){
            if((i % 50) == 0){std::cout << i << std::endl;}
            Deck cards = Deck();    //új pakli  
            cards.my_shuffle(repeat, 0.5, row, get_gen()); //keverés
            std::transform(avg.begin(), avg.end(), cards.seq.begin(), avg.begin(), std::plus<double>());
            }

        std::transform(avg.begin(), avg.end(), avg.begin(), [n](double& x){return x/n;});
        print_to_file(avg);    
      
}

void kings(int n)
{
    bool kings = false;
    std::vector<int> counter(n);
    for(int i = 0; i < n; i++)
    {   
        std::cout << i << std::endl;
        Deck cards = Deck();
       while(kings != true)
        {
            std::shuffle(cards.begin(), cards.end(), get_gen());
            kings = cards.four_kings();
            counter[i] ++;
        }
        kings = false;

    }
    
    double average = accumulate( counter.begin(), counter.end(), 0.0)/n;
    std::cout << "Average of fully random shuffling = " << average << std::endl;
    std::ofstream myfile;
    myfile.open ("4kings_vs_efficiency.txt");

for(int j = 1; j < 11; j++){
    std::vector<int> counter(n);
    std::cout << j*0.1 << std::endl;
    
    for(int i = 0; i < n; i++)
    {
        kings = false;
        while(kings != true)
        {
            if(counter[i] % 1000 == 0){std::cout << counter[i] << std::endl;}
            Deck cards = Deck(); 
            cards.my_shuffle(8, 0.05*j, 0, get_gen()); //last argument: how many same coloured we want in a row - 0 means 4kings
            cards.lift(get_gen());   //emelés 
            kings = cards.four_kings();
            counter[i] ++;
            
        }
        

    }
    double average = accumulate( counter.begin(), counter.end(), 0.0)/n;
    myfile << 0.1*j << "\t" << average << std::endl;

}
 myfile.close();

}



int main(int, char**) {

   // sequence(1000, 100, 2);        //(max number of shuffling, repeat, sequence): search for same coloured sequence   s 
  //  kings(100);                    //(repeat): search for 4 kings in a row 

}


