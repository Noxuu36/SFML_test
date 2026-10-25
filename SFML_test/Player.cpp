/*
 * =====================================================================================
 * GUIDE : COMMENT GÉRER UN CONFLIT DE MERGE GIT (MERGE CONFLICT RESOLUTION)
 * =====================================================================================
 * Lorsqu'un conflit de merge survient (par exemple lors de la fusion avec 'master') :
 *
 * 1. BALISES DANS LE FICHIER :
 *    Git insère des marqueurs de conflit autour du code divergent :
 *    <<<<<<< fix-player-attack-movement-... (Vos modifications actuelles - Current Change)
 *        float attOffset = isFacingLeft ? -attackHitbox.getSize().x : 0.f;
 *        attackHitbox.setPosition(...);
 *    =======
 *        code provenant de la branche cible (Incoming Change)
 *    >>>>>>> master
 *
 * 2. RÉSOLUTION DANS VS CODE :
 *    - VS Code affiche des liens cliquables au-dessus du conflit :
 *        * "Accept Current Change"  : Conserve uniquement vos modifications (partie supérieure).
 *        * "Accept Incoming Change" : Conserve uniquement les modifications de master (partie inférieure).
 *        * "Accept Both Changes"    : Garde les deux versions l'une après l'autre.
 *    - Vous pouvez également modifier le code manuellement puis effacer les lignes <<<<<<<, =======, >>>>>>>.
 *
 * 3. VALIDER LA RÉSOLUTION :
 *    - Sauvegardez le fichier (Ctrl + S).
 *    - Marquez le conflit comme résolu et créez le commit :
 *        git add Player.cpp
 *        git commit -m "Résolution du conflit de merge"
 *        git push
 * =====================================================================================
 */

#include "Player.h"

Player::Player()
    : scale(4.f), speed(200.f), gravity(1100.f), jumpForce(-500.f),
    maxFallSpeed(1000.f), groundY(1000.f), isOnGround(true), isAttacking(false),
    attackHitboxActive(false), maxHealth(3), currentHealth(3),
    invincibilityTimer(0.f), invincibilityDuration(1.0f), healthBarLoaded(false)
{
    // Charger la texture de la barre de vie
    if (healthBarTexture.loadFromFile("assets/life/Changing Healthbar Full Heart Set.png") ||
        healthBarTexture.loadFromFile("SFML_test/assets/life/Changing Healthbar Full Heart Set.png")) {
        healthBarLoaded = true;
        healthBarSprite.setTexture(healthBarTexture);
        healthBarSprite.setScale(3.f, 3.f);
        healthBarSprite.setPosition(20.f, 20.f);

        // Découpe initiale : 4 barres empilées en colonne (frame 0 = full)
        unsigned int texWidth = healthBarTexture.getSize().x;
        unsigned int frameHeight = healthBarTexture.getSize().y / 4;
        healthBarSprite.setTextureRect(sf::IntRect(0, 0, texWidth, frameHeight));
    }
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

void Player::takeDamage(int amount) {
    if (invincibilityTimer > 0.f || currentHealth <= 0) return;

    currentHealth -= amount;
    if (currentHealth < 0) currentHealth = 0;
    invincibilityTimer = invincibilityDuration;
}

int Player::getHealth() const {
    return currentHealth;
}

bool Player::isInvincible() const {
    return invincibilityTimer > 0.f;
}

void Player::update(float deltaTime) {
    if (invincibilityTimer > 0.f) {
        invincibilityTimer -= deltaTime;
        if (invincibilityTimer < 0.f) invincibilityTimer = 0.f;
    }

    handleInput();
    updatePhysics(deltaTime);
    updateAnimation(deltaTime);

    // Mettre à jour la sous-image de la barre de vie selon currentHealth (3, 2, 1, 0)
    if (healthBarLoaded) {
        // 3 HP -> ligne 0, 2 HP -> ligne 1, 1 HP -> ligne 2, 0 HP -> ligne 3
        int frameIndex = 3 - currentHealth;
        if (frameIndex < 0) frameIndex = 0;
        if (frameIndex > 3) frameIndex = 3;

        unsigned int texWidth = healthBarTexture.getSize().x;
        unsigned int frameHeight = healthBarTexture.getSize().y / 4;
        healthBarSprite.setTextureRect(sf::IntRect(0, frameIndex * frameHeight, texWidth, frameHeight));
    }

    // Alignement au sol exact et centrage de la hitbox
    sf::Vector2f spritePos = sprite.getPosition();
    hitbox.setPosition(spritePos.x - hitbox.getSize().x / 2.f,
        spritePos.y - hitbox.getSize().y / 2.f + 8.f);

    // Mettre à jour la hitbox d'attaque selon l'orientation
    float facing = (sprite.getScale().x >= 0.f) ? 1.f : -1.f;
    float offset = 10.f * facing;
    if (facing > 0) {
        attackHitbox.setPosition(spritePos.x + offset, spritePos.y - attackHitbox.getSize().y / 2.f + 5.f);
    } else {
        attackHitbox.setPosition(spritePos.x + offset - attackHitbox.getSize().x, spritePos.y - attackHitbox.getSize().y / 2.f + 5.f);
    }
}

void Player::draw(sf::RenderWindow& window) {
    // Clignotement pendant l'invulnérabilité
    if (invincibilityTimer > 0.f) {
        int blink = static_cast<int>(invincibilityTimer * 10.f);
        if (blink % 2 == 0) {
            window.draw(sprite);
        }
    } else {
        window.draw(sprite);
    }

    window.draw(hitbox);
    if (attackHitboxActive) {
        window.draw(attackHitbox);
    }

    // Afficher la barre de vie en haut à gauche
    if (healthBarLoaded) {
        window.draw(healthBarSprite);
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
