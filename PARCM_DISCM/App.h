#pragma once
#include "SFML/Graphics.hpp"
#include "AssetLoader.h"

class App
{
	public:
		sf::RenderWindow window;
		std::vector<sf::Drawable*> drawables;
		AssetLoader assetLoader;
	public:
		void initialize();
		void run();
		void draw();
};

