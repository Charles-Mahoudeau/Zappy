/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** IFileDescriptor
*/

#pragma once

namespace zappy::io {
class IFileDescriptor {
  public:
    IFileDescriptor() = default;
    IFileDescriptor(const IFileDescriptor&) = default;
    IFileDescriptor(IFileDescriptor&&) = default;
    virtual ~IFileDescriptor() = default;

    IFileDescriptor& operator=(const IFileDescriptor&) = default;
    IFileDescriptor& operator=(IFileDescriptor&&) = default;

    [[nodiscard]] virtual int fd() const = 0;
};
}  // namespace zappy::io
