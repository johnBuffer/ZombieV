#include "System/SoundPlayer.hpp"

std::vector<SoundHandler> SoundPlayer::_buffers;

void SoundHandler::update()
{
    livingSounds.remove_if( [](sf::Sound& s){return s.getStatus() == sf::Sound::Stopped;} );
}

size_t SoundPlayer::registerSound(const std::string& filename, size_t maxSounds)
{
    _buffers.push_back(SoundHandler());
    SoundHandler& handler = _buffers.back();
    handler.soundBuffer.loadFromFile(filename);
    handler.maxLivingSounds = maxSounds;

    return _buffers.size()-1;
}

void SoundPlayer::playInstanceOf(size_t soundID)
{
    SoundHandler& handler = _buffers[soundID];
    std::list<sf::Sound>& soundList(handler.livingSounds);

    soundList.push_back(sf::Sound());
    sf::Sound& newSound = soundList.back();
    newSound.setBuffer(_buffers[soundID].soundBuffer);
    newSound.play();

    if (soundList.size() > handler.maxLivingSounds)
    {
        soundList.front().stop();
        soundList.pop_front();
    }

    handler.update();
}

sf::Sound SoundPlayer::getInstanceOf(size_t soundID)
{
    sf::Sound sound(_buffers[soundID].soundBuffer);
    return sound;
}
