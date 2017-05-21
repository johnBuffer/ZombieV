#ifndef EVENTMANAGER_HPP_INCLUDED
#define EVENTMANAGER_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class EventManager
{
public:
    EventManager();
    void init(sf::Window* window);
    sf::Vector2i getMousePosition();

    bool isKeyPressed(sf::Keyboard::Key key) const;
    bool isLeftMousePressed() const;

private:
    sf::Window* __window;
};

#endif // EVENTMANAGER_HPP_INCLUDED
