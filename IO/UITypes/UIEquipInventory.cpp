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
#include "UIEquipInventory.h"

#include "../../Data/ItemData.h"
#include "../../Net/Packets/InventoryPackets.h"
#include "../Components/MapleButton.h"
#include "../UI.h"
#include "nlnx/nx.hpp"

namespace jrc
{
UIEquipInventory::UIEquipInventory(const Inventory& invent)
    : UIDragElement<PosEQINV>({184, 20}), inventory(invent)
{
    icon_positions[Equipslot::CAP] = {43, 25};
    icon_positions[Equipslot::FACEACC] = {43, 91};
    icon_positions[Equipslot::EYEACC] = {43, 68};
    icon_positions[Equipslot::EARRINGS] = {109, 91};
    icon_positions[Equipslot::TOP] = {43, 124};
    icon_positions[Equipslot::PANTS] = {43, 157};
    icon_positions[Equipslot::SHOES] = {76, 190};
    icon_positions[Equipslot::GLOVES] = {10, 157};
    icon_positions[Equipslot::CAPE] = {10, 124};
    icon_positions[Equipslot::SHIELD] = {142, 124};
    icon_positions[Equipslot::WEAPON] = {109, 124};
    icon_positions[Equipslot::RING] = {109, 157};
    icon_positions[Equipslot::RING2] = {142, 157};
    icon_positions[Equipslot::RING3] = {109, 91};
    icon_positions[Equipslot::RING4] = {142, 91};
    icon_positions[Equipslot::PENDANT] = {76, 124};
    icon_positions[Equipslot::TAMEDMOB] = {142, 91};
    icon_positions[Equipslot::SADDLE] = {76, 124};
    icon_positions[Equipslot::MEDAL] = {10, 58};
    icon_positions[Equipslot::BELT] = {76, 157};

    nl::node source = nl::nx::ui["UIWindow2.img"]["Equip"]["character"];
    nl::node petsource = nl::nx::ui["UIWindow2.img"]["Equip"]["pet"];

    sprites.emplace_back(source["backgrnd"]);
    sprites.emplace_back(source["backgrnd2"]);
    sprites.emplace_back(source["backgrnd3_Kanna"]);
    sprites.emplace_back(source["cashPendant"]);
    sprites.emplace_back(source["charmPocket"]);
    sprites.emplace_back(source["emblem"]);

    buttons[BT_TOGGLEPETS] = std::make_unique<MapleButton>(source["BtPet"]);

    textures_pet.emplace_back(petsource["backgrnd"]);
    textures_pet.emplace_back(petsource["backgrnd2"]);
    textures_pet.emplace_back(petsource["backgrnd3"]);

    load_icons();

    dimension = {184, 290};
    active = true;
    show_pet_equips = false;
}

void UIEquipInventory::draw(float alpha) const
{
    UIElement::draw(alpha);

    for (auto [equip_slot, icon] : icons) {
        if (icon) {
            icon->draw(position + icon_positions[equip_slot]);
        }
    }

    if (show_pet_equips) {
        auto position_pet = position + Point<std::int16_t>{184, 0};
        for (const auto& texture : textures_pet) {
            texture.draw(position_pet);
        }
    }
}

Button::State UIEquipInventory::button_pressed(std::uint16_t button_id)
{
    switch (button_id) {
    case BT_TOGGLEPETS:
        show_pet_equips = !show_pet_equips;
        return Button::NORMAL;
    default:
        return Button::PRESSED;
    }
}

void UIEquipInventory::update_slot(Equipslot::Id slot)
{
    if (std::int32_t item_id
        = inventory.get_item_id(InventoryType::EQUIPPED, slot)) {
        const Texture& texture = ItemData::get(item_id).get_icon(false);
        icons[slot] = std::make_unique<Icon>(
            std::make_unique<EquipIcon>(slot), texture, -1);
    } else if (icons[slot]) {
        icons[slot].release();
    }

    clear_tooltip();
}

void UIEquipInventory::load_icons()
{
    icons.clear();

    for (auto iter : Equipslot::values) {
        update_slot(iter);
    }
}

Cursor::State UIEquipInventory::send_cursor(bool pressed,
                                            Point<std::int16_t> cursorpos)
{
    Cursor::State dstate = UIDragElement::send_cursor(pressed, cursorpos);
    if (dragged) {
        clear_tooltip();
        return dstate;
    }

    Equipslot::Id slot = slot_by_position(cursorpos);
    if (auto icon = icons[slot].get()) {
        if (pressed) {
            icon->start_drag(cursorpos - position - icon_positions[slot]);
            UI::get().drag_icon(icon);

            clear_tooltip();
            return Cursor::GRABBING;
        } else {
            show_equip(slot);
            return Cursor::CAN_GRAB;
        }
    } else {
        clear_tooltip();
        return Cursor::IDLE;
    }
}

void UIEquipInventory::double_click(Point<std::int16_t> cursorpos)
{
    Equipslot::Id slot = slot_by_position(cursorpos);
    if (icons[slot]) {
        if (std::int16_t freeslot
            = inventory.find_free_slot(InventoryType::EQUIP)) {
            UnequipItemPacket(slot, freeslot).dispatch();
        }
    }
}

void UIEquipInventory::send_icon(const Icon& icon,
                                 Point<std::int16_t> cursorpos)
{
    if (Equipslot::Id slot = slot_by_position(cursorpos)) {
        icon.drop_on_equips(slot);
    }
}

void UIEquipInventory::toggle_active()
{
    clear_tooltip();
    UIElement::toggle_active();
}

void UIEquipInventory::modify(std::int16_t pos,
                              std::int8_t mode,
                              std::int16_t arg)
{
    Equipslot::Id eqpos = Equipslot::by_id(pos);
    Equipslot::Id eqarg = Equipslot::by_id(arg);
    switch (mode) {
    case 0:
    case 3:
        update_slot(eqpos);
        break;
    case 2:
        update_slot(eqpos);
        update_slot(eqarg);
        break;
    }
}

void UIEquipInventory::show_equip(Equipslot::Id slot)
{
    UI::get().show_equip(Tooltip::EQUIP_INVENTORY, slot);
}

void UIEquipInventory::clear_tooltip()
{
    UI::get().clear_tooltip(Tooltip::EQUIP_INVENTORY);
}

Equipslot::Id
UIEquipInventory::slot_by_position(Point<std::int16_t> cursor_pos) const
{
    for (auto [slot, slot_pos] : icon_positions) {
        Rectangle<std::int16_t> icon_rect{position + slot_pos,
                                          position + slot_pos
                                              + Point<std::int16_t>{32, 32}};
        if (icon_rect.contains(cursor_pos)) {
            return slot;
        }
    }
    return Equipslot::NONE;
}

UIEquipInventory::EquipIcon::EquipIcon(std::int16_t s) noexcept : source(s)
{
}

void UIEquipInventory::EquipIcon::drop_on_stage() const
{
    UnequipItemPacket(source, 0).dispatch();
}

void UIEquipInventory::EquipIcon::drop_on_items(InventoryType::Id tab,
                                                Equipslot::Id eqslot,
                                                std::int16_t slot,
                                                bool equip) const
{
    if (tab != InventoryType::EQUIP) {
        return;
    }

    if (equip) {
        if (eqslot == source) {
            EquipItemPacket(slot, eqslot).dispatch();
        }
    } else {
        UnequipItemPacket(source, slot).dispatch();
    }
}
} // namespace jrc
