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
#include "../Template/Rectangle.h"
#include "Color.h"

namespace jrc
{
class DrawArgument
{
public:
    constexpr DrawArgument() noexcept : DrawArgument(0, 0)
    {
    }

    constexpr DrawArgument(std::int16_t x, std::int16_t y) noexcept
        : DrawArgument(Point<std::int16_t>{x, y})
    {
    }

    constexpr DrawArgument(Point<std::int16_t> p) noexcept
        : DrawArgument(p, 1.0f)
    {
    }

    constexpr DrawArgument(Point<std::int16_t> p, float xs, float ys) noexcept
        : DrawArgument(p, p, xs, ys, 1.0f)
    {
    }

    constexpr DrawArgument(Point<std::int16_t> p,
                           Point<std::int16_t> s) noexcept
        : DrawArgument(p, p, s, 1.0f, 1.0f, 1.0f, 0.0f)
    {
    }

    constexpr DrawArgument(Point<std::int16_t> p, bool flip) noexcept
        : DrawArgument(p, flip, 1.0f)
    {
    }

    constexpr DrawArgument(float ang,
                           Point<std::int16_t> p,
                           float opc) noexcept
        : DrawArgument(ang, p, false, opc)
    {
    }

    constexpr DrawArgument(Point<std::int16_t> p, float opc) noexcept
        : DrawArgument(p, false, opc)
    {
    }

    constexpr DrawArgument(Point<std::int16_t> p, Color color) noexcept
        : DrawArgument(p, p, {}, 1.0f, 1.0f, color, 0.0f)
    {
    }

    constexpr DrawArgument(Point<std::int16_t> p,
                           bool flip,
                           Point<std::int16_t> c) noexcept
        : DrawArgument(p, c, flip ? -1.0f : 1.0f, 1.0f, 1.0f)
    {
    }

    constexpr DrawArgument(Point<std::int16_t> p,
                           Point<std::int16_t> c,
                           float xs,
                           float ys,
                           float opc) noexcept
        : DrawArgument(p, c, {}, xs, ys, opc, 0.0f)
    {
    }

    constexpr DrawArgument(bool flip) noexcept
        : DrawArgument(flip ? -1.0f : 1.0f, 1.0f, 1.0f)
    {
    }

    constexpr DrawArgument(float xs, float ys, float opc) noexcept
        : DrawArgument({}, xs, ys, opc)
    {
    }

    constexpr DrawArgument(Point<std::int16_t> p,
                           float xs,
                           float ys,
                           float opc) noexcept
        : DrawArgument(p, p, xs, ys, opc)
    {
    }

    constexpr DrawArgument(Point<std::int16_t> p,
                           bool flip,
                           float opc) noexcept
        : DrawArgument(p, p, flip ? -1.0f : 1.0f, 1.0f, opc)
    {
    }

    constexpr DrawArgument(float ang,
                           Point<std::int16_t> p,
                           bool flip,
                           float opc) noexcept
        : DrawArgument(p, p, {}, flip ? -1.0f : 1.0f, 1.0f, opc, ang)
    {
    }

    constexpr DrawArgument(Point<std::int16_t> p,
                           Point<std::int16_t> c,
                           Point<std::int16_t> s,
                           float xs,
                           float ys,
                           float opc,
                           float ang) noexcept
        : color(1.0f, 1.0f, 1.0f, opc),
          pos(p),
          center(c),
          stretch(s),
          xscale(xs),
          yscale(ys),
          angle(ang)
    {
    }

    constexpr DrawArgument(Point<std::int16_t> p,
                           Point<std::int16_t> c,
                           Point<std::int16_t> s,
                           float xs,
                           float ys,
                           Color color,
                           float ang) noexcept
        : color(color),
          pos(p),
          center(c),
          stretch(s),
          xscale(xs),
          yscale(ys),
          angle(ang)
    {
    }

    constexpr Point<std::int16_t> getpos() const noexcept
    {
        return pos;
    }

    constexpr Point<std::int16_t> getstretch() const noexcept
    {
        return stretch;
    }

    constexpr float get_xscale() const noexcept
    {
        return xscale;
    }

    constexpr float get_yscale() const noexcept
    {
        return yscale;
    }

    constexpr const Color& get_color() const noexcept
    {
        return color;
    }

    constexpr float get_angle() const noexcept
    {
        return angle;
    }

    constexpr DrawArgument operator+(Point<std::int16_t> argpos) const noexcept
    {
        return {pos + argpos,
                center + argpos,
                stretch,
                xscale,
                yscale,
                color,
                angle};
    }

    constexpr DrawArgument operator+(float argopc) const noexcept
    {
        return {
            pos, center, stretch, xscale, yscale, color.a() * argopc, angle};
    }

    constexpr DrawArgument operator+(const DrawArgument& o) const noexcept
    {
        return {pos + o.pos,
                center + o.center,
                stretch + o.stretch,
                xscale * o.xscale,
                yscale * o.yscale,
                color * o.color,
                angle + o.angle};
    }

    constexpr DrawArgument operator-(const DrawArgument& o) const noexcept
    {
        return {pos - o.pos,
                center - o.center,
                stretch - o.stretch,
                xscale / o.xscale,
                yscale / o.yscale,
                color / o.color,
                angle - o.angle};
    }

    constexpr Rectangle<std::int16_t>
    get_rectangle(Point<std::int16_t> origin,
                  Point<std::int16_t> dimensions) const noexcept
    {
        std::int16_t w = stretch.x();
        if (w == 0) {
            w = dimensions.x();
        }

        std::int16_t h = stretch.y();
        if (h == 0) {
            h = dimensions.y();
        }

        Point<std::int16_t> rlt = pos - center - origin;
        std::int16_t rl = rlt.x();
        std::int16_t rr = rlt.x() + w;
        std::int16_t rt = rlt.y();
        std::int16_t rb = rlt.y() + h;
        std::int16_t cx = center.x();
        std::int16_t cy = center.y();

        return {cx + static_cast<std::int16_t>(xscale * rl),
                cx + static_cast<std::int16_t>(xscale * rr),
                cy + static_cast<std::int16_t>(yscale * rt),
                cy + static_cast<std::int16_t>(yscale * rb)};
    }

private:
    Color color;
    Point<std::int16_t> pos;
    Point<std::int16_t> center;
    Point<std::int16_t> stretch;
    float xscale;
    float yscale;
    float angle;
};
} // namespace jrc
