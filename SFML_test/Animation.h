#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Animation {
public:
    Animation(const std::string& texturePath, int frameWidth, int frameHeight,
        int frameCount, float speed, bool loop = true);

    void update(float deltaTime);
    void reset();
    sf::IntRect getCurrentFrame() const;
    bool isFinished() const;
    const sf::Texture& getTexture() const { return texture; }
    int getFrameWidth() const { return frameWidth; }
    int getFrameHeight() const { return frameHeight; }

private:
    sf::Texture texture;
    int frameWidth;
    int frameHeight;
    int frameCount;
    int currentFrame;
    float frameDuration;
    float elapsedTime;
    bool finished;
    bool loop;

};