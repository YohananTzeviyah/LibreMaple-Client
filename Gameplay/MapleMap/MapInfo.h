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
#include "../../Template/Point.h"
#include "../../Template/Range.h"
#include "../../Template/nullable_ptr.h"
#include "nlnx/node.hpp"

#include <vector>

namespace jrc
{
class Seat
{
public:
    Seat(nl::node source);

    bool in_range(Point<std::int16_t> position) const;
    Point<std::int16_t> get_pos() const;

private:
    Point<std::int16_t> pos;
};

class Ladder
{
public:
    Ladder(nl::node source);

    bool is_ladder() const;
    bool in_range(Point<std::int16_t> position, bool upwards) const;
    bool fell_off(std::int16_t y, bool downwards) const;
    std::int16_t get_x() const;

private:
    std::int16_t x;
    std::int16_t y1;
    std::int16_t y2;
    bool ladder;
};

class MapInfo
{
public:
    MapInfo(nl::node src,
            Range<std::int16_t> walls,
            Range<std::int16_t> borders);
    MapInfo();

    bool is_underwater() const;
    std::string get_bgm() const;
    Range<std::int16_t> get_walls() const;
    Range<std::int16_t> get_borders() const;

    //! Find a seat at the player's position.
    nullable_ptr<const Seat> find_seat(Point<std::int16_t> position) const;
    //! Find a ladder at the player's position. `upwards == false` implies
    //! downwards.
    nullable_ptr<const Ladder> find_ladder(Point<std::int16_t> position,
                                           bool upwards) const;

private:
    std::int32_t fieldlimit;
    bool cloud;
    std::string bgm;
    std::string map_desc;
    std::string map_name;
    std::string street_name;
    std::string map_mark;
    bool swim;
    bool town;
    bool hide_minimap;
    Range<std::int16_t> map_walls;
    Range<std::int16_t> map_borders;
    std::vector<Seat> seats;
    std::vector<Ladder> ladders;
};
} // namespace jrc
