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
#include "ScrollingNotice.h"

#include "../../Constants.h"

namespace jrc
{
ScrollingNotice::ScrollingNotice()
{
    background = {Constants::GAME_VIEW_WIDTH, 20, ColorBox::BLACK, 0.6f};
    backposition = {0, -Constants::VIEW_Y_OFFSET};
    notice = Text(Text::A12M, Text::LEFT, Text::YELLOW);
    xpos.set(0.0);
    active = false;
}

void ScrollingNotice::set_notice(std::string&& n) noexcept
{
    notice.change_text(std::move(n));
    xpos.set(static_cast<double>(Constants::GAME_VIEW_WIDTH));
    active = n.size() > 0;
}

void ScrollingNotice::draw(float alpha) const
{
    if (active) {
        std::int16_t interx
            = static_cast<std::int16_t>(std::round(xpos.get(alpha)));
        auto position
            = Point<std::int16_t>(interx, -Constants::VIEW_Y_OFFSET - 2);
        background.draw(backposition);
        notice.draw(position);
    }
}

void ScrollingNotice::update() noexcept
{
    if (active) {
        xpos -= 0.5;

        auto xmin = static_cast<double>(-notice.width());
        if (xpos.last() < xmin) {
            xpos.set(static_cast<double>(Constants::GAME_VIEW_WIDTH));
        }
    }
}
} // namespace jrc
