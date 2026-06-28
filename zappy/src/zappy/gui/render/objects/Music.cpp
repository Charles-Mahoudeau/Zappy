#include "Music.hpp"

#include <raylib.h>

#include <string>
#include <string_view>

namespace zappy::gui::render {
Music::Music(std::string_view path, bool loop, float volume) {
    _music = LoadMusicStream(std::string{path}.c_str());
    if (_music.ctxData == nullptr) {
        throw MusicException{"Failed to load music from path: " + std::string{path}};
    }
    _music.looping = loop;
    SetMusicVolume(_music, volume);
}
Music::Music(Music&& other) noexcept : _music(other._music) { other._music.ctxData = nullptr; }

Music& Music::operator=(Music&& other) noexcept {
    if (this != &other) {
        if (_music.ctxData != nullptr) {
            UnloadMusicStream(_music);
        }
        _music = other._music;
        other._music.ctxData = nullptr;
    }
    return *this;
}

void Music::play() { PlayMusicStream(_music); }
void Music::stop() { StopMusicStream(_music); }
void Music::pause() { PauseMusicStream(_music); }
void Music::resume() { ResumeMusicStream(_music); }
void Music::setVolume(float volume) { SetMusicVolume(_music, volume); }
void Music::update() { UpdateMusicStream(_music); }
void Music::setLooping(bool loop) { _music.looping = loop; }
void Music::setSpeed(float speed) { SetMusicPitch(_music, speed); }

bool Music::isPlaying() const { return IsMusicStreamPlaying(_music); }
bool Music::isLooping() const { return _music.looping; }

Music::~Music() {
    if (_music.ctxData != nullptr) {
        UnloadMusicStream(_music);
    }
}
}  // namespace zappy::gui::render
