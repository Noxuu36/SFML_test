#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void update(float deltaTime);  //mouvement

    void draw(sf::RenderWindow& window);   //affichange

    sf::FloatRect getHitbox() const;

    void attack();



private:
    // --- Visuel ---
    sf::Texture textureRun;
    sf::Texture textureAttack;
    sf::Texture textureIdle;
    sf::Sprite sprite;


    // --- Collision ---
    sf::RectangleShape hitbox;
    sf::Vector2f hitboxOffset;

    // --- Mouvement ---
    float speed;
    float velocityY;

    // physique

    float gravity;
    bool isonground;
    float jumpForce;
    float groundY;
    float maxFallSpeed;

    //animation
    enum class AnimationState {
        IDLE,
        RUN,
        ATTACK
    };

    AnimationState currentState;
    //animation
    int runFrameWidth;
    int runFrameHeight;

    int attackFrameHeight;
    int attackFrameWidth;
   
    int currentFrame;
    sf::Clock animationClock;   //temps entre 2 frames

    //spécifique a chaque animation
    int runFrameCount;
    float animationSpeed;
    int attackFrameCount;
    bool isAttacking;
    float attackAnimationSpeed;

    int idleFrameHeight;
    int idleFrameWidth;
    int idleFrameCount;
    float idleAnimationSpeed;


};
