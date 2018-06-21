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
#include "UIChangeChannel.h"

#include "../../Gameplay/Stage.h"
#include "../../Net/Packets/GameplayPackets.h"
#include "../Components/MapleButton.h"
#include "../UI.h"
#include "nlnx/nx.hpp"

namespace jrc
{
UIChangeChannel::UIChangeChannel()
    : UIDragElement<Configuration::PositionOf::CHANGE_CHANNEL>{DRAG_AREA},
      selected_channel{Stage::get().get_channel()}
{
    nl::node source = nl::nx::ui["UIWindow2.img"]["Channel"];

    sprites.emplace_back(source["backgrnd"]);
    sprites.emplace_back(source["backgrnd2"]);
    sprites.emplace_back(source["backgrnd3"]);
    sprites.emplace_back(source["world"][Stage::get().get_world()],
                         DrawArgument{WORLD_BADGE_OFFSET});

    nl::node ch_src = source["ch"];
    const std::uint8_t num_channels = Stage::get().get_channel_count();
    auto ch_pos = CH_SPRITE_OFFSET;
    for (unsigned row = 0; row < ROWS; ++row) {
        bool finished_ch_sprites = false;
        for (unsigned col = 0; col < COLS; ++col) {
            unsigned n = row * COLS + col;
            ch_sprites.emplace_back(ch_src[n], DrawArgument{ch_pos});
            ch_pos += {STRIDE_HORIZ, 0};

            if (n == num_channels - 1) {
                finished_ch_sprites = true;
                break;
            }
        }
        if (finished_ch_sprites) {
            break;
        }

        ch_pos.set_x(CH_SPRITE_OFFSET.x());
        ch_pos += {0, STRIDE_VERT};
    }

    buttons[BT_CANCEL] = std::make_unique<MapleButton>(source["BtCancel"]);
    buttons[BT_CHANGE] = std::make_unique<MapleButton>(source["BtChange"]);

    channel_cover[false] = source["channel0"];
    channel_cover[true] = source["channel1"];

    dimension = {WIDTH, HEIGHT};
    active = true;
}

void UIChangeChannel::draw(float alpha) const
{
    UIElement::draw(alpha);

    const auto base_pos = COVER_TEXTURE_OFFSET + position;
    std::uint8_t curr_ch = Stage::get().get_channel();
    auto curr_pos = base_pos
                    + Point<std::int16_t>{curr_ch % COLS * STRIDE_HORIZ,
                                          curr_ch / COLS * STRIDE_VERT};

    channel_cover[false].draw({curr_pos});
    if (curr_ch != selected_channel) {
        auto selected_pos
            = base_pos
              + Point<std::int16_t>{selected_channel % COLS * STRIDE_HORIZ,
                                    selected_channel / COLS * STRIDE_VERT};

        channel_cover[true].draw({selected_pos});
    }

    for (const auto& ch_sprite : ch_sprites) {
        ch_sprite.draw(position, alpha);
    }
}

void UIChangeChannel::toggle_active()
{
    UIElement::toggle_active();
}

Cursor::State UIChangeChannel::send_cursor(bool pressed,
                                           Point<std::int16_t> cursor_pos)
{
    Cursor::State drag_state = UIDragElement::send_cursor(pressed, cursor_pos);
    if (dragged) {
        return drag_state;
    }

    if (pressed) {
        if (auto clicked_ch = channel_by_pos(cursor_pos);
            clicked_ch && *clicked_ch < ch_sprites.size()) {
            selected_channel = *clicked_ch;
        }
    }

    return Cursor::IDLE;
}

void UIChangeChannel::double_click(Point<std::int16_t> cursor_pos)
{
    UIDragElement::double_click(cursor_pos);

    if (auto clicked_ch = channel_by_pos(cursor_pos);
        clicked_ch && *clicked_ch < ch_sprites.size()) {
        selected_channel = *clicked_ch;
        change_channel();
    }
}

void UIChangeChannel::send_key(SentKey key) noexcept
{
    switch (key) {
    case SentKey::UP:
        if (selected_channel < COLS) {
            selected_channel += COLS * ROWS;
        }
        do {
            selected_channel -= COLS;
        } while (selected_channel >= ch_sprites.size());
        break;
    case SentKey::DOWN:
        selected_channel += COLS;
        if (selected_channel >= ch_sprites.size()) {
            selected_channel %= COLS;
        }
        break;
    case SentKey::LEFT:
        if (selected_channel % COLS == 0) {
            selected_channel += COLS - 1;
            while (selected_channel >= ch_sprites.size()) {
                --selected_channel;
            }
        } else {
            --selected_channel;
        }
        break;
    case SentKey::RIGHT:
        if (selected_channel % COLS == COLS - 1) {
            selected_channel -= COLS - 1;
        } else if (selected_channel == ch_sprites.size() - 1) {
            selected_channel -= selected_channel % COLS;
        } else {
            ++selected_channel;
        }
        break;
    case SentKey::ENTER:
        change_channel();
        break;
    default:
        break;
    }
}

Button::State UIChangeChannel::button_pressed(std::uint16_t button_id)
{
    switch (button_id) {
    case BT_CANCEL:
        active = false;
        return Button::NORMAL;
    case BT_CHANGE:
        change_channel();
        return Button::PRESSED;
    default:
        return Button::PRESSED;
    }
}

std::optional<std::uint8_t>
UIChangeChannel::channel_by_pos(Point<std::int16_t> cursor_pos) const noexcept
{
    auto p = cursor_pos - position;
    if (!COVER_AREA.contains(p)) {
        return {};
    }

    p -= COVER_TEXTURE_OFFSET;
    std::int16_t col = p.x() / STRIDE_HORIZ;
    std::int16_t row = p.y() / STRIDE_VERT;

    return {row * COLS + col};
}

void UIChangeChannel::change_channel() noexcept
{
    active = false;

    if (selected_channel != Stage::get().get_channel()) {
        UI::get().disable();
        ChangeChannelPacket{selected_channel}.dispatch();
    }
}
} // namespace jrc
