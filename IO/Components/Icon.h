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
#include "../../Character/Inventory/Inventory.h"
#include "../../Graphics/Texture.h"
#include "../KeyAction.h"

#include <memory>

namespace jrc
{
class Icon
{
public:
    class Type
    {
    public:
        virtual ~Type() = default;

        virtual void drop_on_stage() const = 0;
        virtual void drop_on_equips(Equipslot::Id eqslot) const = 0;
        virtual void drop_on_items(InventoryType::Id tab,
                                   Equipslot::Id eqslot,
                                   std::int16_t slot,
                                   bool equip) const = 0;
        virtual KeyAction::Id get_action_id() const noexcept
        {
            return KeyAction::NO_ACTION;
        }
    };

    class NullType : public Type
    {
        void drop_on_stage() const override
        {
        }
        void drop_on_equips(Equipslot::Id) const override
        {
        }
        void drop_on_items(InventoryType::Id,
                           Equipslot::Id,
                           std::int16_t,
                           bool) const override
        {
        }
    };

    Icon(std::unique_ptr<Type> type, Texture texture, std::int16_t count);
    Icon();

    void draw(Point<std::int16_t> position) const;
    void dragdraw(Point<std::int16_t> cursor_pos) const;
    void drop_on_stage() const;
    void drop_on_equips(Equipslot::Id eqslot) const;
    void drop_on_items(InventoryType::Id tab,
                       Equipslot::Id eqslot,
                       std::int16_t slot,
                       bool equip) const;
    KeyAction::Id get_action_id() const noexcept;
    void start_drag(Point<std::int16_t> offset);
    void reset();
    void set_count(std::int16_t count);
    std::int16_t get_count() const;

private:
    std::unique_ptr<Type> type;
    Texture texture;
    bool showcount;
    std::int16_t count;

    bool dragged;
    Point<std::int16_t> cursoroffset;
};
} // namespace jrc
