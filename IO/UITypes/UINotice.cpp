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
#include "UINotice.h"

#include "../Components/MapleButton.h"
#include "nlnx/nx.hpp"

#include <limits>

namespace jrc
{
UINotice::UINotice(std::string&& q)
{
    nl::node src = nl::nx::ui["Basic.img"]["Notice6"];

    top = src["t"];
    center = src["c"];
    center_box = src["c_box"];
    box = src["box"];
    box2 = src["box2"];
    bottom = src["s"];
    bottom_box = src["s_box"];

    question = {Text::A11M, Text::CENTER, Text::DARKGREY, std::move(q), 200};

    height = question.height();
    dimension = {top.width(), top.height() + height + bottom.height()};
    position = {Constants::GAME_VIEW_WIDTH / 2 - dimension.x() / 2,
                Constants::GAME_VIEW_HEIGHT / 2 - dimension.y() / 2};
}

void UINotice::draw_notice(bool text_field) const
{
    Point<std::int16_t> start = position;

    top.draw(start);
    start.shift_y(top.height());
    center_box.draw(start);
    start.shift_y(center_box.height());

    Point<std::int16_t> text_pos = start;
    box.draw(DrawArgument(text_pos, Point<std::int16_t>{0, height}));
    start.shift_y(box.height() * (height / box.height()));
    box.draw(start);
    start.shift_y(box.height());
    question.draw(text_pos + Point<std::int16_t>{130, -3});

    if (text_field) {
        box2.draw(start);
        start.shift_y(box2.height());
    }

    box.draw(start);
    start.shift_y(box.height());
    bottom_box.draw(start);
}

std::int16_t UINotice::box2offset() const
{
    return top.height() + center_box.height()
           + box.height()
                 * (static_cast<std::int16_t>(1) + height / box.height());
}

UIYesNo::UIYesNo(std::string&& q, std::function<void(bool)> yh)
    : UINotice{std::move(q)}, yes_no_handler{yh}
{
    std::int16_t below_text = UINotice::box2offset();

    nl::node src = nl::nx::ui["Basic.img"];

    buttons[YES] = std::make_unique<MapleButton>(
        src["BtOK4"], Point<std::int16_t>{90, below_text + 1});
    buttons[NO] = std::make_unique<MapleButton>(
        src["BtCancel4"], Point<std::int16_t>{132, below_text + 1});
}

void UIYesNo::draw(float alpha) const
{
    UINotice::draw_notice(false);
    UIElement::draw(alpha);
}

Button::State UIYesNo::button_pressed(std::uint16_t button_id)
{
    switch (button_id) {
    case YES:
        yes_no_handler(true);
        break;
    case NO:
        yes_no_handler(false);
        break;
    }

    active = false;

    return Button::PRESSED;
}

UIEnterNumber::UIEnterNumber(std::string&& q,
                             std::function<void(std::int32_t)> n_handler,
                             std::int32_t mi,
                             std::int32_t ma,
                             std::int32_t de)
    : UINotice(std::move(q))
{
    num_handler = n_handler;
    min = mi;
    max = ma;

    std::int16_t below_text = UINotice::box2offset();

    nl::node src = nl::nx::ui["Basic.img"];

    buttons[OK] = std::make_unique<MapleButton>(
        src["BtOK4"], Point<std::int16_t>{90, below_text + 21});
    buttons[CANCEL] = std::make_unique<MapleButton>(
        src["BtCancel4"], Point<std::int16_t>{132, below_text + 21});

    Rectangle<std::int16_t> area{26, 232, below_text, below_text + 20};
    num_field = {Text::A11M, Text::LEFT, Text::LIGHTGREY, area, 9};
    num_field.set_state(Textfield::FOCUSED);
    num_field.change_text(std::to_string(de));
    num_field.set_enter_callback(
        [this](const std::string& num_str) { handle_string(num_str); });
}

void UIEnterNumber::draw(float alpha) const
{
    UINotice::draw_notice(true);
    UIElement::draw(alpha);

    num_field.draw(position);
}

void UIEnterNumber::update()
{
    UIElement::update();

    num_field.update(position);
}

Cursor::State UIEnterNumber::send_cursor(bool clicked,
                                         Point<std::int16_t> cursor_pos)
{
    if (num_field.get_state() == Textfield::NORMAL) {
        Cursor::State n_state = num_field.send_cursor(cursor_pos, clicked);
        if (n_state != Cursor::IDLE) {
            return n_state;
        }
    }
    return UIElement::send_cursor(clicked, cursor_pos);
}

Button::State UIEnterNumber::button_pressed(std::uint16_t button_id)
{
    switch (button_id) {
    case OK:
        handle_string(num_field.get_text());
        break;
    }

    active = false;

    return Button::PRESSED;
}

void UIEnterNumber::handle_string(const std::string& num_str)
{
    if (!num_str.empty()) {
        const auto num = [&num_str] {
            try {
                return std::stoi(num_str);
            } catch (const std::logic_error&) {
            }
            return std::numeric_limits<std::int32_t>::lowest();
        }();

        if (num >= min && num <= max) {
            num_handler(num);
            active = false;
        }
    }

    buttons[OK]->set_state(Button::NORMAL);
}
} // namespace jrc
