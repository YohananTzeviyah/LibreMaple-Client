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
constexpr std::string_view MACS = "68-5D-43-F8-B8-6C, 7A-79-19-8B-31-3F";
constexpr std::string_view HWID = "685D43F8_B86C7A79";

//! Tell the server which character was picked.
//! Opcode: SELECT_CHAR(19)
class SelectCharPacket : public OutPacket
{
public:
    SelectCharPacket(std::int32_t cid) : OutPacket(SELECT_CHAR)
    {
        write_int(cid);
        write_string(MACS);
        write_string(HWID);
    }
};

//! Registers a pic and tells the server which character was picked.
//! Opcode: REGISTER_PIC(29)
class RegisterPicPacket : public OutPacket
{
public:
    RegisterPicPacket(std::int32_t cid, std::string_view pic)
        : OutPacket(REGISTER_PIC)
    {
        skip(1);

        write_int(cid);
        write_string(MACS);
        write_string(HWID);
        write_string(pic);
    }
};

//! Requests using the specified character with the specified pic.
//! Opcode: SELECT_CHAR_PIC(30)
class SelectCharPicPacket : public OutPacket
{
public:
    SelectCharPicPacket(std::string_view pic, std::int32_t cid)
        : OutPacket(SELECT_CHAR_PIC)
    {
        write_string(pic);
        write_int(cid);
        write_string(MACS);
        write_string(HWID);
    }
};

//! Requests deleting the specified character with the specified pic.
//! Opcode: DELETE_PIC(23)
class DeleteCharPacket : public OutPacket
{
public:
    DeleteCharPacket(std::string_view pic, std::int32_t cid)
        : OutPacket(DELETE_CHAR)
    {
        write_string(pic);
        write_int(cid);
    }
};
} // namespace jrc
