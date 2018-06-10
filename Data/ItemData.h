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
#include "../Graphics/Texture.h"
#include "../Template/BoolPair.h"
#include "../Template/Cache.h"

namespace jrc
{
//! Class that represents an item loaded from the game's files. Contains all
//! shared data between concrete items.
class ItemData : public Cache<ItemData>
{
public:
    //! Returns wether the item was loaded correctly.
    [[nodiscard]] bool is_valid() const noexcept;
    //! Returns wether the item was loaded correctly.
    explicit operator bool() const noexcept;

    //! Returns the item id.
    [[nodiscard]] std::int32_t get_id() const noexcept;
    //! Returns the item price.
    [[nodiscard]] std::int32_t get_price() const noexcept;
    //! Returns the item's name loaded from the String.nx file.
    [[nodiscard]] std::string_view get_name() const noexcept;
    //! Returns the item's description loaded from the String.nx file.
    [[nodiscard]] std::string_view get_desc() const noexcept;
    //! Return the item category (also the node name).
    [[nodiscard]] std::string_view get_category() const noexcept;
    //! Returns one of the item's icons. For each item there is a 'raw' icon
    //! and an icon with a drop shadow.
    [[nodiscard]] const Texture& get_icon(bool raw) const noexcept;

private:
    //! Allow the cache to use the constructor.
    friend Cache<ItemData>;
    //! Creates an item from the game's Item.nx with the specified id.
    ItemData(std::int32_t itemid);

    [[nodiscard]] static std::string_view
    get_equip_category_str(std::int32_t id) noexcept;

    BoolPair<Texture> icons;
    std::int32_t item_id;
    std::int32_t price;
    std::string name;
    std::string desc;
    std::string category;

    bool valid;
};
} // namespace jrc
