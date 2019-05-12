#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <numeric>
#include<functional>

   auto get_gen = [](){
        std::random_device rd;
        std::mt19937 gen(rd());
        return gen;
    };
//class for a single card
class Card
{

    public:
        enum Suit{Begin_s, HEARTS = Begin_s, CLUBS, SPADES, DIAMONDS,End_s};
        enum Value{Begin, TWO = Begin, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE, End};

        

        //constructor for a card
        Card(Suit suit, Value value): card_suit{suit}, card_value{value} {}
        Card(Card const&) = default;
        Card( Card && ) = default;
        //Copy and Move assignment operators implemented by the compiler:
	    Card& operator=(Card const&) = default;
	    Card& operator=(Card &&) = default;
            

        //getters
        Suit  get_suit()const{return card_suit;}
        Value get_value()const{return card_value;}

    private:
        Suit card_suit;
        Value card_value;
    
};

//streaming a card 
std::ostream& operator<<(std::ostream& o, Card& c)
	{
        Card::Value v = c.get_value();
        Card::Suit s = c.get_suit();
        switch(s)
        {
            case Card::Suit::HEARTS: o << "Hearts "; break;
            case Card::Suit::CLUBS: o << "Clubs ";break;
            case Card::Suit::SPADES: o << "Spades ";break;
            case Card::Suit::DIAMONDS: o << "Diamonds ";break;

        }
        switch(v)
        {
            case Card::Value::TWO: o << "2" ;break;
            case Card::Value::THREE: o << "3" ;break;
            case Card::Value::FOUR: o << "4" ;break;
            case Card::Value::FIVE: o << "5" ;break;
            case Card::Value::SIX: o << "6" ;break;
            case Card::Value::SEVEN: o<< "7" ;break;
            case Card::Value::EIGHT: o << "8" ;break;
            case Card::Value::NINE: o << "9" ;break;
            case Card::Value::TEN: o << "10" ;break;
            case Card::Value::JACK: o << "J" ;break;
            case Card::Value::QUEEN: o << "Q" ;break;
            case Card::Value::KING: o << "K" ;break;
            case Card::Value::ACE: o << "A" ;break;

        } 
		return o;
	}


//class for the deck
class Deck{
private:
        std::vector<Card> data;
        

public:

    std::vector<double> seq;
    
    //constructor for the deck
        Deck()
        {

            for(int i = Card::Suit::Begin_s; i < Card::Suit::End_s; ++i )
            {
                for(int j = Card::Value::Begin; j < Card::Value::End; ++j)
                {
                    
                    data.push_back(Card(Card::Suit(i),Card::Value(j)));
                }
            }
        }
    Deck(Deck const&) = default;
    Deck( Deck && )= default;


 
    
    int size()const
    {
        return static_cast<int>(data.size());
    }
    

    Card& operator[]( int i ){ return data[i]; } 
    Card const& operator[]( int i )const{ return data[i]; } 

    //iterators and reverse iterators
    auto begin()
	{
		return data.begin();
	}


	auto end()
	{
		return data.end();
	}

    auto cbegin()
	{
		return data.cbegin();
	}


	auto cend()
	{
		return data.cend();
	}


        auto rbegin()
	{
		return data.rbegin();
	}


	auto rend()
	{
		return data.rend();
	}

    template <typename F>
    int separate_deck( int a, int b, F f)
    {
        std::uniform_int_distribution<> distr(a, b); 
        int bias = distr(f);        
        int NumberOfCards = static_cast<int>(data.size());
        return NumberOfCards/2 + bias;
    }

    std::vector<double> get_weights(double const& efficiency)
    {
        
        int num_of_elements;
        int i = 0;
        if(efficiency > 0. && efficiency <= 0.3){num_of_elements = 4;}
        if(efficiency > 0.3 && efficiency <= 0.65){num_of_elements = 3;}
        if(efficiency > 0.65 && efficiency < 1.){num_of_elements = 2;}
        if(efficiency == 1. ){num_of_elements = 1;}
        std::vector<double> weights(num_of_elements);

        auto elem =  [&i, &efficiency, &num_of_elements](double& fir, double& sec){ 
        fir = std::exp(-i*i*efficiency); 
        i++;
        if(i == num_of_elements-1){ 
            sec = std::exp(-i*i*efficiency);
        } 
        return sec;
    };
        //get normalized weights
        std::exclusive_scan(weights.begin()+1, weights.end(),weights.begin(), 1., elem); 
        double sum =std::accumulate(weights.begin(), weights.end(), 0.);
        std::transform(weights.begin(), weights.end(), weights.begin(), [sum](double& x){return x/sum;});
        
        return weights;
    }

    template <typename F>
    auto lift(F f)
    {
        int separate = separate_deck(-10, 10, f);
        int deck_size = static_cast<int>(data.size());
        std::rotate(data.begin(), data.begin()+separate, data.end());   
        return *this;

    }


    /*
    my_shuffle: pakli kb. kettéválasztása -> jobb és bal oldali félpaklik. Fogom a jobb paklit (left_size-tól felfelé),és
    berakom a balpakli lapjai közé -> left_size + 1, right_size -1 minden rotate() után. Egészen addig tart, amíg át nem
    érek a bal paklin (ahová rakom), vagy el nem fogy a jobb pakli (ahonnan rakom). Ideális esetben a bal pakli minden egyes
    lapja után bekerül egy lap a jobb pakliból. Ha marad a jobb pakliban, a maradék a balpakli tetejére kerül.
    Véletlenszerűen változhat: 
    *szétválasztás mennyire tér el a felétől
    *hányadik lap után rakunk be újat (where) ideális eset where = 2, azaz minden lap után
    *hányadik lapot tudjuk berakni (which)  ideális eset which = 1, azaz minden alkalommal egyet
    *hány lapot tudunk egyszerre berakni (mult): mult = 0, ha 1
    Repeat: műveletet hányszor ismételje?
    Efficiency: keverés hatékonysága 0-1 közötti érték

    */
    template <typename F>
    auto my_shuffle(int repeat, double const& efficiency, int row, F f)
    { 
        seq.resize(repeat);
        int deck_size = static_cast<int>(data.size());
        int counter = 0; 
         
        std::vector<double> weights = get_weights(efficiency); 
        
        while(counter < repeat){
            int left_size = separate_deck(-3, 3, f);        //lapok kettéválasztása +/- 3 lapos bizonytalansággal
            int right_size =   deck_size -  left_size; 
            int which = left_size;    //which card to place where
            int where = 1;

            while((where < left_size) && (which < deck_size))
                { 
                int r1,r2;
                if(static_cast<int>(weights.size()) > 1)
                {
                    std::discrete_distribution<> distr(weights.begin(), weights.end());
                    r1 = distr(f) + 1;      //mennyit rakunk be a jobb pakliból a bal oldaliba
                    r2 = distr(f) + 2;      //hány lapot ugrunk, ahova berakjuk az új lap(oka)t
                }else{
                    r1 = 1;
                    r2 = 2;
                }
                int which_to = r1+which;
                if(which_to > deck_size){which_to = deck_size; r1 = deck_size-which_to;}
                std::rotate(data.begin()+where, data.begin() + which, data.begin()+which_to); 
                which += r1;
                where += r2;
                left_size += r1;
                right_size -= r1;
                }
                //if cards are still  in the right half deck, put them on the top of the left deck
                if(right_size != 0)
                    {
                        std::rotate(data.begin(), data.end() - right_size, data.end());
                        right_size = 0;
                        left_size = deck_size;

                    }
                if(row > 0)
                {
                    seq[counter] = analyze(row);
                }
                counter++;
            }
        return *this;
            
    }
        int analyze(int number)
    {
        int counter = 1;        //count same coloured cards
        int row = 0;            //count sequences
        int card = 1;           //count cards to avoid neglecting the last row
        
        auto f = [&counter, &row, &number, &card](auto& x, auto& y){
        if(x.get_suit() == y.get_suit()){counter++;}
        else{
            if(counter == number){row ++;}
            counter = 1;}
        card++;
        if(card == 52 && counter == number){row ++;}
        return y;
        };

        std::accumulate(data.begin()+1, data.end(), data[0], f);
       
        return row;
        
    }

        bool four_kings()
    {
        int counter = 1;
        bool kings = false;
        auto f = [&counter](auto& x, auto& y){if(x.get_value() == 11 && y.get_value() == 11) {counter++;} return y;};
        std::accumulate(data.begin()+1, data.end(), data[0], f);
        if(counter == 4)
        {   
           kings = true;
        }
        
        return kings;
    }

};
    //streaming the whole deck
    std::ostream& operator<<(std::ostream& o, Deck& d)
    {
        for(int i = 0;i < d.size(); i++)
        {
            o << d[i] << std::endl;
        }
        return o;
    }
    //is there a row with 3 cards from the same color?



  






 







