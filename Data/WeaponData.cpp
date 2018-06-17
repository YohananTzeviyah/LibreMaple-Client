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
#include "WeaponData.h"

#include "../Console.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

namespace jrc
{
WeaponData::WeaponData(std::int32_t equip_id)
    : equip_data(EquipData::get(equip_id))
{
    std::int32_t prefix = equip_id / 10000;
    type = Weapon::by_value(prefix);
    two_handed = prefix == Weapon::STAFF
                 || (prefix >= Weapon::SWORD_2H && prefix <= Weapon::POLEARM)
                 || prefix == Weapon::CROSSBOW;

    nl::node src = nl::nx::character["Weapon"][str::concat(
        '0', std::to_string(equip_id), ".img")]["info"];

    attack_speed = static_cast<std::uint8_t>(src["attackSpeed"]);
    attack = static_cast<std::uint8_t>(src["attack"]);

    nl::node soundsrc = nl::nx::sound["Weapon.img"][src["sfx"]];

    if (soundsrc["Attack2"].data_type() == nl::node::type::audio) {
        use_sounds[false] = soundsrc["Attack"];
        use_sounds[true] = soundsrc["Attack2"];
    } else {
        use_sounds[false] = soundsrc["Attack"];
        use_sounds[true] = soundsrc["Attack"];
    }

    afterimage = src["afterImage"].get_string();
}

bool WeaponData::is_valid() const noexcept
{
    return equip_data.is_valid();
}

WeaponData::operator bool() const noexcept
{
    return is_valid();
}

bool WeaponData::is_two_handed() const noexcept
{
    return two_handed;
}

std::uint8_t WeaponData::get_speed() const noexcept
{
    return attack_speed;
}

std::uint8_t WeaponData::get_attack() const noexcept
{
    return attack;
}

std::string_view WeaponData::get_speed_string() const noexcept
{
    switch (attack_speed) {
    case 1:
        return "FAST (1)";
    case 2:
        return "FAST (2)";
    case 3:
        return "FAST (3)";
    case 4:
        return "FAST (4)";
    case 5:
        return "NORMAL (5)";
    case 6:
        return "NORMAL (6)";
    case 7:
        return "SLOW (7)";
    case 8:
        return "SLOW (8)";
    case 9:
        return "SLOW (9)";
    default:
        return {};
    }
}

std::uint8_t WeaponData::get_attack_delay() const noexcept
{
    if (type == Weapon::NONE) {
        return 0;
    } else {
        return 50 - 25 / attack_speed;
    }
}

Weapon::Type WeaponData::get_type() const noexcept
{
    return type;
}

Sound WeaponData::get_use_sound(bool degenerate) const noexcept
{
    return use_sounds[degenerate];
}

std::string_view WeaponData::get_afterimage() const noexcept
{
    return afterimage;
}

const EquipData& WeaponData::get_equip_data() const noexcept
{
    return equip_data;
}
} // namespace jrc
