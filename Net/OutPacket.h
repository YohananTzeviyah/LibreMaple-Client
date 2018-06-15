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
#include "../Template/Point.h"

#include <cstdint>
#include <string>
#include <vector>

namespace jrc
{
//! A packet to be sent to the server. Used as a base class to create specific
//! packets.
class OutPacket
{
public:
    enum Opcode : std::uint16_t;

    //! Construct a packet by writing its opcode.
    OutPacket(std::int16_t opcode);

    bool dispatch() noexcept;

protected:
    //! Skip a number of bytes (filled with zeroes).
    void skip(std::size_t count);
    //! Write a byte.
    void write_byte(std::int8_t ch);
    //! Write a short.
    void write_short(std::int16_t sh);
    //! Write an int.
    void write_int(std::int32_t in);
    //! Write a long.
    void write_long(std::int64_t lg);

    //! Write a point, one short for x and one for y.
    void write_point(Point<std::int16_t> point);
    //! Write a timestamp as an integer.
    void write_time();
    //! Write a string. Writes the length as a short
    //! and then each individual character as a byte.
    void write_string(std::string_view str);

private:
    std::vector<std::int8_t> bytes;
};

//! Opcodes for `OutPacket`s associated with version 83 of the game.
enum OutPacket::Opcode : std::uint16_t {
    // Login
    LOGIN = 0x01,
    SERVERLIST_REREQUEST = 0x04,
    CHARLIST_REQUEST = 0x05,
    STATUS_REQUEST = 0x06,
    ACCEPT_TOS = 0x07,
    SET_GENDER = 0x08,
    AFTER_LOGIN = 0x09,
    REGISTER_PIN = 0x0A,
    SERVERLIST_REQUEST = 0x0B,
    SELECT_CHAR = 0x13,
    PLAYER_LOGIN = 0x14,
    NAME_CHAR = 0x15,
    CREATE_CHAR = 0x16,
    DELETE_CHAR = 0x17,
    PONG = 0x18,
    REGISTER_PIC = 0x1D,
    SELECT_CHAR_PIC = 0x1E,

    // Gameplay 1
    CHANGEMAP = 0x26,
    MOVE_PLAYER = 0x29,
    CLOSE_ATTACK = 0x2C,
    RANGED_ATTACK = 0x2D,
    MAGIC_ATTACK = 0x2E,
    TAKE_DAMAGE = 0x30,

    // Messaging
    GENERAL_CHAT = 0x31,

    // NPC interaction
    TALK_TO_NPC = 0x3A,
    NPC_TALK_MORE = 0x3C,
    NPC_SHOP_ACTION = 0x3D,

    // Inventory
    GATHER_ITEMS = 0x45,
    SORT_ITEMS = 0x46,
    MOVE_ITEM = 0x47,
    USE_ITEM = 0x48,
    SCROLL_EQUIP = 0x56,

    // Player
    SPEND_AP = 0x57,
    SPEND_SP = 0x5A,

    // Skill
    USE_SKILL = 0x5B,

    // Gameplay 2
    PARTY_OPERATION = 0x7C,
    CHANGE_KEYMAP = 0x87,
    MOVE_MONSTER = 0xBC,
    PICKUP_ITEM = 0xCA,
    PLAYER_UPDATE = 0xDF,

    // Custom
    HASH_CHECK = 0x7530
};
} // namespace jrc
