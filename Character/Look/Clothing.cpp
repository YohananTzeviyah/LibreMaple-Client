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
#include "Clothing.h"

#include "../../Data/EquipData.h"
#include "../../Data/WeaponData.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

#include <string>
#include <unordered_set>

namespace jrc
{
using namespace std::string_literals;

Clothing::Clothing(std::int32_t id, const BodyDrawinfo& drawinfo) : item_id(id)
{
    const EquipData& equipdata = EquipData::get(item_id);

    equip_slot = equipdata.get_eq_slot();
    if (equip_slot == Equipslot::WEAPON) {
        two_handed = WeaponData::get(item_id).is_two_handed();
    } else {
        two_handed = false;
    }

    constexpr std::size_t NON_WEAPON_TYPES = 15;
    constexpr std::size_t WEAPON_OFFSET = NON_WEAPON_TYPES + 15;
    constexpr std::size_t WEAPON_TYPES = 20;
    constexpr Layer layers[NON_WEAPON_TYPES] = {Layer::CAP,
                                                Layer::FACEACC,
                                                Layer::EYEACC,
                                                Layer::EARRINGS,
                                                Layer::TOP,
                                                Layer::MAIL,
                                                Layer::PANTS,
                                                Layer::SHOES,
                                                Layer::GLOVE,
                                                Layer::SHIELD,
                                                Layer::CAPE,
                                                Layer::RING,
                                                Layer::PENDANT,
                                                Layer::BELT,
                                                Layer::MEDAL};

    Layer chlayer;
    std::size_t index = (static_cast<std::size_t>(item_id) / 10000ul) - 100ul;
    if (index < NON_WEAPON_TYPES) {
        chlayer = layers[index];
    } else if (index >= WEAPON_OFFSET
               && index < WEAPON_OFFSET + WEAPON_TYPES) {
        chlayer = Layer::WEAPON;
    } else {
        chlayer = Layer::CAPE;
    }

    std::string str_id = std::to_string(item_id);
    str_id.insert(0, "0", 1);
    str_id += ".img";
    nl::node src
        = nl::nx::character[equipdata.get_item_data().get_category()][str_id];
    nl::node info = src["info"];

    vslot = info["vslot"].get_string();

    switch (std::int32_t standno = info["stand"]) {
    case 1:
        stand = Stance::STAND1;
        break;
    case 2:
        stand = Stance::STAND2;
        break;
    default:
        stand = two_handed ? Stance::STAND2 : Stance::STAND1;
        break;
    }

    switch (std::int32_t walkno = info["walk"]) {
    case 1:
        walk = Stance::WALK1;
        break;
    case 2:
        walk = Stance::WALK2;
        break;
    default:
        walk = two_handed ? Stance::WALK2 : Stance::WALK1;
        break;
    }

    for (auto iter : Stance::names) {
        Stance::Id stance = iter.first;
        const std::string& stancename = iter.second;

        nl::node stancenode = src[stancename];
        if (!stancenode) {
            continue;
        }

        for (std::uint8_t frame = 0; nl::node framenode = stancenode[frame];
             ++frame) {
            for (nl::node partnode : framenode) {
                std::string part = partnode.name();
                if (!partnode
                    || partnode.data_type() != nl::node::type::bitmap) {
                    continue;
                }

                Layer z = chlayer;
                std::string zs = partnode["z"];
                if (part == "mailArm") {
                    z = Layer::MAILARM;
                } else {
                    auto sublayer_iter = sublayernames.find(zs);
                    if (sublayer_iter != sublayernames.end()) {
                        z = sublayer_iter->second;
                    }
                }

                std::string parent;
                Point<std::int16_t> parentpos;
                for (const auto& mapnode : partnode["map"]) {
                    if (mapnode.data_type() == nl::node::type::vector) {
                        parent = mapnode.name();
                        parentpos = mapnode;
                    }
                }

                // nl::node mapnode = partnode["map"];
                Point<std::int16_t> shift;
                switch (equip_slot) {
                case Equipslot::FACEACC:
                    shift -= parentpos;
                    break;
                case Equipslot::SHOES:
                case Equipslot::GLOVES:
                case Equipslot::TOP:
                case Equipslot::PANTS:
                case Equipslot::CAPE:
                    shift = drawinfo.get_body_pos(stance, frame) - parentpos;
                    break;
                case Equipslot::CAP:
                case Equipslot::EARRINGS:
                case Equipslot::EYEACC:
                    shift = drawinfo.get_face_pos(stance, frame) - parentpos;
                    break;
                case Equipslot::SHIELD:
                case Equipslot::WEAPON:
                    if (parent == "handMove") {
                        shift += drawinfo.get_hand_pos(stance, frame);
                    } else if (parent == "hand") {
                        shift += drawinfo.get_arm_pos(stance, frame);
                    } else if (parent == "navel") {
                        shift += drawinfo.get_body_pos(stance, frame);
                    }
                    shift -= parentpos;
                    break;
                default:
                    break;
                }

                stances[stance][z]
                    .emplace(frame, partnode)
                    ->second.shift(shift);
            }
        }
    }

    static const std::unordered_set<std::int32_t> transparents = {1002186};
    transparent = transparents.count(item_id) > 0;
}

void Clothing::draw(Stance::Id stance,
                    Layer layer,
                    std::uint8_t frame,
                    const DrawArgument& args) const
{
    auto range = stances[stance][layer].equal_range(frame);
    for (auto iter = range.first; iter != range.second; ++iter) {
        iter->second.draw(args);
    }
}

bool Clothing::contains_layer(Stance::Id stance, Layer layer) const noexcept
{
    return !stances[stance][layer].empty();
}

bool Clothing::is_transparent() const noexcept
{
    return transparent;
}

bool Clothing::is_two_handed() const noexcept
{
    return two_handed;
}

std::int32_t Clothing::get_id() const noexcept
{
    return item_id;
}

Stance::Id Clothing::get_stand() const noexcept
{
    return stand;
}

Stance::Id Clothing::get_walk() const noexcept
{
    return walk;
}

Equipslot::Id Clothing::get_equip_slot() const noexcept
{
    return equip_slot;
}

std::string_view Clothing::get_vslot() const noexcept
{
    return vslot;
}

const std::unordered_map<std::string, Clothing::Layer> Clothing::sublayernames
    = {
        // WEAPON
        {"weaponOverHand"s, Layer::WEAPON_OVER_HAND},
        {"weaponOverGlove"s, Layer::WEAPON_OVER_GLOVE},
        {"weaponOverBody"s, Layer::WEAPON_OVER_BODY},
        {"weaponBelowArm"s, Layer::WEAPON_BELOW_ARM},
        {"weaponBelowBody"s, Layer::WEAPON_BELOW_BODY},
        {"backWeaponOverShield"s, Layer::BACKWEAPON},
        // SHIELD
        {"shieldOverHair"s, Layer::SHIELD_OVER_HAIR},
        {"shieldBelowBody"s, Layer::SHIELD_BELOW_BODY},
        {"backShield"s, Layer::BACKSHIELD},
        // GLOVE
        {"gloveWrist"s, Layer::WRIST},
        {"gloveOverHair"s, Layer::GLOVE_OVER_HAIR},
        {"gloveOverBody"s, Layer::GLOVE_OVER_BODY},
        {"gloveWristOverHair"s, Layer::WRIST_OVER_HAIR},
        {"gloveWristOverBody"s, Layer::WRIST_OVER_BODY},
        // CAP
        {"capOverHair"s, Layer::CAP_OVER_HAIR},
        {"capBelowBody"s, Layer::CAP_BELOW_BODY},
};
} // namespace jrc
