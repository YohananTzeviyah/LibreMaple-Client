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
#include "SkillBook.h"

#include "../Data/SkillData.h"

namespace jrc
{
void Skillbook::set_skill(std::int32_t id,
                          std::int32_t level,
                          std::int32_t master_level,
                          std::int64_t expire) noexcept
{
    skill_entries[id] = {level, master_level, expire};
}

bool Skillbook::has_skill(std::int32_t id) const noexcept
{
    return skill_entries.count(id) > 0;
}

std::int32_t Skillbook::get_level(std::int32_t id) const noexcept
{
    auto iter = skill_entries.find(id);
    if (iter == skill_entries.end()) {
        return 0;
    }

    return iter->second.level;
}

std::int32_t Skillbook::get_master_level(std::int32_t id) const noexcept
{
    auto iter = skill_entries.find(id);
    if (iter == skill_entries.end()) {
        return 0;
    }

    return iter->second.master_level;
}

std::int64_t Skillbook::get_expiration(std::int32_t id) const noexcept
{
    auto iter = skill_entries.find(id);
    if (iter == skill_entries.end()) {
        return 0;
    }

    return iter->second.expiration;
}

const std::unordered_map<int32_t, Skillbook::SkillEntry>&
Skillbook::get_entries() const noexcept
{
    return skill_entries;
}

boost::container::flat_map<std::int32_t, std::int32_t>
Skillbook::collect_passives() const noexcept
{
    boost::container::flat_map<std::int32_t, std::int32_t> passives;
    for (auto [id, entry] : skill_entries) {
        if (SkillData::get(id).is_passive()) {
            passives.emplace(id, entry.level);
        }
    }
    return passives;
}
} // namespace jrc
