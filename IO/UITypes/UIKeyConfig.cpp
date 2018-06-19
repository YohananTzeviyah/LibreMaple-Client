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
#include "UIKeyConfig.h"

#include "../../Data/ItemData.h"
#include "../../Net/Packets/InventoryPackets.h"
#include "../Components/MapleButton.h"
#include "../KeyAction.h"
#include "../UI.h"
#include "nlnx/nx.hpp"

namespace jrc
{
UIKeyConfig::UIKeyConfig()
    : UIDragElement<Configuration::PositionOf::KEY_CONFIG>({622, 20})
{
    nl::node source = nl::nx::ui["UIWindow2.img"]["KeyConfig"];

    sprites.emplace_back(source["backgrnd"]);
    sprites.emplace_back(source["backgrnd2"]);
    sprites.emplace_back(source["backgrnd3"]);

    buttons[BT_CANCEL] = std::make_unique<MapleButton>(
        source["BtCancel"], Point<std::int16_t>{422, 58});
    buttons[BT_DEFAULT] = std::make_unique<MapleButton>(source["BtDefault"]);
    buttons[BT_DELETE] = std::make_unique<MapleButton>(source["BtDelete"]);
    buttons[BT_OK] = std::make_unique<MapleButton>(source["BtOK"]);
    buttons[BT_QUICKSLOT]
        = std::make_unique<MapleButton>(source["BtQuickSlot"]);

    nl::node icon_data = source["icon"];

    icons[KeyAction::Id::EQUIPMENT_TAB] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::EQUIPMENT_TAB),
        icon_data[0],
        -1);
    icons[KeyAction::Id::INVENTORY] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::INVENTORY), icon_data[1], -1);
    icons[KeyAction::Id::CHAR_STATS] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::CHAR_STATS),
        icon_data[2],
        -1);
    icons[KeyAction::Id::SKILL_TAB] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::SKILL_TAB), icon_data[3], -1);
    icons[KeyAction::Id::BUDDY_TAB] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::BUDDY_TAB), icon_data[4], -1);
    icons[KeyAction::Id::WORLD_MAP] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::WORLD_MAP), icon_data[5], -1);
    icons[KeyAction::Id::MESSENGER] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::MESSENGER), icon_data[6], -1);
    icons[KeyAction::Id::MINI_MAP] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::MINI_MAP), icon_data[7], -1);
    icons[KeyAction::Id::QUEST_LOG] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::QUEST_LOG), icon_data[8], -1);
    icons[KeyAction::Id::KEY_CONFIG] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::KEY_CONFIG),
        icon_data[9],
        -1);
    icons[KeyAction::Id::TO_ALL] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::TO_ALL), icon_data[10], -1);
    icons[KeyAction::Id::WHISPER] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::WHISPER), icon_data[11], -1);
    icons[KeyAction::Id::TO_PARTY] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::TO_PARTY), icon_data[12], -1);
    icons[KeyAction::Id::TO_FRIEND] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::TO_FRIEND),
        icon_data[13],
        -1);
    icons[KeyAction::Id::MAIN_MENU] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::MAIN_MENU),
        icon_data[14],
        -1);
    icons[KeyAction::Id::TOGGLE_QUICK_SLOT] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::TOGGLE_QUICK_SLOT),
        icon_data[15],
        -1);
    icons[KeyAction::Id::CHAT_WINDOW] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::CHAT_WINDOW),
        icon_data[16],
        -1);
    icons[KeyAction::Id::GUILD_TAB] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::GUILD_TAB),
        icon_data[17],
        -1);
    icons[KeyAction::Id::TO_GUILD] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::TO_GUILD), icon_data[18], -1);
    icons[KeyAction::Id::PARTY_TAB] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::PARTY_TAB),
        icon_data[19],
        -1);
    icons[KeyAction::Id::HELPER] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::HELPER), icon_data[20], -1);
    icons[KeyAction::Id::MONSTER_BOOK] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::MONSTER_BOOK),
        icon_data[21],
        -1);
    icons[KeyAction::Id::CASH_SHOP] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::CASH_SHOP),
        icon_data[22],
        -1);
    icons[KeyAction::Id::TO_ALLIANCE] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::TO_ALLIANCE),
        icon_data[23],
        -1);
    icons[KeyAction::Id::PARTY_SEARCH] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::PARTY_SEARCH),
        icon_data[24],
        -1);
    icons[KeyAction::Id::TO_TALK] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::TO_TALK), icon_data[25], -1);
    icons[KeyAction::Id::MEDALS] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::MEDALS), icon_data[26], -1);
    icons[KeyAction::Id::BOSS] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::BOSS), icon_data[27], -1);
    icons[KeyAction::Id::TO_EXPED] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::TO_EXPED), icon_data[28], -1);
    icons[KeyAction::Id::PROFESSION] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::PROFESSION),
        icon_data[29],
        -1);
    icons[KeyAction::Id::ITEM_POT] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::ITEM_POT), icon_data[30], -1);
    icons[KeyAction::Id::EVENT] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::EVENT), icon_data[31], -1);
    icons[KeyAction::Id::LEVEL_UP_GUIDE] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::LEVEL_UP_GUIDE),
        icon_data[32],
        -1);
    icons[KeyAction::Id::SILENT_CRUSADE] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::SILENT_CRUSADE),
        icon_data[33],
        -1);
    icons[KeyAction::Id::BITS] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::BITS), icon_data[34], -1);
    icons[KeyAction::Id::CHECK_NEW_TALK] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::CHECK_NEW_TALK),
        icon_data[35],
        -1);
    icons[KeyAction::Id::TOTS_KNOWHOW] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::TOTS_KNOWHOW),
        icon_data[36],
        -1);
    icons[KeyAction::Id::EPISODE] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::EPISODE), icon_data[37], -1);
    // icons[KeyAction::Id::HELPER_2] =
    //     std::make_unique<Icon>(
    //         std::make_unique<KeyIcon>(KeyAction::Id::HELPER_2),
    //         icon_data["38_"],
    //         -1);
    icons[KeyAction::Id::GUIDE] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::GUIDE), icon_data[39], -1);
    icons[KeyAction::Id::TO_SPOUSE] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::TO_SPOUSE),
        icon_data[40],
        -1);
    icons[KeyAction::Id::EQUIP_ENHANCE] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::EQUIP_ENHANCE),
        icon_data[41],
        -1);
    icons[KeyAction::Id::PICK_UP] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::PICK_UP), icon_data[50], -1);
    icons[KeyAction::Id::SIT] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::SIT), icon_data[51], -1);
    icons[KeyAction::Id::ATTACK] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::ATTACK), icon_data[52], -1);
    icons[KeyAction::Id::JUMP] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::JUMP), icon_data[53], -1);
    icons[KeyAction::Id::NPC_CHAT] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::NPC_CHAT), icon_data[54], -1);
    icons[KeyAction::Id::FACE_1] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::FACE_1), icon_data[100], -1);
    icons[KeyAction::Id::FACE_2] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::FACE_2), icon_data[101], -1);
    icons[KeyAction::Id::FACE_3] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::FACE_3), icon_data[102], -1);
    icons[KeyAction::Id::FACE_4] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::FACE_4), icon_data[103], -1);
    icons[KeyAction::Id::FACE_5] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::FACE_5), icon_data[104], -1);
    icons[KeyAction::Id::FACE_6] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::FACE_6), icon_data[105], -1);
    icons[KeyAction::Id::FACE_7] = std::make_unique<Icon>(
        std::make_unique<KeyIcon>(KeyAction::Id::FACE_7), icon_data[106], -1);

    dimension = {622, 374};
    reload_mappings();
    active = true;
}

void UIKeyConfig::draw(float alpha) const
{
    UIElement::draw(alpha);

    for (auto [slot_id, action_id] : slot_mappings.left) {
        if (auto icons_iter = icons.find(action_id); icons_iter.second()) {
            icons_iter.second()->draw(position + slot_pos({slot_id, true}));
        }
    }

    for (auto [slot_id, action_id] : palette_slots.left) {
        if (auto icons_iter = icons.find(action_id); icons_iter.second()) {
            icons_iter.second()->draw(position + slot_pos({slot_id, false}));
        }
    }
}

void UIKeyConfig::toggle_active()
{
    UIElement::toggle_active();
}

void UIKeyConfig::double_click(Point<std::int16_t>)
{
}

void UIKeyConfig::send_icon(const Icon& icon, Point<std::int16_t> cursor_pos)
{
    /* Console::get().print("sending icon at "
                         + (cursor_pos - position).to_string()); */
    if (auto slot = slot_by_position(cursor_pos); slot) {
        adjust_mapping(*slot, icon.get_action_id());
    }
}

Cursor::State UIKeyConfig::send_cursor(bool pressed,
                                       Point<std::int16_t> cursor_pos)
{
    Cursor::State drag_state = UIDragElement::send_cursor(pressed, cursor_pos);
    if (dragged) {
        return drag_state;
    }

    auto slot_ = slot_by_position(cursor_pos);
    if (!slot_) {
        return Cursor::IDLE;
    }
    auto slot = *slot_;

    auto& map = slot.second ? slot_mappings : palette_slots;
    if (auto action_iter = map.left.find(slot.first);
        action_iter != map.left.end()) {
        if (auto icon_iter = icons.find(action_iter->second);
            icon_iter.second()) {
            if (pressed) {
                icon_iter.second()->start_drag(cursor_pos - position
                                               - slot_pos(slot));
                UI::get().drag_icon(icon_iter.second().get());

                dragged_from = slot;

                return Cursor::GRABBING;
            } else {
                return Cursor::CAN_GRAB;
            }
        }
    }

    return Cursor::IDLE;
}

Button::State UIKeyConfig::button_pressed(std::uint16_t button_id)
{
    switch (button_id) {
    case Buttons::BT_DEFAULT:
        UI::get().emplace<UIKeyConfigNotice>(
            UIKeyConfigNotice::RESET_TO_DEFAULT,
            [this](bool ok) noexcept {
                if (ok) {
                    reset_to_default();
                }
            },
            position,
            dimension,
            true);
        return Button::NORMAL;
    case Buttons::BT_CANCEL:
        if (dirty) {
            UI::get().emplace<UIKeyConfigNotice>(
                UIKeyConfigNotice::SAVE_CHANGES,
                [this](bool yes) noexcept {
                    if (yes) {
                        commit_mappings();
                    }
                    UIElement::deactivate();
                    reload_mappings();
                },
                position,
                dimension,
                false);
        } else {
            UIElement::deactivate();
        }
        return Button::NORMAL;
    case Buttons::BT_OK:
        commit_mappings();
        UIElement::deactivate();
        return Button::NORMAL;
    case Buttons::BT_DELETE:
        UI::get().emplace<UIKeyConfigNotice>(
            UIKeyConfigNotice::CLEAR_ALL_SHORTCUTS,
            [this](bool yes) noexcept {
                if (yes) {
                    clear_mappings();
                }
            },
            position,
            dimension,
            false);
        return Button::NORMAL;
    default:
        return Button::PRESSED;
    }
}

void UIKeyConfig::reload_mappings() noexcept
{
    clear();

    for (auto [slot_id, mapping] : UI::get().get_keyboard().get_maplekeys()) {
        slot_mappings.insert({slot_id, mapping.action});
    }

    refresh_palette();

    dirty = false;
}

void UIKeyConfig::reset_to_default() noexcept
{
    clear();

    for (auto [key_id, action_id] : DEFAULT_MAPPINGS) {
        slot_mappings.insert({key_id, KeyAction::action_by_id(action_id)});
    }

    refresh_palette();

    dirty = true;
}

void UIKeyConfig::clear_mappings() noexcept
{
    std::uint8_t first_empty = 0;
    for (auto [_, action_id] : slot_mappings.left) {
        if (icons[action_id]) {
            auto palette_iter = palette_slots.left.find(first_empty);
            while (first_empty < PALETTE_COLS * PALETTE_ROWS
                   && palette_iter != palette_slots.left.end()) {
                ++first_empty;
                palette_iter = palette_slots.left.find(first_empty);
            }
            if (first_empty >= PALETTE_COLS * PALETTE_ROWS) {
                break;
            }

            palette_slots.insert({first_empty, action_id});
            ++first_empty;
        }
    }

    slot_mappings.left.clear();

    dirty = true;
}

bool UIKeyConfig::commit_mappings() noexcept
{
    auto& keyboard = UI::get().get_keyboard_mut();
    keyboard.clear_mappings();

    for (auto [key, action] : slot_mappings.left) {
        keyboard.assign(key, KeyType::type_by_action(action), action);
    }

    dirty = false;

    return keyboard.send_mappings();
}

void UIKeyConfig::clear() noexcept
{
    slot_mappings.left.clear();
    palette_slots.left.clear();
}

void UIKeyConfig::refresh_palette() noexcept
{
    std::uint8_t i = 0;
    for (const auto& icon_iter : icons) {
        if (icon_iter.second && !slot_mappings.right.count(icon_iter.first)) {
            bimap::assign(palette_slots, i, icon_iter.first);
            ++i;
        }
    }
}

void UIKeyConfig::adjust_mapping(std::pair<std::uint8_t, bool> slot,
                                 KeyAction::Id action) noexcept
{
    auto [from_slot, is_from_key] = dragged_from;
    auto [to_slot, is_to_key] = slot;

    auto& to_map = is_to_key ? slot_mappings : palette_slots;
    auto& from_map = is_from_key ? slot_mappings : palette_slots;

    if (is_from_key == is_to_key) {
        if (auto to_iter = to_map.left.find(to_slot);
            to_iter != to_map.left.end()) {
            KeyAction::Id temp = to_iter->second;
            to_map.left.replace_data(to_iter, action);
            bimap::assign(to_map, from_slot, temp);
        } else {
            bimap::assign(to_map, to_slot, action);
        }
    } else {
        if (auto to_iter = to_map.left.find(to_slot);
            to_iter != to_map.left.end()) {
            KeyAction::Id temp = to_iter->second;
            to_map.left.replace_data(to_iter, action);
            bimap::assign(from_map, from_slot, temp);
        } else {
            from_map.left.erase(from_slot);
            bimap::assign(to_map, to_slot, action);
        }
    }

    dirty = true;
}

std::optional<std::pair<std::uint8_t, bool>>
UIKeyConfig::slot_by_position(Point<std::int16_t> cursor_pos_) const noexcept
{
    auto cursor_pos = cursor_pos_ - position;

    if (PALETTE_AREA.contains(cursor_pos)) {
        auto normalized = cursor_pos - PALETTE_POSITION;
        auto col = normalized.x() / PALETTE_STRIDE;
        auto row = normalized.y() / PALETTE_STRIDE;

        return {{row * PALETTE_COLS + col, false}};
    }

    // Special-casing the top-left 32x32 rect since [0, 0] is the default
    // "position" for non-existent slots.
    if (Rectangle<std::int16_t>{{0, 0}, {32, 32}}.contains(cursor_pos)) {
        return {};
    }

    std::uint8_t slot_ix = 0;
    for (auto slot_pos : SLOT_POSITIONS) {
        Rectangle<std::int16_t> slot_rect{slot_pos, slot_pos + 32};
        if (slot_rect.contains(cursor_pos)) {
            return {{slot_ix, true}};
        }

        ++slot_ix;
    }

    return {};
}

Point<std::int16_t>
UIKeyConfig::slot_pos(std::pair<std::uint8_t, bool> slot) noexcept
{
    auto [s, key_slot] = slot;
    if (key_slot) {
        return SLOT_POSITIONS[s];
    } else {
        return PALETTE_POSITION
               + Point<std::int16_t>{s % PALETTE_COLS * PALETTE_STRIDE,
                                     s / PALETTE_COLS * PALETTE_STRIDE};
    }
}

UIKeyConfig::KeyIcon::KeyIcon(KeyAction::Id action_id) noexcept
    : action_id(action_id)
{
}

KeyAction::Id UIKeyConfig::KeyIcon::get_action_id() const noexcept
{
    return action_id;
}

UIKeyConfig::UIKeyConfigNotice::UIKeyConfigNotice(
    NoticeType type,
    std::function<void(bool)> yn_handler,
    Point<std::int16_t> parent_pos,
    Point<std::int16_t> parent_dim,
    bool ok_cancel)
    : yes_no_handler(yn_handler)
{
    nl::node src = nl::nx::ui["UIWindow2.img"]["KeyConfig"]["notice"];

    sprites.emplace_back(src[static_cast<std::uint16_t>(type)]);

    nl::node button_src = nl::nx::ui["Basic.img"];

    if (ok_cancel) {
        buttons[OK] = std::make_unique<MapleButton>(
            button_src["BtOK4"], Point<std::int16_t>{152, 53});
        buttons[CANCEL] = std::make_unique<MapleButton>(
            button_src["BtCancel4"], Point<std::int16_t>{202, 53});
    } else {
        buttons[OK] = std::make_unique<MapleButton>(
            button_src["BtYes2"], Point<std::int16_t>{145, 52});
        buttons[CANCEL] = std::make_unique<MapleButton>(
            button_src["BtNo2"], Point<std::int16_t>{200, 52});
    }

    dimension = {260, 84};
    // Centering the child notice on top of the parent.
    position = parent_pos + parent_dim / 2 - dimension / 2;

    active = true;
}

void UIKeyConfig::UIKeyConfigNotice::draw(float inter) const
{
    UIElement::draw(inter);
}

Button::State
UIKeyConfig::UIKeyConfigNotice::button_pressed(std::uint16_t button_id)
{
    switch (static_cast<ButtonType>(button_id)) {
    case ButtonType::OK:
        yes_no_handler(true);
        break;
    case ButtonType::CANCEL:
        yes_no_handler(false);
        break;
    }

    active = false;

    return Button::PRESSED;
}
} // namespace jrc
