#pragma once
#include<SFML/Graphics.hpp>
#include <iostream>
class Background
{
	public:
		Background(const std::string& texturepath, float scrollspeed);
		void update(float deltaTime, float playerVelocityX);
		void draw(sf::RenderWindow& window);
		void setScale(float scalex, float sacley);
		sf::Vector2u getTextureSize() const;


	private:
		sf::Texture texture;
		sf::Sprite sprite1;
		sf::Sprite sprite2;
		float scrollSpeed;
		float offset;	//decalage par fram

};

