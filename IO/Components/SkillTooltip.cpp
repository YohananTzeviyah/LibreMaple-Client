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
#include "SkillTooltip.h"

#include "../../Data/SkillData.h"
#include "../../Util/Misc.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

namespace jrc
{
SkillTooltip::SkillTooltip() : line(318, Geometry::WHITE, 1.0f)
{
    nl::node itemtt = nl::nx::ui["UIToolTip.img"]["Item"];

    frame = itemtt["Frame2"];
    base = itemtt["ItemIcon"]["base"];
    cover = itemtt["ItemIcon"]["cover"];

    skill_id = 0;
}

void SkillTooltip::set_skill(std::int32_t id,
                             std::int32_t level,
                             std::int32_t mlevel,
                             std::int64_t expiration)
{
    if (skill_id == id) {
        return;
    }

    skill_id = id;

    if (skill_id == 0) {
        return;
    }

    const SkillData& data = SkillData::get(id);

    std::int32_t master_level;
    if (mlevel > 0) {
        master_level = mlevel;
    } else {
        master_level = data.get_master_level();
    }

    std::string desc_str{data.get_desc()};
    if (master_level > 0) {
        auto ml_str = std::to_string(master_level);
        std::size_t ml_start = desc_str.find("Master Level", 0, 12);
        std::size_t ml_pos = desc_str.find(':', ml_start) + 2;
        std::size_t ml_end = desc_str.find(']', ml_start);
        if (ml_pos < ml_end && ml_end != std::string::npos) {
            std::size_t ml_size = ml_end - ml_pos;
            desc_str.erase(ml_pos, ml_size);
            desc_str.insert(ml_pos, ml_str);

            // Fixing errors in the files...
            if (ml_start == 0) {
                desc_str.insert(0, "[", 1);
                ++ml_end;
            }
            std::size_t linebreak = desc_str.find("]\\n", ml_start, 3);
            if (linebreak != ml_end) {
                desc_str.insert(ml_end + 1, "\\n", 2);
            }
        } else {
            desc_str.insert(
                0, str::concat("[Master Level: ", std::move(ml_str), "]\\n"));
        }
    }

    if (expiration > 0) {
        // TODO
    } else {
        std::size_t exp_start = desc_str.find("#cAvailable until", 0, 17);
        std::size_t exp_end = desc_str.find('#', exp_start + 1);
        if (exp_start < exp_end && exp_end != std::string::npos) {
            std::size_t expsize = exp_end - exp_start + 1;
            desc_str.erase(exp_start, expsize);
        }
    }

    if (data.is_passive()) {
        desc_str += "\\r#cPassive Skill#";
    }

    icon = data.get_icon(SkillData::NORMAL);
    name = {Text::A12B,
            Text::LEFT,
            Text::WHITE,
            std::string{data.get_name()},
            320};
    desc = {Text::A12M, Text::LEFT, Text::WHITE, std::move(desc_str), 230};
    leveldesc = {
        Text::A12M,
        Text::LEFT,
        Text::WHITE,
        [level, master_level, &data]() {
            if (level > 0) {
                if (level < master_level) {
                    return str::concat("[Current Level: ",
                                       std::to_string(level),
                                       "]\\n",
                                       data.get_level_desc(level),
                                       "\\n",
                                       "[Next Level: ",
                                       std::to_string(level + 1),
                                       "]\\n",
                                       data.get_level_desc(level + 1));
                } else {
                    return str::concat("[Current Level: ",
                                       std::to_string(level),
                                       "]\\n",
                                       data.get_level_desc(level));
                }
            } else {
                if (level < master_level) {
                    return str::concat("[Next Level: ",
                                       std::to_string(level + 1),
                                       "]\\n",
                                       data.get_level_desc(level + 1));
                } else {
                    return std::string{};
                }
            }
        }(),
        330};

    icon_offset = 4 + name.height();
    level_offset = std::max<std::int16_t>(desc.height(), 92) + 16;
    height = icon_offset + level_offset + leveldesc.height();
}

void SkillTooltip::draw(Point<std::int16_t> pos) const
{
    if (skill_id == 0) {
        return;
    }

    frame.draw(pos + Point<std::int16_t>(176, height + 16), 320, height);
    name.draw(pos + Point<std::int16_t>(16, 8));

    pos.shift_y(icon_offset);

    base.draw({pos + Point<std::int16_t>(12, 16)});
    icon.draw({pos + Point<std::int16_t>(22, 90), 2.0f, 2.0f});
    cover.draw({pos + Point<std::int16_t>(12, 16)});

    desc.draw(pos + Point<std::int16_t>(102, 12));

    pos.shift_y(level_offset);

    line.draw(pos + Point<std::int16_t>(14, 4));
    leveldesc.draw(pos + Point<std::int16_t>(12, 12));
}
} // namespace jrc
