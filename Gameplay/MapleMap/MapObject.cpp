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
#include "MapObject.h"

namespace jrc
{
MapObject::MapObject(std::int32_t o, Point<std::int16_t> p) : oid(o)
{
    set_position(p);
    active = true;
}

std::int8_t MapObject::update(const Physics& physics)
{
    physics.move_object(ph_obj);
    return ph_obj.fh_layer;
}

void MapObject::set_position(std::int16_t x, std::int16_t y)
{
    ph_obj.set_x(x);
    ph_obj.set_y(y);
}

void MapObject::set_position(Point<std::int16_t> position)
{
    std::int16_t x = position.x();
    std::int16_t y = position.y();
    set_position(x, y);
}

void MapObject::activate()
{
    active = true;
}

void MapObject::deactivate()
{
    active = false;
}

bool MapObject::is_active() const
{
    return active;
}

std::int8_t MapObject::get_layer() const
{
    return ph_obj.fh_layer;
}

std::int32_t MapObject::get_oid() const
{
    return oid;
}

Point<std::int16_t> MapObject::get_position() const
{
    return ph_obj.get_position();
}
} // namespace jrc
