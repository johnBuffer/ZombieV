#include <iostream>
#include <SFML/Graphics.hpp>

#include "System/GameWorld.hpp"
#include "System/GameRender.hpp"
#include "Bot.hpp"

#define WIN_WIDTH 1600
#define WIN_HEIGHT 900

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Zombie V", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(70);

    GameRender::initialize(WIN_WIDTH, WIN_HEIGHT);
    GameWorld world;
    world.initEventHandler(window);

    Hunter& h = *Hunter::add(Hunter(1000, 1000));
    world.addEntity(&h);

    for (int i(0); i--;)
    {
        world.addEntity(Turret::add(0+i*100, 1800));
        Bot* bot = Bot::add(500+i*50, 1900);
        world.addEntity(bot);
    }

    sf::Mouse::setPosition(sf::Vector2i(WIN_WIDTH/2+100, WIN_HEIGHT/2));

    int waveCount = 1;

    for (int i(100); i--;)
    {
        Zombie* newZombie(Zombie::add(rand()%2000, rand()%1000));
        newZombie->setTarget(&(*Hunter::getObjects().front()));
        world.addEntity(newZombie);
    }
    waveCount++;

    for (int i(0); i<10; ++i)
    {
        Light light;
        light.position = Vec2(rand()%2000, rand()%2000);
        light.color    = sf::Color(rand()%255, rand()%255,rand()%255);
        light.radius   = 150+rand()%150;
        GameRender::getLightEngine().addDurableLight(light);
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
			switch (event.type)
			{
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape) window.close();
				default:
                    break;
			}
        }

        world.update();

        Vec2 p = Hunter::getObjects().front()->getCoord();

        sf::Vector2f playerPosition(p.x, p.y);
        GameRender::setFocus(playerPosition);

        GameRender::clear();

        world.render();
        GameRender::display(&window);

        window.display();
    }

    return 0;
}
