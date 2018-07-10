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
#include "../../Graphics/Text.h"
#include "../Components/Icon.h"
#include "../Components/Slider.h"
#include "../UIDragElement.h"

#include <unordered_map>

namespace jrc
{
//! The Item inventory.
class UIItemInventory
    : public UIDragElement<Configuration::PositionOf::INVENTORY>
{
public:
    static constexpr const Type TYPE = ITEM_INVENTORY;
    static constexpr const bool FOCUSED = false;
    static constexpr const bool TOGGLED = true;

    UIItemInventory(const Inventory& inventory);

    void draw(float inter) const override;
    void update() override;

    void double_click(Point<std::int16_t> cursor_pos) override;
    void send_icon(const Icon& icon, Point<std::int16_t> cursor_pos) override;
    void toggle_active() override;
    bool remove_cursor(bool clicked, Point<std::int16_t> cursor_pos) override;
    Cursor::State send_cursor(bool pressed,
                              Point<std::int16_t> cursor_pos) override;

    void modify(InventoryType::Id type,
                std::int16_t pos,
                std::int8_t mode,
                std::int16_t arg);
    void enable_sort();
    void enable_gather();

    class ItemIcon : public Icon::Type
    {
    public:
        ItemIcon(std::int32_t item_id_,
                 InventoryType::Id source_tab_,
                 Equipslot::Id eq_source_,
                 std::int16_t source_) noexcept;

        void drop_on_stage() const override;
        void drop_on_equips(Equipslot::Id eq_slot) const override;
        void drop_on_items(InventoryType::Id tab,
                           Equipslot::Id,
                           std::int16_t slot,
                           bool) const override;

        std::int32_t get_action_id() const noexcept override;

    private:
        std::int32_t item_id;
        InventoryType::Id source_tab;
        Equipslot::Id eq_source;
        std::int16_t source;
    };

protected:
    Button::State button_pressed(std::uint16_t buttonid) override;

private:
    void show_item(std::int16_t slot);
    void clear_tooltip();
    void load_icons();
    void update_slot(std::int16_t slot);
    bool is_visible(std::int16_t slot) const;
    bool is_not_visible(std::int16_t slot) const;
    std::int16_t slot_by_position(Point<std::int16_t> cursor_pos) const;
    std::uint16_t button_by_tab(InventoryType::Id tab) const;
    Point<std::int16_t> get_slot_pos(std::int16_t slot) const;
    Point<std::int16_t> get_tab_pos(InventoryType::Id tab) const;
    Icon* get_icon(std::int16_t slot);

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

    Animation new_item_slot;
    Animation new_item_tab;
    Texture projectile;
    Text meso_label;
    Slider slider;

    std::unordered_map<std::int16_t, std::unique_ptr<Icon>> icons;

    InventoryType::Id tab;
    std::pair<std::int16_t, std::int16_t> slot_range;
    InventoryType::Id new_tab;
    std::int16_t new_slot;
};
} // namespace jrc
