#include "Monsters.h"
#include <cmath>

// -------------------------------------------------------------
// RAT
// -------------------------------------------------------------
Rat::Rat(float x, float y)
    : Monster(x, y, 30, 100.f)
{
    scale = 3.f;
    attackRange = 35.f;

    animations["idle"] = std::make_unique<Animation>("assets/Monsters/Rat/idle.png", 70, 70, 10, 0.1f, true);
    animations["walk"] = std::make_unique<Animation>("assets/Monsters/Rat/run.png", 70, 70, 8, 0.1f, true);
    animations["attack"] = std::make_unique<Animation>("assets/Monsters/Rat/attack_bite.png", 70, 70, 12, 0.08f, false);
    animations["hurt"] = std::make_unique<Animation>("assets/Monsters/Rat/hurt.png", 70, 70, 3, 0.1f, false);
    animations["death"] = std::make_unique<Animation>("assets/Monsters/Rat/rat-death.png", 70, 70, 6, 0.12f, false);

    currentAnimation = "idle";
    sprite.setTexture(animations[currentAnimation]->getTexture());
    sprite.setScale(scale, scale);

    hitbox.setSize(sf::Vector2f(32.f, 24.f));
    attackHitbox.setSize(sf::Vector2f(30.f, 24.f));
}

void Rat::update(float deltaTime, const sf::Vector2f& playerPos, bool playerAttacking, const sf::FloatRect& playerAttackBox) {
    if (playerAttacking && getHitbox().intersects(playerAttackBox) && state != MonsterState::Hurt && state != MonsterState::Dead) {
        takeDamage(20);
    }

    if (state == MonsterState::Dead) {
        animations[currentAnimation]->update(deltaTime);
        sprite.setTextureRect(animations[currentAnimation]->getCurrentFrame());
        if (isDeathAnimationFinished()) markedForRemoval = true;
        return;
    }

    if (state == MonsterState::Hurt) {
        hurtTimer -= deltaTime;
        animations[currentAnimation]->update(deltaTime);
        sprite.setTextureRect(animations[currentAnimation]->getCurrentFrame());
        if (hurtTimer <= 0.f || animations["hurt"]->isFinished()) {
            state = MonsterState::Idle;
        }
        return;
    }

    sf::Vector2f pos = sprite.getPosition();
    float dx = playerPos.x - pos.x;
    float dist = std::abs(dx);

    isFacingLeft = (dx < 0);
    sprite.setScale(isFacingLeft ? -scale : scale, scale);

    if (dist <= attackRange) {
        if (state != MonsterState::Attack) {
            state = MonsterState::Attack;
            switchAnimation("attack");
        }
    } else if (state != MonsterState::Attack) {
        state = MonsterState::Walk;
        switchAnimation("walk");
        float moveDir = (dx > 0) ? 1.f : -1.f;
        sprite.move(moveDir * speed * deltaTime, 0.f);
    }

    if (state == MonsterState::Attack) {
        int curFrame = animations["attack"]->getCurrentFrameIndex();
        attackHitboxActive = (curFrame >= 4 && curFrame <= 8);
        if (animations["attack"]->isFinished()) {
            state = MonsterState::Idle;
            attackHitboxActive = false;
            switchAnimation("idle");
        }
    } else {
        attackHitboxActive = false;
    }

    animations[currentAnimation]->update(deltaTime);
    sprite.setTextureRect(animations[currentAnimation]->getCurrentFrame());

    pos = sprite.getPosition();
    hitbox.setPosition(pos.x - hitbox.getSize().x / 2.f, pos.y - hitbox.getSize().y / 2.f + 5.f);

    float attOffset = isFacingLeft ? -attackHitbox.getSize().x : 0.f;
    attackHitbox.setPosition(pos.x + (isFacingLeft ? -10.f : 10.f) + attOffset, pos.y - attackHitbox.getSize().y / 2.f + 5.f);
}

// -------------------------------------------------------------
// BAT (Flying)
// -------------------------------------------------------------
Bat::Bat(float x, float y)
    : Monster(x, y - 100.f, 20, 130.f)
{
    scale = 2.5f;
    attackRange = 40.f;

    animations["idle"] = std::make_unique<Animation>("assets/Monsters/Bat/fly.png", 87, 87, 11, 0.08f, true);
    animations["walk"] = std::make_unique<Animation>("assets/Monsters/Bat/fly.png", 87, 87, 11, 0.08f, true);
    animations["attack"] = std::make_unique<Animation>("assets/Monsters/Bat/attack.png", 87, 87, 11, 0.08f, false);
    animations["hurt"] = std::make_unique<Animation>("assets/Monsters/Bat/hurt.png", 87, 87, 3, 0.1f, false);
    animations["death"] = std::make_unique<Animation>("assets/Monsters/Bat/death.png", 87, 87, 4, 0.12f, false);

    currentAnimation = "idle";
    sprite.setTexture(animations[currentAnimation]->getTexture());
    sprite.setScale(scale, scale);

    hitbox.setSize(sf::Vector2f(30.f, 30.f));
    attackHitbox.setSize(sf::Vector2f(35.f, 35.f));
}

void Bat::update(float deltaTime, const sf::Vector2f& playerPos, bool playerAttacking, const sf::FloatRect& playerAttackBox) {
    if (playerAttacking && getHitbox().intersects(playerAttackBox) && state != MonsterState::Hurt && state != MonsterState::Dead) {
        takeDamage(20);
    }

    if (state == MonsterState::Dead) {
        animations[currentAnimation]->update(deltaTime);
        sprite.setTextureRect(animations[currentAnimation]->getCurrentFrame());
        if (isDeathAnimationFinished()) markedForRemoval = true;
        return;
    }

    if (state == MonsterState::Hurt) {
        hurtTimer -= deltaTime;
        animations[currentAnimation]->update(deltaTime);
        sprite.setTextureRect(animations[currentAnimation]->getCurrentFrame());
        if (hurtTimer <= 0.f || animations["hurt"]->isFinished()) {
            state = MonsterState::Idle;
        }
        return;
    }

    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f targetPos = playerPos;
    targetPos.y -= 40.f;

    sf::Vector2f dir = targetPos - pos;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    isFacingLeft = (dir.x < 0);
    sprite.setScale(isFacingLeft ? -scale : scale, scale);

    if (dist <= attackRange) {
        if (state != MonsterState::Attack) {
            state = MonsterState::Attack;
            switchAnimation("attack");
        }
    } else if (state != MonsterState::Attack) {
        state = MonsterState::Walk;
        switchAnimation("walk");
        if (dist > 0.f) {
            dir /= dist;
            sprite.move(dir * speed * deltaTime);
        }
    }

    if (state == MonsterState::Attack) {
        int curFrame = animations["attack"]->getCurrentFrameIndex();
        attackHitboxActive = (curFrame >= 3 && curFrame <= 7);
        if (animations["attack"]->isFinished()) {
            state = MonsterState::Idle;
            attackHitboxActive = false;
            switchAnimation("idle");
        }
    } else {
        attackHitboxActive = false;
    }

    animations[currentAnimation]->update(deltaTime);
    sprite.setTextureRect(animations[currentAnimation]->getCurrentFrame());

    pos = sprite.getPosition();
    hitbox.setPosition(pos.x - hitbox.getSize().x / 2.f, pos.y - hitbox.getSize().y / 2.f);

    float attOffset = isFacingLeft ? -attackHitbox.getSize().x : 0.f;
    attackHitbox.setPosition(pos.x + (isFacingLeft ? -10.f : 10.f) + attOffset, pos.y - attackHitbox.getSize().y / 2.f);
}

// -------------------------------------------------------------
// SLIME
// -------------------------------------------------------------
Slime::Slime(float x, float y)
    : Monster(x, y, 40, 70.f)
{
    scale = 2.5f;
    attackRange = 40.f;

    animations["idle"] = std::make_unique<Animation>("assets/Monsters/Slime/idle.png", 156, 156, 14, 0.1f, true);
    animations["walk"] = std::make_unique<Animation>("assets/Monsters/Slime/walk.png", 156, 156, 6, 0.12f, true);
    animations["attack"] = std::make_unique<Animation>("assets/Monsters/Slime/attack.png", 156, 156, 19, 0.07f, false);
    animations["hurt"] = std::make_unique<Animation>("assets/Monsters/Slime/hurt.png", 156, 156, 3, 0.1f, false);
    animations["death"] = std::make_unique<Animation>("assets/Monsters/Slime/death.png", 156, 156, 11, 0.1f, false);

    currentAnimation = "idle";
    sprite.setTexture(animations[currentAnimation]->getTexture());
    sprite.setScale(scale, scale);

    hitbox.setSize(sf::Vector2f(40.f, 30.f));
    attackHitbox.setSize(sf::Vector2f(45.f, 35.f));
}

void Slime::update(float deltaTime, const sf::Vector2f& playerPos, bool playerAttacking, const sf::FloatRect& playerAttackBox) {
    if (playerAttacking && getHitbox().intersects(playerAttackBox) && state != MonsterState::Hurt && state != MonsterState::Dead) {
        takeDamage(20);
    }

    if (state == MonsterState::Dead) {
        animations[currentAnimation]->update(deltaTime);
        sprite.setTextureRect(animations[currentAnimation]->getCurrentFrame());
        if (isDeathAnimationFinished()) markedForRemoval = true;
        return;
    }

    if (state == MonsterState::Hurt) {
        hurtTimer -= deltaTime;
        animations[currentAnimation]->update(deltaTime);
        sprite.setTextureRect(animations[currentAnimation]->getCurrentFrame());
        if (hurtTimer <= 0.f || animations["hurt"]->isFinished()) {
            state = MonsterState::Idle;
        }
        return;
    }

    sf::Vector2f pos = sprite.getPosition();
    float dx = playerPos.x - pos.x;
    float dist = std::abs(dx);

    isFacingLeft = (dx < 0);
    sprite.setScale(isFacingLeft ? -scale : scale, scale);

    if (dist <= attackRange) {
        if (state != MonsterState::Attack) {
            state = MonsterState::Attack;
            switchAnimation("attack");
        }
    } else if (state != MonsterState::Attack) {
        state = MonsterState::Walk;
        switchAnimation("walk");
        float moveDir = (dx > 0) ? 1.f : -1.f;
        sprite.move(moveDir * speed * deltaTime, 0.f);
    }

    if (state == MonsterState::Attack) {
        int curFrame = animations["attack"]->getCurrentFrameIndex();
        attackHitboxActive = (curFrame >= 7 && curFrame <= 14);
        if (animations["attack"]->isFinished()) {
            state = MonsterState::Idle;
            attackHitboxActive = false;
            switchAnimation("idle");
        }
    } else {
        attackHitboxActive = false;
    }

    animations[currentAnimation]->update(deltaTime);
    sprite.setTextureRect(animations[currentAnimation]->getCurrentFrame());

    pos = sprite.getPosition();
    hitbox.setPosition(pos.x - hitbox.getSize().x / 2.f, pos.y - hitbox.getSize().y / 2.f + 5.f);

    float attOffset = isFacingLeft ? -attackHitbox.getSize().x : 0.f;
    attackHitbox.setPosition(pos.x + (isFacingLeft ? -10.f : 10.f) + attOffset, pos.y - attackHitbox.getSize().y / 2.f + 5.f);
}

// -------------------------------------------------------------
// MIMIC
// -------------------------------------------------------------
Mimic::Mimic(float x, float y)
    : Monster(x, y, 50, 80.f)
{
    scale = 2.5f;
    attackRange = 45.f;

    animations["idle"] = std::make_unique<Animation>("assets/Monsters/Mimic/idle_transformed.png", 146, 146, 9, 0.12f, true);
    animations["walk"] = std::make_unique<Animation>("assets/Monsters/Mimic/walk.png", 146, 146, 6, 0.1f, true);
    animations["attack"] = std::make_unique<Animation>("assets/Monsters/Mimic/attack_1.png", 146, 146, 14, 0.08f, false);
    animations["hurt"] = std::make_unique<Animation>("assets/Monsters/Mimic/hurt.png", 146, 146, 3, 0.1f, false);
    animations["death"] = std::make_unique<Animation>("assets/Monsters/Mimic/death.png", 146, 146, 6, 0.12f, false);

    currentAnimation = "idle";
    sprite.setTexture(animations[currentAnimation]->getTexture());
    sprite.setScale(scale, scale);

    hitbox.setSize(sf::Vector2f(35.f, 35.f));
    attackHitbox.setSize(sf::Vector2f(45.f, 40.f));
}

void Mimic::update(float deltaTime, const sf::Vector2f& playerPos, bool playerAttacking, const sf::FloatRect& playerAttackBox) {
    if (playerAttacking && getHitbox().intersects(playerAttackBox) && state != MonsterState::Hurt && state != MonsterState::Dead) {
        takeDamage(20);
    }

    if (state == MonsterState::Dead) {
        animations[currentAnimation]->update(deltaTime);
        sprite.setTextureRect(animations[currentAnimation]->getCurrentFrame());
        if (isDeathAnimationFinished()) markedForRemoval = true;
        return;
    }

    if (state == MonsterState::Hurt) {
        hurtTimer -= deltaTime;
        animations[currentAnimation]->update(deltaTime);
        sprite.setTextureRect(animations[currentAnimation]->getCurrentFrame());
        if (hurtTimer <= 0.f || animations["hurt"]->isFinished()) {
            state = MonsterState::Idle;
        }
        return;
    }

    sf::Vector2f pos = sprite.getPosition();
    float dx = playerPos.x - pos.x;
    float dist = std::abs(dx);

    isFacingLeft = (dx < 0);
    sprite.setScale(isFacingLeft ? -scale : scale, scale);

    if (dist <= attackRange) {
        if (state != MonsterState::Attack) {
            state = MonsterState::Attack;
            switchAnimation("attack");
        }
    } else if (state != MonsterState::Attack) {
        state = MonsterState::Walk;
        switchAnimation("walk");
        float moveDir = (dx > 0) ? 1.f : -1.f;
        sprite.move(moveDir * speed * deltaTime, 0.f);
    }

    if (state == MonsterState::Attack) {
        int curFrame = animations["attack"]->getCurrentFrameIndex();
        attackHitboxActive = (curFrame >= 4 && curFrame <= 9);
        if (animations["attack"]->isFinished()) {
            state = MonsterState::Idle;
            attackHitboxActive = false;
            switchAnimation("idle");
        }
    } else {
        attackHitboxActive = false;
    }

    animations[currentAnimation]->update(deltaTime);
    sprite.setTextureRect(animations[currentAnimation]->getCurrentFrame());

    pos = sprite.getPosition();
    hitbox.setPosition(pos.x - hitbox.getSize().x / 2.f, pos.y - hitbox.getSize().y / 2.f + 5.f);

    float attOffset = isFacingLeft ? -attackHitbox.getSize().x : 0.f;
    attackHitbox.setPosition(pos.x + (isFacingLeft ? -10.f : 10.f) + attOffset, pos.y - attackHitbox.getSize().y / 2.f + 5.f);
}
