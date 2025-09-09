#include "AssetLoader.h"


AssetLoader::AssetLoader()
{
	this->loadBackground();
	this->loadAssets();
}

void AssetLoader::loadBackground()
{
	sf::Texture* bgTexture = new sf::Texture();
	bgTexture->loadFromFile("Media/Textures/Desert.png");
	bgTexture->setRepeated(true);
	
	sf::Vector2u texSize = bgTexture->getSize();

	this->windowWidth = 1920.f;
	this->windowHeight = 1080.f;

	this->bgIMG = new sf::Sprite(*bgTexture);
	this->bgIMG->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i((int)windowWidth, (int)windowHeight)));

	this->offset = sf::Vector2i(0, 0);
}

void AssetLoader::loadAssets()
{
	
}

void AssetLoader::moveBackground(float deltaTime)
{
	float speed = 750.0f * deltaTime;

	offset.y += (int)(speed);

	this->bgIMG->setTextureRect(sf::IntRect(offset, sf::Vector2i((int)windowWidth, (int)windowHeight)));

}

void AssetLoader::updateFPSCounter()
{

}

void AssetLoader::update(float deltaTime)
{
	this->moveBackground(deltaTime);
}

void AssetLoader::draw(sf::RenderWindow* window)
{
}
