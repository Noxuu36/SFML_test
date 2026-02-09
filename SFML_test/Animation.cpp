#include "Animation.h"
#include <iostream>

Animation::Animation(const std::string& texturePath, int frameWidth, int frameHeight,
    int frameCount, float speed, bool loop)
    : frameWidth(frameWidth), frameHeight(frameHeight), frameCount(frameCount),
    currentFrame(0), frameDuration(speed), elapsedTime(0.f), finished(false),
    loop(loop)
{
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Erreur chargement texture: " << texturePath << std::endl;
    }
}

void Animation::update(float deltaTime) {
    elapsedTime += deltaTime;

    if (elapsedTime >= frameDuration) {
        currentFrame++;
        elapsedTime = 0.f;

        if (currentFrame >= frameCount) {
            if (loop) {
                currentFrame = 0;
            }
            else {
                currentFrame = frameCount - 1;
                finished = true;
            }
        }
    }
}

void Animation::reset() {
    currentFrame = 0;
    elapsedTime = 0.f;
    finished = false;
}

sf::IntRect Animation::getCurrentFrame() const {
    return sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight);
}

bool Animation::isFinished() const {
    return finished;
}