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
#include "../../Template/EnumMap.h"
#include "Layer.h"
#include "Obj.h"
#include "Tile.h"
#include "boost/container/flat_map.hpp"

#include <vector>

namespace jrc
{
//! A tile and obj layer.
class TilesObjs
{
public:
    TilesObjs(nl::node src);
    TilesObjs();

    void draw(Point<std::int16_t> view_pos, float alpha) const;
    void update();

private:
    boost::container::flat_multimap<std::uint8_t, Tile> tiles;
    boost::container::flat_multimap<std::uint8_t, Obj> objs;
};

//! The collection of tile and obj layers on a map.
class MapTilesObjs
{
public:
    MapTilesObjs(nl::node src);
    MapTilesObjs();

    void
    draw(Layer::Id layer, Point<std::int16_t> view_pos, float alpha) const;
    void update();

private:
    EnumMap<Layer::Id, TilesObjs> layers;
};
} // namespace jrc
