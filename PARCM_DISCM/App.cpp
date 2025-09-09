#include "App.h"

void App::initialize()
{
    this->window = sf::RenderWindow(sf::VideoMode({ 1920, 1080 }), "PARCM_DISCM");
    //sf::CircleShape* shape = new sf::CircleShape(100.f);
    //shape->setFillColor(sf::Color::Green);
    //this->drawables.push_back(shape);
    this->assetLoader.registerDrawables(&this->drawables);
}

void App::run()
{
    sf::Clock clock;
    window.setFramerateLimit(60);
    int index = 0;

    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        float dT = deltaTime.asSeconds();
        
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        this->assetLoader.update(dT);
        this->assetLoader.registerDrawableAsset(&index, &this->drawables);
        window.clear();
        this->draw();
        window.display();
    }
}

void App::draw()
{
    for (auto drawable : this->drawables) {
        window.draw(*drawable);
    }
}
