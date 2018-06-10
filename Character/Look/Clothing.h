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
#include "../../Graphics/Texture.h"
#include "../../Template/EnumMap.h"
#include "BodyDrawInfo.h"
#include "EquipSlot.h"

#include <unordered_map>

namespace jrc
{
class Clothing
{
public:
    enum Layer {
        CAPE,
        SHOES,
        PANTS,
        TOP,
        MAIL,
        MAILARM,
        EARRINGS,
        FACEACC,
        EYEACC,
        PENDANT,
        BELT,
        MEDAL,
        RING,
        CAP,
        CAP_BELOW_BODY,
        CAP_OVER_HAIR,
        GLOVE,
        WRIST,
        GLOVE_OVER_HAIR,
        WRIST_OVER_HAIR,
        GLOVE_OVER_BODY,
        WRIST_OVER_BODY,
        SHIELD,
        BACKSHIELD,
        SHIELD_BELOW_BODY,
        SHIELD_OVER_HAIR,
        WEAPON,
        BACKWEAPON,
        WEAPON_BELOW_ARM,
        WEAPON_BELOW_BODY,
        WEAPON_OVER_HAND,
        WEAPON_OVER_BODY,
        WEAPON_OVER_GLOVE,
        NUM_LAYERS
    };

    //! Construct a new equip.
    Clothing(std::int32_t itemid, const BodyDrawinfo& drawinfo);

    //! Draw the equip.
    void draw(Stance::Id stance,
              Layer layer,
              std::uint8_t frame,
              const DrawArgument& args) const;
    //! Check if a part of the equip lies on the specified layer while in the
    //! specified stance.
    [[nodiscard]] bool contains_layer(Stance::Id stance, Layer layer) const
        noexcept;

    //! Return wether the equip is invisble.
    [[nodiscard]] bool is_transparent() const noexcept;
    //! Return wether this equip uses two_handed stances.
    [[nodiscard]] bool is_two_handed() const noexcept;
    //! Return the item id.
    [[nodiscard]] std::int32_t get_id() const noexcept;
    //! Return the equip slot for this cloth.
    [[nodiscard]] Equipslot::Id get_equip_slot() const noexcept;
    //! Return the standing stance to use while equipped.
    [[nodiscard]] Stance::Id get_stand() const noexcept;
    //! Return the walking stance to use while equipped.
    [[nodiscard]] Stance::Id get_walk() const noexcept;
    //! Return the vslot, used to distinguish some layering types.
    [[nodiscard]] std::string_view get_vslot() const noexcept;

private:
    EnumMap<Stance::Id,
            EnumMap<Layer,
                    std::unordered_multimap<std::uint8_t, Texture>,
                    NUM_LAYERS>>
        stances;
    std::int32_t item_id;
    Equipslot::Id equip_slot;
    Stance::Id walk;
    Stance::Id stand;
    std::string vslot;
    bool two_handed;
    bool transparent;

    static const std::unordered_map<std::string, Layer> sublayernames;
};
} // namespace jrc
