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
#include "Obj.h"

#include "nlnx/nx.hpp"

namespace jrc
{
Obj::Obj(nl::node src)
    : animation{nl::nx::map["Obj"][src["oS"] + ".img"][src["l0"]][src["l1"]]
                           [src["l2"]]},
      pos{src["x"], src["y"]},
      z{src["z"]},
      flip{src["f"].get_bool()}

{
}

void Obj::update()
{
    animation.update();
}

void Obj::draw(Point<std::int16_t> view_pos, float inter) const
{
    animation.draw(DrawArgument{pos + view_pos, flip}, inter);
}

std::uint8_t Obj::get_z() const noexcept
{
    return z;
}
} // namespace jrc
