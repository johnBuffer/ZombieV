#include "System/SoundPlayer.hpp"

std::list<sf::Sound>         SoundPlayer::_sounds;
std::vector<sf::SoundBuffer> SoundPlayer::_buffers;

size_t SoundPlayer::registerSound(std::string filename)
{
    sf::SoundBuffer newBuffer;
    _buffers.push_back(newBuffer);
    _buffers.back().loadFromFile(filename);

    return _buffers.size()-1;
}

void SoundPlayer::playInstanceOf(size_t soundID)
{
    _sounds.push_back(sf::Sound());
    _sounds.back().setBuffer(_buffers[soundID]);
    _sounds.back().play();

    update();
}

sf::Sound SoundPlayer::getInstanceOf(size_t soundID)
{
    sf::Sound sound(_buffers[soundID]);
    return sound;
}

void SoundPlayer::update()
{
    _sounds.remove_if( [](sf::Sound& s){return s.getStatus() == sf::Sound::Stopped;} );
}
