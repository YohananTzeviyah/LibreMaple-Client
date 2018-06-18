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
#include "../Configuration.h"
#include "UIElement.h"

namespace jrc
{
template<Configuration::PositionOf P>
//! Base class for UI windows which can be moved with the mouse cursor.
class UIDragElement : public UIElement
{
public:
    bool remove_cursor(bool clicked, Point<std::int16_t> cursor_pos) override
    {
        if (dragged) {
            if (clicked) {
                position = cursor_pos - cursoroffset;
                return true;
            } else {
                dragged = false;
                Configuration::get().set_position_of(P, position);
            }
        }

        return false;
    }

    Cursor::State send_cursor(bool clicked,
                              Point<std::int16_t> cursor_pos) override
    {
        if (clicked) {
            if (dragged) {
                position = cursor_pos - cursoroffset;
                return Cursor::CLICKING;
            } else if (in_drag_range(cursor_pos)) {
                cursoroffset = cursor_pos - position;
                dragged = true;
                return Cursor::CLICKING;
            }
        } else {
            if (dragged) {
                dragged = false;
                Configuration::get().set_position_of(P, position);
            }
        }

        return UIElement::send_cursor(clicked, cursor_pos);
    }

protected:
    UIDragElement(Point<std::int16_t> d) : dragged(false), dragarea(d)
    {
        position = Configuration::get().get_position_of(P);
    }

    bool dragged;
    Point<std::int16_t> dragarea;
    Point<std::int16_t> cursoroffset;

private:
    bool in_drag_range(Point<std::int16_t> cursor_pos) const
    {
        const Rectangle<std::int16_t> bounds{position, position + dragarea};
        return bounds.contains(cursor_pos);
    }
};
} // namespace jrc
