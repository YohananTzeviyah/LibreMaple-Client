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
#include "../Components/TwoSpriteButton.h"
#include "../UIDragElement.h"

namespace jrc
{
//! UI popup for changing in-game settings.
class UIGameSettings
    : public UIDragElement<Configuration::PositionOf::GAME_SETTINGS>
{
public:
    static constexpr const Type TYPE = GAME_SETTINGS;
    static constexpr const bool FOCUSED = false;
    static constexpr const bool TOGGLED = true;

    UIGameSettings();

    void draw(float inter) const override;
    Cursor::State send_cursor(bool clicked,
                              Point<std::int16_t> cursor_pos) override;

    enum Checks : std::uint16_t {
        WHISPERS,
        FRIEND_INVITES,
        CHAT_INVITES,
        TRADE_REQUESTS,
        PARTY_INVITES,
        SIDEKICK_INVITES,
        EXPEDITION_INVITES,
        GUILD_CHAT,
        GUILD_INVITES,
        ALLIANCE_CHAT,
        ALLIANCE_INVITES,
        FAMILY_INVITES,
        FOLLOW,

        NUM_CHECKS
    };

protected:
    Button::State button_pressed(std::uint16_t button_id) override;

private:
    void load_settings() noexcept;
    void commit() const noexcept;

    Texture check_texture;
    std::uint16_t checks_state;

    enum Buttons : std::uint16_t { BT_CANCEL, BT_OK };

    static constexpr const std::int16_t WIDTH = 119;

    static constexpr const std::int16_t HEIGHT = 287;
    static constexpr const std::int16_t STRIDE_VERT = 18;
    static constexpr const std::int16_t DRAG_HEIGHT = 17;

    static constexpr const std::int16_t CHECK_SIDE_LEN = 6;

    static constexpr const Point<std::int16_t> DRAG_AREA{WIDTH, DRAG_HEIGHT};
    static constexpr const Point<std::int16_t> CHECKS_OFFSET{WIDTH - 20, 28};
};
} // namespace jrc
