#include <iostream>
#include <SFML/Graphics.hpp>

#include "System/GameWorld.hpp"
#include "System/GameRender.hpp"

#include "Bot.hpp"
#include "Props/Props.hpp"
#include "Turret.hpp"
#include "Hunter.hpp"
#include "Zombie.hpp"

#define WIN_WIDTH 750
#define WIN_HEIGHT 750

#include "System/Utils.hpp"
#include "HUD.hpp"

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    //sf::RenderWindow window(sf::VideoMode(10, 10), "Zombie V", sf::Style::Default, settings);
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Zombie V", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(60);

    GameRender::initialize(WIN_WIDTH, WIN_HEIGHT);
    GameWorld world;
    world.initEventHandler(window);

    GameRender::setFocus(sf::Vector2f(375, 375));

	Hunter* player = Hunter::create(700, 700);
    world.addEntity(player);

    int frameCount = 0;
    while (window.isOpen())
    {
        ++frameCount;
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

        sf::Clock clock;
        world.update();

        GameRender::clear();
        world.render();
        GameRender::display(&window);

        window.display();
    }

    return 0;
}
