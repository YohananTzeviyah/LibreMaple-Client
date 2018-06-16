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
#include "../Net/Packets/GameplayPackets.h"
#include "../Template/Enumeration.h"
#include "KeyAction.h"
#include "KeyType.h"
#include "Window.h"

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
        KeyAction::Id action;

        Mapping() noexcept : type(KeyType::NONE)
        {
        }

        Mapping(KeyType::Id in_type, KeyAction::Id in_action) noexcept
            : type(in_type), action(in_action)
        {
        }

        explicit operator bool() const noexcept
        {
            return type != KeyType::NONE;
        }

        template<int I>
        auto const& get() const noexcept
        {
            static_assert(I == 0 || I == 1,
                          "Cannot `jrc::Keyboard::Mapping::get<I>()` for "
                          "`I != 0 && I != 1`");

            if constexpr (I == 0) {
                return type;
            } else {
                return action;
            }
        }
    };

    Keyboard() noexcept;
    Keyboard(const Keyboard&) = delete;
    Keyboard(Keyboard&&) = delete;

    void
    assign(std::uint8_t key, std::uint8_t type, KeyAction::Id action) noexcept;

    std::int32_t shift_code() const noexcept;
    std::int32_t ctrl_code() const noexcept;
    KeyAction::Id get_ctrl_action(std::int32_t keycode) const noexcept;
    Mapping get_mapping(std::int32_t keycode) const noexcept;
    Mapping get_maple_mapping(std::uint8_t key_slot) const noexcept;
    const std::unordered_map<std::uint8_t, Mapping>& get_maplekeys() const
        noexcept;
    Mapping get_text_mapping(std::int32_t keycode, bool shift) const noexcept;
    //! Sends a `ChangeKeymapPacket` packet to the server. Returns `true` on
    //! success.
    bool send_mappings() const noexcept;
    void clear_mappings() noexcept;

    static constexpr std::int32_t align_key_parity(std::int32_t keycode) noexcept
    {
        switch (keycode) {
        case GLFW_KEY_RIGHT_ALT:
            return GLFW_KEY_LEFT_ALT;
        case GLFW_KEY_RIGHT_CONTROL:
            return GLFW_KEY_LEFT_CONTROL;
        case GLFW_KEY_RIGHT_SHIFT:
            return GLFW_KEY_LEFT_SHIFT;
        default:
            return keycode;
        }
    }

private:
    std::unordered_map<std::int32_t, Mapping> keymap;
    std::unordered_map<std::uint8_t, Mapping> maplekeys;
    std::unordered_map<std::int32_t, KeyAction::Id> textactions;
    std::unordered_map<std::int32_t, bool> key_state;
};

constexpr std::array<std::int32_t, 90> KEY_TABLE{0,
                                                 0,
                                                 GLFW_KEY_0,
                                                 GLFW_KEY_1,
                                                 GLFW_KEY_2,
                                                 GLFW_KEY_3,
                                                 GLFW_KEY_4,
                                                 GLFW_KEY_5,
                                                 GLFW_KEY_6,
                                                 GLFW_KEY_7,
                                                 GLFW_KEY_8,
                                                 GLFW_KEY_9,
                                                 GLFW_KEY_MINUS,
                                                 GLFW_KEY_EQUAL,
                                                 0,
                                                 0,
                                                 GLFW_KEY_Q,
                                                 GLFW_KEY_W,
                                                 GLFW_KEY_E,
                                                 GLFW_KEY_R,
                                                 GLFW_KEY_T,
                                                 GLFW_KEY_Y,
                                                 GLFW_KEY_U,
                                                 GLFW_KEY_I,
                                                 GLFW_KEY_O,
                                                 GLFW_KEY_P,
                                                 GLFW_KEY_LEFT_BRACKET,
                                                 GLFW_KEY_RIGHT_BRACKET,
                                                 0,
                                                 GLFW_KEY_LEFT_CONTROL,
                                                 GLFW_KEY_A,
                                                 GLFW_KEY_S,
                                                 GLFW_KEY_D,
                                                 GLFW_KEY_F,
                                                 GLFW_KEY_G,
                                                 GLFW_KEY_H,
                                                 GLFW_KEY_J,
                                                 GLFW_KEY_K,
                                                 GLFW_KEY_L,
                                                 GLFW_KEY_SEMICOLON,
                                                 GLFW_KEY_APOSTROPHE,
                                                 GLFW_KEY_GRAVE_ACCENT,
                                                 GLFW_KEY_LEFT_SHIFT,
                                                 GLFW_KEY_BACKSLASH,
                                                 GLFW_KEY_Z,
                                                 GLFW_KEY_X,
                                                 GLFW_KEY_C,
                                                 GLFW_KEY_V,
                                                 GLFW_KEY_B,
                                                 GLFW_KEY_N,
                                                 GLFW_KEY_M,
                                                 GLFW_KEY_COMMA,
                                                 GLFW_KEY_PERIOD,
                                                 0,
                                                 0,
                                                 0,
                                                 GLFW_KEY_LEFT_ALT,
                                                 GLFW_KEY_SPACE,
                                                 0,
                                                 GLFW_KEY_F1,
                                                 GLFW_KEY_F2,
                                                 GLFW_KEY_F3,
                                                 GLFW_KEY_F4,
                                                 GLFW_KEY_F5,
                                                 GLFW_KEY_F6,
                                                 GLFW_KEY_F7,
                                                 GLFW_KEY_F8,
                                                 GLFW_KEY_F9,
                                                 GLFW_KEY_F10,
                                                 GLFW_KEY_F11,
                                                 GLFW_KEY_F12,
                                                 GLFW_KEY_HOME,
                                                 0,
                                                 GLFW_KEY_PAGE_UP,
                                                 0,
                                                 0,
                                                 0,
                                                 0,
                                                 0,
                                                 GLFW_KEY_END,
                                                 0,
                                                 GLFW_KEY_PAGE_DOWN,
                                                 GLFW_KEY_INSERT,
                                                 GLFW_KEY_DELETE,
                                                 0,
                                                 0,
                                                 0,
                                                 GLFW_KEY_F11,
                                                 GLFW_KEY_F12,
                                                 0};
} // namespace jrc

namespace std
{
template<>
struct tuple_size<jrc::Keyboard::Mapping>
    : std::integral_constant<std::size_t, 2> {
};

template<>
struct tuple_element<0, jrc::Keyboard::Mapping> {
    using type = jrc::KeyType::Id;
};

template<>
struct tuple_element<1, jrc::Keyboard::Mapping> {
    using type = std::int32_t;
};
} // namespace std
