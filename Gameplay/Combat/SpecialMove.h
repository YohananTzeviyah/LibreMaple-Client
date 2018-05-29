//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "../../Character/Char.h"
#include "../../Character/Job.h"
#include "../MapleMap/Mob.h"
#include "Attack.h"

namespace jrc
{
// Base class for attacks and buffs.
class SpecialMove
{
public:
    enum ForbidReason {
        FBR_NONE,
        FBR_WEAPONTYPE,
        FBR_HPCOST,
        FBR_MPCOST,
        FBR_BULLETCOST,
        FBR_COOLDOWN,
        FBR_OTHER
    };

    virtual ~SpecialMove()
    {
    }

    virtual void apply_useeffects(Char& user) const = 0;
    virtual void apply_actions(Char& user, Attack::Type type) const = 0;
    virtual void apply_stats(const Char& user, Attack& attack) const = 0;
    virtual void apply_hiteffects(const AttackUser& user,
                                  Mob& target) const = 0;
    virtual Animation get_bullet(const Char& user,
                                 std::int32_t bulletid) const = 0;

    virtual bool is_attack() const = 0;
    virtual bool is_skill() const = 0;
    virtual std::int32_t get_id() const = 0;

    virtual ForbidReason can_use(std::int32_t level,
                                 Weapon::Type weapon,
                                 const Job& job,
                                 std::uint16_t hp,
                                 std::uint16_t mp,
                                 std::uint16_t bullets) const = 0;
};
} // namespace jrc
