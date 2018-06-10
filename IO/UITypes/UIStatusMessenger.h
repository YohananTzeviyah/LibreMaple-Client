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
#pragma once
#include "../../Graphics/Text.h"
#include "../../Template/Interpolated.h"
#include "../UIElement.h"

#include <deque>

namespace jrc
{
class StatusInfo
{
public:
    StatusInfo(std::string&& str, Text::Color color);

    void draw(Point<std::int16_t> position, float alpha) const;
    bool update();

private:
    Text text;
    Text shadow;
    Linear<float> opacity;

    //! 8 seconds.
    static constexpr std::int64_t FADE_DURATION = 8'000;
};

class UIStatusMessenger : public UIElement
{
public:
    static constexpr Type TYPE = STATUSMESSENGER;
    static constexpr bool FOCUSED = false;
    static constexpr bool TOGGLED = false;

    UIStatusMessenger();

    void draw(float alpha) const override;
    void update() override;

    void show_status(Text::Color color, std::string&& message);

private:
    static constexpr std::size_t MAX_MESSAGES = 5ull;

    std::deque<StatusInfo> statusinfos;
};
} // namespace jrc
