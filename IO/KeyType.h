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
#include "KeyAction.h"

#include <array>
#include <cstdint>

namespace jrc::KeyType
{
//! Keytypes determine how a keycode is interpreted.
enum Id : std::uint8_t {
    NONE = 0,
    SKILL = 1,
    ITEM = 2,
    CASH = 3,
    MENU = 4,
    ACTION = 5,
    FACE = 6,
    MACRO = 8,
    LETTER = 9,
    NUMBER = 10,
    LENGTH
};

constexpr Id type_by_id(std::uint8_t id) noexcept
{
    if (id <= NONE || id >= LENGTH) {
        return NONE;
    }

    return static_cast<Id>(id);
}

constexpr Id type_by_action(std::int32_t action) noexcept
{
    constexpr const std::array<Id, KeyAction::LAST> ACTIONS_TO_TYPES{
        MENU,   MENU,   MENU,   MENU,   MENU,   MENU, MENU, MENU, MENU, MENU,
        MENU,   MENU,   MENU,   MENU,   MENU,   MENU, MENU, MENU, MENU, MENU,
        MENU,   MENU,   MENU,   MENU,   MENU,   MENU, MENU, MENU, NONE, NONE,
        NONE,   NONE,   NONE,   NONE,   NONE,   NONE, NONE, NONE, NONE, NONE,
        NONE,   NONE,   NONE,   NONE,   NONE,   NONE, NONE, NONE, NONE, NONE,
        ACTION, ACTION, ACTION, ACTION, ACTION, NONE, NONE, NONE, NONE, NONE,
        NONE,   NONE,   NONE,   NONE,   NONE,   NONE, NONE, NONE, NONE, NONE,
        NONE,   NONE,   NONE,   NONE,   NONE,   NONE, NONE, NONE, NONE, NONE,
        NONE,   NONE,   NONE,   NONE,   NONE,   NONE, NONE, NONE, NONE, NONE,
        NONE,   NONE,   NONE,   NONE,   NONE,   NONE, NONE, NONE, NONE, NONE,
        FACE,   FACE,   FACE,   FACE,   FACE,   FACE, FACE};

    if (KeyAction::is_skill(action)) {
        return SKILL;
    } else if (KeyAction::is_item(action)) {
        return ITEM;
    }

    return ACTIONS_TO_TYPES[action];
}
} // namespace jrc::KeyType
