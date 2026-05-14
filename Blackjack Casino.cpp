#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

class Card {
public:
    string suit;
    string rank;
    int value;

    Card(string s, string r, int v) {
        suit = s;
        rank = r;
        value = v;
    }

    void display() {
        cout << rank << " of " << suit;
    }
};

class Deck {
private:
    vector<Card> cards;

public:
    Deck() {
        string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
        string ranks[] = {
            "2", "3", "4", "5", "6", "7",
            "8", "9", "10", "Jack", "Queen", "King", "Ace"
        };

        int values[] = {
            2,3,4,5,6,7,8,9,10,10,10,10,11
        };

        for (string suit : suits) {
            for (int i = 0; i < 13; i++) {
                cards.push_back(Card(suit, ranks[i], values[i]));
            }
        }
    }

    void shuffleDeck() {
        random_shuffle(cards.begin(), cards.end());
    }

    Card dealCard() {
        Card top = cards.back();
        cards.pop_back();
        return top;
    }
};

class Player {
public:
    vector<Card> hand;

    void addCard(Card c) {
        hand.push_back(c);
    }

    int calculateScore() {
        int total = 0;
        int aceCount = 0;

        for (auto &card : hand) {
            total += card.value;

            if (card.rank == "Ace")
                aceCount++;
        }

        while (total > 21 && aceCount > 0) {
            total -= 10;
            aceCount--;
        }

        return total;
    }

    void showHand(bool hideFirst = false) {

        for (int i = 0; i < hand.size(); i++) {

            if (i == 0 && hideFirst) {
                cout << "[Hidden Card]\n";
            }
            else {
                hand[i].display();
                cout << endl;
            }
        }
    }
};

int main() {

    srand(time(0));

    cout << "===================================\n";
    cout << "         BLACKJACK CASINO\n";
    cout << "===================================\n";

    char playAgain = 'y';

    while (playAgain == 'y' || playAgain == 'Y') {

        Deck deck;
        deck.shuffleDeck();

        Player player;
        Player dealer;

        player.addCard(deck.dealCard());
        player.addCard(deck.dealCard());

        dealer.addCard(deck.dealCard());
        dealer.addCard(deck.dealCard());

        bool playerBust = false;

        while (true) {

            cout << "\n========== YOUR HAND ==========\n";
            player.showHand();

            cout << "Your Score: " << player.calculateScore() << endl;

            cout << "\n========== DEALER HAND ==========\n";
            dealer.showHand(true);

            cout << "\n1. Hit";
            cout << "\n2. Stand";

            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;

            if (choice == 1) {

                player.addCard(deck.dealCard());

                if (player.calculateScore() > 21) {
                    playerBust = true;
                    break;
                }
            }
            else {
                break;
            }
        }

        if (!playerBust) {

            while (dealer.calculateScore() < 17) {
                dealer.addCard(deck.dealCard());
            }
        }

        cout << "\n===================================\n";
        cout << "          FINAL RESULTS\n";
        cout << "===================================\n";

        cout << "\nYour Cards:\n";
        player.showHand();

        cout << "Your Score: " << player.calculateScore() << endl;

        cout << "\nDealer Cards:\n";
        dealer.showHand();

        cout << "Dealer Score: " << dealer.calculateScore() << endl;

        int playerScore = player.calculateScore();
        int dealerScore = dealer.calculateScore();

        cout << "\n========== RESULT ==========\n";

        if (playerBust) {
            cout << "You Busted! Dealer Wins.\n";
        }
        else if (dealerScore > 21) {
            cout << "Dealer Busted! You Win!\n";
        }
        else if (playerScore > dealerScore) {
            cout << "You Win!\n";
        }
        else if (dealerScore > playerScore) {
            cout << "Dealer Wins!\n";
        }
        else {
            cout << "It's a Draw!\n";
        }

        cout << "\nPlay Again? (y/n): ";
        cin >> playAgain;
    }

    cout << "\nThanks for playing!\n";

    return 0;
}
