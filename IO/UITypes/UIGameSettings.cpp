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
#include "UIGameSettings.h"

#include "../../Configuration.h"
#include "../../Gameplay/Stage.h"
#include "../../Net/Packets/GameplayPackets.h"
#include "../Components/MapleButton.h"
#include "../UI.h"
#include "nlnx/nx.hpp"

namespace jrc
{
UIGameSettings::UIGameSettings()
    : UIDragElement<Configuration::PositionOf::GAME_SETTINGS>{DRAG_AREA}
{
    nl::node source = nl::nx::ui["UIWindow2.img"]["GameOpt"];

    sprites.emplace_back(source["backgrnd"]);
    sprites.emplace_back(source["backgrnd2"]);
    sprites.emplace_back(source["backgrnd3"]);

    buttons[BT_CANCEL]
        = std::make_unique<MapleButton>(source["BtCancle" /* [sic] */]);
    buttons[BT_OK] = std::make_unique<MapleButton>(source["BtOK"]);

    check_texture = source["check"];

    load_settings();

    dimension = {WIDTH, HEIGHT};
    active = true;
}

void UIGameSettings::draw(float inter) const
{
    UIElement::draw(inter);

    auto check_pos = position + CHECKS_OFFSET;
    for (std::uint16_t check = 0; check < NUM_CHECKS; ++check) {
        bool checked = checks_state & static_cast<std::uint16_t>(1 << check);
        check_texture.draw({check_pos, checked ? 1.0f : 0.1f});
        check_pos += {0, STRIDE_VERT};
    }
}

Cursor::State UIGameSettings::send_cursor(bool pressed,
                                          Point<std::int16_t> cursor_pos)
{
    Cursor::State drag_state = UIDragElement::send_cursor(pressed, cursor_pos);
    if (dragged) {
        return drag_state;
    }

    auto [normal_x, normal_y] = cursor_pos - position - CHECKS_OFFSET;
    if (normal_x < 0 || normal_x > CHECK_SIDE_LEN || normal_y < 0) {
        return Cursor::IDLE;
    }

    std::uint16_t button_ix = normal_y / STRIDE_VERT;
    if (button_ix >= NUM_CHECKS
        || STRIDE_VERT * button_ix + CHECK_SIDE_LEN < normal_y) {
        return Cursor::IDLE;
    }

    if (pressed) {
        checks_state ^= static_cast<std::uint16_t>(1 << button_ix);
    }

    return Cursor::CAN_CLICK;
}

Button::State UIGameSettings::button_pressed(std::uint16_t button_id)
{
    switch (button_id) {
    case BT_CANCEL:
        active = false;
        load_settings();
        return Button::NORMAL;
    case BT_OK:
        commit();
        active = false;
        return Button::NORMAL;
    default:
        return Button::PRESSED;
    }
}

void UIGameSettings::load_settings() noexcept
{
    checks_state
        = Configuration::get()
              .get_character(std::string{Stage::get().get_player().get_name()})
              .game_settings.flags;
}

void UIGameSettings::commit() const noexcept
{
    Configuration::get()
        .get_character(std::string{Stage::get().get_player().get_name()})
        .game_settings.flags
        = checks_state;
}
} // namespace jrc
