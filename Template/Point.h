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
#include "../Util/Misc.h"
#include "nlnx/node.hpp"

#include <cmath>
#include <limits>
#include <string>

namespace jrc
{
//! Represents a point in two-dimensional space, or possibly a displacement,
//! vector, etc.
template<class T>
class Point
{
public:
    //! Construct a point from a vector property.
    Point(const nl::node& src) noexcept
    {
        a = static_cast<T>(src.x());
        b = static_cast<T>(src.y());
    }

    //! Construct a point from the specified coordinates.
    constexpr Point(T first, T second) noexcept : a(first), b(second)
    {
    }

    //! Construct a point at the origin.
    constexpr Point() noexcept : Point(0, 0)
    {
    }

    //! Return the x-coordinate.
    constexpr T x() const noexcept
    {
        return a;
    }

    //! Return the y-coordinate.
    constexpr T y() const noexcept
    {
        return b;
    }

    //! Return the quadrance, i.e. the squared norm.
    //!
    //! Satisifies `static_cast<T>(std::sqrt(p.quadrance())) == p.norm()`.
    constexpr T quadrance() const noexcept
    {
        return a * a + b * b;
    }

    //! Return the Euclidian norm.
    constexpr T norm() const noexcept
    {
        return static_cast<T>(std::sqrt(quadrance()));
    }

    //! Check whether the x- and y-coordinates are equal.
    constexpr bool straight() const noexcept
    {
        return a == b;
    }

    //! Return a string representation of the point.
    std::string to_string() const noexcept
    {
        return str::concat(
            '(', std::to_string(a), ',', std::to_string(b), ')');
    }

    //! Return the displacement from another point.
    constexpr T disp(Point<T> v) const noexcept
    {
        return Point<T>{a - v.a, b - v.b}.norm();
    }

    //! Set the x-coordinate.
    constexpr void set_x(T v) noexcept
    {
        a = v;
    }

    //! Set the y-coordinate.
    constexpr void set_y(T v) noexcept
    {
        b = v;
    }

    //! Shift the x-coordinate by the specified amount.
    constexpr void shift_x(T v) noexcept
    {
        a += v;
    }

    //! Shift the y-coordinate by the specified amount.
    constexpr void shift_y(T v) noexcept
    {
        b += v;
    }

    //! Shift the coordinates by the specified amounts.
    constexpr void shift(T x, T y) noexcept
    {
        a += x;
        b += y;
    }

    //! Shift the this point by the amounts defined by another point.
    //! Equivalent to `operator +=`.
    constexpr void shift(Point<T> v) noexcept
    {
        a += v.a;
        b += v.b;
    }

    //! Check whether this point is equivalent to the other specified point.
    constexpr bool operator==(const Point<T>& v) const noexcept
    {
        return a == v.a && b == v.b;
    }

    //! Check whether this point is not equivalent to the other specified
    //! point.
    constexpr bool operator!=(const Point<T>& v) const noexcept
    {
        return !(*this == v);
    }

    //! Shift this point by the amounts defined by another point.
    constexpr void operator+=(Point<T> v) noexcept
    {
        a += v.a;
        b += v.b;
    }

    //! Shift this point by the opposite of the amounts defined by another
    //! point.
    constexpr void operator-=(Point<T> v) noexcept
    {
        a -= v.a;
        b -= v.b;
    }

    //! Return a point whose coordinates are the negation of this point's
    //! coordinates.
    constexpr Point<T> operator-() const noexcept
    {
        return {-a, -b};
    }

    //! Return a point whose coordinates are this point's coordinates, after
    //! having added the specified amount to both.
    constexpr Point<T> operator+(T v) const noexcept
    {
        return {a + v, b + v};
    }

    //! Return a point whose coordinates are this point's coordinates, after
    //! having subtracted the specified amount from both.
    constexpr Point<T> operator-(T v) const noexcept
    {
        return {a - v, b - v};
    }

    //! Return a point whose coordinates are this point's coordinates, after
    //! having multiplied both by the specified amount.
    constexpr Point<T> operator*(T v) const noexcept
    {
        return {a * v, b * v};
    }

    //! Return a point whose coordinates are this point's coordinates, after
    //! having divided both by the specified amount.
    constexpr Point<T> operator/(T v) const noexcept
    {
        return {a / v, b / v};
    }

    //! Return a point whose coordinates are the sum of this and another
    //! point's coordinates.
    constexpr Point<T> operator+(Point<T> v) const noexcept
    {
        return {a + v.a, b + v.b};
    }

    //! Return a point whose coordinates are the difference of this and
    //! another point's coordinates.
    constexpr Point<T> operator-(Point<T> v) const noexcept
    {
        return {a - v.a, b - v.b};
    }

    //! Return the Euclidian inner product of this point and the specified
    //! other point.
    constexpr T operator*(Point<T> v) const noexcept
    {
        return a * v.a + b * v.b;
    }

    //! Return a point representing the Hadamard product of this point and the
    //! specified other point.
    constexpr Point<T> hadamard(Point<T> v) const noexcept
    {
        return {a * v.a, b * v.b};
    }

    //! Return the Euclidian inner product of this point and the specified
    //! other point's reciprocal (interpreting reciprocal as reciprocating each
    //! coordinate individually, and for non-floating-point types, division by
    //! a coordinate of `0` is treated as division by `1`).
    constexpr T operator/(Point<T> v) const noexcept
    {
        if constexpr (std::numeric_limits<T>::is_iec559) {
            return a / v.a + b / v.b;
        } else {
            return a / (v.a == 0 ? 1 : v.a) + b / (v.b == 0 ? 1 : v.b);
        }
    }

    //! Return a point representing the Hadamard division of this point by the
    //! specified other point. For non-floating-point types, division by a
    //! coordinate of `0` is treated as division by `1`.
    constexpr Point<T> hadamard_div(Point<T> v) const noexcept
    {
        if constexpr (std::numeric_limits<T>::is_iec559) {
            return {a / v.a, b / v.b};
        } else {
            return {a / (v.a == 0 ? 1 : v.a), b / (v.b == 0 ? 1 : v.b)};
        }
    }

private:
    T a;
    T b;
};
} // namespace jrc
