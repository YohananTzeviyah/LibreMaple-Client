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
#include "EquipStat.h"
#include "Inventory/Weapon.h"

#include <cstdint>
#include <string>
#include <string_view>

namespace jrc
{
class Job
{
public:
    enum Level : std::uint16_t { BEGINNER, FIRST, SECOND, THIRD, FOURTH };

    static Level get_next_level(Level level)
    {
        switch (level) {
        case BEGINNER:
            return FIRST;
        case FIRST:
            return SECOND;
        case SECOND:
            return THIRD;
        default:
            return FOURTH;
        }
    }

    Job(std::uint16_t id);
    Job();

    void change_job(std::uint16_t id);
    bool is_sub_job(std::uint16_t subid) const;
    bool can_use(std::int32_t skill_id) const;
    std::uint16_t get_id() const;
    std::uint16_t get_subjob(Level level) const;
    Level get_level() const;
    std::string_view get_name() const;
    Equipstat::Id get_primary(Weapon::Type weapontype) const;
    Equipstat::Id get_secondary(Weapon::Type weapontype) const;

private:
    std::string get_name(std::uint16_t id) const;

    std::string name;
    std::uint16_t id;
    Level level;
};
} // namespace jrc
