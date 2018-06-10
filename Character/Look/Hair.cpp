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
#include "Hair.h"

#include "../../Console.h"
#include "../../Util/Misc.h"
#include "nlnx/nx.hpp"

namespace jrc
{
Hair::Hair(std::int32_t hairid, const BodyDrawinfo& drawinfo)
{
    nl::node hairnode =
        nl::nx::character["Hair"]
                         [str::concat("000", std::to_string(hairid), ".img")];

    for (const auto& s : Stance::names) {
        auto stance = s.first;
        auto stance_name = s.second;

        nl::node stancenode = hairnode[stance_name];
        if (!stancenode) {
            continue;
        }

        for (std::uint8_t frame = 0; nl::node framenode = stancenode[frame];
             ++frame) {
            for (nl::node layernode : framenode) {
                std::string layer_name = layernode.name();
                auto layer_iter = layers_by_name.find(layer_name);
                if (layer_iter == layers_by_name.end()) {
                    Console::get().print(str::concat(
                        "Warning: Unhandled hair layer (", layer_name, ')'));
                    continue;
                }
                Layer layer = layer_iter->second;

                Point<std::int16_t> brow = layernode["map"]["brow"];
                Point<std::int16_t> shift =
                    drawinfo.get_hair_pos(stance, frame) - brow;

                stances[stance][layer]
                    .emplace(frame, layernode)
                    .first->second.shift(shift);
            }
        }
    }

    name = nl::nx::string["Eqp.img"]["Eqp"]["Hair"][std::to_string(hairid)]
                         ["name"]
                             .get_string();

    constexpr std::size_t NUM_COLORS = 8;
    constexpr char const* haircolors[NUM_COLORS] = {"Black",
                                                    "Red",
                                                    "Orange",
                                                    "Blonde",
                                                    "Green",
                                                    "Blue",
                                                    "Violet",
                                                    "Brown"};
    std::size_t index = static_cast<std::size_t>(hairid) % 10ull;
    color = index < NUM_COLORS ? haircolors[index] : "";
}

void Hair::draw(Stance::Id stance,
                Layer layer,
                std::uint8_t frame,
                const DrawArgument& args) const
{
    auto frameit = stances[stance][layer].find(frame);
    if (frameit == stances[stance][layer].end()) {
        return;
    }

    frameit->second.draw(args);
}

std::string_view Hair::get_name() const noexcept
{
    return name;
}

std::string_view Hair::get_color() const noexcept
{
    return color;
}

const std::unordered_map<std::string, Hair::Layer> Hair::layers_by_name = {
    {"hair", Hair::DEFAULT},
    {"hairBelowBody", Hair::BELOWBODY},
    {"hairOverHead", Hair::OVERHEAD},
    {"hairShade", Hair::SHADE},
    {"backHair", Hair::BACK},
    {"backHairBelowCap", Hair::BELOWCAP}};
} // namespace jrc
