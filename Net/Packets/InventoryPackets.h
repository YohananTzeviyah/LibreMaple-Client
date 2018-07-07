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
#include "../OutPacket.h"

namespace jrc
{
//! Packet which requests that the inventory is sorted.
//! Opcode: GATHER_ITEMS(69)
class GatherItemsPacket : public OutPacket
{
public:
    GatherItemsPacket(InventoryType::Id type) : OutPacket(GATHER_ITEMS)
    {
        write_time();
        write_byte(type);
    }
};

//! Packet which requests that the inventory is sorted.
//! Opcode: SORT_ITEMS(70)
class SortItemsPacket : public OutPacket
{
public:
    SortItemsPacket(InventoryType::Id type) : OutPacket(SORT_ITEMS)
    {
        write_time();
        write_byte(type);
    }
};

//! Packet which requests that an item is moved.
//! Opcode: MOVE_ITEM(71)
class MoveItemPacket : public OutPacket
{
public:
    MoveItemPacket(InventoryType::Id type,
                   std::int16_t slot,
                   std::int16_t action,
                   std::int16_t qty)
        : OutPacket(MOVE_ITEM)
    {
        write_time();
        write_byte(type);
        write_short(slot);
        write_short(action);
        write_short(qty);
    }
};

//! Packet which requests that an item is equipped.
//! Opcode: MOVE_ITEM(71)
class EquipItemPacket : public MoveItemPacket
{
public:
    EquipItemPacket(std::int16_t src, Equipslot::Id dest)
        : MoveItemPacket(InventoryType::EQUIP, src, -dest, 1)
    {
    }
};

//! Packet which requests that an item is unequipped.
//! Opcode: MOVE_ITEM(71)
class UnequipItemPacket : public MoveItemPacket
{
public:
    UnequipItemPacket(std::int16_t src, std::int16_t dest)
        : MoveItemPacket(InventoryType::EQUIPPED, -src, dest, 1)
    {
    }
};

//! A packet which requests that an 'USE' item is used.
//! Opcode: USE_ITEM(72)
class UseItemPacket : public OutPacket
{
public:
    UseItemPacket(std::int16_t slot, std::int32_t itemid) : OutPacket(USE_ITEM)
    {
        write_time();
        write_short(slot);
        write_int(itemid);
    }
};

//! Requests using a scroll on an equip.
//! Opcode: SCROLL_EQUIP(86)
class ScrollEquipPacket : public OutPacket
{
public:
    enum Flag : std::uint8_t {
        NONE = 0x00,
        UNKNOWN = 0x01,
        WHITESCROLL = 0x02
    };

    ScrollEquipPacket(std::int16_t source,
                      Equipslot::Id target,
                      std::uint8_t flags)
        : OutPacket(SCROLL_EQUIP)
    {
        write_time();
        write_short(source);
        write_short(-target);
        write_short(flags);
    }

    ScrollEquipPacket(std::int16_t source, Equipslot::Id target)
        : ScrollEquipPacket(source, target, 0)
    {
    }
};
} // namespace jrc
