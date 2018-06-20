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
#include "UICharSelect.h"

#include "../../Audio/Audio.h"
#include "../../Character/Job.h"
#include "../../Configuration.h"
#include "../../Constants.h"
#include "../../Net/Packets/SelectCharPackets.h"
#include "../Components/AreaButton.h"
#include "../Components/MapleButton.h"
#include "../UI.h"
#include "UICharCreation.h"
#include "UISoftKey.h"
#include "nlnx/nx.hpp"

#include <string_view>

namespace jrc
{
UICharSelect::UICharSelect(std::vector<CharEntry> cs,
                           std::uint8_t c,
                           std::uint8_t s,
                           std::uint8_t channel_id,
                           std::int8_t p)
    : characters(cs), require_pic(p), char_count_absolute(c), slots_absolute(s)
{
    selected_absolute = Configuration::get().account.character;
    selected_relative = selected_absolute % PAGE_SIZE;
    page = selected_absolute / PAGE_SIZE;

    // nl::node title = nl::nx::ui["Login.img"]["Title"];
    nl::node common = nl::nx::ui["Login.img"]["Common"];
    nl::node char_select = nl::nx::ui["Login.img"]["CharSelect"];

    sprites.emplace_back(
        nl::nx::ui["Login.img"]["RaceSelect_new"]["Back"]["1"]);
    sprites.emplace_back(common["frame"], Point<std::int16_t>{400, 290});

    // Post BB
    /*
    sel_world_pos = Point<std::int16_t>(578, 42);
    char_info_pos = Point<std::int16_t>(662, 355);
    buttons[BT_ARBEIT] = std::make_unique<MapleButton>(charsel["arbeit"],
    Point<std::int16_t>(580, 115))); buttons[BT_CARDS] =
    std::make_unique<MapleButton>(charsel["characterCard"],
    Point<std::int16_t>(665, 115)));
    */

    // Pre BB
    char_info_pos = {662, 305};
    sel_world_pos = {578, 112};

    sprites.emplace_back(char_select["charInfo"], char_info_pos);
    sprites.emplace_back(common["selectWorld"], sel_world_pos);
    sprites.emplace_back(char_select["selectedWorld"]["icon"]["15"],
                         sel_world_pos);
    sprites.emplace_back(char_select["selectedWorld"]["name"]["15"],
                         sel_world_pos);
    sprites.emplace_back(char_select["selectedWorld"]["ch"][channel_id],
                         sel_world_pos);

    empty_slot = char_select["buyCharacter"];
    name_tag = char_select["nameTag"];

    buttons[BT_SELECTCHAR] = std::make_unique<MapleButton>(
        char_select["BtSelect"], char_info_pos + Point<std::int16_t>{-76, 72});
    buttons[BT_CREATECHAR] = std::make_unique<MapleButton>(
        char_select["BtNew"], Point<std::int16_t>{200, 495});
    buttons[BT_DELETECHAR] = std::make_unique<MapleButton>(
        char_select["BtDelete"], Point<std::int16_t>{320, 495});
    buttons[BT_PAGELEFT] = std::make_unique<MapleButton>(
        char_select["pageL"], Point<std::int16_t>{100, 490});
    buttons[BT_PAGERIGHT] = std::make_unique<MapleButton>(
        char_select["pageR"], Point<std::int16_t>{490, 490});

    for (std::uint8_t i = 0; i < PAGE_SIZE; ++i) {
        buttons[BT_CHAR0 + i] = std::make_unique<AreaButton>(
            Point<std::int16_t>{105 + 120 * (i % 4), 170 + 200 * (i > 3)},
            Point<std::int16_t>{50, 80});
    }

    level_set = {char_select["lv"], Charset::CENTER};

    name_label = {Text::A18M, Text::CENTER};
    for (std::size_t i = 0; i < NUM_LABELS; ++i) {
        info_labels[i] = {Text::A11M, Text::RIGHT};
    }

    for (const auto& entry : characters) {
        char_looks.emplace_back(entry.look);
        name_tags.emplace_back(
            name_tag, Text::A13M, Text::WHITE, std::string{entry.stats.name});
    }

    update_counts();
    update_selection();

    position = {0, 0};
    dimension = {Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT};
    active = true;
}

void UICharSelect::draw(float alpha) const
{
    UIElement::draw(alpha);

    for (std::uint8_t i = 0; i < char_count_relative; ++i) {
        Point<std::int16_t> charpos = get_char_pos(i);
        std::uint8_t index = i + page * PAGE_SIZE;
        char_looks[index].draw(charpos, alpha);
        name_tags[index].draw(charpos);
    }

    if (selected_relative < char_count_relative) {
        const StatsEntry& stats = characters[selected_relative].stats;

        std::string levelstr = std::to_string(stats.stats[Maplestat::LEVEL]);
        std::int16_t lvx = level_set.draw(
            levelstr, char_info_pos + Point<std::int16_t>(23, -93));
        level_set.draw('l',
                       char_info_pos + Point<std::int16_t>(-7 - lvx / 2, -93));

        name_label.draw(char_info_pos + Point<std::int16_t>(0, -85));

        for (std::size_t i = 0; i < NUM_LABELS; ++i) {
            Point<std::int16_t> labelpos = char_info_pos + get_label_pos(i);
            info_labels[i].draw(labelpos);
        }
    }

    for (std::uint8_t i = char_count_relative; i < slots_relative; ++i) {
        Point<std::int16_t> position_slot(130 + (120 * (i % 4)),
                                          250 + (200 * (i > 3)));
        empty_slot.draw(position_slot, alpha);
    }
}

void UICharSelect::update()
{
    UIElement::update();

    empty_slot.update();

    for (auto& chit : char_looks) {
        chit.update(Constants::TIMESTEP);
    }
}

Button::State UICharSelect::button_pressed(std::uint16_t bid)
{
    if (bid >= BT_CHAR0) {
        name_tags[selected_absolute].set_selected(false);
        char_looks[selected_absolute].set_stance(Stance::STAND1);
        buttons[BT_CHAR0 + selected_relative]->set_state(Button::NORMAL);

        selected_relative = static_cast<std::uint8_t>(bid - BT_CHAR0);
        selected_absolute = selected_relative + page * PAGE_SIZE;
        update_selection();

        return Button::IDENTITY;
    } else {
        switch (bid) {
        case BT_SELECTCHAR:
            send_selection();
            return Button::NORMAL;
        case BT_CREATECHAR:
            active = false;
            UI::get().emplace<UICharCreation>();
            return Button::NORMAL;
        case BT_DELETECHAR:
            send_deletion();
            return Button::PRESSED;
        case BT_PAGERIGHT:
            ++page;
            update_counts();
            update_selection();
            return Button::IDENTITY;
        case BT_PAGELEFT:
            --page;
            update_counts();
            update_selection();
            return Button::IDENTITY;
        default:
            return Button::PRESSED;
        }
    }
}

void UICharSelect::update_selection()
{
    if (selected_relative >= char_count_relative) {
        return;
    }

    char_looks[selected_absolute].set_stance(Stance::WALK1);
    name_tags[selected_absolute].set_selected(true);

    buttons[BT_CHAR0 + selected_relative]->set_state(Button::PRESSED);
    name_label.change_text(characters[selected_relative].stats.name);

    for (std::size_t i = 0; i < NUM_LABELS; ++i) {
        info_labels[i].change_text(get_label_string(i));
    }
}

void UICharSelect::update_counts()
{
    if (page > 0) {
        buttons[BT_PAGELEFT]->set_state(Button::NORMAL);
    } else {
        buttons[BT_PAGELEFT]->set_state(Button::DISABLED);
    }

    if (page < slots_absolute / PAGE_SIZE) {
        buttons[BT_PAGERIGHT]->set_state(Button::NORMAL);
    } else {
        buttons[BT_PAGERIGHT]->set_state(Button::DISABLED);
    }

    char_count_relative = char_count_absolute;
    if (char_count_relative > (page + 1) * PAGE_SIZE) {
        char_count_relative = PAGE_SIZE;
    } else if (char_count_relative < page * PAGE_SIZE) {
        char_count_relative = 0;
    } else {
        char_count_relative -= page * PAGE_SIZE;
    }

    if (selected_absolute >= char_count_absolute) {
        selected_absolute = 0;
    }

    selected_relative = selected_absolute % PAGE_SIZE;
    if (selected_relative >= char_count_relative) {
        selected_relative = 0;
    }

    slots_relative = slots_absolute - page * PAGE_SIZE;
    if (slots_relative > PAGE_SIZE) {
        slots_relative = PAGE_SIZE;
    }

    if (char_count_relative < slots_relative) {
        buttons[BT_CREATECHAR]->set_state(Button::NORMAL);
    } else {
        buttons[BT_CREATECHAR]->set_state(Button::DISABLED);
    }

    if (char_count_relative > 0) {
        buttons[BT_DELETECHAR]->set_state(Button::NORMAL);
    } else {
        buttons[BT_DELETECHAR]->set_state(Button::DISABLED);
    }

    for (std::uint8_t i = 0; i < PAGE_SIZE; ++i) {
        if (i < char_count_relative) {
            buttons[BT_CHAR0 + i]->set_state(Button::NORMAL);
        } else {
            buttons[BT_CHAR0 + i]->set_state(Button::DISABLED);
        }
    }
}

void UICharSelect::send_selection()
{
    if (selected_relative >= char_count_relative) {
        buttons[BT_SELECTCHAR]->set_state(Button::MOUSE_OVER);
        return;
    }

    Configuration::get().account.character = selected_absolute;
    std::int32_t cid = characters[selected_absolute].cid;
    switch (require_pic) {
    case 0:
        UI::get().emplace<UISoftkey>([cid](std::string_view pic) {
            UI::get().disable();
            RegisterPicPacket(cid, pic).dispatch();
        });
        break;
    case 1:
        UI::get().emplace<UISoftkey>([cid](std::string_view pic) {
            UI::get().disable();
            SelectCharPicPacket(pic, cid).dispatch();
        });
        break;
    case 2:
        UI::get().disable();
        Sound{Sound::SELECT_CHAR}.play();
        SelectCharPacket{cid}.dispatch();
        break;
    default:
        Console::get().print("require_pic = " + std::to_string(require_pic));
        break;
    }
}

void UICharSelect::send_deletion()
{
    if (selected_relative >= char_count_relative) {
        buttons[BT_DELETECHAR]->set_state(Button::MOUSE_OVER);
        return;
    }

    std::int32_t cid = characters[selected_absolute].cid;
    UI::get().emplace<UISoftkey>([cid](std::string_view pic) {
        UI::get().disable();
        DeleteCharPacket{pic, cid}.dispatch();
    });
}

void UICharSelect::add_character(CharEntry character)
{
    char_looks.emplace_back(character.look);
    name_tags.emplace_back(
        name_tag, Text::A13M, Text::WHITE, std::string{character.stats.name});

    characters.emplace_back(std::move(character));

    ++char_count_absolute;
    ++char_count_relative;

    update_counts();
    update_selection();
}

void UICharSelect::remove_char(std::int32_t cid)
{
    std::size_t index = 0;
    for (const auto& character : characters) {
        if (character.cid == cid) {
            break;
        }

        ++index;
    }

    if (index == characters.size()) {
        return;
    }

    characters.erase(characters.begin() + index);
    char_looks.erase(char_looks.begin() + index);
    name_tags.erase(name_tags.begin() + index);

    --char_count_absolute;
    --char_count_relative;
    update_counts();
    update_selection();
}

const CharEntry& UICharSelect::get_character(std::int32_t cid)
{
    for (const auto& character : characters) {
        if (character.cid == cid) {
            return character;
        }
    }

    Console::get().print(
        __func__,
        str::concat("Warning: Invalid CID (", std::to_string(cid), ')'));

    static const CharEntry null_entry{{}, {}, 0};
    return null_entry;
}

std::string UICharSelect::get_label_string(std::size_t label) const
{
    const StatsEntry& stats = characters[selected_relative].stats;
    switch (label) {
    case JOB:
        return std::string{Job(stats.stats[Maplestat::JOB]).get_name()};
    case WORLDRANK:
        return std::to_string(stats.rank.first);
    case JOBRANK:
        return std::to_string(stats.job_rank.first);
    case STR:
        return std::to_string(stats.stats[Maplestat::STR]);
    case DEX:
        return std::to_string(stats.stats[Maplestat::DEX]);
    case INT:
        return std::to_string(stats.stats[Maplestat::INT]);
    case LUK:
        return std::to_string(stats.stats[Maplestat::LUK]);
    default:
        return {};
    }
}

Point<std::int16_t> UICharSelect::get_label_pos(std::size_t label) const
{
    switch (label) {
    case JOB:
        return {72, -48};
    case WORLDRANK:
        return {72, -24};
    case JOBRANK:
        return {72, -4};
    case STR:
        return {-5, 26};
    case DEX:
        return {-5, 48};
    case INT:
        return {72, 26};
    case LUK:
        return {72, 48};
    default:
        return {};
    }
}

Point<std::int16_t> UICharSelect::get_char_pos(std::size_t i) const
{
    std::int16_t x = 130 + 120 * (i % 4);
    std::int16_t y = 250 + 200 * (i > 3);
    return {x, y};
}
} // namespace jrc
