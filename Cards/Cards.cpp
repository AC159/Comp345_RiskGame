#include "Cards.h"
#include <experimental/random>


Deck::Deck() = default;

Deck::Deck(const Deck &deck) {
    for (const Card* card : cards) {
        Card* c = new Card(*c); // create a deep copy of card objects
        this->cards.push_back(c);
    }
}

Deck &Deck::operator=(const Deck &deck) {
    // Check if the current object and the passed object is the same
    if (this == &deck) return *this;

    // Deallocate all the memory associated with this Deck of cards
    for (const Card* card : cards) {
        delete card;
        card = nullptr;
    }
    this->cards.clear();

    for (const Card* card : deck.cards) {
        Card* c = new Card(*c); // create a deep copy of card objects
        this->cards.push_back(c);
    }
    return *this;
}

ostream& operator<<(ostream &out, const Deck &deck) {
    for (const Card* c : deck.cards) {
        out << *c << endl;
    }
    return out;
}

Card* Deck::draw() {
    // randomly draw a card from the deck, remove it from the deck and return a pointer to that card
    int nbrOfCards = this->cards.size()-1;
    int random = experimental::randint(0, nbrOfCards);
    Card* c =this->cards.at(random);
    this->cards.erase(this->cards.begin() + random);
    return c;
}

Deck::~Deck() {
    for (const Card* card : cards) {
        delete card;
        card = nullptr;
    }
}
