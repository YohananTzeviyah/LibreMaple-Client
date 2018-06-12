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
#include "../../Constants.h"
#include "../../Template/Interpolated.h"
#include "../../Template/Point.h"
#include "../../Util/Misc.h"

namespace jrc
{
//! Struct that contains all properties for movement calculations.
struct MovingObject {
    Linear<double> x;
    Linear<double> y;
    double hspeed = 0.0;
    double vspeed = 0.0;

    void normalize()
    {
        x.normalize();
        y.normalize();
    }

    void move()
    {
        x += hspeed;
        y += vspeed;
    }

    void set_x(double d)
    {
        x.set(d);
    }

    void set_y(double d)
    {
        y.set(d);
    }

    void limit_x(double d)
    {
        x = d;
        hspeed = 0.0;
    }

    void limit_y(double d)
    {
        y = d;
        vspeed = 0.0;
    }

    void move_x_until(double d, std::uint16_t delay)
    {
        if (delay) {
            double h_delta = d - x.get();
            hspeed = Constants::TIMESTEP * h_delta / delay;
        }
    }

    void move_y_until(double d, std::uint16_t delay)
    {
        if (delay) {
            double v_delta = d - y.get();
            vspeed = Constants::TIMESTEP * v_delta / delay;
        }
    }

    bool h_mobile() const
    {
        return hspeed != 0.0;
    }

    bool v_mobile() const
    {
        return vspeed != 0.0;
    }

    bool mobile() const
    {
        return h_mobile() || v_mobile();
    }

    double crnt_x() const
    {
        return x.get();
    }

    double crnt_y() const
    {
        return y.get();
    }

    double next_x() const
    {
        return x + hspeed;
    }

    double next_y() const
    {
        return y + vspeed;
    }

    std::int16_t get_x() const
    {
        double rounded = std::round(x.get());
        return static_cast<std::int16_t>(rounded);
    }

    std::int16_t get_y() const
    {
        double rounded = std::round(y.get());
        return static_cast<std::int16_t>(rounded);
    }

    std::int16_t get_last_x() const
    {
        double rounded = std::round(x.last());
        return static_cast<std::int16_t>(rounded);
    }

    std::int16_t get_last_y() const
    {
        double rounded = std::round(y.last());
        return static_cast<std::int16_t>(rounded);
    }

    Point<std::int16_t> get_position() const
    {
        return {get_x(), get_y()};
    }

    std::int16_t get_absolute_x(double viewx, float alpha) const
    {
        double interx = x.normalized() ? std::round(x.get()) : x.get(alpha);
        return math::saturating_cast<std::int16_t>(std::round(interx + viewx));
    }

    std::int16_t get_absolute_y(double viewy, float alpha) const
    {
        double intery = y.normalized() ? std::round(y.get()) : y.get(alpha);
        return math::saturating_cast<std::int16_t>(std::round(intery + viewy));
    }

    Point<std::int16_t>
    get_absolute(double viewx, double viewy, float alpha) const
    {
        return {get_absolute_x(viewx, alpha), get_absolute_y(viewy, alpha)};
    }
};

//! Struct that contains all properties neccessary for physics calculations.
struct PhysicsObject : public MovingObject {
    //! Determines which physics engine to use.
    enum Type { NORMAL, ICE, SWIMMING, FLYING, FIXATED };

    enum Flag : std::uint32_t {
        NO_GRAVITY = 0x00'00'00'01,
        TURN_AT_EDGES = 0x00'00'00'02,
        CHECK_BELOW = 0x00'00'00'04
    };

    Type type = NORMAL;
    std::uint32_t flags = 0;
    std::uint16_t fh_id = 0;
    double fh_slope = 0.0;
    std::int8_t fh_layer = 0;
    double ground_below = 0.0;
    bool on_ground = true;
    bool enable_jd = false;

    double h_force = 0.0;
    double v_force = 0.0;
    double h_acc = 0.0;
    double v_acc = 0.0;

    bool is_flag_set(Flag f)
    {
        return (flags & f) != 0;
    }

    bool is_flag_not_set(Flag f)
    {
        return !is_flag_set(f);
    }

    void set_flag(Flag f)
    {
        flags |= f;
    }

    void clear_flag(Flag f)
    {
        flags &= ~f;
    }

    void clear_flags()
    {
        flags = 0;
    }
};
} // namespace jrc
