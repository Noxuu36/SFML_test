#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <string>
#include "Animation.h"

enum class MonsterState {
    Idle,
    Walk,
    Attack,
    Hurt,
    Dead
};

class Monster {
public:
    Monster(float x, float y, int hp, float speed);
    virtual ~Monster() = default;

    virtual void update(float deltaTime, const sf::Vector2f& playerPos, bool playerAttacking, const sf::FloatRect& playerAttackBox) = 0;
    virtual void draw(sf::RenderWindow& window);

    void takeDamage(int damage);
    bool isDead() const { return state == MonsterState::Dead && isDeathAnimationFinished(); }
    bool isMarkedForRemoval() const { return markedForRemoval; }

    sf::FloatRect getHitbox() const;
    sf::FloatRect getAttackHitbox() const;
    bool isAttacking() const { return state == MonsterState::Attack && attackHitboxActive; }
    int getAttackDamage() const { return attackDamage; }

protected:
    void switchAnimation(const std::string& animName);
    bool isDeathAnimationFinished() const;

    sf::Sprite sprite;
    std::map<std::string, std::unique_ptr<Animation>> animations;
    std::string currentAnimation;

    sf::RectangleShape hitbox;
    sf::RectangleShape attackHitbox;
    bool attackHitboxActive;

    MonsterState state;
    int health;
    int maxHealth;
    float speed;
    float scale;
    float attackRange;
    int attackDamage;
    float groundY;

    bool isFacingLeft;
    bool markedForRemoval;
    float hurtTimer;
};
