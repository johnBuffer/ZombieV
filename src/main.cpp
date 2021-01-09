#include <iostream>
#include <SFML/Graphics.hpp>

#include "System/GameWorld.hpp"
#include "System/GameRender.hpp"

#include "Bot.hpp"
#include "Props/Props.hpp"
#include "Turret.hpp"
#include "Hunter.hpp"
#include "Zombie.hpp"

#define WIN_WIDTH 1600
#define WIN_HEIGHT 900

#include "System/Utils.hpp"

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Zombie V", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    GameRender::initialize(WIN_WIDTH, WIN_HEIGHT);
    GameWorld world;
    world.initEventHandler(window);

    Hunter::registerObject(&world);

	world.initializeWeapons();

    world.getPhyManager().setGravity(Vec2(0, 0));

    Hunter& h = *Hunter::newEntity(MAP_SIZE/2, MAP_SIZE/2);
    world.addEntity(&h);

    int waveCount = 0;

    for (int i(2); i--;)
    {
        world.addEntity(Bot::newEntity(MAP_SIZE / 2 + rand() % 10, MAP_SIZE / 2 + rand() % 10));
    }

    sf::Mouse::setPosition(sf::Vector2i(WIN_WIDTH/2+100, WIN_HEIGHT/2));

    Zombie* newZombie;
    for (int i(100); i--;)
    {
        newZombie = Zombie::newEntity(rand()%MAP_SIZE, rand()%MAP_SIZE);
		EntityID target = h.getID();
		newZombie->setTarget(target);
        world.addEntity(newZombie);
    }

    for (int i(0); i<10; ++i)
    {
        Light light;
        light.position = Vec2(rand()%2000, rand()%2000);
        light.color    = sf::Color(rand()%255, rand()%255,rand()%255);
        light.radius   = 300+rand()%150;
        GameRender::getLightEngine().addDurableLight(light);
    }

    int frameCount = 0;
    float ttime = 0;
    while (window.isOpen())
    {
        ++frameCount;

        if (Zombie::getObjectsCount() == 0)
        {
            ++waveCount;
            for (int i(waveCount*waveCount + 10); i--;)
            {
                Zombie* newZombie(Zombie::newEntity(rand()%MAP_SIZE, rand()%MAP_SIZE));
                //newZombie->setTarget(&(*Hunter::getObjects().front()));
                world.addEntity(newZombie);
            }
        }

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

		int upTime = clock.getElapsedTime().asMilliseconds();
        ttime += upTime;

        Vec2 p = h.getCoord();
		GameRender::setFocus({ p.x, p.y });

        GameRender::clear();

        world.render();
        GameRender::display(&window);

        window.display();
    }

    return 0;
}
