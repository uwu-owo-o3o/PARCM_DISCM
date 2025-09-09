#include "AssetLoader.h"


AssetLoader::AssetLoader()
{
	this->loadBackground();
	this->loadAssets();
	this->loadFpsCounter();
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
	std::string folderPath = "Media/Streaming";
	for (const auto& entry : std::filesystem::directory_iterator(folderPath))
	{
		if (entry.is_regular_file())
		{
			sf::Texture* texture = new sf::Texture();
			if (texture->loadFromFile(entry.path().string()))
			{
				assetTextures.push_back(texture);

				sf::Sprite* sprite = new sf::Sprite(*texture);
				assetSprites.push_back(sprite);

				std::cout << "Loaded: " << entry.path().string() << "\n";
			}
			else
			{
				std::cout << "Failed to load: " << entry.path().string() << "\n";
			}
		}
	}

	this->setAssetPos();
}

void AssetLoader::setAssetPos()
{
	float padding = 11.f;
	int maxCols = 27;

	int col = 0, row = 0;
	for (auto& sprite : assetSprites)
	{
		sf::FloatRect bounds = sprite->getGlobalBounds();

		if (bounds.size.x > 60.0f)
		{
			float scale = 60.0f / bounds.size.x;
			sprite->setScale(sf::Vector2f(scale, scale));
			bounds = sprite->getGlobalBounds(); 
		}

		float x = col * (bounds.size.x + padding);
		float y = row * (bounds.size.y + padding);
		sprite->setPosition(sf::Vector2f(x, y));

		col++;
		if (x + bounds.size.x + padding > windowWidth || col >= maxCols)
		{
			col = 0;
			row++;
		}
	}
}

void AssetLoader::loadFpsCounter()
{	
	this->fpsFont = new sf::Font();

	this->fpsFont->openFromFile("Media/Font/Roboto.ttf");

	this->fpsText = new sf::Text(*this->fpsFont, "FPS: 0", 30);
	this->fpsText->setPosition(sf::Vector2f(1800.0f, 950.0f));
	this->fpsText->setFillColor(sf::Color::Red);
}

void AssetLoader::moveBackground(float deltaTime)
{
	float speed = 750.0f * deltaTime;

	offset.y += (int)(speed);

	this->bgIMG->setTextureRect(sf::IntRect(offset, sf::Vector2i((int)windowWidth, (int)windowHeight)));

}

void AssetLoader::updateFPSCounter(float deltaTime)
{
	float fps = 1.0f / deltaTime;
	this->fpsText->setString("FPS: " + std::to_string((int)(fps)));

}

void AssetLoader::update(float deltaTime)
{
	this->moveBackground(deltaTime);
	this->updateFPSCounter(deltaTime);
}

void AssetLoader::registerDrawables(std::vector<sf::Drawable*>* drawables)
{
	drawables->push_back(this->bgIMG);
	
	for (auto sprite : assetSprites) {
		drawables->push_back(sprite);
	}

	drawables->push_back(this->fpsText);
}

