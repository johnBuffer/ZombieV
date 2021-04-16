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
    //window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    GameRender::initialize(WIN_WIDTH, WIN_HEIGHT);
    GameWorld world;
    world.initEventHandler(window);

    Hunter::registerObject(&world);

	world.initializeWeapons();

    world.getPhyManager().setGravity(Vec2(0, 0));

    Hunter& h = *Hunter::newEntity(static_cast<float>(MAP_SIZE/2), static_cast<float>(MAP_SIZE/2));
    world.addEntity(&h);

    int waveCount = 0;

    for (int i(2); i--;)
    {
        world.addEntity(Bot::newEntity(static_cast<float>(MAP_SIZE / 2 + rand() % 10), static_cast<float>(MAP_SIZE / 2 + rand() % 10)));
    }

    sf::Mouse::setPosition(sf::Vector2i(WIN_WIDTH/2+100, WIN_HEIGHT/2));

    Zombie* newZombie;
    for (int i(100); i--;)
    {
        newZombie = Zombie::newEntity(getRandUnder(static_cast<float>(MAP_SIZE)), getRandUnder(static_cast<float>(MAP_SIZE)));
		EntityID target = h.getID();
		newZombie->setTarget(target);
        world.addEntity(newZombie);
    }

    for (int i(0); i<10; ++i)
    {
        Light light;
        light.position = Vec2(getRandUnder(2000.0f), getRandUnder(2000.0f));
        light.color    = sf::Color(rand()%255, rand()%255,rand()%255);
        light.radius   = getRandFloat(300.0f, 450.0f);
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
                Zombie* newZombie(Zombie::newEntity(getRandUnder(static_cast<float>(MAP_SIZE)), getRandUnder(static_cast<float>(MAP_SIZE))));
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
