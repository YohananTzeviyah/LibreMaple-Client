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
#include "../OutPacket.h"

#include <string_view>

namespace jrc
{
//! Accept the Terms of Service.
//! Opcode: ACCEPT_TOS(0x07)
class TOSPacket : public OutPacket
{
public:
    TOSPacket() : OutPacket(ACCEPT_TOS)
    {
        write_byte(1);
    }
};

//! Request to be logged-in to an account.
//! Opcode: LOGIN(0x01) [LOGIN_PASSWORD(0x01) for OdinMS-based sources]
class LoginPacket : public OutPacket
{
public:
    LoginPacket(std::string_view acc, std::string_view pass) : OutPacket(LOGIN)
    {
        write_string(acc);
        write_string(pass);
    }
};

//! Requests the list of worlds and channels.
//! Opcode: SERVERLIST_REQUEST(0x0B)
class ServerRequestPacket : public OutPacket
{
public:
    ServerRequestPacket() : OutPacket(SERVERLIST_REQUEST)
    {
    }
};

//! Requests the list of characters on a world.
//! Opcode: CHARLIST_REQUEST(0x05)
class CharlistRequestPacket : public OutPacket
{
public:
    CharlistRequestPacket(std::uint8_t world, std::uint8_t channel)
        : OutPacket(CHARLIST_REQUEST)
    {
        write_byte(0);
        write_byte(world);
        write_byte(channel);
    }
};

//! Requests being logged-in to a channel server with the specified character.
//! Opcode: PLAYER_LOGIN(0x14) [PLAYER_LOGGEDIN(0x14) for OdinMS-based sources]
class PlayerLoginPacket : public OutPacket
{
public:
    PlayerLoginPacket(std::int32_t cid) : OutPacket(PLAYER_LOGIN)
    {
        write_int(cid);
    }
};
} // namespace jrc
