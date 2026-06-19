/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Billboard
*/

#pragma once

#include <raylib.h>

#include <cstdint>
#include <stdexcept>
#include <string>

#include "Texture.hpp"

namespace zappy::gui::render {

class Billboard : public Texture {
  public:
    explicit Billboard(std::string_view path);
    ~Billboard();


  private:
    
};

class BillboardException : public std::runtime_error {
  public:
    explicit BillboardException(const std::string& message) : std::runtime_error(message) {}
};
}  // namespace zappy::gui::render
