//////////////////////////////////////////////////////////////////////////////
// This file is part of the LibreMaple MMORPG client                        //
// Copyright © 2018-2019 LibreMaple Team                                    //
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
#include "nlnx/nx.hpp"

#include <functional>

namespace jrc
{
class SliderHoriz
{
public:
    enum State : std::uint8_t { NORMAL, PRESSED, DISABLED, MOUSE_OVER };

    SliderHoriz(nl::node src,
                Range<std::int16_t> horiz,
                std::int16_t y_pos,
                std::int16_t unit_cols,
                std::int16_t col_max_,
                std::function<void(bool rightwards)> on_moved_);
    SliderHoriz() = default;

    bool is_enabled() const noexcept;
    void set_enabled(bool enabled) noexcept;
    void set_cols(std::int16_t new_col,
                  std::int16_t unit_cols,
                  std::int16_t col_max_) noexcept;
    void set_cols(std::int16_t unit_cols, std::int16_t col_max_) noexcept;
    void set_horizontal(Range<std::int16_t> horiz) noexcept;

    void draw(Point<std::int16_t> position) const;
    bool remove_cursor(bool clicked) noexcept;
    Cursor::State send_cursor(Point<std::int16_t> cursor,
                              bool pressed) noexcept;

    [[nodiscard]] State get_state() const noexcept;
    [[nodiscard]] std::int16_t get_col() const noexcept;

private:
    Point<std::int16_t> get_thumb_pos() const noexcept;

    std::function<void(bool rightwards)> on_moved;

    Range<std::int16_t> horizontal;
    Point<std::int16_t> start;
    Point<std::int16_t> end;
    std::int16_t button_width;
    std::int16_t col_width;
    std::int16_t y;
    std::int16_t col;
    std::int16_t col_max;
    State state;
    bool scrolling;
    bool enabled;

    Texture normal;
    Texture pressed;
    Texture disabled;
    Texture mouse_over;
};
} // namespace jrc
