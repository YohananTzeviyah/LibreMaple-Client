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
#include "Misc.h"

namespace jrc
{
namespace string_format
{
void split_number(std::string& input)
{
    for (auto i = input.length(); i > 3; i -= 3) {
        input.insert(i - 3, 1, ',');
    }
}

std::string extend_id(std::int32_t id, std::size_t length)
{
    std::string strid = std::to_string(id);
    if (strid.size() < length) {
        strid.insert(0, length - strid.size(), '0');
    }
    return strid;
}
} // namespace string_format

namespace bytecode
{
bool inline compare(std::int32_t mask, std::int32_t value)
{
    return (mask & value) != 0;
}
} // namespace bytecode
} // namespace jrc
