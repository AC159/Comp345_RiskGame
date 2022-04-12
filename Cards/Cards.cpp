#include <experimental/random>
#include "Cards.h"
#include "../Player/Player.h"

using namespace Cards;

// ======================== Deck class ========================

Deck::Deck() = default;

Deck::Deck(const Deck &deck) {
    for (Card *card: cards) {
        Card *c = card->clone(); // create a deep copy of card objects
        this->cards.push_back(c);
    }
}

Deck &Deck::operator=(const Deck &deck) {
    // Check if the current object and the passed object is the same
    if (this == &deck) return *this;

    // Deallocate all the memory associated with this Deck of cards
    for (const Card *card: this->cards) {
        delete card;
    }
    this->cards.clear();

    for (Card *card: deck.cards) {
        Card *c = card->clone(); // create a deep copy of card objects
        this->cards.push_back(c);
    }
    return *this;
}

std::ostream &Cards::operator<<(std::ostream &out, const Deck &deck) {
    for (const Card *c: deck.cards) {
        out << *c;
    }
    return out;
}

Card *Deck::draw() {
    // re-fill deck if it is empty
    if (this->cards.empty()) {
        this->fillDeckWithCards();
    }

    // randomly draw a card from the deck, remove it from the deck and return a pointer to that card
    int lastCardIndex = this->cards.size() - 1;
    int randomCardIndex = std::experimental::randint(0, lastCardIndex);
    Card *c = this->cards.at(randomCardIndex);
    this->cards.erase(this->cards.begin() + randomCardIndex);
    return c;
}

void Deck::fillDeckWithCards() {
    std::string cardTypes[5] = {"diplomacy", "airlift", "blockade", "reinforcement", "bomb"};
    for (int i = 0; i < 50; i++) {
        int random = std::experimental::randint(0, 4);
        Card *card;
        if (cardTypes[random] == "diplomacy") card = new Diplomacy();
        else if (cardTypes[random] == "airlift") card = new Airlift();
        else if (cardTypes[random] == "blockade") card = new Blockade();
        else if (cardTypes[random] == "reinforcement") card = new Reinforcement();
        else card = new Bomb();
        this->cards.push_back(card);
    }
}

Deck::~Deck() {
    for (const Card *card: cards) {
        delete card;
    }
}

// ======================== Hand class ========================

Hand::Hand() = default;

Hand::Hand(const Hand &hand) {
    for (Card *card: hand.cards) {
        Card *c = card->clone(); // create a deep copy of card objects
        this->cards.push_back(c);
    }
}

Hand &Hand::operator=(const Hand &hand) {
    if (this == &hand) return *this;

    // Deallocate all the memory associated with this Hand of cards
    for (const Card *card: this->cards) {
        delete card;
    }
    this->cards.clear();
    for (Card *card: hand.cards) {
        Card *c = card->clone(); // create a deep copy of card objects
        this->cards.push_back(c);
    }
    return *this;
}

std::ostream &Cards::operator<<(std::ostream &out, const Hand &hand) {
    for (const Card *c: hand.cards) {
        out << *c << std::endl;
    }
    return out;
}

Hand::~Hand() {
    for (const Card *card: this->cards) {
        delete card;
    }
}

// ======================== Card class ========================

std::ostream &Cards::operator<<(std::ostream &out, const Card &card) {
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

Bomb &Bomb::operator=(const Bomb &bomb) {
    if (this == &bomb) return *this;
    this->type = bomb.type;
    return *this;
}

std::string Bomb::getType() const {
    return this->type;
}

Card *Bomb::clone() {
    return new Bomb(*this);
}

void Bomb::play(Players::Player *player, Deck *deck, Graph::Territory *target, Graph::Map *map) {
    // issue and order based on the card type and remove that card from the player's hand of cards and put it back into the deck
    auto *bomb = new Orders::Bomb(player, target, map);
    player->orders->add(bomb);
    std::cout << " (issued by playing a bomb card)" << std::endl;
    auto it = std::find(player->hand->cards.begin(), player->hand->cards.end(),
                        this); // find the current card inside the player's hand
    deck->cards.push_back(*it);
    if (it == player->hand->cards.end()) player->hand->cards.erase(it - 1);
    else player->hand->cards.erase(it);
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

void Reinforcement::play(Players::Player *player, Deck *deck, Graph::Territory *target) {
    // issue and order based on the card type and remove that card from the player's hand of cards and put it back into the deck


    player->reinforcementPool = player->reinforcementPool + 5;
    player->orders->add(new Orders::Deploy(player, target, 5, true));
    std::cout << " (issued by playing a reinforcement card)\n";

    auto it = std::find(player->hand->cards.begin(), player->hand->cards.end(),
                        this); // find the current card inside the player's hand
    deck->cards.push_back(*it);
    if (it == player->hand->cards.end()) player->hand->cards.erase(it - 1);
    else player->hand->cards.erase(it);
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

void Blockade::play(Players::Player *player, Deck *deck, Graph::Territory *target,
                    std::vector<Players::Player *> &players) {
    // issue a blockade order and remove that card from the player's hand of cards and put it back into the deck
    auto *blockade = new Orders::Blockade(player, target, players);
    player->orders->add(blockade);
    std::cout << " (issued by playing a blockade card)" << std::endl;
    auto it = std::find(player->hand->cards.begin(), player->hand->cards.end(),
                        this); // find the current card inside the player's hand
    deck->cards.push_back(*it);
    if (it == player->hand->cards.end()) player->hand->cards.erase(it - 1);
    else player->hand->cards.erase(it);
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

void
Airlift::play(Players::Player *player, Deck *deck, Graph::Territory *source, Graph::Territory *target, int armies) {
    // issue and order based on the card type and remove that card from the player's hand of cards and put it back into the deck
    auto *airLift = new Orders::Airlift(player, source, target, armies);
    player->orders->add(airLift);
    std::cout << " (issued by playing an airlift card)" << std::endl;
    auto it = std::find(player->hand->cards.begin(), player->hand->cards.end(),
                        this); // find the current card inside the player's hand
    deck->cards.push_back(*it);
    if (it == player->hand->cards.end()) player->hand->cards.erase(it - 1);
    else player->hand->cards.erase(it);
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

void Diplomacy::play(Players::Player *player, Players::Player *target, Deck *deck) {
    // issue and order based on the card type and remove that card from the player's hand of cards and put it back into the deck
    auto *negotiate = new Orders::Negotiate(player, target);
    player->orders->add(negotiate);
    std::cout << " (issued by playing a diplomacy card)" << std::endl;
    auto it = std::find(player->hand->cards.begin(), player->hand->cards.end(),
                        this); // find the current card inside the player's hand
    deck->cards.push_back(*it);
    if (it == player->hand->cards.end()) player->hand->cards.erase(it - 1);
    else player->hand->cards.erase(it);
}

Card *Diplomacy::clone() {
    return new Diplomacy(*this);
}

Diplomacy::~Diplomacy() = default;

