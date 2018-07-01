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
#include "Slider.h"

#include "nlnx/nx.hpp"

namespace jrc
{
Slider::Slider(std::int32_t type,
               Range<std::int16_t> ver,
               std::int16_t xp,
               std::int16_t unit_rows,
               std::int16_t row_max,
               std::function<void(bool)> on_moved)
{
    vertical = ver;
    x = xp;
    on_moved = on_moved;

    start = {x, vertical.first()};
    end = {x, vertical.second()};

    nl::node src = nl::nx::ui["Basic.img"]["VScr" + std::to_string(type)];

    nl::node dsrc = src["disabled"];

    d_base = dsrc["base"];
    d_next = dsrc["next"];
    d_prev = dsrc["prev"];

    nl::node esrc = src["enabled"];

    base = esrc["base"];

    prev = {esrc["prev0"], esrc["prev1"], start};
    next = {esrc["next0"], esrc["next1"], end};
    thumb = {esrc["thumb0"], esrc["thumb1"]};

    button_height = d_next.get_dimensions().y();

    set_rows(unit_rows, row_max);

    enabled = true;
    scrolling = false;
}

Slider::Slider() : Slider(0, {}, 0, 0, 0, {})
{
}

bool Slider::is_enabled() const
{
    return enabled;
}

void Slider::set_enabled(bool en)
{
    enabled = en;
}

void Slider::set_rows(std::int16_t new_row,
                      std::int16_t unit_rows,
                      std::int16_t row_max_)
{
    row_max = row_max_ - unit_rows;
    if (row_max > 0) {
        row_height = (vertical.length() - button_height * 2) / row_max;
    } else {
        row_height = 0;
    }
    row = new_row;
}

void Slider::set_rows(std::int16_t unit_rows, std::int16_t row_max_)
{
    set_rows(0, unit_rows, row_max_);
}

void Slider::set_vertical(Range<std::int16_t> ver)
{
    vertical = ver;
    start = {x, vertical.first()};
    end = {x, vertical.second()};
    prev.set_position(start);
    next.set_position(end);
    if (row_max > 0) {
        row_height = (vertical.length() - button_height * 2) / row_max;
    } else {
        row_height = 0;
    }
}

void Slider::draw(Point<std::int16_t> position) const
{
    Point<std::int16_t> fill{0, vertical.length() + button_height};

    if (enabled) {
        base.draw({position + start, fill});
        if (row_height > 0) {
            thumb.draw({position + get_thumb_pos()});
        }
        prev.draw({position});
        next.draw({position});
    } else {
        d_base.draw({position + start, fill});
        d_prev.draw({position});
        d_next.draw({position});
    }
}

bool Slider::remove_cursor(bool clicked)
{
    if (scrolling) {
        scrolling = clicked;
        return clicked;
    } else {
        thumb.set_state(Button::NORMAL);
        next.set_state(Button::NORMAL);
        prev.set_state(Button::NORMAL);
        return false;
    }
}

Point<std::int16_t> Slider::get_thumb_pos() const
{
    std::int16_t y = row < row_max
                         ? vertical.first() + row * row_height + button_height
                         : vertical.second() - button_height * 2 - 2;
    return {x, y};
}

Cursor::State Slider::send_cursor(Point<std::int16_t> cursor, bool pressed)
{
    Point<std::int16_t> relative = cursor - start;
    if (scrolling) {
        if (pressed) {
            std::int16_t thumby = row * row_height + button_height * 2;
            std::int16_t delta = relative.y() - thumby;
            if (delta > row_height / 2 && row < row_max) {
                ++row;
                on_moved(false);
            } else if (delta < -row_height / 2 && row > 0) {
                --row;
                on_moved(true);
            }
            return Cursor::CLICKING;
        } else {
            scrolling = false;
        }
    } else if (relative.x() < 0 || relative.y() < 0 || relative.x() > 8
               || relative.y() > vertical.second()) {
        thumb.set_state(Button::NORMAL);
        next.set_state(Button::NORMAL);
        prev.set_state(Button::NORMAL);
        return Cursor::IDLE;
    }

    Point<std::int16_t> thumbpos = get_thumb_pos();
    if (thumb.bounds(thumbpos).contains(cursor)) {
        if (pressed) {
            scrolling = true;
            thumb.set_state(Button::PRESSED);
            return Cursor::CLICKING;
        } else {
            thumb.set_state(Button::MOUSE_OVER);
            return Cursor::V_SCROLL;
        }
    } else {
        thumb.set_state(Button::NORMAL);
    }

    if (prev.bounds(Point<std::int16_t>{}).contains(cursor)) {
        if (pressed) {
            if (row > 0) {
                --row;
                on_moved(true);
            }

            prev.set_state(Button::PRESSED);
            return Cursor::CLICKING;
        } else {
            prev.set_state(Button::MOUSE_OVER);
            return Cursor::CAN_CLICK;
        }
    } else {
        prev.set_state(Button::NORMAL);
    }

    if (next.bounds(Point<std::int16_t>{}).contains(cursor)) {
        if (pressed) {
            if (row < row_max) {
                ++row;
                on_moved(false);
            }

            next.set_state(Button::PRESSED);
            return Cursor::CLICKING;
        } else {
            next.set_state(Button::MOUSE_OVER);
            return Cursor::CAN_CLICK;
        }
    } else {
        next.set_state(Button::NORMAL);
    }

    if (pressed) {
        auto y_offset = static_cast<double>(relative.y() - button_height * 2);
        auto cursor_row
            = static_cast<std::int16_t>(std::round(y_offset / row_height));
        if (cursor_row < 0) {
            cursor_row = 0;
        } else if (cursor_row > row_max) {
            cursor_row = row_max;
        }

        std::int16_t delta = row - cursor_row;
        while (delta > 0) {
            --delta;
            on_moved(true);
        }
        while (delta < 0) {
            ++delta;
            on_moved(false);
        }

        row = cursor_row;
    }

    return Cursor::IDLE;
}
} // namespace jrc
