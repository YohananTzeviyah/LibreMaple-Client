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
#include "../../Graphics/Texture.h"
#include "nlnx/node.hpp"

namespace jrc
{
class MapleFrame
{
public:
    MapleFrame(nl::node src);
    MapleFrame();

    void draw(Point<std::int16_t> position,
              std::int16_t width,
              std::int16_t height) const;

private:
    Texture center;
    Texture east;
    Texture northeast;
    Texture north;
    Texture northwest;
    Texture west;
    Texture southwest;
    Texture south;
    Texture southeast;
    std::int16_t x_tile;
    std::int16_t y_tile;
};
} // namespace jrc
