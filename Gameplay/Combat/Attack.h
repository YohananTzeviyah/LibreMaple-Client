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
#include "../../Template/Rectangle.h"

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace jrc
{
struct Attack {
    enum Type { CLOSE, RANGED, MAGIC };

    enum DamageType { DMG_WEAPON, DMG_MAGIC, DMG_FIXED };

    Type type = CLOSE;
    DamageType damage_type = DMG_WEAPON;

    double min_damage = 1.0;
    double max_damage = 1.0;
    float critical = 0.0f;
    float ignore_def = 0.0f;
    std::int32_t matk = 0;
    std::int32_t accuracy = 0;
    std::int32_t fix_damage = 0;
    std::int16_t player_level = 1;

    std::uint8_t hit_count = 0;
    std::uint8_t mob_count = 0;
    std::uint8_t speed = 0;
    std::uint8_t stance = 0;
    std::int32_t skill = 0;
    std::int32_t bullet = 0;

    Point<std::int16_t> origin;
    Rectangle<std::int16_t> range;
    float h_range = 1.0f;
    bool to_left = false;
};

struct MobAttack {
    Attack::Type type = Attack::CLOSE;
    std::int32_t watk = 0;
    std::int32_t matk = 0;
    std::int32_t mob_id = 0;
    std::int32_t oid = 0;
    Point<std::int16_t> origin;
    bool valid = false;

    //! Create a mob attack for touch damage.
    MobAttack(std::int32_t watk,
              Point<std::int16_t> origin,
              std::int32_t mobid,
              std::int32_t oid) noexcept
        : watk(watk), mob_id(mobid), oid(oid), origin(origin), valid(true)
    {
    }

    MobAttack() noexcept = default;

    explicit operator bool() const
    {
        return valid;
    }
};

struct MobAttackResult {
    std::int32_t damage;
    std::int32_t mob_id;
    std::int32_t oid;
    std::uint8_t direction;

    MobAttackResult(const MobAttack& attack,
                    std::int32_t damage,
                    std::uint8_t direction)
        : damage(damage),
          mob_id(attack.mob_id),
          oid(attack.oid),
          direction(direction)
    {
    }
};

struct AttackResult {
    AttackResult(const Attack& attack)
        : type(attack.type),
          hit_count(attack.hit_count),
          skill(attack.skill),
          bullet(attack.bullet),
          stance(attack.stance),
          speed(attack.speed),
          to_left(attack.to_left)
    {
    }

    AttackResult() = default;

    Attack::Type type;
    std::int32_t attacker = 0;
    std::uint8_t mob_count = 0;
    std::uint8_t hit_count = 1;
    std::int32_t skill = 0;
    std::int32_t charge = 0;
    std::int32_t bullet = 0;
    std::uint8_t level = 0;
    std::uint8_t display = 0;
    std::uint8_t stance = 0;
    std::uint8_t speed = 0;
    bool to_left = false;
    std::unordered_map<std::int32_t,
                       std::vector<std::pair<std::int32_t, bool>>>
        damage_lines;
    std::int32_t first_oid;
    std::int32_t last_oid;
};

struct AttackUser {
    std::int32_t skill_level;
    std::uint16_t level;
    bool second_weapon;
    bool flip;
};
} // namespace jrc
