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
#include "Body.h"

#include "../../Console.h"
#include "../../Util/Misc.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

namespace jrc
{
Body::Body(std::int32_t skin, const BodyDrawinfo& draw_info)
{
    std::string str_img =
        str::concat("000020", string_format::extend_id(skin, 2), ".img");
    nl::node bodynode = nl::nx::character[str_img];
    str_img[3] = '1';
    nl::node head_node = nl::nx::character[str_img];

    for (auto iter : Stance::names) {
        Stance::Id stance = iter.first;
        const std::string& stance_name = iter.second;

        nl::node stance_node = bodynode[stance_name];
        if (!stance_node) {
            continue;
        }

        for (std::uint8_t frame = 0; nl::node frame_node = stance_node[frame];
             ++frame) {
            for (nl::node part_node : frame_node) {
                std::string part = part_node.name();
                if (part != "delay" && part != "face") {
                    std::string z = part_node["z"];
                    Layer layer = layer_by_name(z);
                    if (layer == Layer::NONE) {
                        continue;
                    }

                    Point<std::int16_t> shift;
                    switch (layer) {
                    case HAND_BELOW_WEAPON:
                        shift = draw_info.get_hand_pos(stance, frame);
                        shift -= part_node["map"]["handMove"];
                        break;
                    default:
                        shift = draw_info.get_body_pos(stance, frame);
                        shift -= part_node["map"]["navel"];
                        break;
                    }

                    stances[stance][layer]
                        .emplace(frame, part_node)
                        .first->second.shift(shift);
                }
            }

            if (nl::node head_s_f_node =
                    head_node[stance_name][frame]["head"]) {
                Point<std::int16_t> shift =
                    draw_info.get_head_pos(stance, frame);

                stances[stance][Layer::HEAD]
                    .emplace(frame, head_s_f_node)
                    .first->second.shift(shift);
            }
        }
    }

    static constexpr const std::size_t NUM_SKIN_TYPES = 12ull;
    static constexpr const char* const skin_types[NUM_SKIN_TYPES] = {"Light",
                                                                     "Tan",
                                                                     "Dark",
                                                                     "Pale",
                                                                     "Blue",
                                                                     "Green",
                                                                     "",
                                                                     "",
                                                                     "",
                                                                     "Grey",
                                                                     "Pink",
                                                                     "Red"};
    auto index = static_cast<std::size_t>(skin);
    name = index < NUM_SKIN_TYPES ? skin_types[index] : "";
}

void Body::draw(Stance::Id stance,
                Layer layer,
                std::uint8_t frame,
                const DrawArgument& args) const
{
    auto frame_it = stances[stance][layer].find(frame);
    if (frame_it == stances[stance][layer].end()) {
        return;
    }

    frame_it->second.draw(args);
}

std::string_view Body::get_name() const noexcept
{
    return name;
}

Body::Layer Body::layer_by_name(const std::string& name)
{
    auto layer_iter = layers_by_name.find(name);
    if (layer_iter == layers_by_name.end()) {
        Console::get().print(
            str::concat("Warning: Unhandled body layer (", name, ')'));
        return Layer::NONE;
    }
    return layer_iter->second;
}

const std::unordered_map<std::string, Body::Layer> Body::layers_by_name = {
    {"body", Body::BODY},
    {"backBody", Body::BODY},
    {"arm", Body::ARM},
    {"armBelowHead", Body::ARM_BELOW_HEAD},
    {"armBelowHeadOverMailChest", Body::ARM_BELOW_HEAD_OVER_MAIL},
    {"armOverHair", Body::ARM_OVER_HAIR},
    {"armOverHairBelowWeapon", Body::ARM_OVER_HAIR_BELOW_WEAPON},
    {"handBelowWeapon", Body::HAND_BELOW_WEAPON},
    {"handOverHair", Body::HAND_OVER_HAIR},
    {"handOverWeapon", Body::HAND_OVER_WEAPON},
    {"head", Body::HEAD}};
} // namespace jrc
