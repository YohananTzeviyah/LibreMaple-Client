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
#include "Portal.h"

namespace jrc
{
Portal::Portal(const Animation* a,
               Type t,
               std::string&& nm,
               bool intramap,
               Point<std::int16_t> p,
               std::int32_t tid,
               std::string&& tnm)
    : animation(a),
      type(t),
      name(nm),
      position(p),
      warpinfo(tid, intramap, std::move(tnm), std::move(nm))
{
    touched = false;
}

Portal::Portal()
    : Portal(nullptr, SPAWN, "", false, Point<std::int16_t>(), 0, "")
{
}

void Portal::update(Point<std::int16_t> playerpos)
{
    touched = bounds().contains(playerpos);
}

void Portal::draw(Point<std::int16_t> viewpos, float inter) const
{
    if (!animation || (type == HIDDEN && !touched)) {
        return;
    }

    animation->draw(position + viewpos, inter);
}

std::string Portal::get_name() const
{
    return name;
}

Portal::Type Portal::get_type() const
{
    return type;
}

Point<std::int16_t> Portal::get_position() const
{
    return position;
}

Rectangle<std::int16_t> Portal::bounds() const
{
    auto lt = position + Point<std::int16_t>(-25, -100);
    auto rb = position + Point<std::int16_t>(25, 25);
    return Rectangle<std::int16_t>(lt, rb);
}

Portal::WarpInfo Portal::getwarpinfo() const
{
    return warpinfo;
}
} // namespace jrc
