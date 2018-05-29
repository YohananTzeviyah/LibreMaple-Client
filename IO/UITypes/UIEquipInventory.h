//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "../../Template/EnumMap.h"
#include "../Components/EquipTooltip.h"
#include "../Components/Icon.h"
#include "../UIDragElement.h"

namespace jrc
{
// The Equip inventory.
class UIEquipInventory : public UIDragElement<PosEQINV>
{
public:
    static constexpr Type TYPE = EQUIPINVENTORY;
    static constexpr bool FOCUSED = false;
    static constexpr bool TOGGLED = true;

    UIEquipInventory(const Inventory& inventory);

    void draw(float inter) const override;

    void toggle_active() override;
    void doubleclick(Point<std::int16_t> position) override;
    void send_icon(const Icon& icon, Point<std::int16_t> position) override;
    Cursor::State send_cursor(bool pressed, Point<std::int16_t> position) override;

    void modify(std::int16_t pos, std::int8_t mode, std::int16_t arg);

protected:
    Button::State button_pressed(std::uint16_t buttonid) override;

private:
    void show_equip(Equipslot::Id slot);
    void clear_tooltip();
    void load_icons();
    void update_slot(Equipslot::Id slot);
    Equipslot::Id slot_by_position(Point<std::int16_t> position) const;

    class EquipIcon : public Icon::Type
    {
    public:
        EquipIcon(std::int16_t source);

        void drop_on_stage() const override;
        void drop_on_equips(Equipslot::Id) const override
        {
        }
        void drop_on_items(InventoryType::Id tab,
                           Equipslot::Id eqslot,
                           std::int16_t slot,
                           bool equip) const override;

    private:
        std::int16_t source;
    };

    enum Buttons { BT_TOGGLEPETS };

    const Inventory& inventory;

    std::vector<Texture> textures_pet;
    EnumMap<Equipslot::Id, Point<std::int16_t>> iconpositions;
    EnumMap<Equipslot::Id, std::unique_ptr<Icon>> icons;

    bool showpetequips;
};
} // namespace jrc
