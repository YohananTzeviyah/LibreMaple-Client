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
#include "SkillData.h"

#include "../Character/SkillId.h"
#include "../Util/Misc.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

#include <string_view>
#include <unordered_set>

namespace jrc
{
SkillData::SkillData(std::int32_t id)
{
    // Locate sources
    std::string strid = string_format::extend_id(id, 7);
    nl::node src = nl::nx::skill[str::concat(
        std::string_view(strid).substr(0, 3), ".img")]["skill"][strid];
    nl::node strsrc = nl::nx::string["Skill.img"][strid];

    // Load icons
    icons = {src["icon"], src["iconDisabled"], src["iconMouseOver"]};

    // Load strings
    name = strsrc["name"].get_string();
    desc = strsrc["desc"].get_string();

    for (std::int32_t level = 1;
         nl::node sub = strsrc["h" + std::to_string(level)];
         ++level) {
        levels.emplace(level, sub);
    }

    // Load stats
    nl::node levelsrc = src["level"];
    for (auto sub : levelsrc) {
        float damage = (float)sub["damage"] / 100;
        std::int32_t matk = sub["mad"];
        std::int32_t fixdamage = sub["fix_damage"];
        std::int32_t mastery = sub["mastery"];
        std::uint8_t attackcount =
            (std::uint8_t)sub["attackCount"].get_integer(1);
        std::uint8_t mobcount = (std::uint8_t)sub["mobCount"].get_integer(1);
        std::uint8_t bulletcount =
            (std::uint8_t)sub["bulletCount"].get_integer(1);
        std::int16_t bulletcost =
            (std::int16_t)sub["bulletConsume"].get_integer(bulletcount);
        std::int32_t hpcost = sub["hpCon"];
        std::int32_t mpcost = sub["mpCon"];
        float chance = (float)sub["prop"].get_real(100.0) / 100;
        float critical = 0.0f;
        float ignoredef = 0.0f;
        float hrange = (float)sub["range"].get_real(100.0) / 100;
        Rectangle<std::int16_t> range = sub;
        std::int32_t level =
            string_conversion::or_default<std::int32_t>(sub.name(), -1);
        stats.emplace(std::piecewise_construct,
                      std::forward_as_tuple(level),
                      std::forward_as_tuple(damage,
                                            matk,
                                            fixdamage,
                                            mastery,
                                            attackcount,
                                            mobcount,
                                            bulletcount,
                                            bulletcost,
                                            hpcost,
                                            mpcost,
                                            chance,
                                            critical,
                                            ignoredef,
                                            hrange,
                                            range));
    }

    element = src["elemAttr"].get_string();
    reqweapon = Weapon::by_value(100 + (std::int32_t)src["weapon"]);
    masterlevel = static_cast<std::int32_t>(stats.size());
    passive = (id % 10000) / 1000 == 0;
    flags = flags_of(id);
    invisible = src["invisible"].get_bool();
}

std::int32_t SkillData::flags_of(std::int32_t id) const noexcept
{
    static const std::unordered_map<std::int32_t, std::int32_t> skill_flags = {
        // Beginner
        {SkillId::THREE_SNAILS, ATTACK},
        // Warrior
        {SkillId::POWER_STRIKE, ATTACK},
        {SkillId::SLASH_BLAST, ATTACK},
        // Fighter
        // Page
        // Crusader
        {SkillId::SWORD_PANIC, ATTACK},
        {SkillId::AXE_PANIC, ATTACK},
        {SkillId::SWORD_COMA, ATTACK},
        {SkillId::AXE_COMA, ATTACK},
        // Hero
        {SkillId::RUSH_HERO, ATTACK},
        {SkillId::BRANDISH, ATTACK},
        // Page
        // White Knight
        {SkillId::CHARGE, ATTACK},
        // Paladin
        {SkillId::RUSH_PALADIN, ATTACK},
        {SkillId::BLAST, ATTACK},
        {SkillId::HEAVENS_HAMMER, ATTACK},
        // Spearman
        // Dragon Knight
        {SkillId::DRAGON_BUSTER, ATTACK},
        {SkillId::DRAGON_FURY, ATTACK},
        {SkillId::PA_BUSTER, ATTACK},
        {SkillId::PA_FURY, ATTACK},
        {SkillId::SACRIFICE, ATTACK},
        {SkillId::DRAGONS_ROAR, ATTACK},
        // Dark Knight
        {SkillId::RUSH_DK, ATTACK},
        // Mage
        {SkillId::ENERGY_BOLT, ATTACK | RANGED},
        {SkillId::MAGIC_CLAW, ATTACK | RANGED},
        // F/P Mage
        {SkillId::SLOW_FP, ATTACK},
        {SkillId::FIRE_ARROW, ATTACK | RANGED},
        {SkillId::POISON_BREATH, ATTACK | RANGED},
        // F/P ArchMage
        {SkillId::EXPLOSION, ATTACK},
        {SkillId::POISON_BREATH, ATTACK},
        {SkillId::SEAL_FP, ATTACK},
        {SkillId::ELEMENT_COMPOSITION_FP, ATTACK | RANGED},
        //
        {SkillId::FIRE_DEMON, ATTACK},
        {SkillId::PARALYZE, ATTACK | RANGED},
        {SkillId::METEOR_SHOWER, ATTACK}};

    auto iter = skill_flags.find(id);
    if (iter == skill_flags.end()) {
        return NONE;
    }

    return iter->second;
}

bool SkillData::is_passive() const noexcept
{
    return passive;
}

bool SkillData::is_attack() const noexcept
{
    return !passive && (flags & ATTACK);
}

bool SkillData::is_invisible() const noexcept
{
    return invisible;
}

std::int32_t SkillData::get_master_level() const noexcept
{
    return masterlevel;
}

Weapon::Type SkillData::get_required_weapon() const noexcept
{
    return reqweapon;
}

const SkillData::Stats& SkillData::get_stats(std::int32_t level) const noexcept
{
    auto iter = stats.find(level);
    if (iter == stats.end()) {
        static constexpr Stats null_stats{
            0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, {}};
        return null_stats;
    }
    return iter->second;
}

std::string_view SkillData::get_name() const noexcept
{
    return name;
}

std::string_view SkillData::get_desc() const noexcept
{
    return desc;
}

std::string_view SkillData::get_level_desc(std::int32_t level) const noexcept
{
    auto iter = levels.find(level);
    if (iter == levels.end()) {
        static constexpr std::string_view null_level =
            "Missing level description.";
        return null_level;
    } else {
        return iter->second;
    }
}

const Texture& SkillData::get_icon(Icon icon) const noexcept
{
    return icons[icon];
}
} // namespace jrc
