#include <SFML/Graphics.hpp>
#include "Player.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(2000, 1200), "SFML Exemple");
    window.setFramerateLimit(60);

    Player player;
    Player test;

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();
        player.update(dt);
        

        window.clear();

        player.draw(window);
        

        window.display();
    }

    return 0;
}

//player.draw(window);