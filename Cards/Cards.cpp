#include "Cards.h"
#include <experimental/random>

using namespace Cards;

// ======================== Deck class ========================

Deck::Deck() = default;

Deck::Deck(const Deck &deck) {
    for (Card* card : cards) {
        Card* c = card->clone(); // create a deep copy of card objects
        this->cards.push_back(c);
    }
}

Deck &Deck::operator=(const Deck &deck) {
    // Check if the current object and the passed object is the same
    if (this == &deck) return *this;

    // Deallocate all the memory associated with this Deck of cards
    for (const Card* card : this->cards) {
        delete card;
        card = nullptr;
    }
    this->cards.clear();

    for (Card* card : deck.cards) {
        Card* c = card->clone(); // create a deep copy of card objects
        this->cards.push_back(c);
    }
    return *this;
}

std::ostream& operator<<(std::ostream &out, const Deck &deck) {
    for (const Card* c : deck.cards) {
        out << *c;
    }
    return out;
}

Card* Deck::draw() {
    // randomly draw a card from the deck, remove it from the deck and return a pointer to that card
    int nbrOfCards = this->cards.size()-1;
    int random = std::experimental::randint(0, nbrOfCards);
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

// ======================== Hand class ========================

Hand::Hand() = default;

Hand::Hand(const Hand &hand) {
    for (Card* card : hand.cards) {
        Card* c = card->clone(); // create a deep copy of card objects
        this->cards.push_back(c);
    }
}

Hand &Hand::operator=(const Hand &hand) {
    if (this ==  &hand) return *this;

    // Deallocate all the memory associated with this Hand of cards
    for (const Card* card : this->cards) {
        delete card;
        card = nullptr;
    }
    this->cards.clear();
    for (Card* card : hand.cards) {
        Card* c = card->clone(); // create a deep copy of card objects
        this->cards.push_back(c);
    }
    return *this;
}

std::ostream &operator<<(std::ostream &out, const Hand &hand) {
    for (const Card* c : hand.cards) {
        out << *c << std::endl;
    }
    return out;
}

Hand::~Hand() {
    for (const Card* card : this->cards) {
        delete card;
        card = nullptr;
    }
}

// ======================== Card class ========================

std::ostream& Cards::operator<<(std::ostream &out, const Card &card) {
    out << "Card type: " << card.getType() << std::endl;
    return out;
}

Card::~Card() = default;

// ======================== Bomb class ========================

Bomb::Bomb() {
    this->type = "bomb";
}

Bomb::Bomb(const Bomb &bomb) {
    this->type = bomb.type;
}

Bomb& Bomb::operator=(const Bomb &bomb) {
    if (this == &bomb) return *this;
    this->type = bomb.type;
    return *this;
}

std::string Bomb::getType() const {
    return this->type;
}

Card* Bomb::clone() {
    return new Bomb(*this);
}

void Bomb::play(Player &player, Deck &deck) {
//    Order* order = new Bomb();

}

Bomb::~Bomb() = default;


// ======================== Reinforcement class ========================

Reinforcement::Reinforcement() {
    this->type = "reinforcement";
}

Reinforcement::Reinforcement(const Reinforcement &r) {
    this->type = r.type;
}

Reinforcement &Reinforcement::operator=(const Reinforcement &r) {
    if (this == &r) return *this;
    this->type = r.type;
    return *this;
}

std::string Reinforcement::getType() const {
    return this->type;
}

void Reinforcement::play(Player &player, Deck &deck) {

}

Card *Reinforcement::clone() {
    return new Reinforcement(*this);
}

Reinforcement::~Reinforcement() = default;

// ======================== Blockade class ========================

Blockade::Blockade() {
    this->type = "blockade";
}

Blockade::Blockade(const Blockade &blockade) {
    this->type = blockade.type;
}

Blockade &Blockade::operator=(const Blockade &blockade) {
    if (this == &blockade) return *this;
    this->type = blockade.type;
    return *this;
}

std::string Blockade::getType() const {
    return this->type;
}

void Blockade::play(Player &player, Deck &deck) {

}

Card *Blockade::clone() {
    return new Blockade(*this);
}

Blockade::~Blockade() = default;

// ======================== Airlift class ========================

Airlift::Airlift() {
    this->type = "airlift";
}

Airlift::Airlift(const Airlift &airlift) {
    this->type = airlift.type;
}

Airlift &Airlift::operator=(const Airlift &airlift) {
    if (this == &airlift) return *this;
    this->type = airlift.type;
    return *this;
}

std::string Airlift::getType() const {
    return this->type;
}

void Airlift::play(Player &player, Deck &deck) {

}

Card *Airlift::clone() {
    return new Airlift(*this);
}

Airlift::~Airlift() = default;

// ======================== Diplomacy class ========================

Diplomacy::Diplomacy() {
    this->type = "diplomacy";
}

Diplomacy::Diplomacy(const Diplomacy &diplomacy) {
    this->type = diplomacy.type;
}

Diplomacy &Diplomacy::operator=(const Diplomacy &diplomacy) {
    if (this == &diplomacy) return *this;
    this->type = diplomacy.type;
    return *this;
}

std::string Diplomacy::getType() const {
    return this->type;
}

void Diplomacy::play(Player &player, Deck &deck) {

}

Card *Diplomacy::clone() {
    return new Diplomacy(*this);
}

Diplomacy::~Diplomacy() = default;

