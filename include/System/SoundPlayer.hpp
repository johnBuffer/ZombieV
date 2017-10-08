#ifndef SOUNDPLAYER_HPP_INCLUDED
#define SOUNDPLAYER_HPP_INCLUDED

#include <SFML/Audio.hpp>
#include <string>
#include <list>

class SoundPlayer
{
public:
    static size_t     registerSound(std::string filename);
    static sf::Sound& playInstanceOf(size_t soundID, float power=100.0f);
    static sf::Sound  getInstanceOf(size_t soundID);

private:
    static std::list<sf::Sound>         _sounds;
    static std::vector<sf::SoundBuffer> _buffers;

    static void update();
};

#endif // SOUNDPLAYER_HPP_INCLUDED
