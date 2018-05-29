//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "KeyAction.h"
#include "KeyType.h"

#include <cstdint>
#include <tuple>
#include <unordered_map>

namespace jrc
{
class Keyboard
{
public:
    struct Mapping {
        KeyType::Id type;
        std::int32_t action;

        Mapping() : type(KeyType::NONE), action(0)
        {
        }

        Mapping(KeyType::Id in_type, std::int32_t in_action)
            : type(in_type), action(in_action)
        {
        }

        template<int I>
        auto const& get() const {
            if constexpr (I == 0) {
                return type;
            } else if constexpr (I == 1) {
                return action;
            }
        }
    };

    Keyboard();

    //! Throws
    void load_from_toml();
    void assign(std::uint8_t key, std::uint8_t type, std::int32_t action);

    std::int32_t shiftcode() const;
    std::int32_t ctrlcode() const;
    KeyAction::Id get_ctrl_action(std::int32_t keycode) const;
    Mapping get_mapping(std::int32_t keycode) const;
    Mapping get_text_mapping(std::int32_t keycode, bool shift) const;

private:
    std::unordered_map<std::int32_t, Mapping> keymap;
    std::unordered_map<std::int32_t, Mapping> maplekeys;
    std::unordered_map<std::int32_t, KeyAction::Id> textactions;
    std::unordered_map<std::int32_t, bool> keystate;
};
} // namespace jrc

namespace std
{
template<> struct tuple_size<jrc::Keyboard::Mapping> :
    std::integral_constant<std::size_t, 2> {};

template<> struct tuple_element<0, jrc::Keyboard::Mapping> {
    using type = jrc::KeyType::Id;
};
template<> struct tuple_element<1, jrc::Keyboard::Mapping> {
    using type = std::int32_t;
};
}
