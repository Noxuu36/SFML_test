#include "Player.h"

Player::Player()
    : scale(4.f), speed(200.f), gravity(1100.f), jumpForce(-500.f),
    maxFallSpeed(1000.f), groundY(1000.f), isOnGround(true), isAttacking(false),
    attackHitboxActive(false)
{
    // Charger les animations (loop=true sauf pour attack)
    animations["idle"] = std::make_unique<Animation>(
        "assets/player/herochar_idle_anim_strip_4.png", 16, 16, 4, 0.2f, true);
    animations["run"] = std::make_unique<Animation>(
        "assets/player/herochar_run_anim_strip_6.png", 16, 16, 6, 0.11f, true);
    animations["attack"] = std::make_unique<Animation>(
        "assets/player/herochar_sword_attack_anim_strip_4.png", 32, 16, 4, 0.15f, false);  // loop=false

    currentAnimation = "idle";  //anim de base
    sprite.setTexture(animations[currentAnimation]->getTexture());
    sprite.setScale(scale, scale);
    sprite.setPosition(400.f, groundY);

    velocity = sf::Vector2f(0.f, 0.f);  // Initialiser velocity

    // Hitbox Joueur (hurtbox)
    hitbox.setSize(sf::Vector2f(28.f, 48.f));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1.f);

    // Hitbox d'attaque
    attackHitbox.setSize(sf::Vector2f(56.f, 56.f));
    attackHitbox.setFillColor(sf::Color::Transparent);
    attackHitbox.setOutlineColor(sf::Color::Green);
    attackHitbox.setOutlineThickness(1.f);
}

float Player::getVelocityX() const {
    return velocity.x;
}

void Player::switchAnimation(const std::string& animName) {
    if (currentAnimation != animName) {
        currentAnimation = animName;
        animations[animName]->reset();
        sprite.setTexture(animations[animName]->getTexture());

        int frameWidth = animations[animName]->getFrameWidth();
        int frameHeight = animations[animName]->getFrameHeight();
        sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    }
}

void Player::handleInput() {
    bool moving = false;

    // Saut
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround) {
        velocity.y = jumpForce;
        isOnGround = false;
    }

    // Attaque
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !isAttacking) {
        isAttacking = true;
        switchAnimation("attack");
    }

    // Mouvement horizontal
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        sprite.setScale(-scale, scale);
        velocity.x = -speed;
        moving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        sprite.setScale(scale, scale);
        velocity.x = speed;
        moving = true;
    }
    else {
        velocity.x = 0.f;
    }

    // Changer l'animation selon le mouvement seulement si pas en train d'attaquer
    if (!isAttacking) {
        if (moving) {
            switchAnimation("run");
        }
        else {
            switchAnimation("idle");
        }
    }
}

void Player::updatePhysics(float deltaTime) {
    if (!isOnGround) {
        velocity.y += gravity * deltaTime;
        if (velocity.y > maxFallSpeed) {
            velocity.y = maxFallSpeed;
        }
    }

    sprite.move(velocity.x * deltaTime, velocity.y * deltaTime);

    sf::Vector2f position = sprite.getPosition();
    float playerBottom = position.y + (16 * scale / 2.f);

    if (playerBottom >= groundY) {
        position.y = groundY - (16 * scale / 2.f);
        velocity.y = 0.f;
        isOnGround = true;
        sprite.setPosition(position);
    }
}

void Player::updateAnimation(float deltaTime) {
    animations[currentAnimation]->update(deltaTime);
    sprite.setTextureRect(animations[currentAnimation]->getCurrentFrame());

    // Activer l'attack hitbox au milieu de l'attaque (frames 1 & 2 sur 4)
    if (isAttacking) {
        int currentFrame = animations["attack"]->getCurrentFrameIndex();
        if (currentFrame == 1 || currentFrame == 2) {
            attackHitboxActive = true;
        } else {
            attackHitboxActive = false;
        }
    } else {
        attackHitboxActive = false;
    }

    // Fin de l'attaque
    if (isAttacking && animations["attack"]->isFinished()) {
        isAttacking = false;
        attackHitboxActive = false;
        bool moving = sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        if (moving) {
            switchAnimation("run");
        }
        else {
            switchAnimation("idle");
        }
    }
}

void Player::update(float deltaTime) {
    handleInput();
    updatePhysics(deltaTime);
    updateAnimation(deltaTime);

    // Alignement au sol exact et centrage de la hitbox
    sf::Vector2f spritePos = sprite.getPosition();
    hitbox.setPosition(spritePos.x - hitbox.getSize().x / 2.f,
        spritePos.y - hitbox.getSize().y / 2.f + 8.f);

    // Mettre à jour la hitbox d'attaque selon l'orientation (étendue vers le bas)
    float facing = (sprite.getScale().x >= 0.f) ? 1.f : -1.f;
    float offset = 15.f * facing;
    if (facing > 0) {
        attackHitbox.setPosition(spritePos.x + offset, spritePos.y - attackHitbox.getSize().y / 2.f + 8.f);
    } else {
        attackHitbox.setPosition(spritePos.x + offset - attackHitbox.getSize().x, spritePos.y - attackHitbox.getSize().y / 2.f + 8.f);
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(hitbox);
    if (attackHitboxActive) {
        window.draw(attackHitbox);
    }
}

sf::FloatRect Player::getHitbox() const {
    return hitbox.getGlobalBounds();
}

sf::FloatRect Player::getAttackHitbox() const {
    return attackHitbox.getGlobalBounds();
}

bool Player::isAttackActive() const {
    return attackHitboxActive;
}
