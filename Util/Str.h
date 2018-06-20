//          Copyright LibreMaple Team 2018 - 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)
#pragma once

#include <cstdio>
#include <string>
#include <type_traits>
#include <utility>

namespace jrc::str
{
template<typename N, typename = std::enable_if_t<std::is_integral_v<N>>>
[[nodiscard]] std::string to_hex(N n) noexcept
{
    static_assert(sizeof(N) == sizeof(unsigned char)
                      || sizeof(N) == sizeof(unsigned short)
                      || sizeof(N) == sizeof(unsigned)
                      || sizeof(N) == sizeof(unsigned long)
                      || sizeof(N) == sizeof(unsigned long long),
                  "Unrecognized integral type.");

    char ch_buf[19];
    int len;

    if constexpr (sizeof(N) == sizeof(unsigned char)) {
        len = std::snprintf(
            ch_buf, 19, "0x%.2hhX", reinterpret_cast<unsigned char>(n));
    } else if constexpr (sizeof(N) == sizeof(unsigned short)) {
        len = std::snprintf(
            ch_buf, 19, "0x%.2hX", reinterpret_cast<unsigned short>(n));
    } else if constexpr (sizeof(N) == sizeof(unsigned)) {
        len = std::snprintf(
            ch_buf, 19, "0x%.2X", reinterpret_cast<unsigned>(n));
    } else if constexpr (sizeof(N) == sizeof(unsigned long)) {
        len = std::snprintf(
            ch_buf, 19, "0x%.2lX", reinterpret_cast<unsigned long>(n));
    } else {
        len = std::snprintf(
            ch_buf, 19, "0x%.2llX", reinterpret_cast<unsigned long long>(n));
    }

    return std::string(ch_buf, static_cast<std::string::size_type>(len));
}

template<typename T>
[[nodiscard]] std::string::size_type get_len(const T& t) noexcept
{
    using no_cv_t = std::remove_cv_t<T>;

    static_assert(
        !std::is_same_v<no_cv_t,
                        const char*> && !std::is_same_v<no_cv_t, char*>,
        "Don't try to `concat()` together `char*`s. If needed, call "
        "`strlen()` yourself by convering your `char*` to a "
        "`std::string_view`. `char` arrays with compile-time size "
        "work fine.");

    if constexpr (std::is_same_v<std::remove_reference_t<no_cv_t>, char>) {
        return 1;
    } else if constexpr (std::is_array_v<T> && std::rank_v<T> == 1
                         && std::extent_v<T>> 0) {
        static_assert(
            std::is_same_v<std::decay_t<std::remove_extent_t<no_cv_t>>, char>,
            "Got an array, expected it to be of `char`.");

        return std::extent_v<T> - 1;
    } else {
        return t.length();
    }
}

/*
template<typename... Args>
[[nodiscard]] std::string concat(const Args&... args) noexcept
{
    std::string::size_type total_len = (0 + ... + get_len(args));

    std::string s;
    s.reserve(total_len);

    ((s += args), ...);
    return s;
}
*/
template<typename... Args>
[[nodiscard]] std::string concat(Args&&... args) noexcept
{
    std::string::size_type total_len = (0 + ... + get_len(args));

    std::string s;
    s.reserve(total_len);

    ((s += std::forward<Args>(args)), ...);
    return s;
}
} // namespace jrc::str
