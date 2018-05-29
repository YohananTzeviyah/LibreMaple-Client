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
#include "../../Graphics/Text.h"
#include "../Components/Icon.h"
#include "../Components/Slider.h"
#include "../UIDragElement.h"

namespace jrc
{
// The Item inventory.
class UIItemInventory : public UIDragElement<PosINV>
{
public:
    static constexpr Type TYPE = ITEMINVENTORY;
    static constexpr bool FOCUSED = false;
    static constexpr bool TOGGLED = true;

    UIItemInventory(const Inventory& inventory);

    void draw(float inter) const override;
    void update() override;

    void doubleclick(Point<std::int16_t> position) override;
    void send_icon(const Icon& icon, Point<std::int16_t> position) override;
    void toggle_active() override;
    bool remove_cursor(bool clicked, Point<std::int16_t> cursorpos) override;
    Cursor::State send_cursor(bool pressed, Point<std::int16_t> position) override;

    void modify(InventoryType::Id type, std::int16_t pos, std::int8_t mode, std::int16_t arg);
    void enable_sort();
    void enable_gather();

protected:
    Button::State button_pressed(std::uint16_t buttonid) override;

private:
    void show_item(std::int16_t slot);
    void clear_tooltip();
    void load_icons();
    void update_slot(std::int16_t slot);
    bool is_visible(std::int16_t slot) const;
    bool is_not_visible(std::int16_t slot) const;
    std::int16_t slot_by_position(Point<std::int16_t> position) const;
    std::uint16_t button_by_tab(InventoryType::Id tab) const;
    Point<std::int16_t> get_slotpos(std::int16_t slot) const;
    Point<std::int16_t> get_tabpos(InventoryType::Id tab) const;
    Icon* get_icon(std::int16_t slot);

    class ItemIcon : public Icon::Type
    {
    public:
        ItemIcon(InventoryType::Id sourcetab,
                 Equipslot::Id eqsource,
                 std::int16_t source);

        void drop_on_stage() const override;
        void drop_on_equips(Equipslot::Id eqslot) const override;
        void drop_on_items(InventoryType::Id tab,
                           Equipslot::Id,
                           std::int16_t slot,
                           bool) const override;

    private:
        InventoryType::Id sourcetab;
        Equipslot::Id eqsource;
        std::int16_t source;
    };

    enum Buttons {
        BT_TAB_EQUIP,
        BT_TAB_USE,
        BT_TAB_ETC,
        BT_TAB_SETUP,
        BT_TAB_CASH,
        BT_DROPMESO,
        BT_POINTS,
        BT_GATHER,
        BT_SORT,
        BT_EXPAND,
        BT_ITEMPOT,
        BT_UPGRADE,
        BT_MAGNIFY,
        BT_BITCASE
    };

    const Inventory& inventory;

    Animation newitemslot;
    Animation newitemtab;
    Texture projectile;
    Text mesolabel;
    Slider slider;

    std::map<std::int16_t, std::unique_ptr<Icon>> icons;

    InventoryType::Id tab;
    std::pair<std::int16_t, std::int16_t> slotrange;
    InventoryType::Id newtab;
    std::int16_t newslot;
};
} // namespace jrc
