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
#include "UIBuffList.h"

#include "../../Data/ItemData.h"
#include "../../Util/Misc.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

#include <cstring>

namespace jrc
{
BuffIcon::BuffIcon(std::int32_t buff, std::int32_t dur)
    : cover{IconCover::BUFF, dur - FLASH_TIME}
{
    buffid = buff;
    duration = dur;
    opacity.set(1.0f);
    opcstep = -0.05f;

    if (buffid >= 0) {
        std::string str_id = string_format::extend_id(buffid, 7);
        char str_id_buf[7];
        std::memcpy(str_id_buf, str_id.data(), 3);
        std::memcpy(str_id_buf + 3, ".img", 4);
        nl::node src =
            nl::nx::skill[std::string_view{str_id_buf, 7}]["skill"][str_id];
        icon = src["icon"];
    } else {
        icon = ItemData::get(-buffid).get_icon(true);
    }
}

void BuffIcon::draw(Point<std::int16_t> position, float alpha) const
{
    icon.draw({position, opacity.get(alpha)});
    cover.draw(position + Point<std::int16_t>(1, -31), alpha);
}

bool BuffIcon::update()
{
    if (duration <= FLASH_TIME) {
        opacity += opcstep;

        bool fadedout = opcstep < 0.0f && opacity.last() <= 0.0f;
        bool fadedin = opcstep > 0.0f && opacity.last() >= 1.0f;
        if (fadedout || fadedin) {
            opcstep = -opcstep;
        }
    }

    cover.update();

    duration -= Constants::TIMESTEP;
    return duration < Constants::TIMESTEP;
}

UIBuffList::UIBuffList()
{
    position = {750, 40};
    active = true;
}

void UIBuffList::draw(float alpha) const
{
    Point<std::int16_t> icpos = position;
    for (auto& icon : icons) {
        icon.second.draw(icpos, alpha);
        icpos.shift_x(-32);
    }
}

void UIBuffList::update()
{
    for (auto iter = icons.begin(); iter != icons.end();) {
        bool expired = iter->second.update();
        if (expired) {
            iter = icons.erase(iter);
        } else {
            ++iter;
        }
    }
}

Cursor::State UIBuffList::send_cursor(bool pressed,
                                      Point<std::int16_t> cursorposition)
{
    return UIElement::send_cursor(pressed, cursorposition);
}

void UIBuffList::add_buff(std::int32_t buffid, std::int32_t duration)
{
    icons.emplace(std::piecewise_construct,
                  std::forward_as_tuple(buffid),
                  std::forward_as_tuple(buffid, duration));
}
} // namespace jrc
