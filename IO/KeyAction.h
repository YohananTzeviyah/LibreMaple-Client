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
#include <cstdint>

namespace jrc::KeyAction
{
//! Maple-specific keycodes, sent via the Keymap Packet.
enum Id : std::int32_t {
    EQUIPMENT_TAB = 0,
    INVENTORY = 1,
    CHAR_STATS = 2,
    SKILL_TAB = 3,
    BUDDY_TAB = 4,
    WORLD_MAP = 5,
    MESSENGER = 6,
    MINI_MAP = 7,
    QUEST_LOG = 8,
    KEY_CONFIG = 9,
    TO_ALL = 10,
    WHISPER = 11,
    TO_PARTY = 12,
    TO_FRIEND = 13,
    MAIN_MENU = 14,
    TOGGLE_QUICK_SLOT = 15,
    CHAT_WINDOW = 16,
    GUILD_TAB = 17,
    TO_GUILD = 18,
    PARTY_TAB = 19,
    HELPER = 20,
    MONSTER_BOOK = 21,
    CASH_SHOP = 22,
    TO_ALLIANCE = 23,
    PARTY_SEARCH = 24,
    TO_TALK = 25,
    MEDALS = 26,
    BOSS = 27,
    TO_EXPED = 28,
    PROFESSION = 29,
    ITEM_POT = 30,
    EVENT = 31,
    LEVEL_UP_GUIDE = 32,
    SILENT_CRUSADE = 33,
    BITS = 34,
    CHECK_NEW_TALK = 35,
    TOTS_KNOWHOW = 36,
    EPISODE = 37,
    HELPER_2 = 38, // ???
    GUIDE = 39,
    TO_SPOUSE = 40,
    EQUIP_ENHANCE = 41,
    PICK_UP = 50,
    SIT = 51,
    ATTACK = 52,
    JUMP = 53,
    NPC_CHAT = 54,
    LEFT = 60,
    RIGHT = 61,
    UP = 62,
    DOWN = 63,
    BACK = 64,
    RETURN = 65,
    SPACE = 66,
    TAB = 67,
    COPY = 68,
    PASTE = 69,
    NO_ACTION = 70,
    FACE_1 = 100,
    FACE_2 = 101,
    FACE_3 = 102,
    FACE_4 = 103,
    FACE_5 = 104,
    FACE_6 = 105,
    FACE_7 = 106,

    LAST
};

constexpr bool is_key_action(std::int32_t id) noexcept
{
    return id >= 0 && id < static_cast<std::int32_t>(LAST);
}

#ifdef NDEBUG
constexpr
#else
inline
#endif
    Id
    action_by_id(std::int32_t id) noexcept
{
#ifndef NDEBUG
    if (!is_key_action(id)) {
        Console::get().print("KeyAction::action_by_id(" + std::to_string(id)
                             + ") is not valid");
    }
#endif

    return static_cast<Id>(id);
}

constexpr bool is_item(std::int32_t id) noexcept
{
    return id >= 1'000'000;
}

//! We use negative numbers to represent skill IDs, e.g. a skill with ID
//! 1,000,001 is represented as `-1'000'001`.
constexpr bool is_skill(std::int32_t id) noexcept
{
    return id < 0;
}
} // namespace jrc::KeyAction
