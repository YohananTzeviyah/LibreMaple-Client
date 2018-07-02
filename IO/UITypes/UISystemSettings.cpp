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
#include "UISystemSettings.h"

#include "../../Configuration.h"
#include "../../Gameplay/Stage.h"
#include "../../Net/Packets/GameplayPackets.h"
#include "../Components/MapleButton.h"
#include "../UI.h"
#include "nlnx/nx.hpp"

namespace jrc
{
UISystemSettings::UISystemSettings()
    : UIDragElement<Configuration::PositionOf::SYSTEM_SETTINGS>{DRAG_AREA}
{
    nl::node source = nl::nx::ui["UIWindow2.img"]["SysOpt"];

    sprites.emplace_back(source["backgrnd"]);
    sprites.emplace_back(source["backgrnd2"]);

    buttons[BT_CANCEL]
        = std::make_unique<MapleButton>(source["BtCancle" /* [sic] */]);
    buttons[BT_OK] = std::make_unique<MapleButton>(source["BtOK"]);

    nl::node scroll_src = source["scroll"];
    sliders[PIC_QUALITY] = {scroll_src,
                            {SLIDER_LEFT, SLIDER_RIGHT},
                            SLIDER_Y_OFFSET,
                            0,
                            1,
                            [](std::int16_t, bool) {}};
    sliders[BGM_VOL] = {scroll_src,
                        {SLIDER_LEFT, SLIDER_RIGHT_SHORT},
                        SLIDER_Y_OFFSET + SLIDER_STRIDE * 2,
                        0,
                        50,
                        [](std::int16_t new_col, bool) {
                            Music::set_bgm_volume(new_col * 2);
                        }};
    sliders[SFX_VOL] = {scroll_src,
                        {SLIDER_LEFT, SLIDER_RIGHT_SHORT},
                        SLIDER_Y_OFFSET + SLIDER_STRIDE * 3,
                        0,
                        50,
                        [](std::int16_t new_col, bool) {
                            Sound::set_sfx_volume(new_col * 2);
                        }};
    sliders[CURSOR_SPEED] = {scroll_src,
                             {SLIDER_LEFT, SLIDER_RIGHT},
                             SLIDER_Y_OFFSET + SLIDER_STRIDE * 5,
                             0,
                             2,
                             [](std::int16_t, bool) {}};
    sliders[HP_ALERT] = {scroll_src,
                         {SLIDER_LEFT, SLIDER_RIGHT},
                         SLIDER_Y_OFFSET + SLIDER_STRIDE * 6,
                         0,
                         50,
                         [](std::int16_t, bool) {}};
    sliders[MP_ALERT] = {scroll_src,
                         {SLIDER_LEFT, SLIDER_RIGHT},
                         SLIDER_Y_OFFSET + SLIDER_STRIDE * 7,
                         0,
                         50,
                         [](std::int16_t, bool) {}};

    check_texture = source["check"];

    load_settings();

    dimension = {WIDTH, HEIGHT};
    active = true;
}

void UISystemSettings::draw(float inter) const
{
    UIElement::draw(inter);

    for (std::uint16_t check = 0; check < NUM_CHECKS; ++check) {
        bool checked = checks_state & static_cast<std::uint16_t>(1 << check);
        check_texture.draw(
            {position + CHECK_POSITIONS[check], checked ? 1.0f : 0.1f});
    }

    for (const auto slider_iter : sliders) {
        slider_iter.second.draw(position);
    }
}

Cursor::State UISystemSettings::send_cursor(bool pressed,
                                            Point<std::int16_t> cursor_pos)
{
    Cursor::State drag_state = UIDragElement::send_cursor(pressed, cursor_pos);
    if (dragged) {
        return drag_state;
    }

    const auto normalized = cursor_pos - position;
    for (auto slider_iter : sliders) {
        if (Cursor::State slider_state
            = slider_iter.second.send_cursor(normalized, pressed);
            slider_state != Cursor::IDLE) {
            return slider_state;
        }
    }

    if (auto check = check_by_pos(normalized); check) {
        if (pressed) {
            switch (auto c = *check; c) {
            case RESOLUTION_800_600:
                checks_state
                    |= static_cast<std::uint16_t>(1 << RESOLUTION_800_600);
                checks_state
                    &= ~static_cast<std::uint16_t>(1 << RESOLUTION_1024_768);
                break;
            case RESOLUTION_1024_768:
                checks_state
                    |= static_cast<std::uint16_t>(1 << RESOLUTION_1024_768);
                checks_state
                    &= ~static_cast<std::uint16_t>(1 << RESOLUTION_800_600);
                break;
            case WINDOWED:
                checks_state |= static_cast<std::uint16_t>(1 << WINDOWED);
                checks_state &= ~static_cast<std::uint16_t>(1 << FULL_SCREEN);
                break;
            case FULL_SCREEN:
                checks_state |= static_cast<std::uint16_t>(1 << FULL_SCREEN);
                checks_state &= ~static_cast<std::uint16_t>(1 << WINDOWED);
                break;
            case MINIMAP_NORMAL:
                checks_state
                    |= static_cast<std::uint16_t>(1 << MINIMAP_NORMAL);
                checks_state
                    &= ~static_cast<std::uint16_t>(1 << MINIMAP_SIMPLE);
                break;
            case MINIMAP_SIMPLE:
                checks_state
                    |= static_cast<std::uint16_t>(1 << MINIMAP_SIMPLE);
                checks_state
                    &= ~static_cast<std::uint16_t>(1 << MINIMAP_NORMAL);
                break;
            default:
                checks_state ^= static_cast<std::uint16_t>(1 << c);
                break;
            }
        }
        return Cursor::CAN_CLICK;
    }

    return Cursor::IDLE;
}

Button::State UISystemSettings::button_pressed(std::uint16_t button_id)
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

void UISystemSettings::load_settings() noexcept
{
    checks_state = 0b00000000'00000000;

    sliders[PIC_QUALITY].set_cols(
        Configuration::get().video.low_quality ? 0 : 1, 0, 1);
    checks_state |= 1 << RESOLUTION_1024_768;
    sliders[BGM_VOL].set_cols(
        Configuration::get().audio.volume.music / 2, 0, 50);
    Music::set_bgm_volume(Configuration::get().audio.volume.music);
    if (!Configuration::get().audio.music) {
        checks_state |= 1 << BGM_MUTE;
    }
    sliders[SFX_VOL].set_cols(
        Configuration::get().audio.volume.sound_effects / 2, 0, 50);
    Sound::set_sfx_volume(Configuration::get().audio.volume.sound_effects);
    if (!Configuration::get().audio.sound_effects) {
        checks_state |= 1 << SFX_MUTE;
    }
    sliders[CURSOR_SPEED].set_cols(1, 0, 2);
    sliders[CURSOR_SPEED].set_enabled(false);
    sliders[HP_ALERT].set_cols(Configuration::get().ui.hp_alert / 2, 0, 50);
    sliders[MP_ALERT].set_cols(Configuration::get().ui.mp_alert / 2, 0, 50);
    if (Configuration::get().ui.shake_screen) {
        checks_state |= 1 << SHAKE_SCREEN;
    }
    if (Configuration::get().video.fullscreen) {
        checks_state |= 1 << FULL_SCREEN;
    } else {
        checks_state |= 1 << WINDOWED;
    }
    if (Configuration::get().ui.simple_minimap) {
        checks_state |= 1 << MINIMAP_SIMPLE;
    } else {
        checks_state |= 1 << MINIMAP_NORMAL;
    }
}

void UISystemSettings::commit() const noexcept
{
    Configuration::get().video.low_quality
        = sliders[PIC_QUALITY].get_col() == 0;
    Configuration::get().audio.volume.music = sliders[BGM_VOL].get_col() * 2;
    Music::set_bgm_volume(Configuration::get().audio.volume.music);
    Configuration::get().audio.music = !(checks_state & (1 << BGM_MUTE));
    Configuration::get().audio.volume.sound_effects
        = sliders[SFX_VOL].get_col() * 2;
    Sound::set_sfx_volume(Configuration::get().audio.volume.sound_effects);
    Configuration::get().audio.sound_effects
        = !(checks_state & (1 << SFX_MUTE));
    Configuration::get().ui.hp_alert = sliders[HP_ALERT].get_col() * 2;
    Configuration::get().ui.mp_alert = sliders[MP_ALERT].get_col() * 2;
    Configuration::get().ui.shake_screen = checks_state & (1 << SHAKE_SCREEN);
    Configuration::get().video.fullscreen = checks_state & (1 << FULL_SCREEN);
    Configuration::get().ui.simple_minimap
        = checks_state & (1 << MINIMAP_SIMPLE);
}

std::optional<UISystemSettings::Checks>
UISystemSettings::check_by_pos(Point<std::int16_t> pos) noexcept
{
    if (auto check_pos_iter = std::find_if(
            CHECK_POSITIONS.begin(),
            CHECK_POSITIONS.end(),
            [pos](auto p) {
                return Rectangle<std::int16_t>{p, p + CHECK_SIDE_LEN}.contains(
                    pos);
            });
        check_pos_iter != CHECK_POSITIONS.end()) {
        return {static_cast<Checks>(check_pos_iter - CHECK_POSITIONS.begin())};
    }

    return {};
}
} // namespace jrc
