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
    window.setFramerateLimit(60);

    GameRender::initialize(WIN_WIDTH, WIN_HEIGHT);
    GameWorld world;
    world.initEventHandler(window);

    Hunter& h = *Hunter::add(Hunter(MAP_SIZE/2, MAP_SIZE/2));
    world.addEntity(&h);

    for (int i(25); i--;)
    {
        //world.addEntity(Turret::add(0+i*100, 1800));
        //Bot* bot = Bot::add(1500+rand()%1000, 1500+ rand()%1000);
        //Bot* newBot(Bot::add(rand()%MAP_SIZE, rand()%MAP_SIZE));
        Bot* newBot(Bot::add(MAP_SIZE/2+2, MAP_SIZE/2-2));
        world.addEntity(newBot);
    }

    sf::Mouse::setPosition(sf::Vector2i(WIN_WIDTH/2+100, WIN_HEIGHT/2));

    int waveCount = 1;

    for (int i(4000); i--;)
    {
        Zombie* newZombie(Zombie::add(rand()%MAP_SIZE, rand()%MAP_SIZE));
        //newZombie->setTarget(&(*Hunter::getObjects().front()));
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

        /*std::cout << "Bullets      count : " << Bullet::getObjects().size() << std::endl;
        std::cout << "Zombies      count : " << Zombie::getObjects().size() << std::endl;
        std::cout << "Explosions   count : " << Explosion::getObjects().size() << std::endl;
        std::cout << "Smokes       count : " << Smoke::getObjects().size() << std::endl;
        std::cout << "BulletShells count : " << BulletShell::getObjects().size() << std::endl;
        std::cout << "===================================\n" << std::endl;*/

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
