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
#include "Combat.h"

#include "../../Character/SkillId.h"
#include "../../IO/Messages.h"
#include "../../Net/Packets/AttackAndSkillPackets.h"

namespace jrc
{
Combat::Combat(Player& in_player, MapChars& in_chars, MapMobs& in_mobs)
    : player(in_player),
      chars(in_chars),
      mobs(in_mobs),
      attack_results(
          [&](const AttackResult& attack) { apply_attack(attack); }),
      bullet_effects(
          [&](const BulletEffect& effect) { apply_bullet_effect(effect); }),
      damage_effects(
          [&](const DamageEffect& effect) { apply_damage_effect(effect); })
{
}

void Combat::draw(double viewx, double viewy, float alpha) const
{
    for (auto& be : bullets) {
        be.bullet.draw(viewx, viewy, alpha);
    }
    for (auto& dn : damage_numbers) {
        dn.draw(viewx, viewy, alpha);
    }
}

void Combat::update()
{
    attack_results.update();
    bullet_effects.update();
    damage_effects.update();

    bullets.erase(
        std::remove_if(bullets.begin(),
                       bullets.end(),
                       [this](BulletEffect& mb) {
                           std::int32_t target_oid =
                               mb.damage_effect.target_oid;
                           if (mobs.contains(target_oid)) {
                               mb.target =
                                   mobs.get_mob_head_position(target_oid);
                               bool apply = mb.bullet.update(mb.target);
                               if (apply) {
                                   apply_damage_effect(mb.damage_effect);
                               }
                               return apply;
                           } else {
                               return mb.bullet.update(mb.target);
                           }
                       }),
        bullets.end());

    damage_numbers.erase(
        std::remove_if(damage_numbers.begin(),
                       damage_numbers.end(),
                       [](DamageNumber& dn) { return dn.update(); }),
        damage_numbers.end());
}

void Combat::use_move(std::int32_t move_id)
{
    if (!player.can_attack()) {
        return;
    }

    const SpecialMove& move = get_move(move_id);

    SpecialMove::ForbidReason reason = player.can_use(move);
    Weapon::Type weapontype = player.get_stats().get_weapon_type();
    switch (reason) {
    case SpecialMove::FBR_NONE:
        apply_move(move);
        break;
    default:
        ForbidSkillMessage(reason, weapontype).drop();
        break;
    }
}

void Combat::apply_move(const SpecialMove& move)
{
    if (move.is_attack()) {
        Attack attack = player.prepare_attack(move.is_skill());

        move.apply_useeffects(player);
        move.apply_actions(player, attack.type);

        player.set_afterimage(move.get_id());

        move.apply_stats(player, attack);

        AttackResult result = mobs.send_attack(attack);
        result.attacker = player.get_oid();
        extract_effects(player, move, result);

        apply_use_movement(move);
        apply_result_movement(move, result);

        AttackPacket(result).dispatch();
    } else {
        move.apply_useeffects(player);
        move.apply_actions(player, Attack::MAGIC);

        std::int32_t moveid = move.get_id();
        std::int32_t level = player.get_skills().get_level(moveid);
        UseSkillPacket(moveid, level).dispatch();
    }
}

void Combat::apply_use_movement(const SpecialMove& move)
{
    switch (move.get_id()) {
    case SkillId::TELEPORT_FP:
    case SkillId::IL_TELEPORT:
    case SkillId::PRIEST_TELEPORT:
        break;
    case SkillId::FLASH_JUMP:
        break;
    }
}

void Combat::apply_result_movement(const SpecialMove& move,
                                   const AttackResult& result)
{
    switch (move.get_id()) {
    case SkillId::RUSH_HERO:
    case SkillId::RUSH_PALADIN:
    case SkillId::RUSH_DK:
        apply_rush(result);
        break;
    }
}

void Combat::apply_rush(const AttackResult& result)
{
    if (result.mob_count == 0)
        return;

    Point<std::int16_t> mob_position = mobs.get_mob_position(result.last_oid);
    std::int16_t targetx = mob_position.x();
    player.rush(targetx);
}

void Combat::apply_bullet_effect(const BulletEffect& effect)
{
    bullets.push_back(effect);
    if (bullets.back().bullet.settarget(effect.target)) {
        apply_damage_effect(effect.damage_effect);
        bullets.pop_back();
    }
}

void Combat::apply_damage_effect(const DamageEffect& effect)
{
    Point<std::int16_t> head_position =
        mobs.get_mob_head_position(effect.target_oid);
    damage_numbers.push_back(effect.number);
    damage_numbers.back().set_x(head_position.x());

    const SpecialMove& move = get_move(effect.move_id);
    mobs.apply_damage(
        effect.target_oid, effect.damage, effect.to_left, effect.user, move);
}

void Combat::push_attack(const AttackResult& attack)
{
    attack_results.push(400, attack);
}

void Combat::apply_attack(const AttackResult& attack)
{
    if (nullable_ptr<OtherChar> ouser = chars.get_char(attack.attacker)) {
        OtherChar& user = *ouser;
        user.update_skill(attack.skill, attack.level);
        user.update_speed(attack.speed);

        const SpecialMove& move = get_move(attack.skill);
        move.apply_useeffects(user);

        if (Stance::Id stance = Stance::by_id(attack.stance)) {
            user.attack(stance);
        } else {
            move.apply_actions(user, attack.type);
        }

        user.set_afterimage(attack.skill);

        extract_effects(user, move, attack);
    }
}

void Combat::extract_effects(const Char& user,
                             const SpecialMove& move,
                             const AttackResult& result)
{
    AttackUser attackuser = {user.get_skill_level(move.get_id()),
                             user.get_level(),
                             user.is_two_handed(),
                             !result.to_left};
    if (result.bullet) {
        Bullet bullet{move.get_bullet(user, result.bullet),
                      user.get_position(),
                      result.to_left};

        for (auto& line : result.damage_lines) {
            std::int32_t oid = line.first;
            if (mobs.contains(oid)) {
                std::vector<DamageNumber> numbers =
                    place_numbers(oid, line.second);
                Point<std::int16_t> head = mobs.get_mob_head_position(oid);

                std::size_t i = 0;
                for (auto& number : numbers) {
                    DamageEffect effect{attackuser,
                                        number,
                                        line.second[i].first,
                                        result.to_left,
                                        oid,
                                        move.get_id()};
                    bullet_effects.emplace(user.get_attack_delay(i),
                                           std::move(effect),
                                           bullet,
                                           head);
                    ++i;
                }
            }
        }

        if (result.damage_lines.empty()) {
            std::int16_t xshift = result.to_left ? -400 : 400;
            Point<std::int16_t> target =
                user.get_position() + Point<std::int16_t>(xshift, -26);
            for (std::uint8_t i = 0; i < result.hit_count; ++i) {
                DamageEffect effect{attackuser, {}, 0, false, 0, 0};
                bullet_effects.emplace(user.get_attack_delay(i),
                                       std::move(effect),
                                       bullet,
                                       target);
            }
        }
    } else {
        for (auto& line : result.damage_lines) {
            std::int32_t oid = line.first;
            if (mobs.contains(oid)) {
                std::vector<DamageNumber> numbers =
                    place_numbers(oid, line.second);

                std::size_t i = 0;
                for (auto& number : numbers) {
                    damage_effects.emplace(user.get_attack_delay(i),
                                           attackuser,
                                           number,
                                           line.second[i].first,
                                           result.to_left,
                                           oid,
                                           move.get_id());

                    ++i;
                }
            }
        }
    }
}

std::vector<DamageNumber> Combat::place_numbers(
    std::int32_t oid,
    const std::vector<std::pair<std::int32_t, bool>>& damage_lines)
{
    std::vector<DamageNumber> numbers;
    std::int16_t head = mobs.get_mob_head_position(oid).y();
    for (auto& line : damage_lines) {
        std::int32_t amount = line.first;
        bool critical = line.second;
        DamageNumber::Type type =
            critical ? DamageNumber::CRITICAL : DamageNumber::NORMAL;
        numbers.emplace_back(type, amount, head);

        head -= DamageNumber::row_height(critical);
    }
    return numbers;
}

void Combat::show_buff(std::int32_t cid,
                       std::int32_t skill_id,
                       std::int8_t level)
{
    if (nullable_ptr<OtherChar> ouser = chars.get_char(cid)) {
        OtherChar& user = *ouser;
        user.update_skill(skill_id, level);

        const SpecialMove& move = get_move(skill_id);
        move.apply_useeffects(user);
        move.apply_actions(user, Attack::MAGIC);
    }
}

void Combat::show_player_buff(std::int32_t skill_id)
{
    get_move(skill_id).apply_useeffects(player);
}

const SpecialMove& Combat::get_move(std::int32_t move_id)
{
    if (move_id == 0)
        return regular_attack;

    auto iter = skills.find(move_id);
    if (iter == skills.end()) {
        iter = skills.emplace(move_id, move_id).first;
    }
    return iter->second;
}
} // namespace jrc
