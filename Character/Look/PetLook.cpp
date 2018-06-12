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
#include "PetLook.h"

#include "../../Constants.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

namespace jrc
{
PetLook::PetLook(std::int32_t iid,
                 std::string&& nm,
                 std::int32_t uqid,
                 Point<std::int16_t> pos,
                 std::uint8_t st,
                 std::int32_t)
{
    item_id = iid;
    name = std::move(nm);
    unique_id = uqid;
    set_position(pos.x(), pos.y());
    set_stance(st);

    namelabel = {Text::A13M,
                 Text::CENTER,
                 Text::WHITE,
                 Text::NAMETAG,
                 std::string{name}};

    std::string str_id = std::to_string(iid);

    nl::node src = nl::nx::item["Pet"][str_id + ".img"];

    animations[MOVE] = src["move"];
    animations[STAND] = src["stand0"];
    animations[JUMP] = src["jump"];
    animations[ALERT] = src["alert"];
    animations[PRONE] = src["prone"];
    animations[FLY] = src["fly"];
    animations[HANG] = src["hang"];

    nl::node effsrc = nl::nx::effect["PetEff.img"][str_id];

    animations[WARP] = effsrc["warp"];
}

PetLook::PetLook() noexcept
{
    item_id = 0;
    name = "";
    unique_id = 0;
    stance = Stance::STAND;
}

void PetLook::draw(double viewx, double viewy, float alpha) const
{
    Point<std::int16_t> absp = phobj.get_absolute(viewx, viewy, alpha);

    animations[stance].draw(DrawArgument(absp, flip), alpha);
    namelabel.draw(absp);
}

void PetLook::update(const Physics& physics,
                     Point<std::int16_t> char_pos) noexcept
{
    static constexpr double PET_WALK_FORCE = 0.35;
    static constexpr double PET_FLY_FORCE = 0.2;

    Point<std::int16_t> cur_pos = phobj.get_position();
    switch (stance) {
    case STAND:
    case MOVE:
        if (cur_pos.disp(char_pos) > 150) {
            set_position(char_pos.x(), char_pos.y());
        } else {
            if (char_pos.x() - cur_pos.x() > 50) {
                phobj.h_force = PET_WALK_FORCE;
                flip = true;
                set_stance(MOVE);
            } else if (char_pos.x() - cur_pos.x() < -50) {
                phobj.h_force = -PET_WALK_FORCE;
                flip = false;
                set_stance(MOVE);
            } else {
                phobj.h_force = 0.0;
                set_stance(STAND);
            }
        }
        phobj.type = PhysicsObject::NORMAL;
        phobj.clear_flag(PhysicsObject::NO_GRAVITY);
        break;
    case HANG:
        set_position(char_pos.x(), char_pos.y());
        phobj.set_flag(PhysicsObject::NO_GRAVITY);
        break;
    case FLY:
        if (char_pos.disp(cur_pos) > 250) {
            set_position(char_pos.x(), char_pos.y());
        } else {
            if (char_pos.x() - cur_pos.x() > 50) {
                phobj.h_force = PET_FLY_FORCE;
                flip = true;
            } else if (char_pos.x() - cur_pos.x() < -50) {
                phobj.h_force = -PET_FLY_FORCE;
                flip = false;
            } else {
                phobj.h_force = 0.0f;
            }

            if (char_pos.y() - cur_pos.y() > 50.0f) {
                phobj.v_force = PET_FLY_FORCE;
            } else if (char_pos.y() - cur_pos.y() < -50.0f) {
                phobj.v_force = -PET_FLY_FORCE;
            } else {
                phobj.v_force = 0.0f;
            }
        }
        phobj.type = PhysicsObject::FLYING;
        phobj.clear_flag(PhysicsObject::NO_GRAVITY);
        break;
    default:
        // TODO: Handle all the other cases
        break;
    }

    physics.move_object(phobj);

    animations[stance].update();
}

void PetLook::set_position(std::int16_t x, std::int16_t y) noexcept
{
    phobj.set_x(x);
    phobj.set_y(y);
}

void PetLook::set_stance(Stance st) noexcept
{
    if (stance != st) {
        stance = st;
        animations[stance].reset();
    }
}

void PetLook::set_stance(std::uint8_t stancebyte) noexcept
{
    flip = stancebyte % 2 == 1;
    stance = stance_by_value(stancebyte);
}

std::int32_t PetLook::get_item_id() const noexcept
{
    return item_id;
}

PetLook::Stance PetLook::get_stance() const noexcept
{
    return stance;
}
} // namespace jrc
