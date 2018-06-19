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
#include "UINotice.h"

#include <algorithm>

namespace jrc
{
//! The in-game keybinding configuration UI.
class UIKeyConfig : public UIDragElement<Configuration::PositionOf::KEY_CONFIG>
{
public:
    static constexpr const Type TYPE = KEY_CONFIG;
    static constexpr const bool FOCUSED = false;
    static constexpr const bool TOGGLED = true;

    UIKeyConfig();

    void draw(float inter) const override;

    void toggle_active() override;
    void double_click(Point<std::int16_t> cursor_pos) override;
    void send_icon(const Icon& icon, Point<std::int16_t> cursor_pos) override;
    Cursor::State send_cursor(bool pressed,
                              Point<std::int16_t> cursor_pos) override;

protected:
    Button::State button_pressed(std::uint16_t button_id) override;

private:
    void reload_mappings() noexcept;
    void reset_to_default() noexcept;
    void clear_mappings() noexcept;
    bool commit_mappings() noexcept;
    void clear() noexcept;
    void refresh_palette() noexcept;
    void adjust_mapping(std::pair<std::uint8_t, bool> slot,
                        KeyAction::Id action) noexcept;
    std::optional<std::pair<std::uint8_t, bool>>
    slot_by_position(Point<std::int16_t> p) const noexcept;
    static Point<std::int16_t>
    slot_pos(std::pair<std::uint8_t, bool> slot) noexcept;

    class KeyIcon : public Icon::Type
    {
    public:
        KeyIcon(KeyAction::Id action_id) noexcept;

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
    EnumMap<KeyAction::Id, std::unique_ptr<Icon>> icons;
    bimap::unordered_bimap<std::uint8_t, KeyAction::Id> palette_slots;
    //! The `bool` is `true` when the slot is a key slot, and `false` when it
    //! is a palette slot.
    std::pair<std::uint8_t, bool> dragged_from;
    bool dirty;

    class UIKeyConfigNotice : public UIElement
    {
    public:
        static constexpr const Type TYPE = NOTICE;
        static constexpr const bool FOCUSED = true;
        static constexpr const bool TOGGLED = false;

        enum NoticeType : std::uint16_t {
            RESET_TO_DEFAULT,
            CLEAR_ALL_SHORTCUTS,
            SAVE_CHANGES
        };

        UIKeyConfigNotice(NoticeType type,
                          std::function<void(bool)> yn_handler,
                          Point<std::int16_t> parent_pos,
                          Point<std::int16_t> parent_dim,
                          bool ok_cancel);

        void draw(float inter) const override;

    protected:
        Button::State button_pressed(std::uint16_t button_id) override;

    private:
        std::function<void(bool)> yes_no_handler;

        enum ButtonType : std::uint16_t { OK, CANCEL };
    };

    //! The `n`th element of this array represents the position (relative to
    //! the position of the `UIKeyConfig`) of the key slot for the key with
    //! an ID of `n`.
    static constexpr const std::array<Point<std::int16_t>, 90> SLOT_POSITIONS{
        {{0, 0},     {0, 0},     {351, 65},  {45, 65},   {79, 65},
         {113, 65},  {147, 65},  {181, 65},  {215, 65},  {249, 65},
         {283, 65},  {317, 65},  {385, 65},  {419, 65},  {0, 0},
         {0, 0},     {61, 99},   {95, 99},   {129, 99},  {163, 99},
         {197, 99},  {231, 99},  {265, 99},  {299, 99},  {333, 99},
         {367, 99},  {401, 99},  {435, 99},  {0, 0},     {19, 198},
         {78, 132},  {112, 132}, {146, 132}, {180, 132}, {214, 132},
         {248, 132}, {282, 132}, {316, 132}, {350, 132}, {384, 132},
         {418, 132}, {11, 65},   {36, 165},  {469, 99},  {95, 165},
         {129, 165}, {163, 165}, {197, 165}, {231, 165}, {265, 165},
         {299, 165}, {333, 165}, {367, 165}, {0, 0},     {0, 0},
         {0, 0},     {120, 198}, {231, 198}, {0, 0},     {79, 28},
         {113, 28},  {147, 28},  {181, 28},  {223, 28},  {257, 28},
         {291, 28},  {325, 28},  {367, 28},  {401, 28},  {435, 28},
         {469, 28},  {545, 65},  {0, 0},     {579, 65},  {0, 0},
         {0, 0},     {0, 0},     {0, 0},     {0, 0},     {545, 99},
         {0, 0},     {579, 99},  {511, 65},  {511, 99},  {0, 0},
         {0, 0},     {0, 0},     {0, 0},     {0, 0},     {0, 0}}};

    static constexpr const std::int16_t PALETTE_ROWS = 3, PALETTE_COLS = 18;
    //! The position (relative to the position of the `UIKeyConfig`) of the
    //! upper-left-most palette slot.
    static constexpr const Point<std::int16_t> PALETTE_POSITION{5, 267};
    //! x-axis and y-axis spacing (i.e., square) between palette slots.
    static constexpr const std::int16_t PALETTE_STRIDE = 34;
    //! Full area that the palette takes up.
    static constexpr const Rectangle<std::int16_t> PALETTE_AREA{
        PALETTE_POSITION,
        PALETTE_POSITION
            + Point<std::int16_t>{PALETTE_STRIDE * PALETTE_COLS,
                                  PALETTE_STRIDE* PALETTE_ROWS}};

    //! Each element of this array represents a mapping from key ID (on the
    //! left) to action ID (on the right). This particular set of mappings
    //! is the default for GMS v83.
    static constexpr const std::array<std::pair<std::uint8_t, std::int32_t>,
                                      40>
        DEFAULT_MAPPINGS{
            {{18, 0},   {65, 106}, {2, 10},   {23, 1},   {3, 12},   {4, 13},
             {5, 18},   {6, 24},   {16, 8},   {17, 5},   {19, 4},   {25, 19},
             {26, 14},  {27, 15},  {31, 2},   {34, 17},  {35, 11},  {37, 3},
             {38, 20},  {40, 16},  {43, 9},   {44, 50},  {45, 51},  {46, 6},
             {50, 7},   {56, 53},  {59, 100}, {60, 101}, {61, 102}, {62, 103},
             {63, 104}, {64, 105}, {57, 54},  {48, 22},  {29, 52},  {7, 21},
             {24, 25},  {33, 26},  {41, 23},  {39, 27}}};
};
} // namespace jrc
