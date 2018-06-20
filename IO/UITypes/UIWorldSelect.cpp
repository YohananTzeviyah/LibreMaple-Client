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
#include "UIWorldSelect.h"

#include "../../Configuration.h"
#include "../../Constants.h"
#include "../../Gameplay/Stage.h"
#include "../../Graphics/Sprite.h"
#include "../../IO/Components/MapleButton.h"
#include "../../IO/Components/TwoSpriteButton.h"
#include "../../IO/UI.h"
#include "../../Net/Packets/LoginPackets.h"
#include "nlnx/nx.hpp"

namespace jrc
{
UIWorldSelect::UIWorldSelect(std::vector<World>&& worlds_)
    : UIElement({0, 0}, {Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT}),
      worlds(std::move(worlds_))
{
    world_id = Configuration::get().account.world;
    channel_id = Configuration::get().account.channel;

    nl::node back = nl::nx::map["Back"]["login.img"]["back"];
    nl::node world_src
        = nl::nx::ui["Login.img"]["WorldSelect"]["BtWorld"]["release"];
    nl::node channel_src = nl::nx::ui["Login.img"]["WorldSelect"]["BtChannel"];
    nl::node frame = nl::nx::ui["Login.img"]["Common"]["frame"];

    sprites.emplace_back(back["11"], Point<std::int16_t>{370, 300});
    sprites.emplace_back(world_src["layer:bg"], Point<std::int16_t>{650, 45});
    sprites.emplace_back(frame, Point<std::int16_t>{400, 290});

    buttons[BT_ENTERWORLD] = std::make_unique<MapleButton>(
        channel_src["button:GoWorld"], Point<std::int16_t>{200, 170});

    if (worlds.size() <= 0) {
        return;
    }

    const World& world = worlds.front();

    buttons[BT_WORLD0] = std::make_unique<MapleButton>(
        world_src["button:15"], Point<std::int16_t>{650, 20});
    buttons[BT_WORLD0]->set_state(Button::PRESSED);

    sprites.emplace_back(channel_src["layer:bg"],
                         Point<std::int16_t>{200, 170});
    sprites.emplace_back(channel_src["release"]["layer:15"],
                         Point<std::int16_t>{200, 170});

    if (channel_id >= world.channel_count) {
        channel_id = 0;
    }

    for (std::uint8_t i = 0; i < world.channel_count; ++i) {
        nl::node ch_node = channel_src["button:" + std::to_string(i)];
        buttons[BT_CHANNEL0 + i]
            = std::make_unique<TwoSpriteButton>(ch_node["normal"]["0"],
                                                ch_node["keyFocused"]["0"],
                                                Point<std::int16_t>{200, 170});
        if (i == channel_id) {
            buttons[BT_CHANNEL0 + i]->set_state(Button::PRESSED);
        }
    }
}

void UIWorldSelect::draw(float alpha) const
{
    UIElement::draw(alpha);
}

std::uint8_t UIWorldSelect::get_world_id() const
{
    return world_id;
}

std::uint8_t UIWorldSelect::get_channel_id() const
{
    return channel_id;
}

Button::State UIWorldSelect::button_pressed(std::uint16_t id)
{
    if (id == BT_ENTERWORLD) {
        UI::get().disable();

        Stage::get().set_world(world_id);
        Stage::get().set_channel(channel_id);
        Stage::get().set_channel_count(worlds.at(world_id).channel_count);
        CharlistRequestPacket{world_id, channel_id}.dispatch();

        return Button::PRESSED;
    } else if (id >= BT_WORLD0 && id < BT_CHANNEL0) {
        buttons[BT_WORLD0 + world_id]->set_state(Button::NORMAL);
        world_id = static_cast<std::uint8_t>(id - BT_WORLD0);
        return Button::PRESSED;
    } else {
        buttons[BT_CHANNEL0 + channel_id]->set_state(Button::NORMAL);
        channel_id = static_cast<std::uint8_t>(id - BT_CHANNEL0);
        return Button::PRESSED;
    }
}
} // namespace jrc
