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
#include "Template/Singleton.h"

#include <chrono>
#include <cstdint>

namespace jrc
{
//! Small class for measuring elapsed time between game loops.
class Timer : public Singleton<Timer>
{
public:
    Timer()
    {
        start();
    }

    ~Timer() = default;

    //! Start the timer by setting the last measurement to now.
    void start()
    {
        point = clock::now();
    }

    //! Return time elapsed since the last measurement.
    std::int64_t stop()
    {
        clock::time_point last = point;
        point = clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            point - last);
        return duration.count();
    }

private:
    using clock = std::chrono::high_resolution_clock;

    clock::time_point point;
};
} // namespace jrc
