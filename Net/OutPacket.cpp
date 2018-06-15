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
#include "OutPacket.h"

#include "Session.h"

#include <chrono>

namespace jrc
{
OutPacket::OutPacket(std::int16_t opcode)
{
    write_short(opcode);
}

bool OutPacket::dispatch() noexcept
{
    return Session::get().write(bytes.data(), bytes.size());
}

void OutPacket::skip(std::size_t count)
{
    for (std::size_t i = 0; i < count; ++i) {
        bytes.push_back(0);
    }
}

void OutPacket::write_byte(std::int8_t ch)
{
    bytes.push_back(ch);
}

void OutPacket::write_short(std::int16_t sh)
{
    for (std::size_t i = 0; i < 2; ++i) {
        write_byte(static_cast<std::int8_t>(sh));
        sh >>= 8;
    }
}

void OutPacket::write_int(std::int32_t in)
{
    for (std::size_t i = 0; i < 4; ++i) {
        write_byte(static_cast<std::int8_t>(in));
        in >>= 8;
    }
}

void OutPacket::write_long(std::int64_t lg)
{
    for (std::size_t i = 0; i < 8; ++i) {
        write_byte(static_cast<std::int8_t>(lg));
        lg >>= 8;
    }
}

void OutPacket::write_time()
{
    auto duration = std::chrono::steady_clock::now().time_since_epoch();
    auto since_epoch =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    auto timestamp = static_cast<std::int32_t>(since_epoch.count());
    write_int(timestamp);
}

void OutPacket::write_point(Point<std::int16_t> position)
{
    write_short(position.x());
    write_short(position.y());
}

void OutPacket::write_string(std::string_view str)
{
    std::int16_t length = static_cast<std::int16_t>(str.length());
    write_short(length);

    for (std::int16_t i = 0; i < length; ++i) {
        write_byte(str[i]);
    }
}
} // namespace jrc
