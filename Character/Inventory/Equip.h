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
#include "EquipQuality.h"

#include <string>

namespace jrc
{
class Equip
{
public:
    enum Potential {
        POT_NONE,
        POT_HIDDEN,
        POT_RARE,
        POT_EPIC,
        POT_UNIQUE,
        POT_LEGENDARY,
        LENGTH
    };

    Equip(std::int32_t item_id,
          std::int64_t expiration,
          const std::string& owner,
          std::int16_t flags,
          std::uint8_t slots,
          std::uint8_t level,
          const EnumMap<Equipstat::Id, std::uint16_t>& stats,
          std::uint8_t itemlevel,
          std::int16_t itemexp,
          std::int32_t vicious);

    std::int32_t get_item_id() const;
    std::int64_t get_expiration() const;
    const std::string& get_owner() const;
    std::int16_t get_flags() const;
    std::uint8_t get_slots() const;
    std::uint8_t get_level() const;
    std::uint8_t get_itemlevel() const;
    std::uint16_t get_stat(Equipstat::Id type) const;
    std::int32_t get_vicious() const;
    Potential get_potrank() const;
    EquipQuality::Id get_quality() const;

private:
    EnumMap<Equipstat::Id, std::uint16_t> stats;
    std::int32_t item_id;
    std::int64_t expiration;
    std::string owner;
    std::int16_t flags;
    std::uint8_t slots;
    std::uint8_t level;
    std::uint8_t itemlevel;
    std::int16_t itemexp;
    std::int32_t vicious;
    Potential potrank;
    EquipQuality::Id quality;
};
} // namespace jrc
