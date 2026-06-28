/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** MusicManager
*/

#pragma once

#include <cstdint>
#include <deque>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string_view>

#include "objects/Music.hpp"
#include "zappy/shared/exception/Exception.hpp"

namespace zappy::gui::render {
class MusicManager {
  public:
    MusicManager();
    ~MusicManager();
    MusicManager(const MusicManager&) = delete;
    MusicManager& operator=(const MusicManager&) = delete;
    MusicManager(MusicManager&& other) noexcept = default;
    MusicManager& operator=(MusicManager&& other) noexcept = default;

    void init();
    void specialInit();
    void update(std::uint32_t timeUnit);
    void update();
    void play();
    void stop();
    void pause();
    void resume();

    void addMusic(std::string_view path, bool loop = true, float volume = 1.0F);

    void setCurrentMusicIndex(unsigned int index);
    void nextMusic();
    void previousMusic();

    void setVolume(float volume);
    void setLooping(bool loop);
    void setSpeed(float speed);

    [[nodiscard]] std::optional<std::reference_wrapper<Music>> currentMusic() const { return _currentMusic; }

    [[nodiscard]] unsigned int currentMusicIndex() const { return _currentMusicIndex; }
    [[nodiscard]] bool isPlaying() const;

  private:
    std::optional<std::reference_wrapper<Music>> _currentMusic;
    std::deque<Music> _musics;
    unsigned int _currentMusicIndex{0};
    bool _playing{false};
};

class MusicManagerException : public zappy::exception::Exception {
  public:
    explicit MusicManagerException(std::string_view message) : zappy::exception::Exception{message} {}
};
};  // namespace zappy::gui::render
