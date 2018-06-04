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
#include "../../Graphics/Animation.h"
#include "MapObject.h"

#include <vector>

namespace jrc
{
class Reactor : public MapObject
{
public:
    Reactor(std::int32_t oid,
            std::int32_t rid,
            std::int8_t state,
            Point<std::int16_t> position);

    void draw(double viewx, double viewy, float alpha) const override;

    void destroy(std::int8_t state, Point<std::int16_t> position);

private:
    std::int32_t oid;
    std::int32_t rid;
    std::int8_t state;

    Animation normal;
};
} // namespace jrc
