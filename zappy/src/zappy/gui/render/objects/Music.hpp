/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Music
*/

#pragma once

#include <raylib.h>

#include <stdexcept>
#include <string_view>

namespace zappy::gui::render {
class Music {
  public:
    Music() = default;
    explicit Music(std::string_view path, bool loop = true, float volume = 1.0F);
    ~Music();
    Music(const Music&) = delete;
    Music& operator=(const Music&) = delete;

    Music(Music&& other) noexcept;
    Music& operator=(Music&& other) noexcept;

    void play();
    void stop();
    void pause();
    void resume();
    void setVolume(float volume);
    void update();
    void setLooping(bool loop);
    void setSpeed(float speed);

    [[nodiscard]] bool isPlaying() const;
    [[nodiscard]] bool isLooping() const;

  protected:
  private:
    ::Music _music{};
};

class MusicException : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
};
}  // namespace zappy::gui::render
