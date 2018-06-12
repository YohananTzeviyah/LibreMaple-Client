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
#include "../Net/Login.h"
#include "../Template/EnumMap.h"
#include "../Template/Rectangle.h"
#include "EquipStat.h"
#include "Inventory/Weapon.h"
#include "Job.h"

#include <string_view>

namespace jrc
{
class CharStats
{
public:
    CharStats(const StatsEntry& entry);
    CharStats();

    void init_total_stats();
    void set_stat(Maplestat::Id stat, std::uint16_t value);
    void set_total(Equipstat::Id stat, std::int32_t value);
    void add_buff(Equipstat::Id stat, std::int32_t value);
    void add_value(Equipstat::Id stat, std::int32_t value);
    void add_percent(Equipstat::Id stat, float percent);
    void close_total_stats();

    void set_weapon_type(Weapon::Type wep_type);
    void set_exp(std::int64_t exp);
    void set_portal(std::uint8_t portal);
    void set_mastery(float mastery);
    void set_damage_percent(float dmg_percent);
    void set_reduce_damage(float reduce_dmg);

    void change_job(std::uint16_t id);

    std::int32_t calculate_damage(std::int32_t mob_atk) const;

    bool is_damage_buffed() const;
    std::uint16_t get_stat(Maplestat::Id stat) const;
    std::int32_t get_total(Equipstat::Id stat) const;
    std::int32_t get_buff_delta(Equipstat::Id stat) const;
    Rectangle<std::int16_t> get_range() const;

    std::int32_t get_map_id() const;
    std::uint8_t get_portal() const;
    std::int64_t get_exp() const;
    std::string_view get_name() const;
    std::string_view get_job_name() const;
    Weapon::Type get_weapon_type() const;
    float get_mastery() const;
    float get_critical() const;
    float get_min_crit() const;
    float get_max_crit() const;
    float get_reduce_damage() const;
    float get_boss_dmg() const;
    float get_ignore_def() const;
    void set_stance(float stance);
    float get_stance() const;
    float get_resistance() const;
    std::int32_t get_max_damage() const;
    std::int32_t get_min_damage() const;
    std::uint16_t get_honor() const;
    void set_attack_speed(std::int8_t speed);
    std::int8_t get_attack_speed() const;
    const Job& get_job() const;

private:
    std::int32_t calculate_accuracy() const;
    std::int32_t get_primary_stat() const;
    std::int32_t get_secondary_stat() const;
    float get_multiplier() const;

    std::string name;
    std::vector<std::int64_t> pet_ids;
    Job job;
    std::int64_t exp;
    std::int32_t map_id;
    std::uint8_t portal;
    std::pair<std::int32_t, std::int8_t> rank;
    std::pair<std::int32_t, std::int8_t> job_rank;
    EnumMap<Maplestat::Id, std::uint16_t> base_stats;
    EnumMap<Equipstat::Id, std::int32_t> total_stats;
    EnumMap<Equipstat::Id, std::int32_t> buff_deltas;
    EnumMap<Equipstat::Id, float> percentages;
    std::int32_t max_damage;
    std::int32_t min_damage;
    std::uint16_t honor;
    std::int8_t attack_speed;
    std::int16_t projectile_range;
    Weapon::Type weapon_type;
    float mastery;
    float critical;
    float min_crit;
    float max_crit;
    float damage_percent;
    float boss_dmg;
    float ignore_def;
    float stance;
    float resist_status;
    float reduce_damage;
};
} // namespace jrc
