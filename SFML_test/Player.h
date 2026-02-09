#pragma once
#include "Animation.h"
#include <iostream>
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void update(float deltaTime);  //mouvement

    void draw(sf::RenderWindow& window);   //affichange

    sf::FloatRect getHitbox() const;

    void handleInput();
    void updatePhysics(float deltaTime);
    void updateAnimation(float deltaTime);
    void switchAnimation(const std::string& animName);
    float getVelocityX() const;




private:
    // --- Visuel ---
    sf::Sprite sprite;
    std::map<std::string, std::unique_ptr <Animation>> animations; //map d'animation associé a un nom
    std::string currentAnimation;
    float scale;

    // --- Collision ---
    sf::RectangleShape hitbox;

    //physique
    sf::Vector2f velocity;
    float speed;
    float gravity;
    float jumpForce;
    float maxFallSpeed;
    float isOnGround;
    bool isAttacking;
    float groundY;


};
