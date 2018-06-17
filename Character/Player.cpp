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
#include "Player.h"

#include "../Constants.h"
#include "../Data/SkillData.h"
#include "../Data/WeaponData.h"
#include "../IO/UI.h"
#include "../IO/UITypes/UIStatsInfo.h"
#include "../Net/Packets/GameplayPackets.h"
#include "../Net/Packets/InventoryPackets.h"
#include "PlayerStates.h"

namespace jrc
{
const PlayerNullState null_state;

const PlayerState* get_state(Char::State state)
{
    static PlayerStandState standing;
    static PlayerWalkState walking;
    static PlayerFallState falling;
    static PlayerProneState lying;
    static PlayerClimbState climbing;
    static PlayerSitState sitting;
    static PlayerFlyState flying;

    switch (state) {
    case Char::STAND:
        return &standing;
    case Char::WALK:
        return &walking;
    case Char::FALL:
        return &falling;
    case Char::PRONE:
        return &lying;
    case Char::LADDER:
    case Char::ROPE:
        return &climbing;
    case Char::SIT:
        return &sitting;
    case Char::SWIM:
        return &flying;
    default:
        return nullptr;
    }
}

Player::Player(const CharEntry& entry)
    : Char(entry.cid, entry.look, std::string{entry.stats.name}),
      stats(entry.stats)
{
    attacking = false;
    underwater = false;

    set_state(STAND);
    set_direction(false);
}

Player::Player() : Char{0, {}, ""}
{
}

void Player::respawn(Point<std::int16_t> pos, bool uw)
{
    set_position(pos.x(), pos.y());
    underwater = uw;
    keys_down.clear();
    attacking = false;
    ladder = nullptr;
    null_state.update_state(*this);
}

void Player::send_action(KeyAction::Id action, bool down)
{
    const PlayerState* pst = get_state(state);
    if (pst) {
        pst->send_action(*this, action, down);
    }
    keys_down[action] = down;
}

void Player::recalc_stats(bool equip_changed)
{
    Weapon::Type weapon_type = get_weapon_type();

    stats.set_weapon_type(weapon_type);
    stats.init_total_stats();

    if (equip_changed) {
        inventory.recalc_stats(weapon_type);
    }

    for (auto stat : Equipstat::values) {
        std::int32_t inventory_total = inventory.get_stat(stat);
        stats.add_value(stat, inventory_total);
    }

    /* // More efficient alternative to `skillbook.collect_passives()` when the
       // order of applying passives doesn't matter. IMO it shouldn't matter,
       // but in the interest of being compliant with standard Maplestory
       // behavior, we retain the order.
    for (const auto& [skill_id, entry] : skillbook.get_entries()) {
        if (SkillData::get(skill_id).is_passive()) {
            passive_buffs.apply_buff(stats, skill_id, entry.level);
        }
    }
    */
    for (auto&& [skill_id, skill_level] : skillbook.collect_passives()) {
        passive_buffs.apply_buff(stats, skill_id, skill_level);
    }

    for (const Buff& buff : buffs.values()) {
        active_buffs.apply_buff(stats, buff.stat, buff.value);
    }

    stats.close_total_stats();

    if (auto stats_info = UI::get().get_element<UIStatsinfo>(); stats_info) {
        stats_info->update_all_stats();
    }
}

void Player::change_equip(std::int16_t slot)
{
    if (std::int32_t itemid
        = inventory.get_item_id(InventoryType::EQUIPPED, slot)) {
        look.add_equip(itemid);
    } else {
        look.remove_equip(Equipslot::by_id(slot));
    }
}

void Player::use_item(std::int32_t itemid)
{
    InventoryType::Id type = InventoryType::by_item_id(itemid);
    if (std::int16_t slot = inventory.find_item(type, itemid)) {
        switch (type) {
        case InventoryType::USE:
            UseItemPacket(slot, itemid).dispatch();
            break;
        default:
            return;
        }
    }
}

void Player::draw(Layer::Id layer,
                  double viewx,
                  double viewy,
                  float alpha) const
{
    if (layer == get_layer()) {
        Char::draw(viewx, viewy, alpha);
    }
}

void Player::draw(double viewx, double viewy, float alpha) const
{
    Char::draw(viewx, viewy, alpha);
}

std::int8_t Player::update(const Physics& physics)
{
    const PlayerState* pst = get_state(state);
    if (pst) {
        pst->update(*this);
        physics.move_object(ph_obj);

        bool aniend = Char::update(physics, get_stance_speed());
        if (aniend && attacking) {
            attacking = false;
            null_state.update_state(*this);
        } else {
            pst->update_state(*this);
        }
    }

    std::uint8_t stancebyte = flip ? state : state + 1;
    Movement newmove(ph_obj, stancebyte);
    bool needupdate = last_move.hasmoved(newmove);
    if (needupdate) {
        MovePlayerPacket(newmove).dispatch();
        last_move = newmove;
    }

    return get_layer();
}

std::int8_t Player::get_integer_attack_speed() const
{
    std::int32_t weapon_id = look.get_equips().get_weapon();
    if (weapon_id <= 0) {
        return 0;
    }

    const WeaponData& weapon = WeaponData::get(weapon_id);

    std::int8_t base_speed = stats.get_attack_speed();
    std::int8_t weapon_speed = weapon.get_speed();
    return base_speed + weapon_speed;
}

void Player::set_direction(bool flipped)
{
    if (!attacking) {
        Char::set_direction(flipped);
    }
}

void Player::set_state(State st)
{
    if (!attacking) {
        Char::set_state(st);

        const PlayerState* pst = get_state(st);
        if (pst) {
            pst->initialize(*this);
        }
    }
}

bool Player::is_attacking() const
{
    return attacking;
}

bool Player::can_attack() const
{
    return !attacking && !is_climbing() && !is_sitting()
           && look.get_equips().has_weapon();
}

SpecialMove::ForbidReason Player::can_use(const SpecialMove& move) const
{
    if (move.is_skill() && state == PRONE) {
        return SpecialMove::FBR_OTHER;
    }

    if (move.is_attack() && (state == LADDER || state == ROPE)) {
        return SpecialMove::FBR_OTHER;
    }

    if (has_cooldown(move.get_id())) {
        return SpecialMove::FBR_COOLDOWN;
    }

    std::int32_t level = skillbook.get_level(move.get_id());
    Weapon::Type weapon = get_weapon_type();
    const Job& job = stats.get_job();
    std::uint16_t hp = stats.get_stat(Maplestat::HP);
    std::uint16_t mp = stats.get_stat(Maplestat::MP);
    std::uint16_t bullets = inventory.get_bullet_count();

    return move.can_use(level, weapon, job, hp, mp, bullets);
}

Attack Player::prepare_attack(bool skill) const
{
    Attack::Type attacktype;
    bool degenerate;
    if (state == PRONE) {
        degenerate = true;
        attacktype = Attack::CLOSE;
    } else {
        Weapon::Type weapontype = get_weapon_type();
        switch (weapontype) {
        case Weapon::BOW:
        case Weapon::CROSSBOW:
        case Weapon::CLAW:
        case Weapon::GUN:
            degenerate = !inventory.has_projectile();
            attacktype = degenerate ? Attack::CLOSE : Attack::RANGED;
            break;
        case Weapon::WAND:
        case Weapon::STAFF:
            degenerate = !skill;
            attacktype = degenerate ? Attack::CLOSE : Attack::MAGIC;
            break;
        default:
            attacktype = Attack::CLOSE;
            degenerate = false;
        }
    }

    Attack attack;
    attack.type = attacktype;
    attack.min_damage = stats.get_min_damage();
    attack.max_damage = stats.get_max_damage();
    if (degenerate) {
        attack.min_damage /= 10;
        attack.max_damage /= 10;
    }
    attack.critical = stats.get_critical();
    attack.ignore_def = stats.get_ignore_def();
    attack.accuracy = stats.get_total(Equipstat::ACC);
    attack.player_level = stats.get_stat(Maplestat::LEVEL);
    attack.range = stats.get_range();
    attack.bullet = inventory.get_bullet_id();
    attack.origin = get_position();
    attack.to_left = !flip;
    attack.speed = static_cast<std::uint8_t>(get_integer_attack_speed());

    return attack;
}

void Player::rush(double target_x)
{
    if (ph_obj.on_ground) {
        std::uint16_t delay = get_attack_delay(1);
        ph_obj.move_x_until(target_x, delay);
        ph_obj.set_flag(PhysicsObject::TURN_AT_EDGES);
    }
}

bool Player::is_invincible() const
{
    if (state == DIED) {
        return true;
    }

    if (has_buff(Buffstat::DARKSIGHT)) {
        return true;
    }

    return Char::is_invincible();
}

MobAttackResult Player::damage(const MobAttack& attack)
{
    std::int32_t damage = stats.calculate_damage(attack.watk);
    show_damage(damage);

    bool from_left = attack.origin.x() > ph_obj.get_x();

    bool missed = damage <= 0;
    bool immovable = ladder || state == DIED;
    bool knockback = !missed && !immovable;
    if (knockback && Randomizer::above(stats.get_stance())) {
        ph_obj.hspeed = from_left ? -1.5 : 1.5;
        ph_obj.v_force -= 3.5;
    }

    std::uint8_t direction = from_left ? 0 : 1;
    return {attack, damage, direction};
}

void Player::give_buff(Buff buff)
{
    buffs[buff.stat] = buff;
}

void Player::cancel_buff(Buffstat::Id stat)
{
    buffs[stat] = {};
}

bool Player::has_buff(Buffstat::Id stat) const
{
    return buffs[stat].value > 0;
}

void Player::change_skill(std::int32_t skill_id,
                          std::int32_t skill_level,
                          std::int32_t master_level,
                          std::int64_t expiration)
{
    std::int32_t old_level = skillbook.get_level(skill_id);
    skillbook.set_skill(skill_id, skill_level, master_level, expiration);

    if (old_level != skill_level) {
        recalc_stats(false);
    }
}

void Player::add_cooldown(std::int32_t skill_id, std::int32_t cool_time)
{
    cooldowns[skill_id] = cool_time;
}

bool Player::has_cooldown(std::int32_t skill_id) const
{
    auto iter = cooldowns.find(skill_id);
    if (iter == cooldowns.end()) {
        return false;
    }

    return iter->second > 0;
}

void Player::change_level(std::uint16_t level)
{
    std::uint16_t oldlevel = get_level();
    if (level > oldlevel) {
        show_effect_id(CharEffect::LEVELUP);
    }
    stats.set_stat(Maplestat::LEVEL, level);
}

std::uint16_t Player::get_level() const
{
    return stats.get_stat(Maplestat::LEVEL);
}

std::int32_t Player::get_skill_level(std::int32_t skill_id) const
{
    return skillbook.get_level(skill_id);
}

void Player::change_job(std::uint16_t job_id)
{
    show_effect_id(CharEffect::JOBCHANGE);
    stats.change_job(job_id);
}

void Player::set_seat(nullable_ptr<const Seat> seat)
{
    if (seat) {
        set_position(seat->get_pos());
        set_state(Char::SIT);
    }
}

void Player::set_ladder(nullable_ptr<const Ladder> ldr)
{
    ladder = ldr;

    if (ladder) {
        ph_obj.set_x(ldr->get_x());
        ph_obj.hspeed = 0.0;
        ph_obj.vspeed = 0.0;
        ph_obj.fh_layer = 7;
        set_state(ldr->is_ladder() ? Char::LADDER : Char::ROPE);
        set_direction(false);
    }
}

float Player::get_walk_force() const
{
    return 0.05f
           + 0.11f * static_cast<float>(stats.get_total(Equipstat::SPEED))
                 / 100;
}

float Player::get_jump_force() const
{
    return 1.0f
           + 3.5f * static_cast<float>(stats.get_total(Equipstat::JUMP)) / 100;
}

float Player::get_climb_force() const
{
    return static_cast<float>(stats.get_total(Equipstat::SPEED)) / 100;
}

float Player::get_fly_force() const
{
    return 0.25f;
}

bool Player::is_underwater() const
{
    return underwater;
}

bool Player::is_key_down(KeyAction::Id action) const
{
    return keys_down.count(action) ? keys_down.at(action) : false;
}

CharStats& Player::get_stats()
{
    return stats;
}

const CharStats& Player::get_stats() const
{
    return stats;
}

Inventory& Player::get_inventory()
{
    return inventory;
}

const Inventory& Player::get_inventory() const
{
    return inventory;
}

Skillbook& Player::get_skills()
{
    return skillbook;
}

Questlog& Player::get_quests()
{
    return questlog;
}

Telerock& Player::get_telerock()
{
    return telerock;
}

Monsterbook& Player::get_monsterbook()
{
    return monsterbook;
}

nullable_ptr<const Ladder> Player::get_ladder() const
{
    return ladder;
}
} // namespace jrc
