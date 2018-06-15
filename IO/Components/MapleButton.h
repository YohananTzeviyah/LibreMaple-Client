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
#include "Button.h"

namespace jrc
{
//! A standard maplestory-button with 4 states and a texture for each state.
class MapleButton : public Button
{
public:
    MapleButton(nl::node src, Point<std::int16_t> position);
    MapleButton(nl::node src, std::int16_t x, std::int16_t y);
    MapleButton(nl::node src);

    void draw(Point<std::int16_t> parent_pos) const;
    Rectangle<std::int16_t> bounds(Point<std::int16_t> parent_pos) const;

private:
    Texture textures[NUM_STATES];
};
} // namespace jrc
