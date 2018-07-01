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
#include "pcg_random.hpp"

#include <cstdint>
#include <random>

namespace jrc
{
//! Can be used to generate random numbers.
class Randomizer
{
public:
    static bool next_bool() noexcept
    {
        return next_int(2) == 1;
    }

    static bool below(float percent) noexcept
    {
        return next_real(1.0f) < percent;
    }

    static bool above(float percent) noexcept
    {
        return next_real(1.0f) > percent;
    }

    template<class T>
    static T next_real(T to) noexcept
    {
        return next_real<T>(static_cast<T>(0), to);
    }

    template<class T>
    static T next_real(T from, T to) noexcept
    {
        if (from >= to) {
            return from;
        }

        std::uniform_real_distribution<T> range{from, to};
        return range(state.rng);
    }

    template<class T>
    static T next_int(T to) noexcept
    {
        return next_int<T>(static_cast<T>(0), to);
    }

    template<class T>
    static T next_int(T from, T to) noexcept
    {
        if (from >= to) {
            return from;
        }

        std::uniform_int_distribution<T> range{from, to - 1};
        return range(state.rng);
    }

    template<class E>
    static E next_enum(E to = E::LENGTH) noexcept
    {
        return next_enum(E(), to);
    }

    template<class E>
    static E next_enum(E from, E to) noexcept
    {
        return static_cast<E>(next_int<std::underlying_type_t<E>>(from, to));
    }

private:
    struct RandomizerState {
        RandomizerState()
            : rng{pcg_extras::seed_seq_from<std::random_device>{}}
        {
        }
        RandomizerState(const RandomizerState&) = delete;
        RandomizerState(RandomizerState&&) = delete;

        ~RandomizerState() noexcept = default;

        RandomizerState& operator=(const RandomizerState&) = delete;
        RandomizerState& operator=(RandomizerState&&) = delete;

        pcg64 rng;
    };

    inline static thread_local RandomizerState state;
};
} // namespace jrc
