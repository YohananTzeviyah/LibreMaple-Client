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
#include "../../Net/Login.h"
#include "../../Template/Interpolated.h"
#include "../../Util/Randomizer.h"
#include "../../Util/TimedBool.h"
#include "Body.h"
#include "BodyDrawInfo.h"
#include "CharEquips.h"
#include "Face.h"
#include "Hair.h"

namespace jrc
{
class CharLook
{
public:
    CharLook(const LookEntry& entry);
    CharLook();

    void reset();
    void draw(const DrawArgument& args, float alpha) const;
    void draw(Point<std::int16_t> pos,
              bool flip,
              Stance::Id inter_stance,
              Expression::Id inter_expression) const;
    bool update(std::uint16_t timestep);

    void set_hair(std::int32_t hairid);
    void set_body(std::int32_t skinid);
    void set_face(std::int32_t faceid);
    void add_equip(std::int32_t item_id);
    void remove_equip(Equipslot::Id slot);

    void attack(bool degenerate);
    void attack(Stance::Id new_stance);
    void set_stance(Stance::Id new_stance);
    void set_expression(Expression::Id new_expression);
    void set_action(const std::string& ac_str);
    void set_direction(bool mirrored);
    void set_alerted(std::int64_t millis);

    bool is_twohanded(Stance::Id stance) const;
    std::uint16_t get_attack_delay(std::size_t no,
                                   std::uint8_t first_frame) const;
    std::uint8_t get_frame() const;
    Stance::Id get_stance() const;

    const Body* get_body() const;
    const Hair* get_hair() const;
    const Face* get_face() const;
    const CharEquips& get_equips() const;

    //! Initialize draw_info.
    static void init();

private:
    void update_two_handed();
    void draw(const DrawArgument& args,
              Stance::Id inter_stance,
              Expression::Id inter_expression,
              std::uint8_t inter_frame,
              std::uint8_t inter_exp_frame) const;
    std::uint16_t get_delay(Stance::Id stance, std::uint8_t frame) const;
    std::uint8_t get_next_frame(Stance::Id stance, std::uint8_t frame) const;
    Stance::Id get_attack_stance(std::uint8_t attack, bool degenerate) const;

    Nominal<Stance::Id> stance;
    Nominal<std::uint8_t> st_frame;
    std::uint16_t st_elapsed;

    Nominal<Expression::Id> expression;
    Nominal<std::uint8_t> exp_frame;
    std::uint16_t exp_elapsed;

    bool flip;

    const BodyAction* action;
    std::string action_str;
    std::uint8_t act_frame;

    const Body* body;
    const Hair* hair;
    const Face* face;
    CharEquips equips;

    TimedBool alerted;

    static BodyDrawinfo draw_info;
    static std::unordered_map<std::int32_t, Hair> hair_styles;
    static std::unordered_map<std::int32_t, Face> face_types;
    static std::unordered_map<std::int32_t, Body> body_types;
};
} // namespace jrc
