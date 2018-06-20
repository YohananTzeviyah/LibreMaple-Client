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
#include "../../Template/BoolPair.h"
#include "../UIDragElement.h"
#include "boost/container/static_vector.hpp"

#include <optional>

namespace jrc
{
//! UI popup for changing channels.
class UIChangeChannel
    : public UIDragElement<Configuration::PositionOf::CHANGE_CHANNEL>
{
public:
    static constexpr const Type TYPE = CHANGE_CHANNEL;
    static constexpr const bool FOCUSED = false;
    static constexpr const bool TOGGLED = true;

    UIChangeChannel();

    void draw(float inter) const override;
    void toggle_active() override;

    Cursor::State send_cursor(bool pressed,
                              Point<std::int16_t> cursor_pos) override;
    void double_click(Point<std::int16_t> cursor_pos) override;

    enum class SentKey { NONE, UP, DOWN, LEFT, RIGHT, ENTER };

    void send_key(SentKey key) noexcept;

protected:
    Button::State button_pressed(std::uint16_t button_id) override;

private:
    std::optional<std::uint8_t>
    channel_by_pos(Point<std::int16_t> cursor_pos) const noexcept;

    void change_channel();

    enum Buttons : std::uint16_t { BT_CANCEL, BT_CHANGE };

    static constexpr const std::int16_t WIDTH = 370;
    static constexpr const std::uint8_t COLS = 5;
    static constexpr const std::int16_t STRIDE_HORIZ = 70;

    static constexpr const std::int16_t HEIGHT = 168;
    static constexpr const std::uint8_t ROWS = 4;
    static constexpr const std::int16_t STRIDE_VERT = 20;
    static constexpr const std::int16_t DRAG_HEIGHT = 20;

    static constexpr const Point<std::int16_t> DRAG_AREA{WIDTH, DRAG_HEIGHT};
    static constexpr const Point<std::int16_t> CH_SPRITE_OFFSET{21, 61};
    static constexpr const Point<std::int16_t> COVER_TEXTURE_OFFSET{12, 56};
    static constexpr const Point<std::int16_t> WORLD_BADGE_OFFSET{16, 30};

    static constexpr const Rectangle<std::int16_t> COVER_AREA{
        COVER_TEXTURE_OFFSET,
        COVER_TEXTURE_OFFSET
            + Point<std::int16_t>{COLS * STRIDE_HORIZ, ROWS* STRIDE_VERT}};

    boost::container::static_vector<Sprite, COLS * ROWS> ch_sprites;
    BoolPair<Texture> channel_cover;
    std::uint8_t selected_channel;
};
} // namespace jrc
