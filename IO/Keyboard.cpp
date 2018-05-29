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
#include "Keyboard.h"

#include "../cpptoml/include/cpptoml.h"
#include <GLFW/glfw3.h>

#include <cstdint>

namespace jrc
{
constexpr std::int32_t KEY_TABLE[90] = {0,
                                  0,
                                  48,
                                  49,
                                  50,
                                  51,
                                  52,
                                  53,
                                  54,
                                  55,
                                  56,
                                  57, // number keys, up to key 0
                                  0,
                                  0,
                                  0,
                                  0,
                                  'Q',
                                  'W',
                                  'E',
                                  'R',
                                  'T',
                                  'Z',
                                  'U',
                                  'I',
                                  'O',
                                  'P',
                                  0,
                                  0,
                                  0, // first letter row, up to key 28
                                  GLFW_KEY_LEFT_CONTROL,
                                  'A',
                                  'S',
                                  'D',
                                  'F',
                                  'G',
                                  'H',
                                  'J',
                                  'K',
                                  'L',
                                  0,
                                  0,
                                  0, // second row, up to 41
                                  GLFW_KEY_LEFT_SHIFT,
                                  0,
                                  'Y',
                                  'X',
                                  'C',
                                  'V',
                                  'B',
                                  'N',
                                  'M',
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  GLFW_KEY_LEFT_ALT,
                                  GLFW_KEY_SPACE,
                                  0, // third row, up to 58
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
                                  GLFW_KEY_F12, // up to 70
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
                                  0,
                                  0,
                                  0};

    constexpr char const* KEY_NAMES[90] = {
                                          "",
                                          "",
                                          "1",
                                          "2",
                                          "3",
                                          "4",
                                          "5",
                                          "6",
                                          "7",
                                          "8",
                                          "9",
                                          "0", // number keys, up to key 0
                                          "",
                                          "",
                                          "",
                                          "",
                                          "Q",
                                          "W",
                                          "E",
                                          "R",
                                          "T",
                                          "Z",
                                          "U",
                                          "I",
                                          "O",
                                          "P",
                                          "",
                                          "",
                                          "", // first letter row, up to key 28
                                          "Left Control",
                                          "A",
                                          "S",
                                          "D",
                                          "F",
                                          "G",
                                          "H",
                                          "J",
                                          "K",
                                          "L",
                                          "",
                                          "",
                                          "", // second row, up to 41
                                          "Left Shift",
                                          "",
                                          "Y",
                                          "X",
                                          "C",
                                          "V",
                                          "B",
                                          "N",
                                          "M",
                                          "",
                                          "",
                                          "",
                                          "",
                                          "",
                                          "Left Alt",
                                          "Space",
                                          "", // third row, up to 58
                                          "F1",
                                          "F2",
                                          "F3",
                                          "F4",
                                          "F5",
                                          "F6",
                                          "F7",
                                          "F8",
                                          "F9",
                                          "F10",
                                          "F11",
                                          "F12", // up to 70
                                          "Home",
                                          "",
                                          "Page Up",
                                          "",
                                          "",
                                          "",
                                          "",
                                          "",
                                          "End",
                                          "",
                                          "Page Down",
                                          "Insert",
                                          "Delete",
                                          "",
                                          "",
                                          "",
                                          "",
                                          "",
                                          ""
    };

Keyboard::Keyboard()
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

std::int32_t Keyboard::shiftcode() const
{
    return GLFW_KEY_LEFT_SHIFT;
}

std::int32_t Keyboard::ctrlcode() const
{
    return GLFW_KEY_LEFT_CONTROL;
}

KeyAction::Id Keyboard::get_ctrl_action(std::int32_t keycode) const
{
    switch (keycode) {
    case GLFW_KEY_C:
        return KeyAction::COPY;
    case GLFW_KEY_V:
        return KeyAction::PASTE;
    default:
        return KeyAction::NOACTION;
    }
}

void Keyboard::load_from_toml()
{
    return;
    static std::unordered_map<std::string, std::uint8_t> key_name_map(90);
    if (key_name_map.empty()) {
        std::uint8_t k = 0;
        for (const auto key_name : KEY_NAMES) {
            key_name_map.try_emplace(key_name, k);
            ++k;
        }
    }

    const auto key_config = cpptoml::parse_file("keyboard.toml");
    if (!key_config) {
        throw std::runtime_error("Could not parse keyboard.toml");
    }
    const auto keybinds = key_config->get_table("keybinds");
    if (!keybinds) {
        throw std::runtime_error(
            R"(keyboard.toml has no table named "keybinds")");
    }

    for (const auto& [key_name, _] : *keybinds) {
        const auto mapped_name = key_name_map.find(key_name);
        if (mapped_name != key_name_map.end()) {
            const auto k = mapped_name->second;
            if (const auto o_arr = keybinds->get_array_of<std::int64_t>(key_name)) {
                const auto arr = *o_arr;
                const auto type = static_cast<std::uint8_t>(arr.at(0));
                const auto action = static_cast<std::int32_t>(arr.at(1));

                assign(k, type, action);
            } else {
                throw std::runtime_error("Expected an array of integers");
            }
        } else {
            throw std::runtime_error(
                R"(Unrecognized key name in keyboard.toml: ")" + key_name + '"');
        }
    }
}

void Keyboard::assign(std::uint8_t key, std::uint8_t tid, std::int32_t action)
{
    if (KeyType::Id type = KeyType::typebyid(tid)) {
        Mapping mapping{type, action};
        keymap[KEY_TABLE[key]] = mapping;
        maplekeys[key] = mapping;
    }
}

Keyboard::Mapping Keyboard::get_text_mapping(std::int32_t keycode, bool shift) const
{
    if (textactions.count(keycode)) {
        return {KeyType::ACTION, textactions.at(keycode)};
    } else if (keycode >= 48 && keycode <= 64) {
        return {KeyType::NUMBER, keycode - (shift ? 15 : 0)};
    } else if (keycode >= 65 && keycode <= 90) {
        return {KeyType::LETTER, keycode + (shift ? 0 : 32)};
    } else {
        switch (keycode) {
        case GLFW_KEY_LEFT:
        case GLFW_KEY_RIGHT:
        case GLFW_KEY_UP:
        case GLFW_KEY_DOWN:
            return keymap.at(keycode);
        default:
            return {};
        }
    }
}

Keyboard::Mapping Keyboard::get_mapping(std::int32_t keycode) const
{
    auto iter = keymap.find(keycode);
    if (iter == keymap.end()) {
        return {};
    }

    return iter->second;
}
} // namespace jrc
