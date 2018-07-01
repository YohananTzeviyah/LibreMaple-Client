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
#include "SliderHoriz.h"

namespace jrc
{
SliderHoriz::SliderHoriz(nl::node src,
                         Range<std::int16_t> horiz,
                         std::int16_t y_pos,
                         std::int16_t unit_cols,
                         std::int16_t col_max_,
                         std::function<void(bool)> on_moved_)
    : on_moved{on_moved_},
      horizontal{horiz},
      start{horizontal.first(), y_pos},
      end{horizontal.second(), y_pos},
      y{y_pos},
      state{NORMAL},
      scrolling{false},
      enabled{true},
      normal{src["0"]},
      pressed{src["1"]},
      disabled{src["2"]},
      mouse_over{src["3"]}
{
    button_width = normal.get_dimensions().x();
    set_cols(unit_cols, col_max_);
}

bool SliderHoriz::is_enabled() const noexcept
{
    return enabled;
}

void SliderHoriz::set_enabled(bool en) noexcept
{
    enabled = en;
    if (!enabled) {
        state = DISABLED;
    }
}

void SliderHoriz::set_cols(std::int16_t new_col,
                           std::int16_t unit_cols,
                           std::int16_t col_max_) noexcept
{
    col_max = col_max_ - unit_cols;
    if (col_max > 0) {
        col_width = horizontal.length() / col_max;
    } else {
        col_width = 0;
    }
    col = new_col;
}

void SliderHoriz::set_cols(std::int16_t unit_cols,
                           std::int16_t col_max_) noexcept
{
    set_cols(0, unit_cols, col_max_);
}

void SliderHoriz::set_horizontal(Range<std::int16_t> horiz) noexcept
{
    horizontal = horiz;
    start = {horizontal.first(), y};
    end = {horizontal.second(), y};

    if (col_max > 0) {
        col_width = horizontal.length() / col_max;
    } else {
        col_width = 0;
    }
}

void SliderHoriz::draw(Point<std::int16_t> position) const
{
    if (col_width <= 0) {
        return;
    }

    const auto& texture = [this] {
        switch (state) {
        case NORMAL:
            return normal;
        case PRESSED:
            return pressed;
        case DISABLED:
            return disabled;
        case MOUSE_OVER:
            return mouse_over;
        }
    }();
    texture.draw({position + get_thumb_pos()});
}

bool SliderHoriz::remove_cursor(bool clicked) noexcept
{
    if (scrolling) {
        scrolling = clicked;
        state = clicked ? PRESSED : NORMAL;
        return clicked;
    }

    state = NORMAL;
    return false;
}

Point<std::int16_t> SliderHoriz::get_thumb_pos() const noexcept
{
    std::int16_t x = col < col_max ? horizontal.first()
                                         + col * horizontal.length() / col_max
                                   : horizontal.second();
    return {x, y};
}

Cursor::State SliderHoriz::send_cursor(Point<std::int16_t> cursor,
                                       bool cursor_pressed) noexcept
{
    if (!enabled) {
        return Cursor::IDLE;
    }

    Point<std::int16_t> relative = cursor - start;
    if (scrolling) {
        if (cursor_pressed) {
            std::int16_t thumb_x
                = col * horizontal.length() / col_max + button_width / 2;
            std::int16_t delta = relative.x() - thumb_x;
            if (delta > col_width / 2 && col < col_max) {
                ++col;
                on_moved(false);
            } else if (delta < -col_width / 2 && col > 0) {
                --col;
                on_moved(true);
            }

            state = PRESSED;
            return Cursor::CLICKING;
        } else {
            scrolling = false;
        }
    }

    if (relative.x() < 0 || relative.y() < 0
        || relative.x()
               > horizontal.second() - button_width * 2 - button_width / 2
        || relative.y() > normal.get_dimensions().y()) {
        state = NORMAL;
        return Cursor::IDLE;
    }

    if (cursor_pressed) {
        auto x_offset = static_cast<double>(relative.x() - button_width / 2);
        auto cursor_col = static_cast<std::int16_t>(
            std::round(x_offset * col_max / horizontal.length()));
        if (cursor_col < 0) {
            cursor_col = 0;
        } else if (cursor_col > col_max) {
            cursor_col = col_max;
        }

        std::int16_t delta = col - cursor_col;
        while (delta > 0) {
            --delta;
            on_moved(true);
        }
        while (delta < 0) {
            ++delta;
            on_moved(false);
        }

        col = cursor_col;
        scrolling = true;
        state = PRESSED;

        return Cursor::CLICKING;
    }

    state = MOUSE_OVER;

    return Cursor::CAN_CLICK;
}

SliderHoriz::State SliderHoriz::get_state() const noexcept
{
    return state;
}

std::int16_t SliderHoriz::get_col() const noexcept
{
    return col;
}
} // namespace jrc
