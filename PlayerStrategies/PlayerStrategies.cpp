#include "PlayerStrategies.h"


PlayerStrategies::PlayerStrategies(Players::Player *p) {
    this->player = p;
}

PlayerStrategies::PlayerStrategies(const PlayerStrategies &ps) {
    this->player = ps.player;
}

PlayerStrategies::~PlayerStrategies() = default;

Benevolent::Benevolent(Players::Player *p) : PlayerStrategies(p) {}

Benevolent::Benevolent(const Benevolent &b) : PlayerStrategies(b.player) {}

Benevolent& Benevolent::operator=(const Benevolent &b) {
    this->player = b.player;
}

std::ostream &operator<<(std::ostream &out, const Benevolent &b) {
    out << "Player " << b.player->getName() << " is playing the Benevolent strategy" << std::endl;
    return out;
}

Benevolent::~Benevolent() = default;
