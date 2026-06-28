#include "MusicManager.hpp"

#include <raylib.h>

#include <algorithm>
#include <cstdint>
#include <format>
#include <functional>
#include <iostream>

namespace zappy::gui::render {

MusicManager::MusicManager() : _available{initAudioDevice()} {
    if (!_available) {
        std::cerr << "MusicManager: audio device unavailable, music disabled" << std::endl;
    }
}

MusicManager::~MusicManager() {
    _currentMusic.reset();
    _musics.clear();
    if (IsAudioDeviceReady()) {
        CloseAudioDevice();
    }
}

void MusicManager::init() {
    SetMasterVolume(1.0F);
    if (_currentMusic.has_value()) {
        _currentMusic.value().get().play();
        _playing = true;
    } else {
        std::cerr << "MusicManager: No music to play" << std::endl;
        _playing = false;
    }
}

void MusicManager::specialInit() {
    addMusic("assets/music/ambient.mp3", true, 0.5F);
    addMusic("assets/music/waiting.mp3", true, 0.5F);
    init();
}

void MusicManager::update(std::uint32_t timeUnit) {
    if (_currentMusic.has_value()) {
        const float speed = std::clamp(1.0F + (static_cast<float>(timeUnit) / 200.0F), 1.0F, 5.0F);
        _currentMusic.value().get().setSpeed(speed);
        _currentMusic.value().get().update();
    }
}

void MusicManager::update() {
    if (_currentMusic.has_value()) {
        _currentMusic.value().get().update();
    }
}

void MusicManager::play() {
    if (_currentMusic.has_value()) {
        _currentMusic.value().get().play();
        _playing = true;
    }
}

void MusicManager::stop() {
    if (_currentMusic.has_value()) {
        _currentMusic.value().get().stop();
        _playing = false;
    }
}

void MusicManager::pause() {
    if (_currentMusic.has_value()) {
        _currentMusic.value().get().pause();
        _playing = false;
    }
}

void MusicManager::resume() {
    if (_currentMusic.has_value()) {
        _currentMusic.value().get().resume();
        _playing = true;
    }
}

void MusicManager::addMusic(std::string_view path, bool loop, float volume) {
    if (!_available) {
        return;
    }
    _musics.emplace_back(path, loop, volume);
    if (_musics.size() == 1) {
        setCurrentMusicIndex(0);
    }
}

bool MusicManager::isPlaying() const { return _playing; }

bool MusicManager::initAudioDevice() {
    InitAudioDevice();
    return IsAudioDeviceReady();
}

void MusicManager::setCurrentMusicIndex(unsigned int index) {
    const auto musicSize = static_cast<unsigned int>(_musics.size());
    if (index < musicSize) {
        stop();
        _currentMusicIndex = index;
        _currentMusic = std::ref(_musics.at(_currentMusicIndex));
        play();
    } else {
        throw MusicManagerException{
            std::format("Invalid music index: {}. Must be between 0 and {}", index, musicSize - 1)};
    }
}
void MusicManager::nextMusic() {
    if (_musics.empty()) {
        throw MusicManagerException{"No music available to play next"};
    }
    const auto nextIndex = (_currentMusicIndex + 1) % static_cast<unsigned int>(_musics.size());
    setCurrentMusicIndex(nextIndex);
}

void MusicManager::previousMusic() {
    if (_musics.empty()) {
        throw MusicManagerException{"No music available to play previous"};
    }
    const auto previousIndex =
        (_currentMusicIndex == 0) ? static_cast<unsigned int>(_musics.size()) - 1 : _currentMusicIndex - 1;
    setCurrentMusicIndex(previousIndex);
}

void MusicManager::setVolume(float volume) {
    if (_currentMusic.has_value()) {
        _currentMusic.value().get().setVolume(volume);
    } else {
        std::cerr << "MusicManager: No current music to set volume" << std::endl;
    }
}
void MusicManager::setLooping(bool loop) {
    if (_currentMusic.has_value()) {
        _currentMusic.value().get().setLooping(loop);
    } else {
        std::cerr << "MusicManager: No current music to set looping" << std::endl;
    }
}

void MusicManager::setSpeed(float speed) {
    if (_currentMusic.has_value()) {
        _currentMusic.value().get().setSpeed(speed);
    } else {
        std::cerr << "MusicManager: No current music to set speed" << std::endl;
    }
}
}  // namespace zappy::gui::render
