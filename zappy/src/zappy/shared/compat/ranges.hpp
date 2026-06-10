/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ranges
*/

#pragma once

#include <ranges>

/// @brief Compatibility layer for std::ranges.
namespace zappy::compat::ranges {
#ifndef ZAPPY_COMPAT_RANGES_FORCE
#ifdef ZAPPY_COMPAT_FORCE
#define ZAPPY_COMPAT_RANGES_FORCE
#endif
#endif

#if !defined(ZAPPY_COMPAT_RANGES_FORCE) && defined(__cpp_lib_ranges_to_container)
template <typename ContainerT>
inline constexpr auto to = std::ranges::to<ContainerT>;
#else
template <typename ContainerT>
struct ClosureTo {
    template <typename RangeT>
    friend constexpr auto operator|(RangeT&& range, ClosureTo /*closure*/) {
        auto rangeRef = std::forward<RangeT>(range);

        return ContainerT{
            rangeRef.begin(),
            rangeRef.end(),
        };
    }
};

/// @brief Compatibility layer for std::ranges::to.
template <typename ContainerT>
[[nodiscard]] constexpr auto to() {
    return ClosureTo<ContainerT>{};
}
#endif
}  // namespace zappy::compat::ranges
