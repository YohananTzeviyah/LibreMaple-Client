//////////////////////////////////////////////////////////////////////////////
// This file is part of the LibreMaple MMORPG client                        //
// Copyright © 2018-2019 LibreMaple Team                                    //
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
#include "../Keyboard.h"
#include "../UIDragElement.h"

#include <unordered_map>

namespace jrc
{
//! The in-game keybinding configuration UI.
class UIKeyConfig : public UIDragElement<PosKEYCONFIG>
{
public:
    static constexpr Type TYPE = KEYCONFIG;
    static constexpr bool FOCUSED = false;
    static constexpr bool TOGGLED = true;

    UIKeyConfig() noexcept;

    void draw(float inter) const override;

    void toggle_active() override;
    void double_click(Point<std::int16_t> cursor_pos) override;
    void send_icon(const Icon& icon, Point<std::int16_t> cursor_pos) override;
    Cursor::State send_cursor(bool pressed,
                              Point<std::int16_t> cursor_pos) override;

protected:
    Button::State button_pressed(std::uint16_t buttonid) override;

private:
    void reload_mappings() noexcept;
    void update_key_slot(std::uint8_t key_slot,
                         KeyAction::Id action_id) noexcept;
    std::uint8_t slot_by_position(Point<std::int16_t> position) const noexcept;

    class KeyIcon : public Icon::Type
    {
    public:
        KeyIcon(KeyAction::Id action_id) noexcept;

        void drop_on_stage() const override{};
        void drop_on_equips(Equipslot::Id) const override
        {
        }
        void drop_on_items(InventoryType::Id tab,
                           Equipslot::Id eqslot,
                           std::int16_t slot,
                           bool equip) const override{};
        KeyAction::Id get_action_id() const noexcept override;

    private:
        KeyAction::Id action_id;
    };

    enum class Buttons : std::uint16_t {
        BT_CANCEL,
        BT_DEFAULT,
        BT_DELETE,
        BT_OK,
        BT_QUICKSLOT
    };

    constexpr std::array<Point<std::int16_t>, 90> SLOT_POSITIONS{
        {{0, 0},    {0, 0},    {32, 32},  {64, 32},  {96, 32},  {128, 32},
         {160, 32}, {192, 32}, {224, 32}, {256, 32}, {288, 32}, {320, 32},
         {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},
         {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},
         {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},
         {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},
         {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},
         {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},
         {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},
         {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},
         {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},
         {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},
         {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},
         {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},
         {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0},    {0, 0}}};
    std::unordered_map<KeyAction::Id, std::uint8_t> action_mappings;
    std::unordered_map<std::uint8_t, KeyAction::Id> slot_mappings;
    std::unordered_map<KeyAction::Id, std::unique_ptr<Icon>> icons;
};
} // namespace jrc
