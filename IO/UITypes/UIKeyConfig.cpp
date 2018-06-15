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

#include <algorithm>

namespace jrc
{
UIKeyConfig::UIKeyConfig() noexcept : UIDragElement<PosKEYCONFIG>({622, 20})
{
    const auto source = nl::nx::ui["UIWindow2.img"]["KeyConfig"];

    sprites.emplace_back(source["backgrnd"]);
    sprites.emplace_back(source["backgrnd2"]);
    sprites.emplace_back(source["backgrnd3"]);

    buttons[BT_CANCEL] = std::make_unique<MapleButton>(
        source["BtCancel"], Point<std::int16_t>{422, 58});
    buttons[BT_DEFAULT] = std::make_unique<MapleButton>(source["BtDefault"]);
    buttons[BT_DELETE] = std::make_unique<MapleButton>(source["BtDelete"]);
    buttons[BT_OK] = std::make_unique<MapleButton>(source["BtOK"]);
    buttons[BT_QUICKSLOT] =
        std::make_unique<MapleButton>(source["BtQuickSlot"]);

    const auto icon_data = source["icon"];

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

    for (const auto& [action_id, icon] : icons) {
        if (auto iter = slot_mappings.right.find(action_id);
            iter != slot_mappings.right.end()) {
            icon->draw(position + SLOT_POSITIONS[iter->second]);
        }
    }
}

Button::State UIKeyConfig::button_pressed(std::uint16_t id)
{
    switch (id) {
    case Buttons::BT_DEFAULT:
        reset_to_default();
        return Button::NORMAL;
    case Buttons::BT_CANCEL:
        UIElement::deactivate();
        reload_mappings();
        return Button::NORMAL;
    case Buttons::BT_OK:
        commit_mappings();
        UIElement::deactivate();
        return Button::NORMAL;
    default:
        return Button::PRESSED;
    }
}

void UIKeyConfig::reload_mappings() noexcept
{
    slot_mappings.left.clear();
    for (auto [slot_id, mapping] : UI::get().get_keyboard().get_maplekeys()) {
        update_key_slot(slot_id, mapping.action);
    }
}

void UIKeyConfig::reset_to_default() noexcept
{
    slot_mappings.left.clear();
    for (auto [key_id, action_id] : DEFAULT_MAPPINGS) {
        slot_mappings.insert({key_id, KeyAction::action_by_id(action_id)});
    }
}

bool UIKeyConfig::commit_mappings() const noexcept
{
    auto& keyboard = UI::get().get_keyboard_mut();
    keyboard.clear_mappings();

    for (auto [key, action] : slot_mappings) {
        keyboard.assign(key, KeyType::type_by_action(action), action);
    }

    return keyboard.send_mappings();
}

void UIKeyConfig::update_key_slot(std::uint8_t key_slot,
                                  KeyAction::Id action_id) noexcept
{
    bimap::assign(slot_mappings, key_slot, action_id);
}

Cursor::State UIKeyConfig::send_cursor(bool pressed,
                                       Point<std::int16_t> cursor_pos)
{
    Cursor::State drag_state = UIDragElement::send_cursor(pressed, cursor_pos);
    if (dragged) {
        return drag_state;
    }

    std::uint8_t slot = slot_by_position(cursor_pos);
    if (!slot) {
        return Cursor::IDLE;
    }
    if (auto action_iter = slot_mappings.left.find(slot);
        action_iter != slot_mappings.left.end()) {
        if (auto icon_iter = icons.find(action_iter->second);
            icon_iter != icons.end() && icon_iter->second) {
            if (pressed) {
                icon_iter->second->start_drag(cursor_pos - position -
                                              SLOT_POSITIONS[slot]);
                UI::get().drag_icon(icon_iter->second.get());

                return Cursor::GRABBING;
            } else {
                return Cursor::CAN_GRAB;
            }
        }
    }

    return Cursor::IDLE;
}

void UIKeyConfig::double_click(Point<std::int16_t>)
{
}

void UIKeyConfig::send_icon(const Icon& icon, Point<std::int16_t> cursor_pos)
{
    if (std::uint8_t slot = slot_by_position(cursor_pos); slot) {
        update_key_slot(slot, icon.get_action_id());
    }
}

void UIKeyConfig::toggle_active()
{
    UIElement::toggle_active();
}

std::uint8_t
UIKeyConfig::slot_by_position(Point<std::int16_t> cursor_pos) const noexcept
{
    std::uint8_t slot_ix = 0;
    for (auto slot_pos : SLOT_POSITIONS) {
        Rectangle<std::int16_t> slot_rect{position + slot_pos,
                                          position + slot_pos + 32};
        if (slot_rect.contains(cursor_pos)) {
            return slot_ix;
        }

        ++slot_ix;
    }

    return 0;
}

UIKeyConfig::KeyIcon::KeyIcon(KeyAction::Id action_id) noexcept
    : action_id(action_id)
{
}

KeyAction::Id UIKeyConfig::KeyIcon::get_action_id() const noexcept
{
    return action_id;
}
} // namespace jrc
