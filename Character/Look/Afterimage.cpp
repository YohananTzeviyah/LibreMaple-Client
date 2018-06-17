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
#include "Afterimage.h"

#include "../../Util/Misc.h"
#include "nlnx/nx.hpp"

#include <string_view>

namespace jrc
{
Afterimage::Afterimage(std::int32_t skill_id,
                       std::string_view name,
                       std::string_view stance_name,
                       std::int16_t level)
{
    nl::node src;
    if (skill_id > 0) {
        std::string str_id = string_format::extend_id(skill_id, 7);
        src = nl::nx::skill[str::concat(std::string_view(str_id).substr(0, 3),
                                        ".img")]["skill"][str_id]["afterimage"]
                           [name][stance_name];
    }

    if (!src) {
        src = nl::nx::character["Afterimage"][str::concat(name, ".img")]
                               [level / 10][stance_name];
    }

    range = src;
    first_frame = 0;
    displayed = false;

    for (nl::node sub : src) {
        std::uint8_t frame
            = string_conversion::or_default<std::uint8_t>(sub.name(), 255);
        if (frame < 255) {
            animation = sub;
            first_frame = frame;
        }
    }
}

Afterimage::Afterimage() noexcept
{
    first_frame = 0;
    displayed = true;
}

void Afterimage::draw(std::uint8_t st_frame,
                      const DrawArgument& args,
                      float alpha) const
{
    if (!displayed && st_frame >= first_frame) {
        animation.draw(args, alpha);
    }
}

void Afterimage::update(std::uint8_t st_frame, std::uint16_t timestep)
{
    if (!displayed && st_frame >= first_frame) {
        displayed = animation.update(timestep);
    }
}

std::uint8_t Afterimage::get_first_frame() const
{
    return first_frame;
}

Rectangle<std::int16_t> Afterimage::get_range() const
{
    return range;
}
} // namespace jrc
