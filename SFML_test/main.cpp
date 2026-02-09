#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Background.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(2000, 1200), "SFML Exemple");
    window.setFramerateLimit(60);

    Background background("assets/background/background.png", 0.5f);
   
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = background.getTextureSize();

    float scaleX = (float) windowSize.x / textureSize.x;
    float scaleY = (float) windowSize.y / textureSize.y;

    background.setScale(scaleX, scaleY);

    Player player;

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();
        player.update(dt);
        background.update(dt, player.getVelocityX());
        

        window.clear();

        background.draw(window);

        player.draw(window);
        

        window.display();
    }

    return 0;
}

