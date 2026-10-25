#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Monster.h"
#include "Player.h"

class MonsterManager {
public:
    MonsterManager(float screenWidth, float groundY);

    void update(float deltaTime, Player& player);
    void draw(sf::RenderWindow& window);

private:
    void spawnRandomMonster();

    std::vector<std::unique_ptr<Monster>> monsters;
    float spawnTimer;
    float spawnInterval;
    std::size_t maxMonsters;
    float screenWidth;
    float groundY;
};
