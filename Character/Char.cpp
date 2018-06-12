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
#include "Char.h"

#include "../Constants.h"
#include "../Data/WeaponData.h"
#include "../Util/Misc.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

#include <algorithm>
#include <cmath>

namespace jrc
{
Char::Char(std::int32_t o, const CharLook& lk, std::string&& name) noexcept
    : MapObject(o),
      look(lk),
      name_label({Text::A13M,
                  Text::CENTER,
                  Text::WHITE,
                  Text::NAMETAG,
                  std::move(name)})
{
}

void Char::draw(double viewx, double viewy, float alpha) const
{
    Point<std::int16_t> absp = ph_obj.get_absolute(viewx, viewy, alpha);

    effects.draw_below(absp, alpha);

    Color color;
    if (invincible) {
        float phi = invincible.alpha() * 30.0f;
        float rgb = 0.9f - 0.5f * std::abs(std::sin(phi));
        color = {rgb, rgb, rgb, 1.0f};
    } else {
        color = Color::WHITE;
    }
    look.draw({absp, color}, alpha);

    afterimage.draw(look.get_frame(), {absp, flip}, alpha);

    if (iron_body) {
        float ibalpha = iron_body.alpha();
        float scale = 1.0f + ibalpha;
        float opacity = 1.0f - ibalpha;
        look.draw({absp, scale, scale, opacity}, alpha);
    }

    for (auto& pet : pets) {
        if (pet.get_item_id()) {
            pet.draw(viewx, viewy, alpha);
        }
    }

    name_label.draw(absp);
    chat_balloon.draw(absp - Point<std::int16_t>(0, 85));

    effects.draw_above(absp, alpha);

    for (auto& number : damage_numbers) {
        number.draw(viewx, viewy, alpha);
    }
}

bool Char::update(const Physics& physics, float speed)
{
    damage_numbers.erase(std::remove_if(damage_numbers.begin(),
                                        damage_numbers.end(),
                                        [](auto& dn) { return dn.update(); }),
                         damage_numbers.end());
    effects.update();
    chat_balloon.update();
    invincible.update();
    iron_body.update();

    for (auto& pet : pets) {
        if (pet.get_item_id()) {
            switch (state) {
            case LADDER:
            case ROPE:
                pet.set_stance(PetLook::HANG);
                break;
            case SWIM:
                pet.set_stance(PetLook::FLY);
                break;
            default:
                if (pet.get_stance() == PetLook::HANG ||
                    pet.get_stance() == PetLook::FLY) {
                    pet.set_stance(PetLook::STAND);
                }
            }
            pet.update(physics, get_position());
        }
    }

    std::uint16_t stancespeed = 0;
    if (speed >= 1.0f / Constants::TIMESTEP) {
        stancespeed = static_cast<std::uint16_t>(Constants::TIMESTEP * speed);
    }
    afterimage.update(look.get_frame(), stancespeed);
    return look.update(stancespeed);
}

float Char::get_stance_speed() const
{
    if (attacking)
        return get_real_attack_speed();

    switch (state) {
    case WALK:
        return static_cast<float>(std::abs(ph_obj.hspeed));
    case LADDER:
    case ROPE:
        return static_cast<float>(std::abs(ph_obj.vspeed));
    default:
        return 1.0f;
    }
}

float Char::get_real_attack_speed() const
{
    std::int8_t speed = get_integer_attack_speed();
    return 1.7f - static_cast<float>(speed) / 10.0f;
}

std::uint16_t Char::get_attack_delay(std::size_t no) const
{
    std::uint8_t first_frame = afterimage.get_first_frame();
    std::uint16_t delay = look.get_attackdelay(no, first_frame);
    float fspeed = get_real_attack_speed();
    return static_cast<std::uint16_t>(delay / fspeed);
}

std::int8_t Char::update(const Physics& physics)
{
    update(physics, 1.0f);
    return get_layer();
}

std::int8_t Char::get_layer() const
{
    return is_climbing() ? static_cast<std::int8_t>(7) : ph_obj.fh_layer;
}

void Char::show_attack_effect(Animation to_show, std::int8_t z)
{
    float attackspeed = get_real_attack_speed();
    effects.add(to_show, {flip}, z, attackspeed);
}

void Char::show_effect_id(CharEffect::Id to_show)
{
    effects.add(char_effects[to_show]);
}

void Char::show_iron_body()
{
    iron_body.set_for(500);
}

void Char::show_damage(std::int32_t damage)
{
    std::int16_t start_y = ph_obj.get_y() - 60;
    std::int16_t x = ph_obj.get_x() - 10;
    damage_numbers.emplace_back(DamageNumber::TO_PLAYER, damage, start_y, x);

    look.set_alerted(5'000);
    invincible.set_for(2'000);
}

void Char::speak(std::string&& line)
{
    chat_balloon.change_text(std::move(line));
}

void Char::change_look(Maplestat::Id stat, std::int32_t id)
{
    switch (stat) {
    case Maplestat::SKIN:
        look.set_body(id);
        break;
    case Maplestat::FACE:
        look.set_face(id);
        break;
    case Maplestat::HAIR:
        look.set_hair(id);
        break;
    default:
        break;
    }
}

void Char::set_state(std::uint8_t state_byte)
{
    if (state_byte % 2 == 1) {
        set_direction(false);
        state_byte -= 1;
    } else {
        set_direction(true);
    }

    Char::State newstate = by_value(state_byte);
    set_state(newstate);
}

void Char::set_expression(std::int32_t exp_id)
{
    Expression::Id expression = Expression::byaction(exp_id);
    look.set_expression(expression);
}

void Char::attack(const std::string& action)
{
    look.set_action(action);

    attacking = true;
    look.set_alerted(5'000);
}

void Char::attack(Stance::Id stance)
{
    look.attack(stance);

    attacking = true;
    look.set_alerted(5'000);
}

void Char::attack(bool degenerate)
{
    look.attack(degenerate);

    attacking = true;
    look.set_alerted(5'000);
}

void Char::set_afterimage(std::int32_t skill_id)
{
    std::int32_t weapon_id = look.get_equips().get_weapon();
    if (weapon_id <= 0) {
        return;
    }

    const WeaponData& weapon = WeaponData::get(weapon_id);

    std::string stance_name = Stance::names[look.get_stance()];
    std::int16_t weapon_level =
        weapon.get_equip_data().get_req_stat(Maplestat::LEVEL);
    auto ai_name = weapon.get_afterimage();
    afterimage = {skill_id, ai_name, stance_name, weapon_level};
}

const Afterimage& Char::get_afterimage() const
{
    return afterimage;
}

void Char::set_direction(bool f)
{
    flip = f;
    look.set_direction(f);
}

void Char::set_state(State st)
{
    state = st;

    Stance::Id stance = Stance::by_state(state);
    look.set_stance(stance);
}

void Char::add_pet(std::uint8_t index,
                   std::int32_t iid,
                   std::string&& name,
                   std::int32_t uniqueid,
                   Point<std::int16_t> pos,
                   std::uint8_t stance,
                   std::int32_t fhid)
{
    if (index > 2) {
        return;
    }

    pets[index] = PetLook(iid, std::move(name), uniqueid, pos, stance, fhid);
}

void Char::remove_pet(std::uint8_t index, bool hunger)
{
    if (index > 2) {
        return;
    }

    pets[index] = PetLook();
    if (hunger) {
        // TODO ?
    }
}

bool Char::is_invincible() const
{
    return invincible == true;
}

bool Char::is_sitting() const
{
    return state == SIT;
}

bool Char::is_climbing() const
{
    return state == LADDER || state == ROPE;
}

bool Char::is_two_handed() const
{
    return look.get_equips().is_two_handed();
}

Weapon::Type Char::get_weapon_type() const
{
    std::int32_t weapon_id = look.get_equips().get_weapon();
    if (weapon_id <= 0) {
        return Weapon::NONE;
    }

    return WeaponData::get(weapon_id).get_type();
}

bool Char::get_flip() const
{
    return flip;
}

std::string_view Char::get_name() const
{
    return name_label.get_text();
}

CharLook& Char::get_look()
{
    return look;
}

const CharLook& Char::get_look() const
{
    return look;
}

PhysicsObject& Char::get_phobj()
{
    return ph_obj;
}

void Char::init()
{
    CharLook::init();

    nl::node src = nl::nx::effect["BasicEff.img"];
    for (auto path : CharEffect::PATHS) {
        char_effects.emplace(path.first, src.resolve(path.second));
    }
}

EnumMap<CharEffect::Id, Animation> Char::char_effects;
} // namespace jrc
