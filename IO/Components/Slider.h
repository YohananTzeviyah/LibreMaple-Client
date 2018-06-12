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
#include "../Cursor.h"
#include "TwoSpriteButton.h"

#include <functional>
#include <memory>

namespace jrc
{
class Slider
{
public:
    Slider(std::int32_t type,
           Range<std::int16_t> vertical,
           std::int16_t x,
           std::int16_t unit_rows,
           std::int16_t row_max,
           std::function<void(bool upwards)> on_moved);
    Slider();

    bool is_enabled() const;
    void set_enabled(bool enabled);
    void set_rows(std::int16_t new_row,
                  std::int16_t unit_rows,
                  std::int16_t row_max_);
    void set_rows(std::int16_t unit_rows, std::int16_t row_max);
    void set_vertical(Range<std::int16_t> vertical);

    void draw(Point<std::int16_t> position) const;
    bool remove_cursor(bool clicked);
    Cursor::State send_cursor(Point<std::int16_t> cursor, bool pressed);

private:
    Point<std::int16_t> get_thumb_pos() const;

    std::function<void(bool upwards)> on_moved;

    Range<std::int16_t> vertical;
    Point<std::int16_t> start;
    Point<std::int16_t> end;
    std::int16_t button_height;
    std::int16_t row_height;
    std::int16_t x;
    std::int16_t row;
    std::int16_t row_max;
    bool scrolling;
    bool enabled;

    Texture d_base;
    Texture d_next;
    Texture d_prev;
    Texture base;
    TwoSpriteButton next;
    TwoSpriteButton prev;
    TwoSpriteButton thumb;
};
} // namespace jrc
