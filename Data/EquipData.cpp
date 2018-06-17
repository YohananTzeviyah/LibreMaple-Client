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
#include "EquipData.h"

#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

namespace jrc
{
EquipData::EquipData(std::int32_t id) : itemdata(ItemData::get(id))
{
    std::string str_id = std::to_string(id);
    str_id.insert(0, "0", 1);
    str_id += ".img";
    nl::node src = nl::nx::character[itemdata.get_category()][str_id]["info"];

    cash = src["cash"].get_bool();
    tradeblock = src["tradeBlock"].get_bool();
    slots = src["tuc"];
    reqstats[Maplestat::LEVEL] = src["reqLevel"];
    reqstats[Maplestat::JOB] = src["reqJob"];
    reqstats[Maplestat::STR] = src["reqSTR"];
    reqstats[Maplestat::DEX] = src["reqDEX"];
    reqstats[Maplestat::INT] = src["reqINT"];
    reqstats[Maplestat::LUK] = src["reqLUK"];
    defstats[Equipstat::STR] = src["incSTR"];
    defstats[Equipstat::DEX] = src["incDEX"];
    defstats[Equipstat::INT] = src["incINT"];
    defstats[Equipstat::LUK] = src["incLUK"];
    defstats[Equipstat::WATK] = src["incPAD"];
    defstats[Equipstat::WDEF] = src["incPDD"];
    defstats[Equipstat::MAGIC] = src["incMAD"];
    defstats[Equipstat::MDEF] = src["incMDD"];
    defstats[Equipstat::HP] = src["incMHP"];
    defstats[Equipstat::MP] = src["incMMP"];
    defstats[Equipstat::ACC] = src["incACC"];
    defstats[Equipstat::AVOID] = src["incEVA"];
    defstats[Equipstat::HANDS] = src["incHANDS"];
    defstats[Equipstat::SPEED] = src["incSPEED"];
    defstats[Equipstat::JUMP] = src["incJUMP"];

    static constexpr std::size_t NON_WEAPON_TYPES = 15;
    static constexpr std::size_t WEAPON_OFFSET = NON_WEAPON_TYPES + 15;
    static constexpr std::size_t WEAPON_TYPES = 20;
    auto index = static_cast<std::size_t>(id / 10000 - 100);
    if (index < NON_WEAPON_TYPES) {
        static constexpr const char* const types[NON_WEAPON_TYPES]
            = {"HAT",
               "FACE ACCESSORY",
               "EYE ACCESSORY",
               "EARRINGS",
               "TOP",
               "OVERALL",
               "BOTTOM",
               "SHOES",
               "GLOVES",
               "SHIELD",
               "CAPE",
               "RING",
               "PENDANT",
               "BELT",
               "MEDAL"};
        static constexpr Equipslot::Id equipslots[NON_WEAPON_TYPES]
            = {Equipslot::CAP,
               Equipslot::FACEACC,
               Equipslot::EYEACC,
               Equipslot::EARRINGS,
               Equipslot::TOP,
               Equipslot::TOP,
               Equipslot::PANTS,
               Equipslot::SHOES,
               Equipslot::GLOVES,
               Equipslot::SHIELD,
               Equipslot::CAPE,
               Equipslot::RING,
               Equipslot::PENDANT,
               Equipslot::BELT,
               Equipslot::MEDAL};

        type = types[index];
        eqslot = equipslots[index];
    } else if (index >= WEAPON_OFFSET
               && index < WEAPON_OFFSET + WEAPON_TYPES) {
        static constexpr const char* const types[WEAPON_TYPES]
            = {"ONE-HANDED SWORD",
               "ONE-HANDED AXE",
               "ONE-HANDED MACE",
               "DAGGER",
               "",
               "",
               "",
               "WAND",
               "STAFF",
               "",
               "TWO-HANDED SWORD",
               "TWO-HANDED AXE",
               "TWO-HANDED MACE",
               "SPEAR",
               "POLEARM",
               "BOW",
               "CROSSBOW",
               "CLAW",
               "KNUCKLE",
               "GUN"};

        std::size_t weaponindex = index - WEAPON_OFFSET;
        type = types[weaponindex];
        eqslot = Equipslot::WEAPON;
    } else {
        type = "CASH";
        eqslot = Equipslot::NONE;
    }
}

bool EquipData::is_valid() const noexcept
{
    return itemdata.is_valid();
}

EquipData::operator bool() const noexcept
{
    return is_valid();
}

bool EquipData::is_weapon() const noexcept
{
    return eqslot == Equipslot::WEAPON;
}

std::int16_t EquipData::get_req_stat(Maplestat::Id stat) const noexcept
{
    return reqstats[stat];
}

std::int16_t EquipData::get_def_stat(Equipstat::Id stat) const noexcept
{
    return defstats[stat];
}

Equipslot::Id EquipData::get_eq_slot() const noexcept
{
    return eqslot;
}

std::string_view EquipData::get_type() const noexcept
{
    return type;
}

const ItemData& EquipData::get_item_data() const noexcept
{
    return itemdata;
}
} // namespace jrc
