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
#include "Physics.h"

#include <functional>

namespace jrc
{
const double GRAVFORCE = 0.14;
const double SWIMGRAVFORCE = 0.03;
const double FRICTION = 0.3;
const double SLOPEFACTOR = 0.1;
const double GROUNDSLIP = 3.0;
const double FLYFRICTION = 0.05;
const double SWIMFRICTION = 0.08;

Physics::Physics(nl::node src)
{
    fht = src;
}

Physics::Physics() = default;

void Physics::move_object(PhysicsObject& phobj) const
{
    // Determine which platform the object is currently on.
    fht.update_fh(phobj);

    // Use the appropriate physics for the terrain the object is on.
    switch (phobj.type) {
    case PhysicsObject::NORMAL:
        move_normal(phobj);
        fht.limit_movement(phobj);
        break;
    case PhysicsObject::FLYING:
        move_flying(phobj);
        fht.limit_movement(phobj);
        break;
    case PhysicsObject::SWIMMING:
        move_swimming(phobj);
        fht.limit_movement(phobj);
        break;
    case PhysicsObject::FIXATED:
        break;
    case PhysicsObject::ICE:
        // TODO
        break;
    }

    // Move the object forward.
    phobj.move();
}

void Physics::move_normal(PhysicsObject& phobj) const
{
    phobj.v_acc = 0.0;
    phobj.h_acc = 0.0;
    if (phobj.on_ground) {
        phobj.v_acc += phobj.v_force;
        phobj.h_acc += phobj.h_force;

        if (phobj.h_acc == 0.0 && phobj.hspeed < 0.1 && phobj.hspeed > -0.1) {
            phobj.hspeed = 0.0;
        } else {
            double inertia = phobj.hspeed / GROUNDSLIP;
            double slopef = phobj.fh_slope;
            if (slopef > 0.5) {
                slopef = 0.5;
            } else if (slopef < -0.5) {
                slopef = -0.5;
            }
            phobj.h_acc -=
                (FRICTION + SLOPEFACTOR * (1.0 + slopef * -inertia)) * inertia;
        }
    } else if (phobj.is_flag_not_set(PhysicsObject::NO_GRAVITY)) {
        phobj.v_acc += GRAVFORCE;
    }
    phobj.h_force = 0.0;
    phobj.v_force = 0.0;

    phobj.hspeed += phobj.h_acc;
    phobj.vspeed += phobj.v_acc;
}

void Physics::move_flying(PhysicsObject& phobj) const
{
    phobj.h_acc = phobj.h_force;
    phobj.v_acc = phobj.v_force;
    phobj.h_force = 0.0;
    phobj.v_force = 0.0;

    phobj.h_acc -= FLYFRICTION * phobj.hspeed;
    phobj.v_acc -= FLYFRICTION * phobj.vspeed;

    phobj.hspeed += phobj.h_acc;
    phobj.vspeed += phobj.v_acc;

    if (phobj.h_acc == 0.0 && phobj.hspeed < 0.1 && phobj.hspeed > -0.1) {
        phobj.hspeed = 0.0;
    }

    if (phobj.v_acc == 0.0 && phobj.vspeed < 0.1 && phobj.vspeed > -0.1) {
        phobj.vspeed = 0.0;
    }
}

void Physics::move_swimming(PhysicsObject& phobj) const
{
    phobj.h_acc = phobj.h_force;
    phobj.v_acc = phobj.v_force;
    phobj.h_force = 0.0;
    phobj.v_force = 0.0;

    phobj.h_acc -= SWIMFRICTION * phobj.hspeed;
    phobj.v_acc -= SWIMFRICTION * phobj.vspeed;

    if (phobj.is_flag_not_set(PhysicsObject::NO_GRAVITY)) {
        phobj.v_acc += SWIMGRAVFORCE;
    }

    phobj.hspeed += phobj.h_acc;
    phobj.vspeed += phobj.v_acc;

    if (phobj.h_acc == 0.0 && phobj.hspeed < 0.1 && phobj.hspeed > -0.1) {
        phobj.hspeed = 0.0;
    }
    if (phobj.v_acc == 0.0 && phobj.vspeed < 0.1 && phobj.vspeed > -0.1) {
        phobj.vspeed = 0.0f;
    }
}

Point<std::int16_t> Physics::get_y_below(Point<std::int16_t> position) const
{
    std::int16_t ground = fht.get_y_below(position);
    return Point<std::int16_t>(position.x(), ground - 1);
}

const Footholdtree& Physics::get_fht() const
{
    return fht;
}
} // namespace jrc
