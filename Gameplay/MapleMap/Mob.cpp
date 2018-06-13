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
#include "Mob.h"

#include "../../Constants.h"
#include "../../Net/Packets/GameplayPackets.h"
#include "../../Util/Misc.h"
#include "../Movement.h"
#include "nlnx/nx.hpp"

#include <algorithm>
#include <functional>

namespace jrc
{
Mob::Mob(std::int32_t oid,
         std::int32_t mob_id,
         std::int8_t mode,
         std::uint8_t stance,
         std::uint16_t fh,
         bool new_spawn,
         std::int8_t tm,
         Point<std::int16_t> position)
    : MapObject(oid)
{
    std::string strid = string_format::extend_id(mob_id, 7);
    const nl::node src = nl::nx::mob[strid + ".img"];

    nl::node info = src["info"];

    level = info["level"];
    watk = info["PADamage"];
    matk = info["MADamage"];
    wdef = info["PDDamage"];
    mdef = info["MDDamage"];
    accuracy = info["acc"];
    avoid = info["eva"];
    knockback = info["pushed"];
    speed = info["speed"];
    fly_speed = info["flySpeed"];
    touch_damage = info["bodyAttack"].get_bool();
    undead = info["undead"].get_bool();
    no_flip = info["noFlip"].get_bool();
    not_attack = info["notAttack"].get_bool();
    can_jump = src["jump"].size() > 0;
    can_fly = src["fly"].size() > 0;
    can_move = src["move"].size() > 0 || can_fly;

    if (can_fly) {
        animations[STAND] = src["fly"];
        animations[MOVE] = src["fly"];
    } else {
        animations[STAND] = src["stand"];
        animations[MOVE] = src["move"];
    }
    animations[JUMP] = src["jump"];
    animations[HIT] = src["hit1"];
    animations[DIE] = src["die1"];

    name =
        nl::nx::string["Mob.img"][std::to_string(mob_id)]["name"].get_string();

    nl::node sndsrc = nl::nx::sound["Mob.img"][strid];

    hit_sound = sndsrc["Damage"];
    die_sound = sndsrc["Die"];

    speed += 100;
    speed *= 0.001f;

    fly_speed += 100;
    fly_speed *= 0.0005f;

    if (can_fly) {
        ph_obj.type = PhysicsObject::FLYING;
    }

    id = mob_id;
    team = tm;
    set_position(position);
    set_control(mode);
    ph_obj.fh_id = fh;
    ph_obj.set_flag(PhysicsObject::TURN_AT_EDGES);

    hp_percent = 0;
    dying = false;
    dead = false;
    fading = false;
    await_death = false;
    set_stance(stance);
    fly_direction = STRAIGHT;
    counter = 0;

    name_label = {Text::A13M,
                  Text::CENTER,
                  Text::WHITE,
                  Text::NAMETAG,
                  std::string{name}};

    if (new_spawn) {
        fade_in = true;
        opacity.set(0.0f);
    } else {
        fade_in = false;
        opacity.set(1.0f);
    }

    if (control && stance == Stance::STAND) {
        next_move();
    }
}

void Mob::set_stance(std::uint8_t stancebyte)
{
    flip = (stancebyte % 2) == 0;
    if (!flip) {
        stancebyte -= 1;
    }

    if (stancebyte < MOVE) {
        stancebyte = MOVE;
    }

    set_stance(static_cast<Stance>(stancebyte));
}

void Mob::set_stance(Stance newstance)
{
    if (stance != newstance) {
        stance = newstance;

        animations.at(stance).reset();
    }
}

std::int8_t Mob::update(const Physics& physics)
{
    if (!active) {
        return ph_obj.fh_layer;
    }

    bool aniend = animations.at(stance).update();
    if (aniend && stance == DIE) {
        dead = true;
    }

    if (fading) {
        opacity -= 0.025f;
        if (opacity.last() < 0.025f) {
            opacity.set(0.0f);
            fading = false;
            dead = true;
        }
    } else if (fade_in) {
        opacity += 0.025f;
        if (opacity.last() > 0.975f) {
            opacity.set(1.0f);
            fade_in = false;
        }
    }

    if (dead) {
        active = false;
        return -1;
    }

    effects.update();
    do_show_hp.update();

    if (!dying) {
        if (!can_fly) {
            if (ph_obj.is_flag_not_set(PhysicsObject::TURN_AT_EDGES)) {
                flip = !flip;
                ph_obj.set_flag(PhysicsObject::TURN_AT_EDGES);

                if (stance == HIT) {
                    set_stance(STAND);
                }
            }
        }

        switch (stance) {
        case MOVE:
            if (can_fly) {
                ph_obj.h_force = flip ? fly_speed : -fly_speed;
                switch (fly_direction) {
                case UPWARDS:
                    ph_obj.v_force = -fly_speed;
                    break;
                case DOWNWARDS:
                    ph_obj.v_force = fly_speed;
                    break;
                default:
                    break;
                }
            } else {
                ph_obj.h_force = flip ? speed : -speed;
            }
            break;
        case HIT:
            if (can_move) {
                double KBFORCE = ph_obj.on_ground ? 0.2 : 0.1;
                ph_obj.h_force = flip ? -KBFORCE : KBFORCE;
            }
            break;
        case JUMP:
            ph_obj.v_force = -5.0;
            break;
        default:
            break;
        }

        physics.move_object(ph_obj);

        if (control) {
            ++counter;

            bool next;
            switch (stance) {
            case HIT:
                next = counter > 200;
                break;
            case JUMP:
                next = ph_obj.on_ground;
                break;
            default:
                next = aniend && counter > 200;
                break;
            }

            if (next) {
                next_move();
                update_movement();
                counter = 0;
            }
        }
    } else {
        ph_obj.normalize();
        physics.get_fht().update_fh(ph_obj);
    }

    return ph_obj.fh_layer;
}

void Mob::next_move()
{
    if (can_move) {
        switch (stance) {
        case HIT:
        case STAND:
            set_stance(MOVE);
            flip = Randomizer::next_bool();
            break;
        case MOVE:
        case JUMP:
            if (can_jump && ph_obj.on_ground && Randomizer::below(0.25f)) {
                set_stance(JUMP);
            } else {
                switch (Randomizer::next_int(3)) {
                case 0:
                    set_stance(STAND);
                    break;
                case 1:
                    set_stance(MOVE);
                    flip = false;
                    break;
                case 2:
                    set_stance(MOVE);
                    flip = true;
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }

        if (stance == MOVE && can_fly) {
            fly_direction = Randomizer::next_enum(NUM_DIRECTIONS);
        }
    } else {
        set_stance(STAND);
    }
}

void Mob::update_movement()
{
    MoveMobPacket(oid,
                  1,
                  0,
                  0,
                  0,
                  0,
                  0,
                  0,
                  get_position(),
                  Movement(ph_obj, value_of(stance, flip)))
        .dispatch();
}

void Mob::draw(double viewx, double viewy, float alpha) const
{
    Point<std::int16_t> absp = ph_obj.get_absolute(viewx, viewy, alpha);
    Point<std::int16_t> headpos = get_head_position(absp);

    effects.draw_below(absp, alpha);

    if (!dead) {
        float interopc = opacity.get(alpha);

        animations.at(stance).draw(
            DrawArgument(absp, flip && !no_flip, interopc), alpha);

        if (do_show_hp) {
            name_label.draw(absp);

            if (!dying && hp_percent > 0) {
                hp_bar.draw(headpos, hp_percent);
            }
        }
    }

    effects.draw_above(absp, alpha);
}

void Mob::set_control(std::int8_t mode)
{
    control = mode > 0;
    aggro = mode == 2;
}

void Mob::send_movement(Point<std::int16_t> start,
                        std::vector<Movement>&& in_movements)
{
    if (control) {
        return;
    }

    set_position(start);

    movements = std::forward<decltype(in_movements)>(in_movements);

    if (movements.empty()) {
        return;
    }

    const Movement& lastmove = movements.front();

    std::uint8_t laststance = lastmove.newstate;
    set_stance(laststance);

    ph_obj.fh_id = lastmove.fh;
}

Point<std::int16_t> Mob::get_head_position(Point<std::int16_t> position) const
{
    Point<std::int16_t> head = animations.at(stance).get_head();
    position.shift_x((flip && !no_flip) ? -head.x() : head.x());
    position.shift_y(head.y());

    return position;
}

void Mob::kill(std::int8_t kill_type)
{
    switch (kill_type) {
    case 0:
        active = false;
        break;
    case 1:
        dying = true;
        if (await_death) {
            apply_death();
        }
        break;
    case 2:
        fading = true;
        dying = true;
        break;
    default:
        break;
    }
}

void Mob::show_hp(std::int8_t percent, std::uint16_t player_level)
{
    if (hp_percent == 0) {
        std::int16_t delta = player_level - level;
        if (delta > 9) {
            name_label.change_color(Text::YELLOW);
        } else if (delta < -9) {
            name_label.change_color(Text::RED);
        }
    }

    if (percent > 100) {
        percent = 100;
    } else if (percent < 0) {
        percent = 0;
    }
    hp_percent = percent;
    do_show_hp.set_for(2000);
}

void Mob::show_effect(const Animation& animation,
                      std::int8_t pos,
                      std::int8_t z,
                      bool f)
{
    if (!active) {
        return;
    }

    Point<std::int16_t> shift;
    switch (pos) {
    case 0:
        shift = get_head_position({});
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    default:
        break;
    }
    effects.add(animation, {shift, f}, z);
}

float Mob::calculate_hit_chance(std::int16_t level_delta,
                                std::int32_t player_accuracy) const
{
    auto faccuracy = static_cast<float>(player_accuracy);
    float hitchance =
        faccuracy / (((1.84f + 0.07f * level_delta) * avoid) + 1.0f);
    if (hitchance < 0.01f) {
        hitchance = 0.01f;
    }

    return hitchance;
}

double Mob::calculate_min_damage(std::int16_t level_delta,
                                 double min_damage,
                                 bool magic) const
{
    double mindamage =
        magic ? min_damage - (1 + 0.01 * level_delta) * mdef * 0.6
              : min_damage * (1 - 0.01 * level_delta) - wdef * 0.6;

    return mindamage < 1.0 ? 1.0 : mindamage;
}

double Mob::calculate_max_damage(std::int16_t level_delta,
                                 double max_damage,
                                 bool magic) const
{
    double maxdamage =
        magic ? max_damage - (1 + 0.01 * level_delta) * mdef * 0.5
              : max_damage * (1 - 0.01 * level_delta) - wdef * 0.5;

    return maxdamage < 1.0 ? 1.0 : maxdamage;
}

std::vector<std::pair<std::int32_t, bool>>
Mob::calculate_damage(const Attack& attack)
{
    double min_damage;
    double max_damage;
    float hit_chance;
    float critical;
    std::int16_t level_delta = level - attack.player_level;
    if (level_delta < 0) {
        level_delta = 0;
    }

    Attack::DamageType damage_type = attack.damage_type;
    switch (damage_type) {
    case Attack::DMG_WEAPON:
    case Attack::DMG_MAGIC:
        min_damage = calculate_min_damage(
            level_delta, attack.min_damage, damage_type == Attack::DMG_MAGIC);
        max_damage = calculate_max_damage(
            level_delta, attack.max_damage, damage_type == Attack::DMG_MAGIC);
        hit_chance = calculate_hit_chance(level_delta, attack.accuracy);
        critical = attack.critical;
        break;
    case Attack::DMG_FIXED:
        min_damage = attack.fix_damage;
        max_damage = attack.fix_damage;
        hit_chance = 1.0f;
        critical = 0.0f;
        break;
    }

    std::vector<std::pair<std::int32_t, bool>> result(attack.hit_count);
    std::generate(result.begin(), result.end(), [&]() {
        return next_damage(min_damage, max_damage, hit_chance, critical);
    });

    update_movement();
    await_death = false;

    return result;
}

std::pair<std::int32_t, bool> Mob::next_damage(double min_damage,
                                               double max_damage,
                                               float hit_chance,
                                               float critical) const
{
    bool hit = Randomizer::below(hit_chance);
    if (!hit) {
        return {0, false};
    }

    constexpr double DAMAGECAP = 999999.0;

    double damage = Randomizer::next_real(min_damage, max_damage);
    bool iscritical = Randomizer::below(critical);
    if (iscritical) {
        damage *= 1.5;
    }

    if (damage < 1) {
        damage = 1;
    } else if (damage > DAMAGECAP) {
        damage = DAMAGECAP;
    }

    auto intdamage = static_cast<std::int32_t>(damage);
    return {intdamage, iscritical};
}

void Mob::apply_damage(std::int32_t damage, bool to_left)
{
    hit_sound.play();

    if (dying && stance != DIE) {
        apply_death();
    } else if (control && is_alive() && damage >= knockback) {
        flip = to_left;
        counter = 170;
        set_stance(HIT);

        update_movement();
        await_death = true;
    }
}

MobAttack Mob::create_touch_attack() const
{
    if (!touch_damage) {
        return {};
    }

    auto minattack = static_cast<std::int32_t>(watk * 0.8f);
    std::int32_t maxattack = watk;
    std::int32_t attack = Randomizer::next_int(minattack, maxattack);
    return {attack, get_position(), id, oid};
}

void Mob::apply_death()
{
    set_stance(DIE);
    die_sound.play();
    dying = true;
}

bool Mob::is_alive() const
{
    return active && !dying;
}

bool Mob::is_in_range(const Rectangle<std::int16_t>& range) const
{
    if (!active) {
        return false;
    }

    Rectangle<std::int16_t> bounds = animations.at(stance).get_bounds();
    bounds.shift(get_position());
    return range.overlaps(bounds);
}

Point<std::int16_t> Mob::get_head_position() const
{
    Point<std::int16_t> position = get_position();
    return get_head_position(position);
}
} // namespace jrc
