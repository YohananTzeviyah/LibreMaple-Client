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
#include "CharLook.h"

#include "../../Constants.h"
#include "../../Data/WeaponData.h"

#include <array>

namespace jrc
{
CharLook::CharLook(const LookEntry& entry)
{
    reset();

    set_body(entry.skin);
    set_hair(entry.hair_id);
    set_face(entry.face_id);

    for (auto& [_, equip] : entry.equips) {
        add_equip(equip);
    }
}

CharLook::CharLook()
{
    reset();

    body = nullptr;
    hair = nullptr;
    face = nullptr;
}

void CharLook::reset()
{
    flip = true;

    action = nullptr;
    action_str = "";
    act_frame = 0;

    set_stance(Stance::STAND1);
    st_frame.set(0);
    st_elapsed = 0;

    set_expression(Expression::DEFAULT);
    exp_frame.set(0);
    exp_elapsed = 0;
}

void CharLook::draw(const DrawArgument& args,
                    Stance::Id inter_stance,
                    Expression::Id inter_expression,
                    std::uint8_t inter_frame,
                    std::uint8_t inter_exp_frame) const
{
    Point<std::int16_t> face_shift =
        draw_info.get_face_pos(inter_stance, inter_frame);
    DrawArgument faceargs =
        args + DrawArgument{face_shift, false, Point<std::int16_t>{}};

    if (Stance::is_climbing(inter_stance)) {
        body->draw(inter_stance, Body::BODY, inter_frame, args);
        equips.draw(
            Equipslot::GLOVES, inter_stance, Clothing::GLOVE, inter_frame, args);
        equips.draw(
            Equipslot::SHOES, inter_stance, Clothing::SHOES, inter_frame, args);
        equips.draw(
            Equipslot::PANTS, inter_stance, Clothing::PANTS, inter_frame, args);
        equips.draw(
            Equipslot::TOP, inter_stance, Clothing::TOP, inter_frame, args);
        equips.draw(
            Equipslot::TOP, inter_stance, Clothing::MAIL, inter_frame, args);
        equips.draw(
            Equipslot::CAPE, inter_stance, Clothing::CAPE, inter_frame, args);
        body->draw(inter_stance, Body::HEAD, inter_frame, args);
        equips.draw(Equipslot::EARRINGS,
                    inter_stance,
                    Clothing::EARRINGS,
                    inter_frame,
                    args);

        switch (equips.getcaptype()) {
        case CharEquips::NONE:
            hair->draw(inter_stance, Hair::BACK, inter_frame, args);
            break;
        case CharEquips::HEADBAND:
            equips.draw(
                Equipslot::CAP, inter_stance, Clothing::CAP, inter_frame, args);
            hair->draw(inter_stance, Hair::BACK, inter_frame, args);
            break;
        case CharEquips::HALFCOVER:
            hair->draw(inter_stance, Hair::BELOWCAP, inter_frame, args);
            equips.draw(
                Equipslot::CAP, inter_stance, Clothing::CAP, inter_frame, args);
            break;
        case CharEquips::FULLCOVER:
            equips.draw(
                Equipslot::CAP, inter_stance, Clothing::CAP, inter_frame, args);
            break;
        case CharEquips::HAIRPIN:
            // TODO
            break;
        }

        equips.draw(Equipslot::SHIELD,
                    inter_stance,
                    Clothing::BACKSHIELD,
                    inter_frame,
                    args);
        equips.draw(Equipslot::WEAPON,
                    inter_stance,
                    Clothing::BACKWEAPON,
                    inter_frame,
                    args);
    } else {
        hair->draw(inter_stance, Hair::BELOWBODY, inter_frame, args);
        equips.draw(
            Equipslot::CAPE, inter_stance, Clothing::CAPE, inter_frame, args);
        equips.draw(Equipslot::SHIELD,
                    inter_stance,
                    Clothing::SHIELD_BELOW_BODY,
                    inter_frame,
                    args);
        equips.draw(Equipslot::WEAPON,
                    inter_stance,
                    Clothing::WEAPON_BELOW_BODY,
                    inter_frame,
                    args);
        equips.draw(Equipslot::CAP,
                    inter_stance,
                    Clothing::CAP_BELOW_BODY,
                    inter_frame,
                    args);
        body->draw(inter_stance, Body::BODY, inter_frame, args);
        equips.draw(Equipslot::GLOVES,
                    inter_stance,
                    Clothing::WRIST_OVER_BODY,
                    inter_frame,
                    args);
        equips.draw(Equipslot::GLOVES,
                    inter_stance,
                    Clothing::GLOVE_OVER_BODY,
                    inter_frame,
                    args);
        equips.draw(
            Equipslot::SHOES, inter_stance, Clothing::SHOES, inter_frame, args);
        body->draw(inter_stance, Body::ARM_BELOW_HEAD, inter_frame, args);

        if (equips.has_overall()) {
            equips.draw(
                Equipslot::TOP, inter_stance, Clothing::MAIL, inter_frame, args);
        } else {
            equips.draw(Equipslot::PANTS,
                        inter_stance,
                        Clothing::PANTS,
                        inter_frame,
                        args);
            equips.draw(
                Equipslot::TOP, inter_stance, Clothing::TOP, inter_frame, args);
        }

        body->draw(
            inter_stance, Body::ARM_BELOW_HEAD_OVER_MAIL, inter_frame, args);
        hair->draw(inter_stance, Hair::DEFAULT, inter_frame, args);
        equips.draw(Equipslot::SHIELD,
                    inter_stance,
                    Clothing::SHIELD_OVER_HAIR,
                    inter_frame,
                    args);
        equips.draw(Equipslot::EARRINGS,
                    inter_stance,
                    Clothing::EARRINGS,
                    inter_frame,
                    args);
        body->draw(inter_stance, Body::HEAD, inter_frame, args);
        hair->draw(inter_stance, Hair::SHADE, inter_frame, args);
        face->draw(inter_expression, inter_exp_frame, faceargs);
        equips.draw(
            Equipslot::FACEACC, inter_stance, Clothing::FACEACC, 0, faceargs);
        equips.draw(Equipslot::EYEACC,
                    inter_stance,
                    Clothing::EYEACC,
                    inter_frame,
                    args);
        equips.draw(Equipslot::SHIELD,
                    inter_stance,
                    Clothing::SHIELD,
                    inter_frame,
                    args);

        switch (equips.getcaptype()) {
        case CharEquips::NONE:
            hair->draw(inter_stance, Hair::OVERHEAD, inter_frame, args);
            break;
        case CharEquips::HEADBAND:
            equips.draw(
                Equipslot::CAP, inter_stance, Clothing::CAP, inter_frame, args);
            hair->draw(inter_stance, Hair::DEFAULT, inter_frame, args);
            hair->draw(inter_stance, Hair::OVERHEAD, inter_frame, args);
            equips.draw(Equipslot::CAP,
                        inter_stance,
                        Clothing::CAP_OVER_HAIR,
                        inter_frame,
                        args);
            break;
        case CharEquips::HALFCOVER:
            hair->draw(inter_stance, Hair::DEFAULT, inter_frame, args);
            equips.draw(
                Equipslot::CAP, inter_stance, Clothing::CAP, inter_frame, args);
            break;
        case CharEquips::FULLCOVER:
            equips.draw(
                Equipslot::CAP, inter_stance, Clothing::CAP, inter_frame, args);
            break;
        default:
            break;
        }

        equips.draw(Equipslot::WEAPON,
                    inter_stance,
                    Clothing::WEAPON_BELOW_ARM,
                    inter_frame,
                    args);
        bool twohanded = is_twohanded(inter_stance);
        if (twohanded) {
            equips.draw(Equipslot::TOP,
                        inter_stance,
                        Clothing::MAILARM,
                        inter_frame,
                        args);
            body->draw(inter_stance, Body::ARM, inter_frame, args);
            equips.draw(Equipslot::WEAPON,
                        inter_stance,
                        Clothing::WEAPON,
                        inter_frame,
                        args);
        } else {
            equips.draw(Equipslot::WEAPON,
                        inter_stance,
                        Clothing::WEAPON,
                        inter_frame,
                        args);
            body->draw(inter_stance, Body::ARM, inter_frame, args);
            equips.draw(Equipslot::TOP,
                        inter_stance,
                        Clothing::MAILARM,
                        inter_frame,
                        args);
        }

        equips.draw(
            Equipslot::GLOVES, inter_stance, Clothing::WRIST, inter_frame, args);
        equips.draw(
            Equipslot::GLOVES, inter_stance, Clothing::GLOVE, inter_frame, args);
        equips.draw(Equipslot::WEAPON,
                    inter_stance,
                    Clothing::WEAPON_OVER_GLOVE,
                    inter_frame,
                    args);

        body->draw(inter_stance, Body::HAND_BELOW_WEAPON, inter_frame, args);

        body->draw(inter_stance, Body::ARM_OVER_HAIR, inter_frame, args);
        body->draw(
            inter_stance, Body::ARM_OVER_HAIR_BELOW_WEAPON, inter_frame, args);
        equips.draw(Equipslot::WEAPON,
                    inter_stance,
                    Clothing::WEAPON_OVER_HAND,
                    inter_frame,
                    args);
        equips.draw(Equipslot::WEAPON,
                    inter_stance,
                    Clothing::WEAPON_OVER_BODY,
                    inter_frame,
                    args);
        body->draw(inter_stance, Body::HAND_OVER_HAIR, inter_frame, args);
        body->draw(inter_stance, Body::HAND_OVER_WEAPON, inter_frame, args);

        equips.draw(Equipslot::GLOVES,
                    inter_stance,
                    Clothing::WRIST_OVER_HAIR,
                    inter_frame,
                    args);
        equips.draw(Equipslot::GLOVES,
                    inter_stance,
                    Clothing::GLOVE_OVER_HAIR,
                    inter_frame,
                    args);
    }
}

void CharLook::draw(const DrawArgument& args, float alpha) const
{
    if (!body || !hair || !face) {
        return;
    }

    Point<std::int16_t> ac_move;
    if (action) {
        ac_move = action->get_move();
    }

    DrawArgument rel_args = {ac_move, flip};

    Stance::Id inter_stance = stance.get(alpha);
    Expression::Id inter_expression = expression.get(alpha);
    std::uint8_t inter_frame = st_frame.get(alpha);
    std::uint8_t inter_exp_frame = exp_frame.get(alpha);

    switch (inter_stance) {
    case Stance::STAND1:
    case Stance::STAND2:
        if (alerted) {
            inter_stance = Stance::ALERT;
        }
        break;
    default:
        break;
    }

    draw(rel_args + args,
         inter_stance,
         inter_expression,
         inter_frame,
         inter_exp_frame);
}

void CharLook::draw(Point<std::int16_t> position,
                    bool flipped,
                    Stance::Id inter_stance,
                    Expression::Id inter_expression) const
{
    inter_stance = equips.adjust_stance(inter_stance);
    draw({position, flipped}, inter_stance, inter_expression, 0, 0);
}

bool CharLook::update(std::uint16_t timestep)
{
    if (timestep == 0) {
        stance.normalize();
        st_frame.normalize();
        expression.normalize();
        exp_frame.normalize();

        return false;
    }

    alerted.update();

    bool ani_end = false;
    if (action == nullptr) {
        std::uint16_t delay = get_delay(stance.get(), st_frame.get());
        std::uint16_t delta = delay - st_elapsed;
        if (timestep >= delta) {
            st_elapsed = timestep - delta;

            std::uint8_t nextframe =
                get_next_frame(stance.get(), st_frame.get());
            float threshold = static_cast<float>(delta) / timestep;
            st_frame.next(nextframe, threshold);

            if (st_frame == 0) {
                ani_end = true;
            }
        } else {
            stance.normalize();
            st_frame.normalize();

            st_elapsed += timestep;
        }
    } else {
        std::uint16_t delay = action->get_delay();
        std::uint16_t delta = delay - st_elapsed;
        if (timestep >= delta) {
            st_elapsed = timestep - delta;

            act_frame = draw_info.next_action_frame(action_str, act_frame);
            if (act_frame > 0) {
                action = draw_info.get_action(action_str, act_frame);

                float threshold = static_cast<float>(delta) / timestep;
                stance.next(action->get_stance(), threshold);
                st_frame.next(action->get_frame(), threshold);
            } else {
                ani_end = true;
                action = nullptr;
                action_str = "";
                set_stance(Stance::STAND1);
            }
        } else {
            stance.normalize();
            st_frame.normalize();

            st_elapsed += timestep;
        }
    }

    auto exp_delay = static_cast<std::uint16_t>(
        face->get_delay(expression.get(), exp_frame.get()));
    std::uint16_t exp_delta = exp_delay - exp_elapsed;
    if (timestep >= exp_delta) {
        exp_elapsed = timestep - exp_delta;

        std::uint8_t next_exp_frame =
            face->next_frame(expression.get(), exp_frame.get());
        float fc_threshold = static_cast<float>(exp_delta) / timestep;
        exp_frame.next(next_exp_frame, fc_threshold);

        if (exp_frame == 0) {
            if (expression == Expression::DEFAULT) {
                expression.next(Expression::BLINK, fc_threshold);
            } else {
                expression.next(Expression::DEFAULT, fc_threshold);
            }
        }
    } else {
        expression.normalize();
        exp_frame.normalize();

        exp_elapsed += timestep;
    }

    return ani_end;
}

void CharLook::set_body(std::int32_t skin_id)
{
    auto iter = body_types.find(skin_id);
    if (iter == body_types.end()) {
        iter = body_types
                   .emplace(std::piecewise_construct,
                            std::forward_as_tuple(skin_id),
                            std::forward_as_tuple(skin_id, draw_info))
                   .first;
    }
    body = &iter->second;
}

void CharLook::set_hair(std::int32_t hair_id)
{
    auto iter = hair_styles.find(hair_id);
    if (iter == hair_styles.end()) {
        iter = hair_styles
                   .emplace(std::piecewise_construct,
                            std::forward_as_tuple(hair_id),
                            std::forward_as_tuple(hair_id, draw_info))
                   .first;
    }
    hair = &iter->second;
}

void CharLook::set_face(std::int32_t face_id)
{
    auto iter = face_types.find(face_id);
    if (iter == face_types.end()) {
        iter = face_types.emplace(face_id, face_id).first;
    }
    face = &iter->second;
}

void CharLook::update_two_handed()
{
    Stance::Id base_stance = Stance::base_of(stance.get());
    set_stance(base_stance);
}

void CharLook::add_equip(std::int32_t item_id)
{
    equips.add_equip(item_id, draw_info);
    update_two_handed();
}

void CharLook::remove_equip(Equipslot::Id slot)
{
    equips.remove_equip(slot);
    if (slot == Equipslot::WEAPON) {
        update_two_handed();
    }
}

void CharLook::attack(bool degenerate)
{
    std::int32_t weapon_id = equips.get_weapon();
    if (weapon_id <= 0) {
        return;
    }

    const WeaponData& weapon = WeaponData::get(weapon_id);

    std::uint8_t attack_type = weapon.get_attack();
    if (attack_type == 9 && !degenerate) {
        stance.set(Stance::SHOT);
        set_action("handgun");
    } else {
        stance.set(get_attack_stance(attack_type, degenerate));
        st_frame.set(0);
        st_elapsed = 0;
    }

    weapon.get_use_sound(degenerate).play();
}

void CharLook::attack(Stance::Id new_stance)
{
    if (action || new_stance == Stance::NONE) {
        return;
    }

    switch (new_stance) {
    case Stance::SHOT:
        set_action("handgun");
        break;
    default:
        set_stance(new_stance);
    }
}

void CharLook::set_stance(Stance::Id new_stance)
{
    if (action || new_stance == Stance::NONE) {
        return;
    }

    Stance::Id adjstance = equips.adjust_stance(new_stance);
    if (stance != adjstance) {
        stance.set(adjstance);
        st_frame.set(0);
        st_elapsed = 0;
    }
}

Stance::Id CharLook::get_attack_stance(std::uint8_t attack,
                                       bool degenerate) const
{
    if (stance == Stance::PRONE) {
        return Stance::PRONESTAB;
    }

    enum Attack {
        NONE = 0,
        S1A1M1D = 1,
        SPEAR = 2,
        BOW = 3,
        CROSSBOW = 4,
        S2A2M2 = 5,
        WAND = 6,
        CLAW = 7,
        GUN = 9,
        NUM_ATTACKS
    };

    static const std::array<std::vector<Stance::Id>, NUM_ATTACKS>
        degen_stances{{{Stance::NONE},
                       {Stance::NONE},
                       {Stance::NONE},
                       {Stance::SWINGT1, Stance::SWINGT3},
                       {Stance::SWINGT1, Stance::STABT1},
                       {Stance::NONE},
                       {Stance::NONE},
                       {Stance::SWINGT1, Stance::STABT1},
                       {Stance::NONE},
                       {Stance::SWINGP1, Stance::STABT2}}};

    static const std::array<std::vector<Stance::Id>, NUM_ATTACKS>
        attack_stances{{{Stance::NONE},
                        {Stance::STABO1,
                         Stance::STABO2,
                         Stance::SWINGO1,
                         Stance::SWINGO2,
                         Stance::SWINGO3},
                        {Stance::STABT1, Stance::SWINGP1},
                        {Stance::SHOOT1},
                        {Stance::SHOOT2},
                        {Stance::STABO1,
                         Stance::STABO2,
                         Stance::SWINGT1,
                         Stance::SWINGT2,
                         Stance::SWINGT3},
                        {Stance::SWINGO1, Stance::SWINGO2},
                        {Stance::SWINGO1, Stance::SWINGO2},
                        {Stance::NONE},
                        {Stance::SHOT}}};

    if (attack <= NONE || attack >= NUM_ATTACKS) {
        return Stance::STAND1;
    }

    const auto& stances =
        degenerate ? degen_stances[attack] : attack_stances[attack];
    if (stances.empty()) {
        return Stance::STAND1;
    }

    return stances[Randomizer::next_int(stances.size())];
}

std::uint16_t CharLook::get_delay(Stance::Id st, std::uint8_t fr) const
{
    return draw_info.get_delay(st, fr);
}

std::uint8_t CharLook::get_next_frame(Stance::Id st, std::uint8_t fr) const
{
    return draw_info.next_frame(st, fr);
}

void CharLook::set_expression(Expression::Id new_expression)
{
    if (expression != new_expression) {
        expression.set(new_expression);
        exp_frame.set(0);

        exp_elapsed = 0;
    }
}

void CharLook::set_action(const std::string& ac_str)
{
    if (ac_str == action_str || ac_str.empty()) {
        return;
    }

    if (Stance::Id ac_stance = Stance::by_string(ac_str)) {
        set_stance(ac_stance);
    } else {
        action = draw_info.get_action(ac_str, 0);

        if (action) {
            act_frame = 0;
            st_elapsed = 0;
            action_str = ac_str;

            stance.set(action->get_stance());
            st_frame.set(action->get_frame());
        }
    }
}

void CharLook::set_direction(bool f)
{
    flip = f;
}

void CharLook::set_alerted(std::int64_t millis)
{
    alerted.set_for(millis);
}

bool CharLook::is_twohanded(Stance::Id st) const
{
    switch (st) {
    case Stance::STAND1:
    case Stance::WALK1:
        return false;
    case Stance::STAND2:
    case Stance::WALK2:
        return true;
    default:
        return equips.is_two_handed();
    }
}

std::uint16_t CharLook::get_attack_delay(std::size_t no,
                                         std::uint8_t first_frame) const
{
    if (action) {
        return draw_info.get_attack_delay(action_str, no);
    } else {
        std::uint16_t delay = 0;
        for (std::uint8_t frame = 0; frame < first_frame; ++frame) {
            delay += get_delay(stance.get(), frame);
        }
        return delay;
    }
}

std::uint8_t CharLook::get_frame() const
{
    return st_frame.get();
}

Stance::Id CharLook::get_stance() const
{
    return stance.get();
}

const Body* CharLook::get_body() const
{
    return body;
}

const Hair* CharLook::get_hair() const
{
    return hair;
}

const Face* CharLook::get_face() const
{
    return face;
}

const CharEquips& CharLook::get_equips() const
{
    return equips;
}

void CharLook::init()
{
    draw_info.init();
}

BodyDrawinfo CharLook::draw_info;
std::unordered_map<std::int32_t, Hair> CharLook::hair_styles;
std::unordered_map<std::int32_t, Face> CharLook::face_types;
std::unordered_map<std::int32_t, Body> CharLook::body_types;
} // namespace jrc
