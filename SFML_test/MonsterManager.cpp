#include "MonsterManager.h"
#include "Monsters.h"
#include <cstdlib>
#include <algorithm>

MonsterManager::MonsterManager(float screenWidth, float groundY)
    : screenWidth(screenWidth), groundY(groundY), spawnTimer(0.f),
      spawnInterval(4.f), maxMonsters(6)
{
}

void MonsterManager::spawnRandomMonster() {
    if (monsters.size() >= maxMonsters) return;

    // Choisir aléatoirement le bord gauche ou droit de l'écran
    float spawnX = (std::rand() % 2 == 0) ? -50.f : screenWidth + 50.f;
    int monsterType = std::rand() % 4;

    switch (monsterType) {
        case 0:
            monsters.push_back(std::make_unique<Rat>(spawnX, groundY));
            break;
        case 1:
            monsters.push_back(std::make_unique<Bat>(spawnX, groundY));
            break;
        case 2:
            monsters.push_back(std::make_unique<Slime>(spawnX, groundY));
            break;
        case 3:
            monsters.push_back(std::make_unique<Mimic>(spawnX, groundY));
            break;
    }
}

void MonsterManager::update(float deltaTime, Player& player) {
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.f;
        spawnRandomMonster();
    }

    sf::FloatRect playerBounds = player.getHitbox();
    sf::Vector2f playerPos(playerBounds.left + playerBounds.width / 2.f, playerBounds.top + playerBounds.height / 2.f);
    bool playerAttacking = player.isAttackActive();
    sf::FloatRect playerAttackBox = player.getAttackHitbox();

    for (auto& monster : monsters) {
        monster->update(deltaTime, playerPos, playerAttacking, playerAttackBox);

        // Vérifier si le monstre attaque et touche le joueur
        if (monster->isAttacking() && monster->getAttackHitbox().intersects(playerBounds)) {
            player.takeDamage(1);
        } else if (monster->getHitbox().intersects(playerBounds) && !monster->isDead()) {
            // Degâts de contact basiques si le monstre rentre en collision directe
            player.takeDamage(1);
        }
    }

    // Retirer les monstres morts/marqués pour suppression
    monsters.erase(
        std::remove_if(monsters.begin(), monsters.end(),
            [](const std::unique_ptr<Monster>& m) {
                return m->isMarkedForRemoval();
            }),
        monsters.end());
}

void MonsterManager::draw(sf::RenderWindow& window) {
    for (auto& monster : monsters) {
        monster->draw(window);
    }
}
