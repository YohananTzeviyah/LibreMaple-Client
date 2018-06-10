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
//#pragma once
#include "Npc.h"

#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

namespace jrc
{
Npc::Npc(std::int32_t npc_id_,
         std::int32_t o,
         bool fl,
         std::uint16_t f,
         bool cnt,
         Point<std::int16_t> position)
    : MapObject{o}
{
    std::string str_id = std::to_string(npc_id_);
    str_id.insert(0, 7 - str_id.size(), '0');
    str_id.append(".img");

    nl::node src = nl::nx::npc[str_id];
    nl::node strsrc = nl::nx::string["Npc.img"][std::to_string(npc_id_)];

    std::string link = src["info"]["link"];
    if (link.size() > 0) {
        link.append(".img");
        src = nl::nx::npc[link];
    }

    nl::node info = src["info"];

    hide_name = info["hideName"].get_bool();
    mouse_only = info["talkMouseOnly"].get_bool();
    scripted = info["script"].size() > 0 || info["shop"].get_bool();

    for (const auto& npc_node : src) {
        const std::string state = npc_node.name();
        if (state != "info") {
            animations[state] = npc_node;
            states.push_back(state);
        }

        for (auto speaknode : npc_node["speak"]) {
            lines[state].push_back(strsrc[speaknode.get_string()]);
        }
    }

    name = strsrc["name"].get_string();
    func = strsrc["func"].get_string();

    name_label = {Text::A13B,
                  Text::CENTER,
                  Text::YELLOW,
                  Text::NAMETAG,
                  std::string{name}};
    func_label = {Text::A13B,
                  Text::CENTER,
                  Text::YELLOW,
                  Text::NAMETAG,
                  std::string{func}};

    npc_id = npc_id_;
    flip = !fl;
    control = cnt;
    stance = "stand";

    phobj.fhid = f;
    set_position(position);
}

void Npc::draw(double viewx, double viewy, float alpha) const
{
    Point<std::int16_t> absp = phobj.get_absolute(viewx, viewy, alpha);
    if (animations.count(stance)) {
        animations.at(stance).draw(DrawArgument(absp, flip), alpha);
    }

    if (!hide_name) {
        name_label.draw(absp);
        func_label.draw(absp + Point<std::int16_t>(0, 18));
    }
}

std::int8_t Npc::update(const Physics& physics)
{
    if (!active) {
        return phobj.fhlayer;
    }

    physics.move_object(phobj);

    if (animations.count(stance)) {
        bool aniend = animations.at(stance).update();
        if (aniend && states.size() > 0) {
            std::size_t next_stance = random.next_int(states.size());
            std::string new_stance = states[next_stance];
            set_stance(new_stance);
        }
    }

    return phobj.fhlayer;
}

void Npc::set_stance(std::string_view st) noexcept
{
    if (stance != st) {
        stance = std::string{st};

        auto iter = animations.find(stance);
        if (iter == animations.end()) {
            return;
        }

        iter->second.reset();
    }
}

bool Npc::is_scripted() const noexcept
{
    return scripted;
}

bool Npc::in_range(Point<std::int16_t> cursor_pos,
                   Point<std::int16_t> view_pos) const noexcept
{
    if (!active) {
        return false;
    }

    Point<std::int16_t> absp = get_position() + view_pos;
    Point<std::int16_t> dim = animations.count(stance)
                                  ? animations.at(stance).get_dimensions()
                                  : Point<std::int16_t>();

    return Rectangle<std::int16_t>(absp.x() - dim.x() / 2,
                                   absp.x() + dim.x() / 2,
                                   absp.y() - dim.y(),
                                   absp.y())
        .contains(cursor_pos);
}
} // namespace jrc
