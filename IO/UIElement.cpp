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
#include "UIElement.h"

#include "../Audio/Audio.h"
#include "../Constants.h"

namespace jrc
{
UIElement::UIElement(Point<std::int16_t> p, Point<std::int16_t> d, bool a)
    : position(p), dimension(d), active(a)
{
}

UIElement::UIElement(Point<std::int16_t> p, Point<std::int16_t> d)
    : UIElement(p, d, true)
{
}

UIElement::UIElement() : UIElement({}, {})
{
}

void UIElement::draw(float alpha) const
{
    draw_sprites(alpha);
    draw_buttons(alpha);
}

void UIElement::draw_sprites(float alpha) const
{
    for (const Sprite& sprite : sprites) {
        sprite.draw(position, alpha);
    }
}

void UIElement::draw_buttons(float) const
{
    for (auto& iter : buttons) {
        if (const Button* button = iter.second.get(); button) {
            button->draw(position);
        }
    }
}

void UIElement::update()
{
    for (auto& sprite : sprites) {
        sprite.update();
    }
}

void UIElement::make_active() noexcept
{
    active = true;
}

void UIElement::deactivate() noexcept
{
    active = false;
}

bool UIElement::is_active() const noexcept
{
    return active;
}

void UIElement::toggle_active()
{
    active = !active;
}

Button::State UIElement::button_pressed(std::uint16_t)
{
    return Button::DISABLED;
}

void UIElement::send_icon(const Icon&, Point<std::int16_t>)
{
}

void UIElement::double_click(Point<std::int16_t>)
{
}

bool UIElement::is_in_range(Point<std::int16_t> cursor_pos) const
{
    const Rectangle<std::int16_t> bounds{position, position + dimension};
    return bounds.contains(cursor_pos);
}

bool UIElement::remove_cursor(bool, Point<std::int16_t>)
{
    for (auto& [_, button] : buttons) {
        switch (button->get_state()) {
        case Button::MOUSE_OVER:
            button->set_state(Button::NORMAL);
            break;
        default:
            break;
        }
    }
    return false;
}

Cursor::State UIElement::send_cursor(bool down, Point<std::int16_t> cursor_pos)
{
    Cursor::State ret = down ? Cursor::CLICKING : Cursor::IDLE;

    for (auto& [button_id, button] : buttons) {
        if (button->is_active() &&
            button->bounds(position).contains(cursor_pos)) {
            if (button->get_state() == Button::NORMAL) {
                Sound{Sound::BUTTON_OVER}.play();

                button->set_state(Button::MOUSE_OVER);
                ret = Cursor::CAN_CLICK;
            } else if (button->get_state() == Button::MOUSE_OVER) {
                if (down) {
                    Sound{Sound::BUTTON_CLICK}.play();

                    button->set_state(button_pressed(button_id));

                    ret = Cursor::IDLE;
                } else {
                    ret = Cursor::CAN_CLICK;
                }
            }
        } else if (button->get_state() == Button::MOUSE_OVER) {
            button->set_state(Button::NORMAL);
        }
    }

    return ret;
}
} // namespace jrc
