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
#include "Textfield.h"

#include "../../Constants.h"
#include "../UI.h"

namespace jrc
{
Textfield::Textfield(Text::Font font,
                     Text::Alignment alignment,
                     Text::Color color,
                     Rectangle<std::int16_t> bnd,
                     std::size_t lim) noexcept
    : text_label(font, alignment, color, "", 0, false),
      text(),
      marker(font, alignment, color, "|"),
      show_marker(true),
      marker_pos(0),
      bounds(bnd),
      limit(lim),
      crypt('\0'),
      state(NORMAL)
{
}

Textfield::Textfield() = default;

Textfield::~Textfield() = default;

void Textfield::draw(Point<std::int16_t> parent) const
{
    if (state == DISABLED) {
        return;
    }

    Point<std::int16_t> absp = bounds.get_lt() + parent;
    if (text.size() > 0) {
        text_label.draw(absp);
    }

    if (state == FOCUSED && show_marker) {
        Point<std::int16_t> mpos =
            absp + Point<std::int16_t>{text_label.advance(marker_pos), -1};
        marker.draw(mpos);
    }
}

void Textfield::update(Point<std::int16_t> parent) noexcept
{
    if (state == DISABLED) {
        return;
    }

    parent_pos = parent;

    elapsed += Constants::TIMESTEP;
    if (elapsed > 256) {
        show_marker = !show_marker;
        elapsed = 0;
    }
}

void Textfield::set_state(State st) noexcept
{
    if (state != st) {
        state = st;
        elapsed = 0;
        show_marker = true;

        if (state == FOCUSED) {
            UI::get().focus_text_field(this);
        }
    }
}

void Textfield::set_enter_callback(
    std::function<void(const std::string&)> on_ret) noexcept
{
    on_return = on_ret;
}

void Textfield::set_key_callback(KeyAction::Id key,
                                 std::function<void(void)> action) noexcept
{
    callbacks[key] = action;
}

void Textfield::send_key(KeyType::Id type,
                         std::int32_t key,
                         bool pressed) noexcept
{
    switch (type) {
    case KeyType::ACTION:
        if (pressed) {
            switch (key) {
            case KeyAction::LEFT:
                if (marker_pos > 0) {
                    --marker_pos;
                }
                break;
            case KeyAction::RIGHT:
                if (marker_pos < text.size()) {
                    ++marker_pos;
                }
                break;
            case KeyAction::BACK:
                if (text.size() > 0 && marker_pos > 0) {
                    text.erase(marker_pos - 1, 1);
                    --marker_pos;
                    modify_text(std::string{text});
                }
                break;
            case KeyAction::RETURN:
                if (on_return && text.size() > 0) {
                    on_return(text);
                    text = "";
                    marker_pos = 0;
                    modify_text(std::string{text});
                }
                break;
            case KeyAction::SPACE:
                if (marker_pos > 0 && below_limit()) {
                    text.insert(marker_pos, 1, ' ');
                    ++marker_pos;
                    modify_text(std::string{text});
                }
                break;
            default:
                if (callbacks.count(key)) {
                    callbacks.at(key)();
                }
                break;
            }
        }
        break;
    case KeyType::LETTER:
    case KeyType::NUMBER:
        if (!pressed) {
            auto c = static_cast<char>(key);
            if (below_limit()) {
                text.insert(marker_pos, 1, c);
                ++marker_pos;
                modify_text(std::string{text});
            }
        }
        break;
    default:
        break;
    }
}

void Textfield::add_string(std::string_view str) noexcept
{
    for (char c : str) {
        if (below_limit()) {
            text.insert(marker_pos, 1, c);
            ++marker_pos;
            modify_text(std::string{text});
        }
    }
}

void Textfield::modify_text(std::string&& t) noexcept
{
    text = std::move(t);

    if (crypt != '\0') {
        text_label.change_text(std::string(text.length(), crypt));
    } else {
        text_label.change_text(std::string{text});
    }
}

Cursor::State Textfield::send_cursor(Point<std::int16_t> cursorpos,
                                     bool clicked) noexcept
{
    if (state == DISABLED) {
        return Cursor::IDLE;
    }

    auto abs_bounds = get_bounds();
    if (abs_bounds.contains(cursorpos)) {
        if (clicked) {
            switch (state) {
            case NORMAL:
                set_state(FOCUSED);
                break;
            default:
                break;
            }
            return Cursor::CLICKING;
        } else {
            return Cursor::CANCLICK;
        }
    } else {
        if (clicked) {
            switch (state) {
            case FOCUSED:
                set_state(NORMAL);
                break;
            default:
                break;
            }
        }
        return Cursor::IDLE;
    }
}

void Textfield::change_text(std::string&& t) noexcept
{
    modify_text(std::move(t));
    marker_pos = text.size();
}

void Textfield::set_crypt_char(char c) noexcept
{
    crypt = c;
}

bool Textfield::below_limit() const noexcept
{
    if (limit > 0) {
        return text.size() < limit;
    } else {
        return text_label.advance(text.size()) + 50 <
               bounds.get_horizontal().length();
    }
}

const std::string& Textfield::get_text() const noexcept
{
    return text;
}

bool Textfield::empty() const noexcept
{
    return text.empty();
}

Textfield::State Textfield::get_state() const noexcept
{
    return state;
}

Rectangle<std::int16_t> Textfield::get_bounds() const noexcept
{
    return {bounds.get_lt() + parent_pos, bounds.get_rb() + parent_pos};
}
} // namespace jrc
