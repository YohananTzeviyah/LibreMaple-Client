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
#include "CharStats.h"

#include "StatCaps.h"

namespace jrc
{
CharStats::CharStats(const StatsEntry& s)
    : name(s.name),
      pet_ids(s.pet_ids),
      exp(s.exp),
      map_id(s.map_id),
      portal(s.portal),
      rank(s.rank),
      job_rank(s.job_rank),
      base_stats(s.stats)
{
    job = base_stats[Maplestat::JOB];
    init_total_stats();
}

CharStats::CharStats() = default;

void CharStats::init_total_stats()
{
    total_stats.clear();
    buff_deltas.clear();
    percentages.clear();

    total_stats[Equipstat::HP] = get_stat(Maplestat::MAXHP);
    total_stats[Equipstat::MP] = get_stat(Maplestat::MAXMP);
    total_stats[Equipstat::STR] = get_stat(Maplestat::STR);
    total_stats[Equipstat::DEX] = get_stat(Maplestat::DEX);
    total_stats[Equipstat::INT] = get_stat(Maplestat::INT);
    total_stats[Equipstat::LUK] = get_stat(Maplestat::LUK);
    total_stats[Equipstat::SPEED] = 100;
    total_stats[Equipstat::JUMP] = 100;

    max_damage = 0;
    min_damage = 0;
    honor = 0;
    attack_speed = 0;
    projectile_range = 400;
    mastery = 0.0f;
    critical = 0.05f;
    min_crit = 0.5f;
    max_crit = 0.75f;
    damage_percent = 0.0f;
    boss_dmg = 0.0f;
    ignore_def = 0.0f;
    stance = 0.0f;
    resist_status = 0.0f;
    reduce_damage = 0.0f;
}

void CharStats::close_total_stats()
{
    total_stats[Equipstat::ACC] += calculate_accuracy();

    for (auto [stat, percent] : percentages) {
        std::int32_t total = total_stats[stat];
        total += static_cast<std::int32_t>(total * percent);
        set_total(stat, total);
    }

    std::int32_t primary = get_primary_stat();
    std::int32_t secondary = get_secondary_stat();
    std::int32_t attack = get_total(Equipstat::WATK);
    float multiplier = damage_percent + static_cast<float>(attack) / 100;
    max_damage = static_cast<std::int32_t>((primary + secondary) * multiplier);
    min_damage = static_cast<std::int32_t>(
        (primary * 0.9f * mastery + secondary) * multiplier);
}

std::int32_t CharStats::calculate_accuracy() const
{
    std::int32_t total_dex = get_total(Equipstat::DEX);
    std::int32_t total_luk = get_total(Equipstat::LUK);
    return static_cast<std::int32_t>(total_dex * 0.8f + total_luk * 0.5f);
}

std::int32_t CharStats::get_primary_stat() const
{
    Equipstat::Id primary = job.get_primary(weapon_type);
    return static_cast<std::int32_t>(get_multiplier() * get_total(primary));
}

std::int32_t CharStats::get_secondary_stat() const
{
    Equipstat::Id secondary = job.get_secondary(weapon_type);
    return get_total(secondary);
}

float CharStats::get_multiplier() const
{
    switch (weapon_type) {
    case Weapon::SWORD_1H:
        return 4.0f;
    case Weapon::AXE_1H:
    case Weapon::MACE_1H:
    case Weapon::WAND:
    case Weapon::STAFF:
        return 4.4f;
    case Weapon::DAGGER:
    case Weapon::CROSSBOW:
    case Weapon::CLAW:
    case Weapon::GUN:
        return 3.6f;
    case Weapon::SWORD_2H:
        return 4.6f;
    case Weapon::AXE_2H:
    case Weapon::MACE_2H:
    case Weapon::KNUCKLE:
        return 4.8f;
    case Weapon::SPEAR:
    case Weapon::POLEARM:
        return 5.0f;
    case Weapon::BOW:
        return 3.4f;
    default:
        return 0.0f;
    }
}

void CharStats::set_stat(Maplestat::Id stat, std::uint16_t value)
{
    base_stats[stat] = value;
}

void CharStats::set_total(Equipstat::Id stat, std::int32_t value)
{
    auto iter = EQSTAT_CAPS.find(stat);
    if (iter != EQSTAT_CAPS.end()) {
        std::int32_t cap_value = iter->second;

        if (value > cap_value) {
            value = cap_value;
        }
    }

    total_stats[stat] = value;
}

void CharStats::add_buff(Equipstat::Id stat, std::int32_t value)
{
    std::int32_t current = get_total(stat);
    set_total(stat, current + value);
    buff_deltas[stat] += value;
}

void CharStats::add_value(Equipstat::Id stat, std::int32_t value)
{
    std::int32_t current = get_total(stat);
    set_total(stat, current + value);
}

void CharStats::add_percent(Equipstat::Id stat, float percent)
{
    percentages[stat] += percent;
}

void CharStats::set_weapon_type(Weapon::Type wep_type)
{
    weapon_type = wep_type;
}

void CharStats::set_exp(std::int64_t e)
{
    exp = e;
}

void CharStats::set_portal(std::uint8_t p)
{
    portal = p;
}

void CharStats::set_mastery(float m)
{
    mastery = 0.5f + m;
}

void CharStats::set_damage_percent(float dmg_percent)
{
    damage_percent = dmg_percent;
}

void CharStats::set_reduce_damage(float reduce_dmg)
{
    reduce_damage = reduce_dmg;
}

void CharStats::change_job(std::uint16_t id)
{
    base_stats[Maplestat::JOB] = id;
    job.change_job(id);
}

std::int32_t CharStats::calculate_damage(std::int32_t mob_atk) const
{
    // TODO: This is just random stuff, need to find the actual formula
    // somewhere.
    std::int32_t reduceatk =
        mob_atk / 2 + mob_atk / get_total(Equipstat::WDEF);
    return reduceatk - static_cast<std::int32_t>(reduceatk * reduce_damage);
}

bool CharStats::is_damage_buffed() const
{
    return get_buff_delta(Equipstat::WATK) > 0 ||
           get_buff_delta(Equipstat::MAGIC) > 0;
}

std::uint16_t CharStats::get_stat(Maplestat::Id stat) const
{
    return base_stats[stat];
}

std::int32_t CharStats::get_total(Equipstat::Id stat) const
{
    return total_stats[stat];
}

std::int32_t CharStats::get_buff_delta(Equipstat::Id stat) const
{
    return buff_deltas[stat];
}

Rectangle<std::int16_t> CharStats::get_range() const
{
    return {-projectile_range, -5, -50, 50};
}

std::int32_t CharStats::get_map_id() const
{
    return map_id;
}

std::uint8_t CharStats::get_portal() const
{
    return portal;
}

std::int64_t CharStats::get_exp() const
{
    return exp;
}

std::string_view CharStats::get_name() const
{
    return name;
}

std::string_view CharStats::get_job_name() const
{
    return job.get_name();
}

Weapon::Type CharStats::get_weapon_type() const
{
    return weapon_type;
}

float CharStats::get_mastery() const
{
    return mastery;
}

float CharStats::get_critical() const
{
    return critical;
}

float CharStats::get_min_crit() const
{
    return min_crit;
}

float CharStats::get_max_crit() const
{
    return max_crit;
}

float CharStats::get_reduce_damage() const
{
    return reduce_damage;
}

float CharStats::get_boss_dmg() const
{
    return boss_dmg;
}

float CharStats::get_ignore_def() const
{
    return ignore_def;
}

void CharStats::set_stance(float s)
{
    stance = s;
}

float CharStats::get_stance() const
{
    return stance;
}

float CharStats::get_resistance() const
{
    return resist_status;
}

std::int32_t CharStats::get_max_damage() const
{
    return max_damage;
}

std::int32_t CharStats::get_min_damage() const
{
    return min_damage;
}

std::uint16_t CharStats::get_honor() const
{
    return honor;
}

void CharStats::set_attack_speed(std::int8_t as)
{
    attack_speed = as;
}

std::int8_t CharStats::get_attack_speed() const
{
    return attack_speed;
}

const Job& CharStats::get_job() const
{
    return job;
}
} // namespace jrc
