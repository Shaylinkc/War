#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <random>
#include <ctime>
#include <windows.h>
#include "bigProject.h"

using namespace std;

string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
int difficulty=0;

//creates the game deck
vector<pair<int, string>> createDeck() {
    vector<pair<int, string>> deck;

    for (int i = 0; i < 4; i++) {
        for (int j = 1; j <= 13; j++) {
            deck.push_back(make_pair(j, suits[i]));
        }
    }

    // Declare and initialize random number generator
    random_device rd;
    mt19937 g(rd());

    shuffle(deck.begin(), deck.end(), g);

    return deck;
}

//deals cards to the hands
void dealCards(vector<pair<int, string>>& deck, vector<pair<int, string>>& hand, int numCards) {
    for (int i = 0; i < numCards; i++) {
        hand.push_back(deck.back());
        deck.pop_back();
    }
}

//prints a card
void printCard(int num, string suit){
    if (num == 1 || num == 15){
            cout << "Ace of " << suit << endl;
        }
    else if (num == 11){
            cout << "Jack of " << suit << endl;
        }   
    else if (num == 12){
            cout << "Queen of " << suit << endl;
        } 
    else if (num == 13){
            cout << "King of " << suit << endl;
        }
    else{
            cout << num << " of " << suit << endl;
    }
}

//prints multiple cards
void printCards(vector<pair<int,string>> hand){
    for (int i = 0; i < hand.size(); i++){
        printCard(hand[i].first, hand[i].second);
    }
}

//Player game loop
int playCards(vector<pair<int, string>> playerHand){
    int card;
    cout<<"Your hand: "<<endl;
    while(true){   
        for (int i = 0; i < playerHand.size(); i++) {
            cout<<i+1<<". ";
            printCard(playerHand[i].first,playerHand[i].second);
        }
        cout<< "Which card would you like to play: ";
        cin>>card;

        if (card>playerHand.size() || card<1){
            cout<<"Invalid choice, please try again."<<endl<<endl;
            continue;
        }else{
            break;
        }
    }
    

    return card-1;
}

//AI Game Loop

int aiTurn(vector<pair<int, string>> aiHand, int difficulty){
    int biggest, rand3;
    vector<int> hand;
    for (int i = 0; i < aiHand.size(); i++){
        hand.push_back(aiHand[i].first);
    } 
    
    //1 is easy, 2 is medium, 3 is hard
    switch(difficulty){
        case 1:
            //random choice
            biggest = rand() % aiHand.size();
            break;

        case 2:
            //gets top 3 cards and randomly chooses one
            for(int i = 0; i < hand.size(); i++){
                aceCheck(aiHand, i);
            }
            
            sort(hand.begin(), hand.end());

            /*cout<<"After sorting: "<<endl;
            for (int i = 0; i < hand.size(); i++){
                cout<<hand[i]<<endl;
            }*/

            //gets a random number from 2 to 4
            rand3= rand() % 3+2;

            //uses the card picked from hand to get the index of the same card in aiHand
            for (int i = 0; i < hand.size(); i++){
                if (aiHand[i].first == hand[rand3]){
                    biggest = i;
                    break;
                }
            }
            break;

        case 3:
            biggest = distance(hand.begin(), max_element(hand.begin(), hand.end()));
            break;

        default:
            cout<<"Invalid difficulty, please try again."<<endl<<endl;
            break;
    }
    
    return biggest;
}

void aceCheck(vector<pair<int, string>> &hand, int choice) 
{
    if (hand[choice].first == 1)
    {
        hand[choice].first = 15;
    }
}

int main(){
    bool gameGoing = true;
    vector<pair<int, string>> deck = createDeck();
    vector<pair<int, string>> playerDeck, aiDeck;
    dealCards(deck, playerDeck, 26);
    dealCards(deck, aiDeck, 26);
    vector<pair<int, string>> playerHand, aiHand;

    //initializes AI difficulty

    cout<<"Welcome to 5 Card War!"<<endl;
    cout<<"You will be playing against the computer."<<endl;
    cout<<"The game is simple, you and the computer will each draw 5 cards."<<endl;
    cout<<"You will then choose a card to play, and the computer will do the same."<<endl;
    cout<<"The player with the highest card wins the round, and the whole hand are added to the winners deck."<<endl;
    cout<<"The game ends when one player has no more cards."<<endl;
    cout<<"Good luck!"<<endl<<endl;
    while(true){
        cout<<"Please choose a difficulty level: "<<endl;
        cout<<"1. Easy"<<endl;
        cout<<"2. Medium"<<endl;
        cout<<"3. Hard"<<endl;
        cin>>difficulty;
        if(difficulty>=1 && difficulty<=3){
            break;
        }
        else{
            cout<<"Invalid choice, please try again."<<endl<<endl;
        }
    }


    for(int i = 0; i < 5; i++){
        cout<<endl;
    }

    while(gameGoing) {
        dealCards(playerDeck, playerHand, 5);
        dealCards(aiDeck, aiHand, 5);
        int aiChoice=aiTurn(aiHand, difficulty);
        cout<<endl<<"Player deck size: "<<playerDeck.size()<< " Computer deck size: "<<aiDeck.size()<<endl;
        int playerChoice=playCards(playerHand);
        cout<<endl;
        cout<<"Player chose: ";
        printCard(playerHand[playerChoice].first, playerHand[playerChoice].second);

        cout<<"Computer chose: ";
        printCard(aiHand[aiChoice].first, aiHand[aiChoice].second);

        aceCheck(aiHand, aiChoice);
        aceCheck(playerHand, playerChoice);

        if (aiHand[aiChoice].first == playerHand[playerChoice].first) {
            cout << "It's a tie! No one wins this round!" << endl << endl;
            // Add all cards back to their respective decks
            playerDeck.insert(playerDeck.end(), playerHand.begin(), playerHand.end());
            aiDeck.insert(aiDeck.end(), aiHand.begin(), aiHand.end());
        } else if (aiHand[aiChoice].first >= playerHand[playerChoice].first) {
            cout << "Computer won round!" << endl << endl;
            // Add all cards to the AI's deck
            if (playerDeck.size()==1){
                break;
            }
            aiDeck.insert(aiDeck.end(), playerHand.begin(), playerHand.end());
            aiDeck.insert(aiDeck.end(), aiHand.begin(), aiHand.end());
        } else {
            cout << "Player won this round!" << endl << endl;
            // Add all cards to the player's deck
            if (aiDeck.size()==1){
                break;
            }    
            playerDeck.insert(playerDeck.end(), playerHand.begin(), playerHand.end());
            playerDeck.insert(playerDeck.end(), aiHand.begin(), aiHand.end());
        }
        
        playerHand.clear();
        aiHand.clear();

        if (playerDeck.size()<=0 || aiDeck.size()<=0){
            gameGoing = false;
        }

    }

    if (playerDeck.size()==0){
        cout<<"Computer wins! Better luck next time!"<<endl;
    } else{
        cout<<"Player wins! You're better than a 'puter!"<<endl;
    }
    return 0;
}
