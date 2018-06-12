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
#include "MapTilesObjs.h"

namespace jrc
{
TilesObjs::TilesObjs(nl::node src)
{
    std::string tile_set = src["info"]["tS"];
    tile_set.append(".img", 4);
    for (auto tile_node : src["tile"]) {
        Tile tile{tile_node, tile_set};
        std::int8_t z = tile.get_z();
        tiles.emplace(z, std::move(tile));
    }

    for (auto obj_node : src["obj"]) {
        Obj obj{obj_node};
        std::int8_t z = obj.get_z();
        objs.emplace(z, std::move(obj));
    }
}

TilesObjs::TilesObjs() = default;

void TilesObjs::update()
{
    for (auto& [_, obj] : objs) {
        obj.update();
    }
}

void TilesObjs::draw(Point<std::int16_t> view_pos, float alpha) const
{
    for (auto& [_, obj] : objs) {
        obj.draw(view_pos, alpha);
    }

    for (auto& [_, tile] : tiles) {
        tile.draw(view_pos);
    }
}

MapTilesObjs::MapTilesObjs(nl::node src)
{
    for (auto iter : layers) {
        iter.second = src[iter.first];
    }
}

MapTilesObjs::MapTilesObjs() = default;

void MapTilesObjs::draw(Layer::Id layer,
                        Point<std::int16_t> view_pos,
                        float alpha) const
{
    layers[layer].draw(view_pos, alpha);
}

void MapTilesObjs::update()
{
    for (auto iter : layers) {
        iter.second.update();
    }
}
} // namespace jrc
