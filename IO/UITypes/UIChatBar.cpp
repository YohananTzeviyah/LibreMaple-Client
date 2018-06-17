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
#include "UIChatBar.h"

#include "../../Net/Packets/MessagingPackets.h"
#include "../Components/MapleButton.h"
#include "../UI.h"
#include "nlnx/nx.hpp"

namespace jrc
{
UIChatbar::UIChatbar(Point<std::int16_t> pos)
{
    position = pos;
    dimension = {500, 60};
    chatopen = true;
    drag_chat_top = false;
    chat_rows = 4;
    row_pos = 0;
    row_max = -1;
    lastpos = 0;

    nl::node mainbar = nl::nx::ui["StatusBar2.img"]["mainBar"];

    buttons[BT_OPENCHAT] = std::make_unique<MapleButton>(mainbar["chatOpen"]);
    buttons[BT_CLOSECHAT]
        = std::make_unique<MapleButton>(mainbar["chatClose"]);
    buttons[BT_SCROLLUP] = std::make_unique<MapleButton>(mainbar["scrollUp"]);
    buttons[BT_SCROLLDOWN]
        = std::make_unique<MapleButton>(mainbar["scrollDown"]);
    buttons[BT_CHATTARGETS]
        = std::make_unique<MapleButton>(mainbar["chatTarget"]["base"]);

    buttons[chatopen ? BT_OPENCHAT : BT_CLOSECHAT]->set_active(false);
    buttons[BT_CHATTARGETS]->set_active(chatopen);

    chatspace[false] = mainbar["chatSpace"];
    chatspace[true] = mainbar["chatEnter"];
    chatenter = mainbar["chatSpace2"];
    chatcover = mainbar["chatCover"];

    chattargets[CHT_ALL] = mainbar["chatTarget"]["all"];
    chattargets[CHT_BUDDY] = mainbar["chatTarget"]["friend"];
    chattargets[CHT_GUILD] = mainbar["chatTarget"]["guild"];
    chattargets[CHT_ALLIANCE] = mainbar["chatTarget"]["association"];
    chattargets[CHT_PARTY] = mainbar["chatTarget"]["party"];
    chattargets[CHT_SQUAD] = mainbar["chatTarget"]["expedition"];

    chattarget = CHT_ALL; // Default chat target

    nl::node chat = nl::nx::ui["StatusBar2.img"]["chat"];

    tapbar = chat["tapBar"];
    tapbartop = chat["tapBarOver"];

    chat_box = {502, 1 + chat_rows * CHAT_ROW_HEIGHT, Geometry::BLACK, 0.6f};

    chat_field
        = {Text::A11M, Text::LEFT, Text::BLACK, {{-435, -58}, {-40, -35}}, 0};
    chat_field.set_state(chatopen ? Textfield::NORMAL : Textfield::DISABLED);
    chat_field.set_enter_callback([this](const std::string& msg) {
        auto last = msg.find_last_not_of(' ');
        if (last != std::string::npos) {
            std::string m = msg.substr(0, last + 1);

            GeneralChatPacket{m, true}.dispatch();

            last_entered.push_back(std::move(m));
            lastpos = last_entered.size();
        }
    });
    chat_field.set_key_callback(KeyAction::UP, [&]() {
        if (lastpos > 0) {
            --lastpos;
            chat_field.change_text(std::string{last_entered[lastpos]});
        }
    });
    chat_field.set_key_callback(KeyAction::DOWN, [&]() {
        if (last_entered.size() > 0 && lastpos < last_entered.size() - 1) {
            ++lastpos;
            chat_field.change_text(std::string{last_entered[lastpos]});
        }
    });

    slider = {11,
              Range<std::int16_t>{0, CHAT_ROW_HEIGHT * chat_rows - 14},
              -22,
              chat_rows,
              1,
              [&](bool up) {
                  std::int16_t next = up ? row_pos - 1 : row_pos + 1;
                  if (next >= 0 && next <= row_max)
                      row_pos = next;
              }};
}

void UIChatbar::draw(float inter) const
{
    chatspace[chatopen].draw(position);
    chatenter.draw(position);

    UIElement::draw(inter);

    if (chatopen) {
        tapbartop.draw({position.x() - 576, get_chat_top()});
        chat_box.draw({0, get_chat_top() + 2});

        std::int16_t chatheight = CHAT_ROW_HEIGHT * chat_rows;
        std::int16_t yshift = -chatheight;
        for (std::int16_t i = 0; i < chat_rows; ++i) {
            std::int16_t rowid = row_pos - i;
            if (!row_texts.count(rowid)) {
                break;
            }

            std::int16_t textheight
                = row_texts.at(rowid).height() / CHAT_ROW_HEIGHT;
            while (textheight > 0) {
                yshift += CHAT_ROW_HEIGHT;
                --textheight;
            }
            row_texts.at(rowid).draw({4, get_chat_top() - yshift - 1});
        }

        slider.draw({position.x(), get_chat_top() + 5});

        chattargets[chattarget].draw(position + Point<std::int16_t>(0, 2));
        chatcover.draw(position);
        chat_field.draw(position);
    } else if (row_texts.count(row_max)) {
        row_texts.at(row_max).draw(position + Point<std::int16_t>(-500, -60));
    }
}

void UIChatbar::update()
{
    UIElement::update();

    chat_field.update(position);
}

Button::State UIChatbar::button_pressed(std::uint16_t id)
{
    switch (id) {
    case BT_OPENCHAT:
        chatopen = true;
        buttons[BT_OPENCHAT]->set_active(false);
        buttons[BT_CLOSECHAT]->set_active(true);
        buttons[BT_CHATTARGETS]->set_active(true);
        chat_field.set_state(Textfield::NORMAL);
        break;
    case BT_CLOSECHAT:
        chatopen = false;
        buttons[BT_OPENCHAT]->set_active(true);
        buttons[BT_CLOSECHAT]->set_active(false);
        buttons[BT_CHATTARGETS]->set_active(false);
        chat_field.set_state(Textfield::DISABLED);
        break;
    }

    return Button::NORMAL;
}

bool UIChatbar::is_in_range(Point<std::int16_t> cursor_pos) const
{
    Point<std::int16_t> absp{0, get_chat_top() - 16};
    Point<std::int16_t> dim{500,
                            chat_rows * CHAT_ROW_HEIGHT + CHAT_Y_OFFSET + 16};
    return Rectangle<std::int16_t>{absp, absp + dim}.contains(cursor_pos);
}

bool UIChatbar::remove_cursor(bool clicked, Point<std::int16_t> cursor_pos)
{
    if (slider.remove_cursor(clicked)) {
        return true;
    }

    return UIElement::remove_cursor(clicked, cursor_pos);
}

Cursor::State UIChatbar::send_cursor(bool clicking,
                                     Point<std::int16_t> cursorpos)
{
    if (slider.is_enabled()) {
        auto cursoroffset
            = cursorpos
              - Point<std::int16_t>(position.x(), get_chat_top() + 5);
        Cursor::State sstate = slider.send_cursor(cursoroffset, clicking);
        if (sstate != Cursor::IDLE) {
            return sstate;
        }
    }

    if (chat_field.get_state() == Textfield::NORMAL) {
        Cursor::State tstate = chat_field.send_cursor(cursorpos, clicking);
        if (tstate != Cursor::IDLE) {
            return tstate;
        }
    }

    auto chattop
        = Rectangle<std::int16_t>(0, 502, get_chat_top(), get_chat_top() + 6);
    bool contains = chattop.contains(cursorpos);
    if (drag_chat_top) {
        if (clicking) {
            std::int16_t ydelta = cursorpos.y() - get_chat_top();
            while (ydelta > 0 && chat_rows > MINCHATROWS) {
                --chat_rows;
                ydelta -= CHAT_ROW_HEIGHT;
            }
            while (ydelta < 0 && chat_rows < MAXCHATROWS) {
                ++chat_rows;
                ydelta += CHAT_ROW_HEIGHT;
            }
            chat_box.setheight(1 + chat_rows * CHAT_ROW_HEIGHT);
            slider.set_rows(row_pos, chat_rows, row_max);
            slider.set_vertical({0, CHAT_ROW_HEIGHT * chat_rows - 14});
            return Cursor::CLICKING;
        } else {
            drag_chat_top = false;
        }
    } else if (contains) {
        if (clicking) {
            drag_chat_top = true;
            return Cursor::CLICKING;
        } else {
            return Cursor::CAN_CLICK;
        }
    }

    return UIElement::send_cursor(clicking, cursorpos);
}

void UIChatbar::send_line(std::string&& line, LineType type)
{
    ++row_max;
    row_pos = row_max;

    slider.set_rows(row_pos, chat_rows, row_max);

    row_texts.emplace(std::piecewise_construct,
                      std::forward_as_tuple(row_max),
                      std::forward_as_tuple(Text::A12M,
                                            Text::LEFT,
                                            [](auto type) {
                                                switch (type) {
                                                case RED:
                                                    return Text::DARKRED;
                                                case BLUE:
                                                    return Text::MEDIUMBLUE;
                                                case YELLOW:
                                                    return Text::YELLOW;
                                                default:
                                                    return Text::WHITE;
                                                }
                                            }(type),
                                            std::move(line),
                                            480));
}

std::int16_t UIChatbar::get_chat_top() const
{
    return position.y() - chat_rows * CHAT_ROW_HEIGHT - CHAT_Y_OFFSET;
}
} // namespace jrc
