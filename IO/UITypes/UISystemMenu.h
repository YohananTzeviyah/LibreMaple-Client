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
#pragma once
#include "../../Graphics/Texture.h"
#include "../Components/MapleButton.h"
#include "../UIElement.h"

#include <string_view>

namespace jrc
{
class UISystemMenu : public UIElement
{
public:
    static constexpr const Type TYPE = SYSTEM_MENU;
    static constexpr const bool FOCUSED = false;
    static constexpr const bool TOGGLED = true;

    UISystemMenu();

    void draw(float inter) const override;
    bool remove_cursor(bool clicked, Point<std::int16_t> cursor_pos) override;

protected:
    Button::State button_pressed(std::uint16_t button_id) override;

private:
    Texture top;
    Texture mid;
    Texture bottom;

    enum Buttons : std::uint16_t {
        BT_CHANNEL,
        BT_FARM,
        BT_KEY_SETTING,
        BT_GAME_OPTION,
        BT_SYSTEM_OPTION,
        BT_QUIT,
        NUM_BUTTONS
    };

    static constexpr const std::array<std::string_view, NUM_BUTTONS>
        BUTTON_SRC_NAMES{{"BtChannel",
                          "BtMonsterLife",
                          "BtKeySetting",
                          "BtGameOption",
                          "BtSystemOption",
                          "BtGameQuit"}};

    static constexpr const std::int16_t WIDTH = 79;
    static constexpr const std::int16_t BUTTON_PADDING_HORIZ = 8;

    static constexpr const std::int16_t PADDING_TOP = 20;
    static constexpr const std::int16_t STRIDE_VERT = 27;
    static constexpr const std::int16_t PADDING_BOTTOM = 12;
    static constexpr const std::int16_t HEIGHT
        = PADDING_TOP + STRIDE_VERT * NUM_BUTTONS + PADDING_BOTTOM;

    static constexpr const Point<std::int16_t> POSITION{673, 527};

    static constexpr const std::int16_t ACTIVATION_BUTTON_WIDTH = 55;
    static constexpr const std::int16_t ACTIVATION_BUTTON_HEIGHT = 35;
    static constexpr const Rectangle<std::int16_t> ACTIVATION_BUTTON_AREA{
        POSITION + Point<std::int16_t>{WIDTH / 2, HEIGHT}
            + Point<std::int16_t>{-ACTIVATION_BUTTON_WIDTH / 2, 0},
        POSITION + Point<std::int16_t>{WIDTH / 2, HEIGHT}
            + Point<std::int16_t>{ACTIVATION_BUTTON_WIDTH / 2,
                                  ACTIVATION_BUTTON_HEIGHT}};
};
} // namespace jrc
