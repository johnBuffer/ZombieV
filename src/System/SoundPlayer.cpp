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

sf::Sound& SoundPlayer::playInstanceOf(size_t soundID, float power)
{
    _sounds.push_back(sf::Sound());

    sf::Sound& newSound = _sounds.back();
    newSound.setBuffer(_buffers[soundID]);
    newSound.play();
    newSound.setVolume(power);

    update();

    return newSound;
}

sf::Sound SoundPlayer::getInstanceOf(size_t soundID)
{
    return sf::Sound(_buffers[soundID]);
}

void SoundPlayer::update()
{
    _sounds.remove_if( [](sf::Sound& s){return s.getStatus() == sf::Sound::Stopped;} );
}
