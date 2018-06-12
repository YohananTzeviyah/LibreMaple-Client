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
#include "DamageNumber.h"

#include "../../Constants.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

namespace jrc
{
DamageNumber::DamageNumber(Type t,
                           std::int32_t damage,
                           std::int16_t starty,
                           std::int16_t x) noexcept
{
    type = t;

    if (damage > 0) {
        miss = false;

        std::string number = std::to_string(damage);
        first_num = number[0];
        if (number.length() > 1) {
            rest_num = number.substr(1);
            multiple = true;
        } else {
            rest_num = "";
            multiple = false;
        }

        std::int16_t total = get_advance(first_num, true);
        for (std::size_t i = 0; i < rest_num.length(); ++i) {
            char c = rest_num[i];
            std::int16_t advance;
            if (i < rest_num.length() - 1) {
                char n = rest_num[i + 1];
                advance = (get_advance(c, false) + get_advance(n, false)) / 2;
            } else {
                advance = get_advance(c, false);
            }
            total += advance;
        }
        shift = total / 2;
    } else {
        shift = charsets[type][true].get_w('M') / static_cast<std::int16_t>(2);
        miss = true;
    }

    move_obj.set_x(x);
    move_obj.set_y(starty);
    move_obj.vspeed = -0.25;
    opacity.set(1.5f);
}

DamageNumber::DamageNumber() noexcept = default;

void DamageNumber::draw(double viewx, double viewy, float alpha) const
{
    Point<std::int16_t> absolute = move_obj.get_absolute(viewx, viewy, alpha);
    Point<std::int16_t> position = absolute - Point<std::int16_t>{0, shift};
    float interopc = opacity.get(alpha);

    if (miss) {
        charsets[type][true].draw('M', {position, interopc});
    } else {
        charsets[type][false].draw(first_num, {position, interopc});

        if (multiple) {
            std::int16_t first_advance = get_advance(first_num, true);
            position.shift_x(first_advance);

            for (std::size_t i = 0; i < rest_num.length(); ++i) {
                char c = rest_num[i];
                Point<std::int16_t> yshift = {0, i % 2 ? -2 : 2};
                charsets[type][true].draw(c, {position + yshift, interopc});

                std::int16_t advance;
                if (i < rest_num.length() - 1) {
                    char n = rest_num[i + 1];
                    std::int16_t c_advance = get_advance(c, false);
                    std::int16_t n_advance = get_advance(n, false);
                    advance = (c_advance + n_advance) >> 1;
                } else {
                    advance = get_advance(c, false);
                }

                position.shift_x(advance);
            }
        }
    }
}

std::int16_t DamageNumber::get_advance(char c, bool first) const
{
    static constexpr const std::size_t LENGTH = 10;
    static constexpr const std::array<std::int16_t, LENGTH> advances{
        24, 20, 22, 22, 24, 23, 24, 22, 24, 24};

    std::size_t index = static_cast<unsigned char>(c) - 48ull;
    if (index < LENGTH) {
        std::int16_t advance = advances[index];
        switch (type) {
        case CRITICAL:
            if (first) {
                advance += 8;
            } else {
                advance += 4;
            }
            break;
        default:
            if (first) {
                advance += 2;
            }
        }
        return advance;
    } else {
        return 0;
    }
}

void DamageNumber::set_x(std::int16_t head_x)
{
    move_obj.set_x(head_x);
}

bool DamageNumber::update()
{
    move_obj.move();

    static constexpr const float FADE_STEP =
        Constants::TIMESTEP * 1.0f / FADE_TIME;
    opacity -= FADE_STEP;
    return opacity.last() <= 0.0f;
}

std::int16_t DamageNumber::row_height(bool critical)
{
    return critical ? 36 : 30;
}

void DamageNumber::init()
{
    charsets[NORMAL].set(
        false, nl::nx::effect["BasicEff.img"]["NoRed1"], Charset::LEFT);
    charsets[NORMAL].set(
        true, nl::nx::effect["BasicEff.img"]["NoRed0"], Charset::LEFT);
    charsets[CRITICAL].set(
        false, nl::nx::effect["BasicEff.img"]["NoCri1"], Charset::LEFT);
    charsets[CRITICAL].set(
        true, nl::nx::effect["BasicEff.img"]["NoCri0"], Charset::LEFT);
    charsets[TO_PLAYER].set(
        false, nl::nx::effect["BasicEff.img"]["NoViolet1"], Charset::LEFT);
    charsets[TO_PLAYER].set(
        true, nl::nx::effect["BasicEff.img"]["NoViolet0"], Charset::LEFT);
}

std::array<BoolPair<Charset>, DamageNumber::NUM_TYPES> DamageNumber::charsets;
} // namespace jrc
