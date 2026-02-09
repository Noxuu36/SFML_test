#include "Player.h"

Player::Player()
    : scale(4.f), speed(200.f), gravity(1100.f), jumpForce(-500.f),
    maxFallSpeed(1000.f), groundY(1000.f), isOnGround(true), isAttacking(false)
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

    // Hitbox
    hitbox.setSize(sf::Vector2f(24.f, 32.f));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1.f);
}

float Player::getVelocityX()const {
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

    // Empêcher input pendant l'attaque
    if (isAttacking) {
        velocity.x = 0.f;  // Arrêter le mouvement horizontal
        return;  // Ne pas traiter les autres inputs
    }

    // Saut
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround) {
        velocity.y = jumpForce;
        isOnGround = false;
    }

    // Attaque
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        isAttacking = true;
        velocity.x = 0.f;  // Arrêter le mouvement
        switchAnimation("attack");
        return;  // Ne pas traiter les autres inputs
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

    // Changer l'animation selon le mouvement
    if (moving) {
        switchAnimation("run");
    }
    else {
        switchAnimation("idle");
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

    // Fin de l'attaque
    if (isAttacking && animations["attack"]->isFinished()) {
        isAttacking = false;
        switchAnimation("idle");
    }
}

void Player::update(float deltaTime) {
    handleInput();
    updatePhysics(deltaTime);
    updateAnimation(deltaTime);

    // Centrer la hitbox
    sf::Vector2f spritePos = sprite.getPosition();
    hitbox.setPosition(spritePos.x - hitbox.getSize().x / 2.f,
        spritePos.y - hitbox.getSize().y / 2.f);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(hitbox);
}

sf::FloatRect Player::getHitbox() const {
    return hitbox.getGlobalBounds();
}