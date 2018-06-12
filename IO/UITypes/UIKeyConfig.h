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
#include "../../Util/Misc.h"
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
        void drop_on_items(InventoryType::Id,
                           Equipslot::Id,
                           std::int16_t,
                           bool) const override{};
        KeyAction::Id get_action_id() const noexcept override;

    private:
        KeyAction::Id action_id;
    };

    enum Buttons : std::uint16_t {
        BT_CANCEL,
        BT_DEFAULT,
        BT_DELETE,
        BT_OK,
        BT_QUICKSLOT
    };

    bimap::unordered_bimap<std::uint8_t, KeyAction::Id> slot_mappings;
    std::unordered_map<KeyAction::Id, std::unique_ptr<Icon>> icons{70};
};

constexpr const std::array<Point<std::int16_t>, 90> SLOT_POSITIONS{
    {{0, 0},     {0, 0},     {351, 65},  {45, 65},   {79, 65},   {113, 65},
     {147, 65},  {181, 65},  {215, 65},  {249, 65},  {283, 65},  {317, 65},
     {385, 65},  {419, 65},  {0, 0},     {0, 0},     {61, 99},   {95, 99},
     {129, 99},  {163, 99},  {197, 99},  {231, 99},  {265, 99},  {299, 99},
     {333, 99},  {367, 99},  {401, 99},  {435, 99},  {0, 0},     {19, 200},
     {78, 132},  {112, 132}, {146, 132}, {180, 132}, {214, 132}, {248, 132},
     {282, 132}, {316, 132}, {350, 132}, {384, 132}, {418, 132}, {11, 65},
     {36, 166},  {469, 99},  {95, 165},  {129, 165}, {163, 165}, {197, 165},
     {231, 165}, {265, 165}, {299, 165}, {333, 165}, {367, 165}, {0, 0},
     {0, 0},     {0, 0},     {120, 200}, {231, 200}, {0, 0},     {79, 28},
     {113, 28},  {147, 28},  {181, 28},  {223, 28},  {257, 28},  {291, 28},
     {325, 28},  {367, 28},  {401, 28},  {435, 28},  {469, 28},  {545, 65},
     {0, 0},     {579, 65},  {0, 0},     {0, 0},     {0, 0},     {0, 0},
     {0, 0},     {545, 99},  {0, 0},     {579, 99},  {511, 65},  {511, 99},
     {0, 0},     {0, 0},     {0, 0},     {0, 0},     {0, 0},     {0, 0}}};
} // namespace jrc
