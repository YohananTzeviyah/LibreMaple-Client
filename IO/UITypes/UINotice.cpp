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
    centerbox = src["c_box"];
    box = src["box"];
    box2 = src["box2"];
    bottom = src["s"];
    bottombox = src["s_box"];

    question = {Text::A11M, Text::CENTER, Text::DARKGREY, std::move(q), 200};

    height = question.height();
    dimension = {top.width(), top.height() + height + bottom.height()};
    position = {400 - dimension.x() / 2, 240 - dimension.y() / 2};
}

void UINotice::draw_notice(bool textfield) const
{
    Point<std::int16_t> start = position;

    top.draw(start);
    start.shift_y(top.height());
    centerbox.draw(start);
    start.shift_y(centerbox.height());

    Point<std::int16_t> textpos = start;
    box.draw(DrawArgument(textpos, Point<std::int16_t>(0, height)));
    start.shift_y(box.height() * (height / box.height()));
    box.draw(start);
    start.shift_y(box.height());
    question.draw(textpos + Point<std::int16_t>(130, -3));

    if (textfield) {
        box2.draw(start);
        start.shift_y(box2.height());
    }

    box.draw(start);
    start.shift_y(box.height());
    bottombox.draw(start);
}

std::int16_t UINotice::box2offset() const
{
    return top.height() + centerbox.height() +
           box.height() *
               (static_cast<std::int16_t>(1) + height / box.height());
}

UIYesNo::UIYesNo(std::string&& q, std::function<void(bool)> yh)
    : UINotice(std::move(q))
{
    yes_no_handler = yh;

    std::int16_t belowtext = UINotice::box2offset();

    nl::node src = nl::nx::ui["Basic.img"];

    buttons[YES] =
        std::make_unique<MapleButton>(src["BtOK4"], 90, belowtext + 1);
    buttons[NO] =
        std::make_unique<MapleButton>(src["BtCancel4"], 132, belowtext + 1);
}

void UIYesNo::draw(float alpha) const
{
    UINotice::draw(false);
    UIElement::draw(alpha);
}

Button::State UIYesNo::button_pressed(std::uint16_t buttonid)
{
    switch (buttonid) {
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
                             std::function<void(std::int32_t)> nh,
                             std::int32_t mi,
                             std::int32_t ma,
                             std::int32_t de)
    : UINotice(std::move(q))
{
    num_handler = nh;
    min = mi;
    max = ma;

    std::int16_t belowtext = UINotice::box2offset();

    nl::node src = nl::nx::ui["Basic.img"];

    buttons[OK] =
        std::make_unique<MapleButton>(src["BtOK4"], 90, belowtext + 21);
    buttons[CANCEL] =
        std::make_unique<MapleButton>(src["BtCancel4"], 132, belowtext + 21);

    Rectangle<std::int16_t> area(26, 232, belowtext, belowtext + 20);
    num_field = Textfield(Text::A11M, Text::LEFT, Text::LIGHTGREY, area, 9);
    num_field.set_state(Textfield::FOCUSED);
    num_field.change_text(std::to_string(de));
    num_field.set_enter_callback(
        [this](const std::string& num_str) { handle_string(num_str); });
}

void UIEnterNumber::draw(float alpha) const
{
    UINotice::draw(true);
    UIElement::draw(alpha);

    num_field.draw(position);
}

void UIEnterNumber::update()
{
    UIElement::update();

    num_field.update(position);
}

Cursor::State UIEnterNumber::send_cursor(bool clicked,
                                         Point<std::int16_t> cursorpos)
{
    if (num_field.get_state() == Textfield::NORMAL) {
        Cursor::State nstate = num_field.send_cursor(cursorpos, clicked);
        if (nstate != Cursor::IDLE) {
            return nstate;
        }
    }
    return UIElement::send_cursor(clicked, cursorpos);
}

Button::State UIEnterNumber::button_pressed(std::uint16_t buttonid)
{
    switch (buttonid) {
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
        const auto num = [&num_str]() {
            try {
                return std::stoi(num_str);
            } catch (const std::exception&) {
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
