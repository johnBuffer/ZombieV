#include <iostream>
#include <SFML/Graphics.hpp>

#include "GameWorld.hpp"
#include "GameRender.hpp"

#define WIN_WIDTH 1600
#define WIN_HEIGHT 900

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Zombie V", sf::Style::Fullscreen, settings);
    window.setVerticalSyncEnabled(true);

    GameRender::initialize(WIN_WIDTH, WIN_HEIGHT);
    GameWorld world;
    world.initEventHandler(window);

    world.addHunter(U_2DCoord(500, 500));

    Zombie::loadTexture();

    GameRender::setFocus(&world.getHunter());

    sf::Mouse::setPosition(sf::Vector2i(WIN_WIDTH/2+100, WIN_HEIGHT/2));

    int waveCount = 1;

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

        if (world.getZombies().size() == 0)
        {
            for (int i(waveCount*10); i--;)
            {
                world.addZombie(U_2DCoord(rand()%2000, rand()%2000));
                world.getZombie().setTarget(&world.getHunter());
                world.getZombie().setLife(waveCount*waveCount);
            }
            waveCount++;
        }

        GameRender::clear();
        world.render();
        GameRender::display(&window);

        window.display();

    }

    return 0;
}
