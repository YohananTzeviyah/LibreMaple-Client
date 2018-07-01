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
#include "../../Template/EnumMap.h"
#include "../Components/SliderHoriz.h"
#include "../UIDragElement.h"

#include <algorithm>
#include <optional>

namespace jrc
{
//! UI popup for changing game system settings.
class UISystemSettings
    : public UIDragElement<Configuration::PositionOf::SYSTEM_SETTINGS>
{
public:
    static constexpr const Type TYPE = SYSTEM_SETTINGS;
    static constexpr const bool FOCUSED = false;
    static constexpr const bool TOGGLED = true;

    UISystemSettings();

    void draw(float inter) const override;
    Cursor::State send_cursor(bool clicked,
                              Point<std::int16_t> cursor_pos) override;

    enum Checks : std::uint16_t {
        RESOLUTION_800_600,
        RESOLUTION_1024_768,
        BGM_MUTE,
        SFX_MUTE,
        SHAKE_SCREEN,
        WINDOWED,
        FULL_SCREEN,
        MINIMAP_NORMAL,
        MINIMAP_SIMPLE,

        NUM_CHECKS
    };

protected:
    Button::State button_pressed(std::uint16_t button_id) override;

private:
    void load_settings() noexcept;
    void commit() const noexcept;
    //! Takes a normalized position.
    [[nodiscard]] static std::optional<Checks>
    check_by_pos(Point<std::int16_t> pos) noexcept;

    enum Slider : std::uint8_t {
        PIC_QUALITY,
        BGM_VOL,
        SFX_VOL,
        CURSOR_SPEED,
        HP_ALERT,
        MP_ALERT,

        NUM_SLIDERS
    };

    EnumMap<Slider, SliderHoriz, NUM_SLIDERS> sliders;

    Texture check_texture;
    std::uint16_t checks_state;

    enum Buttons : std::uint16_t { BT_CANCEL, BT_OK };

    static constexpr const std::int16_t WIDTH = 283;
    static constexpr const std::int16_t SLIDER_LEFT = 96;
    static constexpr const std::int16_t SLIDER_RIGHT = 206;
    static constexpr const std::int16_t SLIDER_RIGHT_SHORT = 160;

    static constexpr const std::int16_t HEIGHT = 419;
    static constexpr const std::int16_t DRAG_HEIGHT = 17;
    static constexpr const std::int16_t SLIDER_Y_OFFSET = 31;
    static constexpr const std::int16_t SLIDER_STRIDE = 30;

    static constexpr const std::int16_t CHECK_SIDE_LEN = 6;

    static constexpr const Point<std::int16_t> DRAG_AREA{WIDTH, DRAG_HEIGHT};
    static constexpr const std::array<Point<std::int16_t>, NUM_CHECKS>
        CHECK_POSITIONS{{{69, 64},
                         {169, 64},
                         {227, 94},
                         {227, 124},
                         {69, 274},
                         {69, 327},
                         {69, 340},
                         {69, 365},
                         {151, 365}}};
};
} // namespace jrc
