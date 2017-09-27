#include <iostream>
#include <SFML/Graphics.hpp>

#include "GameWorld.hpp"
#include "GameRender.hpp"

#include "Container.hpp"

#define WIN_WIDTH 1600
#define WIN_HEIGHT 900

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Zombie V", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(false);

    GameRender::initialize(WIN_WIDTH, WIN_HEIGHT);
    GameWorld world;
    world.initEventHandler(window);

    Hunter hunter(1000, 1000);
    world.addEntity(&hunter);

    sf::Mouse::setPosition(sf::Vector2i(WIN_WIDTH/2+100, WIN_HEIGHT/2));

    int waveCount = 1;

    for (int i(1000); i--;)
    {
        Zombie* newZombie = new Zombie(rand()%2000, rand()%2000);
        newZombie->setTarget(&hunter);
        world.addEntity(newZombie);
    }
    waveCount++;

    for (int i(0); i<50; ++i)
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

        Vec2 p = hunter.getShakyPos();
        sf::Vector2f playerPosition(p.x, p.y);
        GameRender::setFocus(playerPosition);

        GameRender::clear();

        world.render();
        GameRender::display(&window);

        window.display();
    }

    return 0;
}
