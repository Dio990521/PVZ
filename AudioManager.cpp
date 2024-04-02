#include "AudioManager.h"
#include "AssetManager.h"
#include "Singleton.h"

void AudioManager::PlaySFX(const string& id, const int volume)
{
	sf::SoundBuffer* buffer = Singleton<AssetManager>::get()->Get<AudioAsset>(id)->GetBuffer();
	sf::Sound* sound = new sf::Sound(*buffer);
    this->sounds.push_back(sound);
    sound->setVolume(volume);
	sound->play();
}

void AudioManager::PlayBGM(const string& filename, const int volume)
{
    sf::Music* bgm = new sf::Music();
    if (!bgm->openFromFile(filename)) {
        std::cerr << "Failed to load background music." << std::endl;
        return;
    }
    this->musics.push_back(bgm);
    bgm->setVolume(volume);
    bgm->play();
}

void AudioManager::Update()
{
    for (int i = 0; i < sounds.size(); ++i)
    {
        if (sounds[i]->getStatus() == sf::SoundSource::Stopped)
        {
            delete sounds[i];
            sounds.erase(sounds.begin() + i);
            --i;
        }
    }

    for (int i = 0; i < musics.size(); ++i)
    {
        if (musics[i]->getStatus() == sf::Music::Stopped)
        {
            delete musics[i];
            musics.erase(musics.begin() + i);
            --i;
        }
    }
}
