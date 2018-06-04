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
#include "Keyboard.h"

#include <cstdint>

namespace jrc
{
Keyboard::Keyboard() noexcept
{
    keymap[GLFW_KEY_LEFT] = {KeyType::ACTION, KeyAction::LEFT};
    keymap[GLFW_KEY_RIGHT] = {KeyType::ACTION, KeyAction::RIGHT};
    keymap[GLFW_KEY_UP] = {KeyType::ACTION, KeyAction::UP};
    keymap[GLFW_KEY_DOWN] = {KeyType::ACTION, KeyAction::DOWN};

    textactions[GLFW_KEY_BACKSPACE] = KeyAction::BACK;
    textactions[GLFW_KEY_ENTER] = KeyAction::RETURN;
    textactions[GLFW_KEY_SPACE] = KeyAction::SPACE;
    textactions[GLFW_KEY_TAB] = KeyAction::TAB;
}

std::int32_t Keyboard::shiftcode() const noexcept
{
    return GLFW_KEY_LEFT_SHIFT;
}

std::int32_t Keyboard::ctrlcode() const noexcept
{
    return GLFW_KEY_LEFT_CONTROL;
}

KeyAction::Id Keyboard::get_ctrl_action(std::int32_t keycode) const noexcept
{
    switch (keycode) {
    case GLFW_KEY_C:
        return KeyAction::COPY;
    case GLFW_KEY_V:
        return KeyAction::PASTE;
    default:
        return KeyAction::NO_ACTION;
    }
}

void Keyboard::assign(std::uint8_t key,
                      std::uint8_t tid,
                      KeyAction::Id action) noexcept
{
    if (KeyType::Id type = KeyType::type_by_id(tid)) {
        Mapping mapping{type, action};
        keymap[KEY_TABLE[key]] = mapping;
        maplekeys[key] = mapping;
    }
}

Keyboard::Mapping Keyboard::get_text_mapping(std::int32_t keycode,
                                             bool shift) const noexcept
{
    if (textactions.count(keycode)) {
        return {KeyType::ACTION, textactions.at(keycode)};
    } else if (keycode >= 48 && keycode <= 64) {
        return {KeyType::NUMBER,
                static_cast<KeyAction::Id>(keycode - (shift ? 15 : 0))};
    } else if (keycode >= 65 && keycode <= 90) {
        return {KeyType::LETTER,
                static_cast<KeyAction::Id>(keycode + (shift ? 0 : 32))};
    } else {
        switch (keycode) {
        case GLFW_KEY_LEFT:
        case GLFW_KEY_RIGHT:
        case GLFW_KEY_UP:
        case GLFW_KEY_DOWN:
            return keymap[keycode];
        default:
            return {};
        }
    }
}

Keyboard::Mapping Keyboard::get_mapping(std::int32_t keycode) const noexcept
{
    const auto iter = keymap.find(keycode);
    if (iter == keymap.end()) {
        return {};
    }

    return iter->second;
}

Keyboard::Mapping Keyboard::get_maple_mapping(std::uint8_t key_slot) const
    noexcept
{
    const auto iter = maplekeys.find(key_slot);
    if (iter == maplekeys.end()) {
        return {};
    }

    return iter->second;
}

const std::unordered_map<std::uint8_t, Keyboard::Mapping>&
Keyboard::get_maplekeys() const noexcept
{
    return maplekeys;
}
} // namespace jrc
