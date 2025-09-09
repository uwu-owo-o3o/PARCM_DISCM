#pragma once
#include "SFML/Graphics.hpp"
#include "iostream"
#include "filesystem"

class AssetLoader
{
	public:
		std::vector<sf::Texture*> assetTextures;
		std::vector<sf::Sprite*> assetSprites;
		sf::Sprite* bgIMG;
		sf::Text* fpsText;
		sf::Font* fpsFont;
	public:
		sf::Vector2i offset;
		float windowWidth;
		float windowHeight;

	public:
		AssetLoader();

	public:

		void loadBackground();
		void loadAssets();
		void setAssetPos();
		void loadFpsCounter();
		void moveBackground(float deltaTime);
		void updateFPSCounter(float deltaTime);
		void update(float deltaTime);
		void registerDrawables(std::vector<sf::Drawable*>* drawables);

};

