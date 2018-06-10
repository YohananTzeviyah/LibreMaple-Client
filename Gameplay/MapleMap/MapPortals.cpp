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
#include "MapPortals.h"

#include "../../Constants.h"
#include "../../Util/Misc.h"
#include "nlnx/nx.hpp"

namespace jrc
{
MapPortals::MapPortals(nl::node src, std::int32_t map_id)
{
    for (const auto& sub : src) {
        auto portal_id =
            string_conversion::or_default<std::int8_t>(sub.name(), -1);
        if (portal_id < 0) {
            continue;
        }

        Portal::Type type = Portal::typebyid(sub["pt"]);
        std::string name = sub["pn"];
        std::string target_name = sub["tn"];
        std::int32_t target_id = sub["tm"];
        Point<std::int16_t> position = {sub["x"], sub["y"]};

        const Animation* animation = &animations[type];
        bool intramap = target_id == map_id;

        portal_ids_by_name.emplace(std::string{name}, portal_id);
        portals_by_id.emplace(std::piecewise_construct,
                              std::forward_as_tuple(portal_id),
                              std::forward_as_tuple(animation,
                                                    type,
                                                    std::move(name),
                                                    intramap,
                                                    position,
                                                    target_id,
                                                    std::move(target_name)));
    }

    cooldown = WARP_CD;
}

MapPortals::MapPortals()
{
    cooldown = WARP_CD;
}

void MapPortals::update(Point<std::int16_t> playerpos)
{
    animations[Portal::REGULAR].update(Constants::TIMESTEP);
    animations[Portal::HIDDEN].update(Constants::TIMESTEP);

    for (auto& iter : portals_by_id) {
        Portal& portal = iter.second;
        switch (portal.get_type()) {
        case Portal::HIDDEN:
        case Portal::TOUCH:
            portal.update(playerpos);
            break;
        default:
            break;
        }
    }

    if (cooldown > 0) {
        --cooldown;
    }
}

void MapPortals::draw(Point<std::int16_t> viewpos, float inter) const
{
    for (auto& ptit : portals_by_id) {
        ptit.second.draw(viewpos, inter);
    }
}

Point<std::int16_t> MapPortals::get_portal_by_id(std::uint8_t portal_id) const
{
    auto iter = portals_by_id.find(portal_id);
    if (iter != portals_by_id.end()) {
        constexpr Point<std::int16_t> ABOVE(0, 30);
        return iter->second.get_position() - ABOVE;
    } else {
        return {};
    }
}

Point<std::int16_t>
MapPortals::get_portal_by_name(const std::string& portal_name) const
{
    auto iter = portal_ids_by_name.find(portal_name);
    if (iter != portal_ids_by_name.end()) {
        return get_portal_by_id(iter->second);
    } else {
        return {};
    }
}

Portal::WarpInfo MapPortals::find_warp_at(Point<std::int16_t> playerpos)
{
    if (cooldown == 0) {
        cooldown = WARP_CD;

        for (const auto& iter : portals_by_id) {
            const Portal& portal = iter.second;
            if (portal.bounds().contains(playerpos)) {
                return portal.getwarpinfo();
            }
        }
    }

    return {};
}

void MapPortals::init()
{
    nl::node src = nl::nx::map["MapHelper.img"]["portal"]["game"];

    animations[Portal::HIDDEN] = src["ph"]["default"]["portalContinue"];
    animations[Portal::REGULAR] = src["pv"];
}

std::unordered_map<Portal::Type, Animation> MapPortals::animations;
} // namespace jrc
