#pragma once
#include "Monster.h"

// Rat Monster
class Rat : public Monster {
public:
    Rat(float x, float y);
    void update(float deltaTime, const sf::Vector2f& playerPos, bool playerAttacking, const sf::FloatRect& playerAttackBox) override;
};

// Bat Monster (Flying slightly above ground)
class Bat : public Monster {
public:
    Bat(float x, float y);
    void update(float deltaTime, const sf::Vector2f& playerPos, bool playerAttacking, const sf::FloatRect& playerAttackBox) override;
};

// Slime Monster
class Slime : public Monster {
public:
    Slime(float x, float y);
    void update(float deltaTime, const sf::Vector2f& playerPos, bool playerAttacking, const sf::FloatRect& playerAttackBox) override;
};

// Mimic Monster
class Mimic : public Monster {
public:
    Mimic(float x, float y);
    void update(float deltaTime, const sf::Vector2f& playerPos, bool playerAttacking, const sf::FloatRect& playerAttackBox) override;
};
