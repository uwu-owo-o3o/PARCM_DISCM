#include "App.h"

void App::initialize()
{
    this->window = sf::RenderWindow(sf::VideoMode({ 1920, 1080 }), "PARCM_DISCM");
    //sf::CircleShape* shape = new sf::CircleShape(100.f);
    //shape->setFillColor(sf::Color::Green);
    //this->drawables.push_back(shape);

    this->drawables.push_back(assetLoader.bgIMG);

}

void App::run()
{
    sf::Clock clock;
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
