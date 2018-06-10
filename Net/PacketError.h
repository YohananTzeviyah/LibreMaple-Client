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
#include "../Util/Str.h"

#include <stdexcept>
#include <string>
#include <string_view>

namespace jrc
{
//! Error which is thrown by packet handling functions.
class PacketError : public std::runtime_error
{
public:
    explicit PacketError(std::string_view msg) noexcept
        : std::runtime_error(str::concat(PREFIX, msg))
    {
    }

    //! Explicit copy constructor so that we can assure the compiler that
    //! it does not throw (because doing so results in UB).
    PacketError(const PacketError& other) noexcept = default;

private:
    static constexpr const std::string_view PREFIX = "[Packet error] ";
};
} // namespace jrc
