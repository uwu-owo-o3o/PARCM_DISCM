#pragma once
#include "SFML/Graphics.hpp"

class AssetLoader
{
	public:
		std::vector<sf::Texture*> assetTextures;
		std::vector<sf::Sprite*> assetSprites;
		sf::Sprite* bgIMG;

	public:
		sf::Vector2i offset;
		float windowWidth;
		float windowHeight;

	public:
		AssetLoader();

	public:

		void loadBackground();
		void loadAssets();
		void moveBackground(float deltaTime);
		void updateFPSCounter();
		void update(float deltaTime);
		void draw(sf::RenderWindow* window);

};

