// SEM 4 - RPPOOP - MINI PROJECT
// BY - Rohaan Advani(111903151) & Varun Taneja(111903158)

// PROGRAM : BUILT A BLACKJACK CARD GAME USING THE FOLLOWING OOP CONCEPTS :

// 1. CLASSES AND OBJECTS 
// 2. CONST FUNCTIONS 
// 3. FRIEND FUNCTIONS 
// 4. ACCESS SPECIFIERS
// 5. INHERITANCE
// 6. POLYMORPHISM 
// 7. CONSTRUCTORS
// 8. DESTRUCTORS
// PROGRAMMING LANGUAGE USED - C++

// LIST OF CLASSES IS AS FOLLOWS :

// 1. CARD 
// 2. HAND
// 3. GENERIC PLAYER
// 4. PLAYER
// 5. HOUSE 
// 6. DECK
// 7. GAME

// INHERITANCE HAS BEEN DONE AS FOLLOWS :

//          CLASS HAND
//          /        \
// CLASS DECK        CLASS GENRIC PLAYER
//                   /                 \
//        CLASS PLAYER                 CLASS HOUSE

// START OF PROGRAM

// STEP 1 :
// INCLUDE PREDEFINED LIBRARIES :
// (IOSTREAM & VECTOR LIBRARY INBUILT FUNCTIONS ARE USED IN THE PROGRAM EXTENSIVELY)

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>

using namespace std;

// STEP 2 :
// CLASS CARD : DEFINES A CARD : [SUIT / RANK / IF FACE UP]

class Card{

public:
	enum suit {CLUBS, DIAMONDS, SPADES, HEARTS};
	
	//Rank starts from 1
	enum rank {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
	
	//Card is the default constructor of the class it has a rank and suit (default Ace of Spades)
    Card(rank r = ACE, suit s = SPADES, bool ifu = true);
    
    //Use of const function value cannot be altered
    int GetValue() const;
    
    //Flip card used for House First card
    void Flip();
    
    //Use of friend function and output stream providing no: of functions for handling objects
	friend ostream& operator<< (ostream& os,const Card& aCard);
	
private:
    //suit & rank are user defined data types defined using enum
    suit m_Suit;
    
    rank m_Rank;
    
    bool m_IsFaceUp;
};

//Copy Constructor Defination stores values of cars in variables
Card::Card(rank r, suit s, bool p): m_Suit(s), m_Rank(r), m_IsFaceUp(p)
{}

//Function flips the card
void Card::Flip(){
    m_IsFaceUp = !(m_IsFaceUp);
}

//Operator Overloading
ostream& operator<< (ostream& os, const Card& aCard){
    
	const string RANKS[] = { "0", "A", "2", "3", "4", "5", "6", "7", "8", "9","10", "J", "Q", "K" };
    const string SUITS[] = { "c", "d", "h", "s" };

    if (aCard.m_IsFaceUp){
        //Defines a card based on Rank and Suit values
        os << RANKS[aCard.m_Rank] << SUITS[aCard.m_Suit];
    }
    else{
        //If card is unknown as its faced down
        os << "XX";
    }
    //returns card as string
    return os;
}

//Calculate Value of card
int Card::GetValue() const{
    
	int value = 0;
	if(m_IsFaceUp){
	    
		value = m_Rank;
		//If index is greater than 10 - All face cards = 10 as per Blackjack Rules
		if(value > 10) {
		    
			value = 10;
		}
	}
	   //returns numerical value of card
	   return value;
}

// STEP 3 :
// CLASS HAND : DEFINES CURRENT HAND OF A PLAYER OR HOUSE IN A GAME

class Hand{
    
protected:
    //Dynamic Array Can resize itself when elem is added or deleted
	vector<Card*> m_Cards;

public:
    //Constructor of Hand Class
    //reserve declares capacity of cards that can be dealt in hand
    //We put 7 here as max players is 7 for single deck game
	Hand(){
	    m_Cards.reserve(7);
	}
	
	//Pushback fn adds new element at end of vector
	void Add(Card* pCard){
	    
	    m_Cards.push_back(pCard);
	}
	
	void Clear(){   
	    
	    //begin fn returns bidirectional iterator to first element of the container
	    //iter is iterator to check same for next player 
		vector<Card*>::iterator iter = m_Cards.begin();
		//end fn gets iterator to pass last element 
		
		//All cards are iterated and cleared
		for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter){   
		    
		    //Freeing the pointer value 
			delete *iter;
			*iter = nullptr;
		}
		//Removes elements of vector which were destroyed leaving container with size 0 
    	m_Cards.clear();
	}

	//Calculates Total in that particular hand dealt 
	int GetTotal() const{
	    
	    //If hand is empty return 0 (No card Dealt)
		if(m_Cards.empty()) {
			return 0;
		}
		
		int total = 0;
		vector<Card*>::const_iterator iter;
		
		//Total up Hand in loop using iteration
		for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter){
		    //Get value function is called in order to add value of each card to total
			total += (*iter)->GetValue();
		}
		
		//total = sum of value of each card
		
		//Handle Ace Case:
		
		//Check if hand dealt contains Ace 
		bool containsAce = false;
		for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter) {
		    
			if ((*iter)->GetValue() == Card::ACE){
				containsAce = true;
			}
		}
		
		if (containsAce && total <= 11){
		    //If Hand includes Ace and Total is less than equal to 11 
		    //total is incremented by 10 as per Blackjack Rules
			total += 10;
		}
		//Return Total of player in hand dealt
    	return total;
	}
	
	//Virtual is a member fn in base class 
	//Destructor declared which clears the Hand after a round
	virtual ~Hand(){
	    
	    Clear();
	}
};

// STEP 4 :
// CLASS GENERIC PLAYER - DEFINES PLAYER OR HOUSE FUNCTIONS

//Extends class hand
class GenericPlayer : public Hand{
    
protected:
	string m_Name;

public:
    //Constructor for class takes name as input and displays
	GenericPlayer(const string& name = ""): m_Name(name)
	{};
	
	//Variable that decides what happens if user chooses hit
	virtual bool IsHitting() const = 0;
	
	//If total is greater than 21 then user/House is bust and thus looses the Game
	bool IsBusted() const{
	    return (GetTotal() > 21);
	}
	
	void Bust() const {
	    cout << m_Name << " busts." << endl;
	}
	
	//Destructor for class
	virtual ~GenericPlayer()
	{};
	
	//Friend Function 
	friend ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer);
};

//Function to print ongoing game 
ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer) {
    
    os << aGenericPlayer.m_Name << ":\t";
    vector<Card*>::const_iterator pCard;
    if (!aGenericPlayer.m_Cards.empty()) {
        
        for (pCard = aGenericPlayer.m_Cards.begin();pCard != aGenericPlayer.m_Cards.end();++pCard){
            
            os << *(*pCard) << "\t";
        }
        if (aGenericPlayer.GetTotal() != 0) {
            cout << "(" << aGenericPlayer.GetTotal() << ")";
        }
    }
    else {
        os << "<empty>";
    }
    return os;
}

// STEP 5 :
// CLASS PLAYER : EXTENDS FUNCTIONS OF GENERIC PLAYER TO DEFINE GAME FOR USER

//Extends class generic player
class Player : public GenericPlayer {

public:
    //Constructor for class
	Player(const string& name = ""):GenericPlayer(name)
	{}
	
	//Destructor for class
	virtual ~Player()
	{}
	
	//Returns values Y incase user Hits 
	virtual bool IsHitting() const {
	    
		cout << m_Name << ", do you want a HIT? (Y/N): ";
		char response;
		cin >> response;
		return (response == 'y' || response == 'Y');
	};
	
	//Declare if player wins
	void Win() const {
	    cout << m_Name << " wins.\n";
	};
	
	//Declare if player loses
	void Lose() const {
	    cout << m_Name << " loses.\n";
	};
	
	//Pushes is for tie
	void Push() const {
	    cout << m_Name << " pushes.\n";
	};
};

// STEP 6 :
// CLASS HOUSE : EXTENDS FUNCTIONS OF GENERIC PLAYER TO DEFINE GAME FOR AI

//extends class generic player
class House : public GenericPlayer {
    
public:
    //Constructor for class
	House(const string& name = "House"):GenericPlayer(name)
	{}
	
	//Destructor for class
    virtual ~House()
    {}
    
    //Check if house hits or not 
    //house cant hit if total >= 16 as per BlackJack Rules
    virtual bool IsHitting() const {
        return (GetTotal() <= 16);
    }
    
    //First card will remain unknown to user till user stays(stops hitting)
    void FlipFirstCard() {
        
		if (!(m_Cards.empty())) {
		    //Flip First(Unknown) Card
			m_Cards[0]->Flip();
		}
		else {
		    
			cout << "No card to flip!\n";
		}
	};
};

// STEP 7 :
// CLASS DECK - EXTENDS FUNCTIONS OF CLASS HAND TO DEFINE DECK AND PROVIDE RANDOM CARD TO PLAYERS

// extends class hand
class Deck : public Hand {
    
public:
    //Constructor for class
    Deck();
    
    //Destructor for class
    virtual ~Deck();
    
    //Builds Deck
    void Populate();
    
    //Uses random card
    void Shuffle();
    
    //Deals Hand
    void Deal(Hand& aHand);
    
    //Asks for more cards if required by player
    void AdditionalCards(GenericPlayer& aGenericPlayer);
};

Deck::Deck(){   
    // Max Cards = 52
    m_Cards.reserve(52);
    Populate();
}

Deck::~Deck()
{}

//Cards are added to the deck 
void Deck::Populate(){
    
    Clear();
	for (int s = Card::CLUBS; s <= Card::HEARTS; ++s){
	    
        for (int r = Card::ACE; r <= Card::KING; ++r){
            
	        Add(new Card(static_cast<Card::rank>(r),
                         static_cast<Card::suit>(s)));
        }
    }
}

//Shuffles Deck as in Random Card is always drawn from the populated deck
void Deck::Shuffle(){
    
    random_shuffle(m_Cards.begin(), m_Cards.end());
}

//Deal cards until available in deck for players still in game
void Deck::Deal(Hand& aHand){ // polymorphism from class Hand

    if (!m_Cards.empty()) {
        //Top card in deck popped and dealt to player
        aHand.Add(m_Cards.back());
        m_Cards.pop_back();
    }
    
    else{
        
        cout << "Out of cards. Unable to deal." << endl;
		exit(0);
    }
}

//If user Hits additional card is required
void Deck::AdditionalCards(GenericPlayer& aGenericPlayer){ // polymorphism from class generic player

    cout << endl;
    
    //if player not busted and is hitting he draws a card
    while (!(aGenericPlayer.IsBusted()) && aGenericPlayer.IsHitting()) {
        
        Deal(aGenericPlayer);             // polymorphism from class generic player
        cout << aGenericPlayer << endl;
        
        //Each time Hit is called Check if player is bust if yes bust player
        if (aGenericPlayer.IsBusted()) {
            //If IsBusted fn is True Player is busted and removed from current game
            aGenericPlayer.Bust();
        }
    }
}

// STEP 8 :
// CLASS GAME : FUNCTIONALITY DEFINES GAME WORKING

class Game{
    
public:
    //Constructor to class
    Game(const vector<string>& names);
    
    //Desructor for class
    ~Game();
    
    void Play();
    
 private:
    //Define players, house and game deck
    Deck m_Deck;
    
    House m_House;
    
    vector<Player> m_Players;
};

Game::Game(const vector<string>& names){
    
    vector<string>::const_iterator pName;
    
    for (pName = names.begin(); pName != names.end(); ++pName){   
        
        //New value is inserted to the vector
        m_Players.push_back(Player(*pName));
    }

    srand(static_cast<unsigned int>(time(0)));
}

Game::~Game()
{}

// Play fn WORKS AS FOLLOWS:
// 1. Populate and shuffle Deck
// 2. Deal 2 cards to all players and house
// 3. Make first card of house unknown
// 4. Display current hand 
// 5. Players can Hit or Stay 
// 6. If player bust remove from current hand 
// 7. After all players have played the house plays 
// 8. House draws card till its Total <= 16 
// 9. If House Busts all players not busted win 
// 10. Else totals of each player compared to house total to declare win/lose 

void Game::Play() {
    
    //Deck is populated
	m_Deck.Populate(); // removed from Game::Game
	
	//Deck is shuffled
    m_Deck.Shuffle();  // removed from Game::Game
    
    //All players and house are dealt min 2 cards
    vector<Player>::iterator pPlayer;
    
    for (int i = 0; i < 2; ++i){   
        //Deal card to all players
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer){
            
            m_Deck.Deal(*pPlayer); // polymorphism from class deck
        }
        
        //Deal card to house
        m_Deck.Deal(m_House);  // polymorphism from class deck
    }
    
    //Flip first card of house as its unknown initially
    //Card should show XX
    m_House.FlipFirstCard();
    
    //Print Data
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) {
        
        cout << *pPlayer << endl;
    }
    
    cout << m_House << endl;
    
    //Additional cards if required can be taken by each player
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) {
        
        m_Deck.AdditionalCards(*pPlayer);  // polymorphism from class deck
    }
    
    //Since players are done playing first card of house is now faced up
    m_House.FlipFirstCard();
    cout << endl << m_House;
    
    //After all players have played the house can take additional cards 
    m_Deck.AdditionalCards(m_House);  // polymorphism from class deck
    
    //If house busted all players who are not busted(still in vector) in game win
    if (m_House.IsBusted()) {
        
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) {
            
            if (!(pPlayer->IsBusted())) {
                
                pPlayer->Win();
            }
        }
    }
    
    //Else compare if player value is greater than house value and declare winner
    else {
        
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer){
            
            //IF PLAYER NOT BUSTED
            if (!(pPlayer->IsBusted())){
                //PLAYER WINS IF TOTAL > HOUSE TOTAL
                if (pPlayer->GetTotal() > m_House.GetTotal()){
                    
                    pPlayer->Win();
                }
                //PLAYER LOOSES IF TOTAL < HOUSE TOTAL
                else if (pPlayer->GetTotal() < m_House.GetTotal()){
                    
                    pPlayer->Lose();
                }
                //PLAYER PUSHES/TIES IF BOTH TOTALS EQUAL
                else{
                    
                    pPlayer->Push();
                }
            }
        }

    }
    //Clear all existing data in vector for next game
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer){
        
        pPlayer->Clear();
    }
    m_House.Clear();
}

// STEP 9 :
// MAIN - RUNS THE PROGRAM

int main(int argc,char** args){

    cout << "\t\tWelcome to Blackjack!\n\n";
    
    // THE FOLLOWING ARE THE RULES OF THE GAME
    cout << "Rules of Game:\n\n";
    
    cout << "1. Players play against the House not Eachother\n";
    cout << "2. Players & House will be dealt 2 cards to start with \n";
    cout << "3. Value of Ace = 1(If Total < 10) else 11\n";
    cout << "4. Value of Face Cards = 10\n";
    cout << "5. Value of other cards is the number itself\n";
    cout << "6. Total up your cards\n";
    cout << "7. Objective of game is to have a greater total than that of the house\n";
    cout << "8. Ask for a Hit if u want an additional card\n";
    cout << "9. If your total goes greater than 21 you are busted and thus loose game\n";
    cout << "10. After all players are done playing the house plays\n";
    cout << "11. The House draws cards till its Total <= 16\n";
    cout << "12. If player total > house total - Player Wins\n";
    cout << "13. If player total < house total - Player Looses\n";
    cout << "14. If player total = house total - Draw / Tie\n";

    int numPlayers = 0;
    
    //1 - 7 players can play at the table
    while (numPlayers < 1 || numPlayers > 7){
        
        cout << "\nEnter no: of players? (1 - 7): ";
        cin >> numPlayers;
    }

    vector<string> names;
    
    string name;
    
    for (int i = 0; i < numPlayers; ++i){
        
        cout << "Enter player name: ";
        cin >> name;
        names.push_back(name);
    }
    
    cout << endl;
    
    //Names are entered into the game and game begins
    Game aGame(names);
    
    char again = 'y';
    
    //Game goes on until again is n or N
    while (again != 'n' && again != 'N') {
        
        aGame.Play();
        
        cout << "\nDo you want to play again? (Y/N): ";
        cin >> again;
    }
    
    return 0;
}

// END OF PROGRAM