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
#include "MapMobs.h"

#include "Mob.h"

#include <algorithm>
#include <iostream>
#include <limits>

namespace jrc
{
void MapMobs::draw(Layer::Id layer,
                   double viewx,
                   double viewy,
                   float alpha) const
{
    mobs.draw(layer, viewx, viewy, alpha);
}

void MapMobs::update(const Physics& physics)
{
    for (; !spawns.empty(); spawns.pop()) {
        const MobSpawn& spawn = spawns.front();

        if (nullable_ptr<Mob> mob = mobs.get(spawn.get_oid())) {
            std::int8_t mode = spawn.get_mode();
            if (mode > 0) {
                mob->set_control(mode);
            }
            mob->activate();
        } else {
            mobs.add(spawn.instantiate());
        }
    }

    mobs.update(physics);
}

void MapMobs::spawn(MobSpawn&& spawn)
{
    spawns.emplace(std::move(spawn));
}

void MapMobs::remove(std::int32_t oid, std::int8_t animation)
{
    if (nullable_ptr<Mob> mob = mobs.get(oid)) {
        mob->kill(animation);
    }
}

void MapMobs::clear()
{
    mobs.clear();
}

void MapMobs::set_control(std::int32_t oid, bool control)
{
    auto mode = static_cast<std::int8_t>(control ? 1 : 0);
    if (nullable_ptr<Mob> mob = mobs.get(oid)) {
        mob->set_control(mode);
    }
}

void MapMobs::send_mobhp(std::int32_t oid,
                         std::int8_t percent,
                         std::uint16_t playerlevel)
{
    if (nullable_ptr<Mob> mob = mobs.get(oid)) {
        mob->show_hp(percent, playerlevel);
    }
}

void MapMobs::send_movement(std::int32_t oid,
                            Point<std::int16_t> start,
                            std::vector<Movement>&& movements)
{
    if (nullable_ptr<Mob> mob = mobs.get(oid)) {
        mob->send_movement(start, std::move(movements));
    }
}

AttackResult MapMobs::send_attack(const Attack& attack)
{
    Point<std::int16_t> origin = attack.origin;
    Rectangle<std::int16_t> range = attack.range;
    std::int16_t h_range =
        static_cast<std::int16_t>(range.l() * attack.h_range);
    if (attack.to_left) {
        range = {origin.x() + h_range,
                 origin.x() + range.r(),
                 origin.y() + range.t(),
                 origin.y() + range.b()};
    } else {
        range = {origin.x() - range.r(),
                 origin.x() - h_range,
                 origin.y() + range.t(),
                 origin.y() + range.b()};
    }

    std::uint8_t mob_count = attack.mob_count;
    AttackResult result = attack;
    for (auto [_, target] : find_closest(range, origin, mob_count)) {
        if (nullable_ptr<Mob> mob = mobs.get(target)) {
            result.damage_lines[target] = mob->calculate_damage(attack);
            ++result.mob_count;

            if (result.mob_count == 1) {
                result.first_oid = target;
            }
            if (result.mob_count == mob_count) {
                result.last_oid = target;
            }
        }
    }

    return result;
}

void MapMobs::apply_damage(std::int32_t oid,
                           std::int32_t damage,
                           bool to_left,
                           const AttackUser& user,
                           const SpecialMove& move)
{
    if (nullable_ptr<Mob> mob = mobs.get(oid)) {
        mob->apply_damage(damage, to_left);

        // Maybe move this into the method above too?
        move.apply_hit_effects(user, *mob);
    }
}

boost::container::flat_map<std::uint16_t, std::int32_t>
MapMobs::find_closest(Rectangle<std::int16_t> range,
                      Point<std::int16_t> origin,
                      std::uint8_t mob_count) const noexcept
{
    if (mob_count == 0) {
        return {};
    }
    if (mob_count == 1) {
        auto closest_oid = std::numeric_limits<std::int32_t>::lowest();
        auto closest_distance = std::numeric_limits<std::uint16_t>::max();
        for (const auto& mmo : mobs) {
            auto mob = static_cast<const Mob*>(mmo.second.get());
            if (mob && mob->is_alive() && mob->is_in_range(range)) {
                std::int32_t oid = mob->get_oid();
                auto distance = static_cast<std::uint16_t>(
                    mob->get_position().disp(origin));

                if (distance < closest_distance) {
                    closest_distance = distance;
                    closest_oid = oid;
                }
            }
        }

        if (closest_oid == std::numeric_limits<std::int32_t>::lowest()) {
            return {};
        } else {
            return {{{closest_distance, closest_oid}}};
        }
    }

    boost::container::flat_map<std::uint16_t, std::int32_t> targets;
    targets.reserve(mob_count + 1);

    for (const auto& mmo : mobs) {
        auto mob = static_cast<const Mob*>(mmo.second.get());
        if (mob && mob->is_alive() && mob->is_in_range(range)) {
            std::int32_t oid = mob->get_oid();
            auto distance =
                static_cast<std::uint16_t>(mob->get_position().disp(origin));
            targets.emplace(distance, oid);

            if (targets.size() > mob_count) {
                auto furthest = targets.end();
                --furthest;
                targets.erase(furthest);
            }
        }
    }

    return targets;
}

bool MapMobs::contains(std::int32_t oid) const
{
    return mobs.contains(oid);
}

std::int32_t MapMobs::find_colliding(const MovingObject& moveobj) const
{
    Range<std::int16_t> horizontal{moveobj.get_last_x(), moveobj.get_x()};
    Range<std::int16_t> vertical{moveobj.get_last_y(), moveobj.get_y()};
    Rectangle<std::int16_t> player_rect{horizontal.smaller(),
                                        horizontal.greater(),
                                        vertical.smaller() -
                                            static_cast<std::int16_t>(50),
                                        vertical.greater()};

    auto iter =
        std::find_if(mobs.begin(), mobs.end(), [player_rect](auto& mmo) {
            nullable_ptr<Mob> mob = mmo.second.get();
            return mob && mob->is_alive() && mob->is_in_range(player_rect);
        });

    if (iter == mobs.end()) {
        return 0;
    }

    return iter->second->get_oid();
}

MobAttack MapMobs::create_attack(std::int32_t oid) const
{
    if (nullable_ptr<const Mob> mob = mobs.get(oid)) {
        return mob->create_touch_attack();
    } else {
        return {};
    }
}

Point<std::int16_t> MapMobs::get_mob_position(std::int32_t oid) const
{
    if (auto mob = mobs.get(oid)) {
        return mob->get_position();
    } else {
        return {};
    }
}

Point<std::int16_t> MapMobs::get_mob_head_position(std::int32_t oid) const
{
    if (nullable_ptr<const Mob> mob = mobs.get(oid)) {
        return mob->get_head_position();
    } else {
        return {};
    }
}
} // namespace jrc
