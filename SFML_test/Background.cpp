#include "Background.h"

Background::Background(const std::string& texturePath, float scrollSpeed)
    : scrollSpeed(scrollSpeed), offset(0.f) {

    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Erreur chargement fond: " << texturePath << std::endl;
    }

    texture.setRepeated(true);

    sprite1.setTexture(texture);
    sprite2.setTexture(texture);

    // Position initiale (sera mise à jour par setScale)
    sprite2.setPosition(texture.getSize().x, 0);
}

void Background::setScale(float scaleX, float scaleY) {
    sprite1.setScale(scaleX, scaleY);
    sprite2.setScale(scaleX, scaleY);

    // Repositionner sprite2 avec l'échelle
    float scaledWidth = texture.getSize().x * scaleX;
    sprite2.setPosition(scaledWidth, 0);
}

void Background::update(float deltaTime, float playerVelocityX) {
    offset += playerVelocityX * scrollSpeed * deltaTime;

    // Récupérer la largeur avec échelle
    float scaledWidth = texture.getSize().x * sprite1.getScale().x;

    // Déplacer les sprites
    sprite1.setPosition(-offset, 0);
    sprite2.setPosition(scaledWidth - offset, 0);

    // Réinitialiser si dépassement
    if (offset >= scaledWidth) {
        offset = 0.f;
    }
    else if (offset <= -scaledWidth) {
        offset = 0.f;
    }
}

void Background::draw(sf::RenderWindow& window) {
    window.draw(sprite1);
    window.draw(sprite2);
}

sf::Vector2u Background::getTextureSize() const {
    return texture.getSize();
}