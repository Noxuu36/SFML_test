#include "Player.h"

#include "Player.h"

Player::Player()
{
    // Charger la texture
    if (!textureRun.loadFromFile("assets/player/herochar_run_anim_strip_6.png")) {
        std::cerr << "Erreur chargement texture player ";
    }

    if (!textureAttack.loadFromFile("assets/player/herochar_sword_attack_anim_strip_4.png")) {
        std::cerr << "Erreur chargement texture attack";
    }

    if (!textureIdle.loadFromFile("assets/player/herochar_idle_anim_strip_4.png")) {
        std::cerr << "Erreur chargement texture iddle";
    }

    //paramètres physique

    gravity = 1100; //gravite en pixel/s
    jumpForce = -500;    //force saut
    velocityY = 0.f;    //vitesse y actuelle
    maxFallSpeed = 1000.f; //vitesse max de chute
    groundY = 700.f;    //sol
    isonground = true;



    // Taille du sprite (si spritesheet, ajuste ici)
    runFrameWidth = 16;
    runFrameHeight = 16;

    attackFrameHeight = 16;
    attackFrameWidth = 32;

    idleFrameHeight = 16;
    idleFrameWidth = 16;


    //animation
    currentState = AnimationState::IDLE;    //animation de base
    currentFrame = 0;
    runFrameCount = 6;
    attackFrameCount = 4;
    idleFrameCount = 4;

    animationSpeed = 0.11f;
    attackAnimationSpeed = 0.2f;
    idleAnimationSpeed = 0.2f;

    isAttacking = false;




    sprite.setTexture(textureRun);
    sprite.setOrigin(runFrameWidth / 2.f, runFrameHeight / 2.f);
    sprite.setScale(4.f, 4.f);
    sprite.setPosition(400.f, groundY - runFrameHeight *  4.f / 2.f);

    sprite.setTextureRect(sf::IntRect(0, 0, runFrameWidth, runFrameHeight));

    // --- Hitbox ---
    hitbox.setSize(sf::Vector2f(24.f, 32.f));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);   // Debug
    hitbox.setOutlineThickness(1.f);

    // Décalage hitbox par rapport au sprite
    hitboxOffset = sf::Vector2f(5.f, 0.f);

    hitbox.setPosition(sprite.getPosition());

    // --- Mouvement ---
    speed = 200.f;
}


void Player::attack() {
    if (!isAttacking) {
        isAttacking = true;
        currentState = AnimationState::ATTACK;
        currentFrame = 0;
        sprite.setTexture(textureAttack);
        sprite.setOrigin(attackFrameWidth / 2.f, attackFrameHeight / 2.f);
        animationClock.restart();
    }
}


void Player::update(float deltaTime)
{
    sf::Vector2f movement(0.f, 0.f);
    bool ismoving = false;
    float scale = 4.f;


    //saut 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && (isonground) && (!isAttacking)) {
        velocityY = jumpForce;
        isonground = false;
    }

    if (!isonground) {  //si saut detecté
        velocityY += gravity * deltaTime;

        if (velocityY > maxFallSpeed) {
            velocityY = maxFallSpeed;
        }
    }

    //attaque
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && (!isAttacking)) {
        attack();
    }

    //gauche
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && (!isAttacking)) {
        sprite.setScale(-scale, scale);
        movement.x -= speed;
        ismoving = true;
    }
    //droite
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && (!isAttacking)) {
        sprite.setScale(scale, scale);
        movement.x += speed;
        ismoving = true;
    }



    // Déplacement horizontale
    sprite.move(movement * deltaTime);

    //deplacement verticale
    sf::Vector2f position = sprite.getPosition();
    position.y += velocityY * deltaTime;

    float playerBottom = position.y + (runFrameHeight * scale / 2.f);
    if (playerBottom >= groundY) {
        position.y = groundY - (runFrameHeight * scale / 2.f);
        velocityY = 0.f;
        isonground = true;
    }

    sprite.setPosition(position);

    // Synchroniser la hitbox avec le sprite
    hitbox.setPosition(sprite.getPosition()+ hitboxOffset);

    if (isAttacking) {
        if (animationClock.getElapsedTime().asSeconds() >= attackAnimationSpeed) {
            currentFrame++;
            if (currentFrame >= attackFrameCount) {
                //fin animation
                isAttacking = false;
                currentFrame = 0;
                currentState = AnimationState::IDLE;
                sprite.setTexture(textureRun);  //retour a la texture de base
            }

            sprite.setTextureRect(sf::IntRect(
                currentFrame * attackFrameWidth,
                0,
                attackFrameWidth,
                attackFrameHeight
            ));
            animationClock.restart();
        }
    }
    else if (ismoving) {

        if (animationClock.getElapsedTime().asSeconds() >= animationSpeed) {
            sprite.setTexture(textureRun);
            currentState = AnimationState::RUN;
            sprite.setOrigin(runFrameWidth / 2.f, runFrameHeight / 2.f);
            currentFrame = (currentFrame + 1) % runFrameCount;
            sprite.setTextureRect(sf::IntRect(
                currentFrame * runFrameWidth,
                0,
                runFrameWidth,
                runFrameHeight

            ));
            animationClock.restart();
        }
    }
    else {
        if (currentState != AnimationState::IDLE) {      //image fixe
            currentState = AnimationState::IDLE;
            sprite.setOrigin(idleFrameWidth / 2.f, idleFrameHeight /2.f);
            currentFrame = 0;
            animationClock.restart();
        }
        if (animationClock.getElapsedTime().asSeconds() >= idleAnimationSpeed) {
            sprite.setTexture(textureIdle);
            currentFrame = (currentFrame + 1) % idleFrameCount;
            sprite.setTextureRect(sf::IntRect(
                currentFrame * idleFrameWidth,
                0,
                idleFrameWidth,
                idleFrameHeight
            ));
            animationClock.restart();
        }
    }
}




void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    window.draw(hitbox); // À retirer quand tu n'en as plus besoin
}

sf::FloatRect Player::getHitbox() const
{
    return hitbox.getGlobalBounds();
}
