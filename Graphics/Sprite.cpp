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
#include "Sprite.h"

namespace jrc
{
Sprite::Sprite(const Animation& a, const DrawArgument& s_args)
    : animation(a), state_args(s_args)
{
}

Sprite::Sprite(nl::node src, const DrawArgument& s_args)
    : animation(src), state_args(s_args)
{
}

Sprite::Sprite(nl::node src) : Sprite(src, {})
{
}

Sprite::Sprite() = default;

void Sprite::draw(Point<std::int16_t> parent_pos, float alpha) const
{
    auto abs_args = state_args + parent_pos;
    animation.draw(abs_args, alpha);
}

bool Sprite::update(std::uint16_t timestep)
{
    return animation.update(timestep);
}

bool Sprite::update()
{
    return animation.update();
}
} // namespace jrc
