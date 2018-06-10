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
#include "../Audio/Audio.h"
#include "../Character/Inventory/Weapon.h"
#include "../Character/Look/Stance.h"
#include "EquipData.h"

namespace jrc
{
//! Contains information about a weapon.
class WeaponData : public Cache<WeaponData>
{
public:
    //! Returns whether the weapon was loaded correctly.
    [[nodiscard]] bool is_valid() const noexcept;
    //! Returns whether the weapon was loaded correctly.
    explicit operator bool() const noexcept;

    //! Return whether this weapon uses twohanded-stances.
    [[nodiscard]] bool is_two_handed() const noexcept;
    //! Return the attack speed.
    [[nodiscard]] std::uint8_t get_speed() const noexcept;
    //! Return the attack type.
    [[nodiscard]] std::uint8_t get_attack() const noexcept;
    //! Return the speed as displayed in a tooltip.
    [[nodiscard]] std::string_view get_speed_string() const noexcept;
    //! Return the attack delay.
    [[nodiscard]] std::uint8_t get_attack_delay() const noexcept;
    //! Return the weapon type.
    [[nodiscard]] Weapon::Type get_type() const noexcept;
    //! Return the sound to play when attacking.
    [[nodiscard]] Sound get_use_sound(bool degenerate) const noexcept;
    //! Return the name of the afterimage.
    [[nodiscard]] std::string_view get_afterimage() const noexcept;
    //! Return the general equip data.
    [[nodiscard]] const EquipData& get_equip_data() const noexcept;

private:
    //! Allow the cache to use the constructor.
    friend Cache<WeaponData>;
    //! Load a weapon from the game files.
    WeaponData(std::int32_t equip_id);

    const EquipData& equip_data;

    Weapon::Type type;
    bool two_handed;
    std::uint8_t attack_speed;
    std::uint8_t attack;
    BoolPair<Sound> use_sounds;
    std::string afterimage;
};
} // namespace jrc
