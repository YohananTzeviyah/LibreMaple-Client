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
#include "UI.h"

#include "../Graphics/GraphicsGL.h"
#include "UIStateGame.h"
#include "UIStateLogin.h"
#include "UITypes/UIChangeChannel.h"
#include "Window.h"

namespace jrc
{
UI::UI() noexcept : state{std::make_unique<UIStateNull>()}, enabled{true}
{
}

void UI::init()
{
    cursor.init();
    change_state(LOGIN);
}

void UI::draw(float alpha) const
{
    state->draw(alpha, cursor.get_position());

    scrolling_notice.draw(alpha);
    cursor.draw(alpha);
}

void UI::update()
{
    state->update();

    scrolling_notice.update();
    cursor.update();
}

void UI::enable()
{
    enabled = true;
}

void UI::disable()
{
    enabled = false;
}

void UI::change_state(State id)
{
    switch (id) {
    case LOGIN:
        Window::get().resize(false);
        state = std::make_unique<UIStateLogin>();
        break;
    case GAME:
        Window::get().resize(true);
        state = std::make_unique<UIStateGame>();
        break;
    }
}

void UI::quit()
{
    quitted = true;
}

bool UI::not_quitted() const
{
    return !quitted;
}

void UI::send_cursor(Point<std::int16_t> cursor_pos,
                     Cursor::State cursor_state)
{
    Cursor::State next_state = state->send_cursor(cursor_state, cursor_pos);
    cursor.set_state(next_state);
    cursor.set_position(cursor_pos);
}

void UI::send_cursor(bool pressed)
{
    Cursor::State cursor_state
        = pressed && enabled ? Cursor::CLICKING : Cursor::IDLE;
    Point<std::int16_t> cursor_pos = cursor.get_position();
    send_cursor(cursor_pos, cursor_state);

    if (focused_text_field && pressed) {
        Cursor::State tstate
            = focused_text_field->send_cursor(cursor_pos, pressed);
        switch (tstate) {
        case Cursor::IDLE:
            focused_text_field = {};
            break;
        default:
            break;
        }
    }
}

void UI::send_cursor(Point<std::int16_t> pos)
{
    send_cursor(pos, cursor.get_state());
}

void UI::doubleclick()
{
    Point<std::int16_t> pos = cursor.get_position();
    state->double_click(pos);
}

void UI::send_key(std::int32_t keycode, bool pressed)
{
    keycode = Keyboard::align_key_parity(keycode);

    if (focused_text_field) {
        bool ctrl = is_key_down[keyboard.ctrl_code()];
        if (ctrl) {
            if (!pressed) {
                KeyAction::Id action = keyboard.get_ctrl_action(keycode);
                switch (action) {
                case KeyAction::COPY:
                    Window::get().set_clipboard(
                        focused_text_field->get_text());
                    break;
                case KeyAction::PASTE:
                    focused_text_field->add_string(
                        Window::get().get_clipboard());
                    break;
                default:
                    break;
                }
            }
        } else {
            bool shift = is_key_down[keyboard.shift_code()];
            Keyboard::Mapping mapping
                = keyboard.get_text_mapping(keycode, shift);
            focused_text_field->send_key(
                mapping.type, mapping.action, pressed);
        }
    } else {
        if (auto cc_ui = get_element<UIChangeChannel>();
            cc_ui && cc_ui->is_active()) {
            auto cc_ui_key = [keycode] {
                switch (keycode) {
                case GLFW_KEY_UP:
                    return UIChangeChannel::SentKey::UP;
                case GLFW_KEY_DOWN:
                    return UIChangeChannel::SentKey::DOWN;
                case GLFW_KEY_LEFT:
                    return UIChangeChannel::SentKey::LEFT;
                case GLFW_KEY_RIGHT:
                    return UIChangeChannel::SentKey::RIGHT;
                case GLFW_KEY_ENTER:
                    return UIChangeChannel::SentKey::ENTER;
                default:
                    return UIChangeChannel::SentKey::NONE;
                }
            }();
            if (cc_ui_key != UIChangeChannel::SentKey::NONE) {
                if (!pressed) {
                    cc_ui->send_key(cc_ui_key);
                }
                is_key_down[keycode] = pressed;
                return;
            }
        }

        Keyboard::Mapping mapping = keyboard.get_mapping(keycode);
        if (mapping.type) {
            state->send_key(mapping.type, mapping.action, pressed);
        }
    }

    is_key_down[keycode] = pressed;
}

void UI::send_menu(KeyAction::Id action)
{
    state->send_key(KeyType::MENU, action, true);
}

void UI::set_scroll_notice(std::string&& notice) noexcept
{
    scrolling_notice.set_notice(std::move(notice));
}

void UI::focus_text_field(Textfield* to_focus) noexcept
{
    if (focused_text_field) {
        focused_text_field->set_state(Textfield::NORMAL);
    }

    focused_text_field = to_focus;
}

void UI::drag_icon(Icon* icon)
{
    state->drag_icon(icon);
}

void UI::add_keymapping(std::uint8_t no,
                        std::uint8_t type,
                        KeyAction::Id action)
{
    keyboard.assign(no, type, action);
}

void UI::clear_tooltip(Tooltip::Parent parent)
{
    state->clear_tooltip(parent);
}

void UI::show_equip(Tooltip::Parent parent, std::int16_t slot)
{
    state->show_equip(parent, slot);
}

void UI::show_item(Tooltip::Parent parent, std::int32_t item_id)
{
    state->show_item(parent, item_id);
}

void UI::show_skill(Tooltip::Parent parent,
                    std::int32_t skill_id,
                    std::int32_t level,
                    std::int32_t master_level,
                    std::int64_t expiration)
{
    state->show_skill(parent, skill_id, level, master_level, expiration);
}

void UI::remove(UIElement::Type type)
{
    focused_text_field = {};
    state->remove(type);
}

const Keyboard& UI::get_keyboard() const noexcept
{
    return keyboard;
}

Keyboard& UI::get_keyboard_mut() noexcept
{
    return keyboard;
}
} // namespace jrc
