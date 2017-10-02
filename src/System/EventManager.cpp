#include "System/EventManager.hpp"
#include <iostream>

EventManager::EventManager()
{

}


void EventManager::init(sf::Window* window)
{
    _window = window;
}

bool EventManager::isKeyPressed(sf::Keyboard::Key key) const
{
    return (sf::Keyboard::isKeyPressed(key));
}

sf::Vector2i EventManager::getMousePosition() const
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*_window);
    mousePos.x -= _window->getSize().x/2;
    mousePos.y -= _window->getSize().y/2;

    return mousePos;
}

bool EventManager::isLeftMousePressed() const
{
    return sf::Mouse::isButtonPressed(sf::Mouse::Left);
}
