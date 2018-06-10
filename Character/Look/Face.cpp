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
#include "Face.h"

#include "../../Console.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

namespace jrc
{
Expression::Id Expression::byaction(std::size_t action)
{
    action -= 98;
    if (action < LENGTH) {
        return static_cast<Id>(action);
    }

    Console::get().print("Unhandled expression id: " + std::to_string(action));
    return DEFAULT;
}

const EnumMap<Expression::Id, std::string> Expression::names = {
    "default", "blink",      "hit",     "smile",   "troubled", "cry",
    "angry",   "bewildered", "stunned", "blaze",   "bowing",   "cheers",
    "chu",     "dam",        "despair", "glitter", "hot",      "hum",
    "love",    "oops",       "pain",    "shine",   "vomit",    "wink"};

Face::Face(std::int32_t face_id)
{
    const std::string face_id_str = std::to_string(face_id);
    nl::node face_node =
        nl::nx::character["Face"][str::concat("000", face_id_str, ".img")];

    for (auto iter : Expression::names) {
        Expression::Id exp = iter.first;
        if (exp == Expression::DEFAULT) {
            expressions[Expression::DEFAULT].emplace(0, face_node["default"]);
        } else {
            nl::node exp_node = face_node[iter.second];
            for (std::uint8_t frame = 0; nl::node framenode = exp_node[frame];
                 ++frame) {
                expressions[exp].emplace(frame, framenode);
            }
        }
    }

    name = nl::nx::string["Eqp.img"]["Eqp"]["Face"][face_id_str]["name"]
               .get_string();
}

void Face::draw(Expression::Id expression,
                std::uint8_t frame,
                const DrawArgument& args) const
{
    auto frameit = expressions[expression].find(frame);
    if (frameit != expressions[expression].end()) {
        frameit->second.texture.draw(args);
    }
}

std::uint8_t Face::next_frame(Expression::Id exp, std::uint8_t frame) const
{
    return expressions[exp].count(frame + 1) ? frame + 1 : 0;
}

std::int16_t Face::get_delay(Expression::Id exp, std::uint8_t frame) const
{
    auto delayit = expressions[exp].find(frame);
    return delayit != expressions[exp].end() ? delayit->second.delay : 100;
}

std::string_view Face::get_name() const noexcept
{
    return name;
}
} // namespace jrc
