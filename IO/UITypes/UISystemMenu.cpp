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
#include "UISystemMenu.h"

#include "../UI.h"
#include "nlnx/nx.hpp"

namespace jrc
{
UISystemMenu::UISystemMenu() : UIElement{POSITION, {WIDTH, HEIGHT}, true}
{
    nl::node source = nl::nx::ui["StatusBar2.img"]["mainBar"]["System"];

    nl::node background = source["backgrnd"];
    top = background["0"];
    mid = background["1"];
    bottom = background["2"];

    std::int16_t y_offset = PADDING_TOP;
    for (std::uint16_t i = 0; i < NUM_BUTTONS; ++i) {
        buttons[i] = std::make_unique<MapleButton>(
            source[BUTTON_SRC_NAMES[i]],
            Point<std::int16_t>{BUTTON_PADDING_HORIZ, y_offset});
        y_offset += STRIDE_VERT;
    }
}

void UISystemMenu::draw(float inter) const
{
    Point<std::int16_t> bg_pos{POSITION};

    top.draw({bg_pos});
    bg_pos += {0, top.height()};

    std::int16_t mid_height = HEIGHT - top.height() - bottom.height();
    mid.draw({bg_pos, {0, mid_height}});
    bg_pos += {0, mid_height};

    bottom.draw({bg_pos});

    UIElement::draw_buttons(inter);
}

Button::State UISystemMenu::button_pressed(std::uint16_t button_id)
{
    switch (button_id) {
    case BT_CHANNEL:
        break;
    case BT_FARM:
        break;
    case BT_KEY_SETTING:
        UI::get().send_menu(KeyAction::KEY_CONFIG);
        break;
    case BT_GAME_OPTION:
        break;
    case BT_SYSTEM_OPTION:
        break;
    case BT_QUIT:
        UI::get().quit();
        break;
    }

    active = false;

    return Button::NORMAL;
}
} // namespace jrc
