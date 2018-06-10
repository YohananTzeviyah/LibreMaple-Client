//////////////////////////////////////////////////////////////////////////////
// This file is part of the LibreMaple MMORPG client                        //
// Copyright © 2015-2016 Daniel Allendorf, 2018-2019 LibreMaple Team        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "../Console.h"
#include "boost/bimap.hpp"
#include "boost/bimap/unordered_set_of.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>

namespace jrc
{
namespace string_conversion
{
template<typename T>
T or_default(const std::string& str, T def) noexcept
{
    try {
        return static_cast<T>(std::stoi(str));
    } catch (const std::exception& ex) {
        Console::get().print(__func__, ex);
        return def;
    }
}

template<typename T>
T or_zero(const std::string& str) noexcept(noexcept(T(0)))
{
    return or_default<T>(str, T(0));
}
}; // namespace string_conversion

namespace string_format
{
//! Format a number string so that each 3 decimal points
//! are seperated by a ',' character.
void split_number(std::string& input);

//! Prefix an id with zeroes so that it has the minimum specified length.
[[nodiscard]] std::string extend_id(std::int32_t id, std::size_t length);
}; // namespace string_format

namespace bytecode
{
//! Check if a bit mask contains the specified value.
bool compare(std::int32_t mask, std::int32_t value);
} // namespace bytecode

namespace math
{
template<typename T, typename U>
T saturating_cast(U u)
{
    static_assert(std::numeric_limits<T>::is_specialized);
    static_assert(std::numeric_limits<U>::is_specialized);

    if constexpr (std::numeric_limits<T>::is_integer) {
        if constexpr (std::numeric_limits<U>::is_integer) {
            if constexpr (sizeof(T) > sizeof(U)) {
                return static_cast<T>(u);
            } else if constexpr (sizeof(T) == sizeof(U)) {
                if constexpr (std::numeric_limits<T>::is_signed ==
                              std::numeric_limits<U>::is_signed) {
                    return static_cast<T>(u);
                } else if constexpr (std::numeric_limits<T>::is_signed) {
                    return static_cast<T>(std::min(
                        u, static_cast<U>(std::numeric_limits<T>::max())));
                } else {
                    return static_cast<T>(std::max(
                        u, static_cast<U>(std::numeric_limits<T>::lowest())));
                }
            } else {
                return static_cast<T>(std::min(
                    std::max(u,
                             static_cast<U>(std::numeric_limits<T>::lowest())),
                    static_cast<U>(std::numeric_limits<T>::max())));
            }
        } else {
            return static_cast<T>(std::min(
                std::max(u, static_cast<U>(std::numeric_limits<T>::lowest())),
                static_cast<U>(std::numeric_limits<T>::max())));
        }
    } else {
        if constexpr (std::numeric_limits<U>::is_integer) {
            return static_cast<T>(u);
        } else {
            if constexpr (sizeof(T) >= sizeof(U)) {
                return static_cast<T>(u);
            } else {
                return static_cast<T>(std::min(
                    std::max(u,
                             static_cast<U>(std::numeric_limits<T>::lowest())),
                    static_cast<U>(std::numeric_limits<T>::max())));
            }
        }
    }
}
} // namespace math

namespace bimap
{
template<typename L, typename R>
using unordered_bimap = boost::bimap<boost::bimaps::unordered_set_of<L>,
                                     boost::bimaps::unordered_set_of<R>>;

template<typename L, typename R>
bool assign(unordered_bimap<L, R>& bm, const L& l, const R& r) noexcept
{
    using val_type = typename unordered_bimap<L, R>::value_type;

    if (auto left_iter = bm.left.find(l); left_iter != bm.left.end()) {
        bm.left.replace_data(left_iter, r);
        return false;
    }

    if (auto right_iter = bm.right.find(r); right_iter != bm.right.end()) {
        bm.right.replace_data(right_iter, l);
        return false;
    }

    bm.insert(std::move(val_type{l, r}));
    return true;
}
template<typename L, typename R>
bool assign(unordered_bimap<L, R>& bm, L&& l, R&& r) noexcept
{
    using val_type = typename unordered_bimap<L, R>::value_type;

    if (auto left_iter = bm.left.find(l); left_iter != bm.left.end()) {
        bm.left.replace_data(left_iter, std::move(r));
        return false;
    }

    if (auto right_iter = bm.right.find(r); right_iter != bm.right.end()) {
        bm.right.replace_data(right_iter, std::move(l));
        return false;
    }

    bm.insert(std::move(val_type{std::move(l), std::move(r)}));
    return true;
}

template<typename L, typename R>
bool left_assign(unordered_bimap<L, R>& bm, const L& l, const R& r) noexcept
{
    using val_type = typename unordered_bimap<L, R>::value_type;

    if (auto left_iter = bm.left.find(l); left_iter != bm.left.end()) {
        bm.left.replace_data(left_iter, r);
        return false;
    }

    bm.insert(std::move(val_type{l, r}));
    return true;
}
template<typename L, typename R>
bool left_assign(unordered_bimap<L, R>& bm, L&& l, R&& r) noexcept
{
    using val_type = typename unordered_bimap<L, R>::value_type;

    if (auto left_iter = bm.left.find(l); left_iter != bm.left.end()) {
        bm.left.replace_data(left_iter, std::move(r));
        return false;
    }

    bm.insert(std::move(val_type{std::move(l), std::move(r)}));
    return true;
}

template<typename L, typename R>
bool right_assign(unordered_bimap<L, R>& bm, const R& r, const L& l) noexcept
{
    using val_type = typename unordered_bimap<L, R>::value_type;

    if (auto right_iter = bm.right.find(r); right_iter != bm.right.end()) {
        bm.right.replace_data(right_iter, l);
        return false;
    }

    bm.insert(std::move(val_type{l, r}));
    return true;
}
template<typename L, typename R>
bool right_assign(unordered_bimap<L, R>& bm, R&& r, L&& l) noexcept
{
    using val_type = typename unordered_bimap<L, R>::value_type;

    if (auto right_iter = bm.right.find(r); right_iter != bm.right.end()) {
        bm.right.replace_data(right_iter, std::move(l));
        return false;
    }

    bm.insert(std::move(val_type{std::move(l), std::move(r)}));
    return true;
}
} // namespace bimap
} // namespace jrc
