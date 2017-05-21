#include "EventManager.hpp"
#include <iostream>

EventManager::EventManager()
{

}


void EventManager::init(sf::Window* window)
{
    __window = window;
}

bool EventManager::isKeyPressed(sf::Keyboard::Key key) const
{
    return (sf::Keyboard::isKeyPressed(key));
}

sf::Vector2i EventManager::getMousePosition()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*__window);
    mousePos.x -= __window->getSize().x/2;
    mousePos.y -= __window->getSize().y/2;

    return mousePos;
}

bool EventManager::isLeftMousePressed() const
{
    return sf::Mouse::isButtonPressed(sf::Mouse::Left);
}
