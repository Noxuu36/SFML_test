#include "Monster.h"

Monster::Monster(float x, float y, int hp, float speed)
    : health(hp), maxHealth(hp), speed(speed), scale(2.f),
      state(MonsterState::Idle), attackRange(40.f), attackDamage(10),
      groundY(y), isFacingLeft(true), markedForRemoval(false),
      attackHitboxActive(false), hurtTimer(0.f)
{
    sprite.setPosition(x, y);

    hitbox.setSize(sf::Vector2f(32.f, 32.f));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1.f);

    attackHitbox.setSize(sf::Vector2f(30.f, 30.f));
    attackHitbox.setFillColor(sf::Color::Transparent);
    attackHitbox.setOutlineColor(sf::Color::Magenta);
    attackHitbox.setOutlineThickness(1.f);
}

void Monster::switchAnimation(const std::string& animName) {
    if (animations.find(animName) != animations.end() && currentAnimation != animName) {
        currentAnimation = animName;
        animations[animName]->reset();
        sprite.setTexture(animations[animName]->getTexture());

        int frameWidth = animations[animName]->getFrameWidth();
        int frameHeight = animations[animName]->getFrameHeight();
        sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    }
}

void Monster::takeDamage(int damage) {
    if (state == MonsterState::Dead) return;

    health -= damage;
    if (health <= 0) {
        health = 0;
        state = MonsterState::Dead;
        if (animations.find("death") != animations.end()) {
            switchAnimation("death");
        } else {
            markedForRemoval = true;
        }
    } else {
        state = MonsterState::Hurt;
        hurtTimer = 0.3f;
        if (animations.find("hurt") != animations.end()) {
            switchAnimation("hurt");
        }
    }
}

bool Monster::isDeathAnimationFinished() const {
    auto it = animations.find("death");
    if (it != animations.end()) {
        return it->second->isFinished();
    }
    return true;
}

void Monster::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(hitbox);
    if (attackHitboxActive) {
        window.draw(attackHitbox);
    }
}

sf::FloatRect Monster::getHitbox() const {
    return hitbox.getGlobalBounds();
}

sf::FloatRect Monster::getAttackHitbox() const {
    return attackHitbox.getGlobalBounds();
}
