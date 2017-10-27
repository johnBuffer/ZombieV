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
    //sf::RenderWindow window(sf::VideoMode(10, 10), "Zombie V", sf::Style::Default, settings);
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Zombie V", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(false);
    //window.setFramerateLimit(60);

    GameRender::initialize(WIN_WIDTH, WIN_HEIGHT);
    GameWorld world;
    world.initEventHandler(window);

    Hunter::registerObject(&world);
    Zombie::registerObject(&world);
    Bot::registerObject(&world);

    Hunter& h = *Hunter::addEntity(MAP_SIZE/2, MAP_SIZE/2);
    world.addEntity(&h);

    std::cout << "H's ID | ptr : " << h.getID() << " | " << &h << std::endl;

    Bot* newBot;
    for (int i(10); i--;)
    {
        //world.addEntity(Turret::add(2000+i*100, 2048));
        //Bot* bot = Bot::add(1500+rand()%1000, 1500+ rand()%1000);
        //newBot = Bot::add(rand()%MAP_SIZE, rand()%MAP_SIZE);
        newBot = Bot::add(MAP_SIZE/2+rand()%10, MAP_SIZE/2+rand()%10);
        world.addEntity(newBot);
    }

    sf::Mouse::setPosition(sf::Vector2i(WIN_WIDTH/2+100, WIN_HEIGHT/2));

    int waveCount = 1;

    EntityID lastID = 0;
    for (int i(1000); i--;)
    {
        Zombie* newZombie(Zombie::addEntity(rand()%MAP_SIZE, rand()%MAP_SIZE));
        //newZombie->setTarget(&(*Hunter::getObjects().front()));
        world.addEntity(newZombie);
        lastID = newZombie->getID();
    }

    std::cout << lastID << std::endl;

    waveCount++;

    for (int i(0); i<100; ++i)
    {
        Light light;
        light.position = Vec2(rand()%2000, rand()%2000);
        light.color    = sf::Color(rand()%255, rand()%255,rand()%255);
        light.radius   = 200+rand()%150;
        GameRender::getLightEngine().addDurableLight(light);
    }

    int frameCount = 0;
    float ttime = 0;
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
        int upTime = clock.getElapsedTime().asMilliseconds();
        ttime += upTime;
        /*system("cls");
        std::cout << "Logic update time  : " << ttime/float(frameCount) << " ms" << std::endl;
        std::cout << "Bodies       count : " << U_2DBody::size() << std::endl;
        std::cout << "Bullets      count : " << Bullet::size() << std::endl;
        std::cout << "Zombies      count : " << Zombie::size() << std::endl;
        std::cout << "Guts         count : " << Guts::size() << std::endl;
        std::cout << "Fire         count : " << Fire::size() << std::endl;
        std::cout << "Explosions   count : " << Explosion::size() << std::endl;
        std::cout << "Smokes       count : " << Smoke::size() << std::endl;
        std::cout << "BulletShells count : " << BulletShell::size() << std::endl;
        std::cout << "===================================\n" << std::endl;*/

        Vec2 p = h.getCoord();

        sf::Vector2f playerPosition(p.x, p.y);
        GameRender::setFocus(playerPosition);

        GameRender::clear();

        world.render();
        GameRender::display(&window);

        window.display();
    }

    return 0;
}
