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
#include "../../Graphics/Animation.h"
#include "../../Graphics/Text.h"
#include "../../Util/Randomizer.h"
#include "../Physics/PhysicsObject.h"
#include "MapObject.h"

#include <string_view>
#include <unordered_map>

namespace jrc
{
//! Represents an NPC on the current map.
//! Implements the `Mapobject` interface to be used in a `Mapobjects` template.
class Npc : public MapObject
{
public:
    //! Constructs an NPC by combining data from game files with
    //! data sent by the server.
    Npc(std::int32_t npc_id_,
        std::int32_t oid,
        bool mirrored,
        std::uint16_t fhid,
        bool control,
        Point<std::int16_t> position);

    //! Draws the current animation and name/function tags.
    void draw(double viewx, double viewy, float alpha) const override;
    //! Updates the current animation and physics.
    std::int8_t update(const Physics& physics) override;

    //! Changes stance and resets animation.
    void set_stance(std::string_view stance) noexcept;

    //! Check whether this is a server-sided NPC.
    [[nodiscard]] bool is_scripted() const noexcept;
    //! Check if the NPC is in range of the cursor.
    bool in_range(Point<std::int16_t> cursor_pos,
                  Point<std::int16_t> view_pos) const noexcept;

private:
    std::unordered_map<std::string, Animation> animations;
    std::unordered_map<std::string, std::vector<std::string>> lines;
    std::vector<std::string> states;
    std::string name;
    std::string func;
    bool hide_name;
    bool scripted;
    bool mouse_only;

    std::int32_t npc_id;
    bool flip;
    std::string stance;
    bool control;

    Text name_label;
    Text func_label;
};
} // namespace jrc
