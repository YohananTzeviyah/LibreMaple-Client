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
#include "../../IO/Components/Charset.h"
#include "../../Template/BoolPair.h"
#include "../../Template/Interpolated.h"
#include "../../Template/Point.h"
#include "../Physics/PhysicsObject.h"

#include <array>

namespace jrc
{
class DamageNumber
{
public:
    static constexpr const std::size_t NUM_TYPES = 3;
    enum Type { NORMAL, CRITICAL, TO_PLAYER };

    DamageNumber(Type type,
                 std::int32_t damage,
                 std::int16_t starty,
                 std::int16_t x = 0) noexcept;
    DamageNumber() noexcept;

    void draw(double viewx, double viewy, float alpha) const;
    void set_x(std::int16_t head_x);
    bool update();

    static std::int16_t row_height(bool critical);
    static void init();

private:
    std::int16_t get_advance(char c, bool first) const;

    static constexpr const std::uint16_t FADE_TIME = 500;

    Type type;
    bool miss;
    bool multiple;
    std::int8_t first_num;
    std::string rest_num;
    std::int16_t shift;
    MovingObject move_obj;
    Linear<float> opacity;

    static std::array<BoolPair<Charset>, NUM_TYPES> charsets;
};
} // namespace jrc
