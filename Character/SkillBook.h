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
#include "boost/container/flat_map.hpp"

#include <cstdint>
#include <unordered_map>

namespace jrc
{
//! Class that stores all information about the skills of an individual
//! character.
class Skillbook
{
public:
    struct SkillEntry {
        std::int32_t level;
        std::int32_t master_level;
        std::int64_t expiration;
    };

    void set_skill(std::int32_t id,
                   std::int32_t level,
                   std::int32_t master_level,
                   std::int64_t expiration) noexcept;

    [[nodiscard]] bool has_skill(std::int32_t id) const noexcept;
    [[nodiscard]] std::int32_t get_level(std::int32_t id) const noexcept;
    [[nodiscard]] std::int32_t get_master_level(std::int32_t id) const
        noexcept;
    [[nodiscard]] std::int64_t get_expiration(std::int32_t id) const noexcept;

    [[nodiscard]] const std::unordered_map<std::int32_t, SkillEntry>&
    get_entries() const noexcept;

    //! Return ID and level of all passive skills.
    //!
    //! An ordered map is used so that lower passive skills don't override
    //! higher ones.
    [[nodiscard]] boost::container::flat_map<std::int32_t, std::int32_t>
    collect_passives() const noexcept;

private:
    std::unordered_map<std::int32_t, SkillEntry> skill_entries;
};
} // namespace jrc
