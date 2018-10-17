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
    //GameWorld world;
    //world.initEventHandler(window);

    //Hunter::registerObject(&world);

	//world.initializeWeapons();

    //world.getPhyManager().setGravity(Vec2(0, 0));
    //world.getPhyManager().setPrecision(2);

	sf::RenderTexture test_texture;
	test_texture.create(WIN_WIDTH, WIN_HEIGHT);
	test_texture.clear();

    //Hunter& h = *Hunter::newEntity(MAP_SIZE/2, MAP_SIZE/2);
    //world.addEntity(&h);

    int waveCount = 0;

    Bot* newBot;
    for (int i(0); i--;)
    {
        //world.addEntity(Turret::add(2000+i*100, 2048));
        //Bot* bot = Bot::add(1500+rand()%1000, 1500+ rand()%1000);
        //newBot = Bot::add(rand()%MAP_SIZE, rand()%MAP_SIZE);
        newBot = Bot::newEntity(MAP_SIZE/2 + rand()%10, MAP_SIZE/2 + rand()%10);
        //world.addEntity(newBot);
    }

    sf::Mouse::setPosition(sf::Vector2i(WIN_WIDTH/2+100, WIN_HEIGHT/2));

    Zombie* newZombie;
    /*for (int i(100); i--;)
    {
        newZombie = Zombie::newEntity(rand()%MAP_SIZE, rand()%MAP_SIZE);
		int target = h.getID();
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
    }*/

	GameRender::setFocus({ 500, 500 });

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

        GameRender::clear();

        //world.render();

		test_texture.clear(sf::Color::Red);
		sf::RectangleShape rect({ 400, 400 });
		rect.setOrigin({ 200, 200 });
		rect.setPosition(800, 450);
		rect.setRotation(frameCount*0.005f);
		test_texture.draw(rect);
		test_texture.display();

		sf::Sprite test_sprite(GameRender::getBlur(test_texture.getTexture()));
        //GameRender::display(&window);
		window.draw(test_sprite);

        window.display();
    }

    return 0;
}
