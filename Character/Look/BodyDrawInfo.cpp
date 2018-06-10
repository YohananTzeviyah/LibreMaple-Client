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
#include "BodyDrawInfo.h"

#include "Body.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

namespace jrc
{
void BodyDrawinfo::init()
{
    nl::node body_node = nl::nx::character["00002000.img"];
    nl::node head_node = nl::nx::character["00012000.img"];

    for (nl::node stance_node : body_node) {
        std::string st_str = stance_node.name();

        std::uint16_t attack_delay = 0;
        for (std::uint8_t frame = 0; nl::node frame_node = stance_node[frame];
             ++frame) {
            bool is_action =
                frame_node["action"].data_type() == nl::node::type::string;
            if (is_action) {
                BodyAction action = frame_node;
                body_actions[st_str][frame] = action;

                if (action.is_attack_frame()) {
                    attack_delays[st_str].push_back(attack_delay);
                }
                attack_delay += action.get_delay();
            } else {
                Stance::Id stance = Stance::by_string(st_str);
                std::int16_t delay = frame_node["delay"];
                if (delay <= 0) {
                    delay = 100;
                }
                stance_delays[stance][frame] =
                    static_cast<std::uint16_t>(delay);

                std::unordered_map<
                    Body::Layer,
                    std::unordered_map<std::string, Point<std::int16_t>>>
                    body_shift_map;
                for (auto part_node : frame_node) {
                    std::string part = part_node.name();
                    if (part != "delay" && part != "face") {
                        std::string z_str = part_node["z"];
                        Body::Layer z = Body::layer_by_name(z_str);

                        for (auto map_node : part_node["map"]) {
                            body_shift_map[z].emplace(map_node.name(),
                                                      map_node);
                        }
                    }
                }

                nl::node head_map = head_node[st_str][frame]["head"]["map"];
                for (auto map_node : head_map) {
                    body_shift_map[Body::HEAD].emplace(map_node.name(),
                                                       map_node);
                }

                body_positions[stance][frame] =
                    body_shift_map[Body::BODY]["navel"];
                arm_positions[stance][frame] =
                    body_shift_map.count(Body::ARM)
                        ? (body_shift_map[Body::ARM]["hand"] -
                           body_shift_map[Body::ARM]["navel"] +
                           body_shift_map[Body::BODY]["navel"])
                        : (body_shift_map[Body::ARM_OVER_HAIR]["hand"] -
                           body_shift_map[Body::ARM_OVER_HAIR]["navel"] +
                           body_shift_map[Body::BODY]["navel"]);
                hand_positions[stance][frame] =
                    body_shift_map[Body::HAND_BELOW_WEAPON]["handMove"];
                head_positions[stance][frame] =
                    body_shift_map[Body::BODY]["neck"] -
                    body_shift_map[Body::HEAD]["neck"];
                face_positions[stance][frame] =
                    body_shift_map[Body::BODY]["neck"] -
                    body_shift_map[Body::HEAD]["neck"] +
                    body_shift_map[Body::HEAD]["brow"];
                hair_positions[stance][frame] =
                    body_shift_map[Body::HEAD]["brow"] -
                    body_shift_map[Body::HEAD]["neck"] +
                    body_shift_map[Body::BODY]["neck"];
            }
        }
    }
}

Point<std::int16_t> BodyDrawinfo::get_body_pos(Stance::Id stance,
                                               std::uint8_t frame) const
{
    auto iter = body_positions[stance].find(frame);
    if (iter == body_positions[stance].end()) {
        return {};
    }

    return iter->second;
}

Point<std::int16_t> BodyDrawinfo::get_arm_pos(Stance::Id stance,
                                              std::uint8_t frame) const
{
    auto iter = arm_positions[stance].find(frame);
    if (iter == arm_positions[stance].end()) {
        return {};
    }

    return iter->second;
}

Point<std::int16_t> BodyDrawinfo::get_hand_pos(Stance::Id stance,
                                               std::uint8_t frame) const
{
    auto iter = hand_positions[stance].find(frame);
    if (iter == hand_positions[stance].end()) {
        return {};
    }

    return iter->second;
}

Point<std::int16_t> BodyDrawinfo::get_head_pos(Stance::Id stance,
                                               std::uint8_t frame) const
{
    auto iter = head_positions[stance].find(frame);
    if (iter == head_positions[stance].end()) {
        return {};
    }

    return iter->second;
}

Point<std::int16_t> BodyDrawinfo::get_hair_pos(Stance::Id stance,
                                               std::uint8_t frame) const
{
    auto iter = hair_positions[stance].find(frame);
    if (iter == hair_positions[stance].end()) {
        return {};
    }

    return iter->second;
}

Point<std::int16_t> BodyDrawinfo::get_face_pos(Stance::Id stance,
                                               std::uint8_t frame) const
{
    auto iter = face_positions[stance].find(frame);
    if (iter == face_positions[stance].end()) {
        return {};
    }

    return iter->second;
}

std::uint8_t BodyDrawinfo::next_frame(Stance::Id stance,
                                      std::uint8_t frame) const
{
    if (stance_delays[stance].count(frame + 1)) {
        return frame + 1;
    } else {
        return 0;
    }
}

std::uint16_t BodyDrawinfo::get_delay(Stance::Id stance,
                                      std::uint8_t frame) const
{
    auto iter = stance_delays[stance].find(frame);
    if (iter == stance_delays[stance].end()) {
        return 100u;
    }

    return iter->second;
}

std::uint16_t BodyDrawinfo::get_attack_delay(const std::string& action,
                                             std::size_t no) const
{
    auto action_iter = attack_delays.find(action);
    if (action_iter != attack_delays.end()) {
        if (no < action_iter->second.size()) {
            return action_iter->second[no];
        }
    }
    return 0;
}

std::uint8_t BodyDrawinfo::next_action_frame(const std::string& action,
                                             std::uint8_t frame) const
{
    auto action_iter = body_actions.find(action);
    if (action_iter != body_actions.end()) {
        if (action_iter->second.count(frame + 1)) {
            return frame + 1;
        }
    }
    return 0;
}

const BodyAction* BodyDrawinfo::get_action(const std::string& action,
                                           std::uint8_t frame) const
{
    auto action_iter = body_actions.find(action);
    if (action_iter != body_actions.end()) {
        auto frame_iter = action_iter->second.find(frame);
        if (frame_iter != action_iter->second.end()) {
            return &(frame_iter->second);
        }
    }
    return nullptr;
}
} // namespace jrc
