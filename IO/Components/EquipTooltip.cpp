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
#include "EquipTooltip.h"

#include "../../Data/EquipData.h"
#include "../../Data/WeaponData.h"
#include "../../Gameplay/Stage.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

#include <cstdlib>

namespace jrc
{
EquipTooltip::EquipTooltip()
{
    nl::node item_tt = nl::nx::ui["UIToolTip.img"]["Item"];

    top = item_tt["Frame"]["top"];
    mid = item_tt["Frame"]["line"];
    line = item_tt["Frame"]["dotline"];
    bot = item_tt["Frame"]["bottom"];
    base = item_tt["ItemIcon"]["base"];
    cover = item_tt["ItemIcon"]["cover"];
    shade = item_tt["ItemIcon"]["shade"];

    potential[Equip::POT_NONE] = {};
    potential[Equip::POT_HIDDEN] = item_tt["ItemIcon"]["0"];
    potential[Equip::POT_RARE] = item_tt["ItemIcon"]["1"];
    potential[Equip::POT_EPIC] = item_tt["ItemIcon"]["2"];
    potential[Equip::POT_UNIQUE] = item_tt["ItemIcon"]["3"];
    potential[Equip::POT_LEGENDARY] = item_tt["ItemIcon"]["4"];

    requirements.push_back(Maplestat::LEVEL);
    requirements.push_back(Maplestat::STR);
    requirements.push_back(Maplestat::DEX);
    requirements.push_back(Maplestat::INT);
    requirements.push_back(Maplestat::LUK);

    req_stat_textures[Maplestat::LEVEL][false]
        = item_tt["Equip"]["Cannot"]["reqLEV"];
    req_stat_textures[Maplestat::LEVEL][true]
        = item_tt["Equip"]["Can"]["reqLEV"];
    req_stat_textures[Maplestat::FAME][false]
        = item_tt["Equip"]["Cannot"]["reqPOP"];
    req_stat_textures[Maplestat::FAME][true]
        = item_tt["Equip"]["Can"]["reqPOP"];
    req_stat_textures[Maplestat::STR][false]
        = item_tt["Equip"]["Cannot"]["reqSTR"];
    req_stat_textures[Maplestat::STR][true]
        = item_tt["Equip"]["Can"]["reqSTR"];
    req_stat_textures[Maplestat::DEX][false]
        = item_tt["Equip"]["Cannot"]["reqDEX"];
    req_stat_textures[Maplestat::DEX][true]
        = item_tt["Equip"]["Can"]["reqDEX"];
    req_stat_textures[Maplestat::INT][false]
        = item_tt["Equip"]["Cannot"]["reqINT"];
    req_stat_textures[Maplestat::INT][true]
        = item_tt["Equip"]["Can"]["reqINT"];
    req_stat_textures[Maplestat::LUK][false]
        = item_tt["Equip"]["Cannot"]["reqLUK"];
    req_stat_textures[Maplestat::LUK][true]
        = item_tt["Equip"]["Can"]["reqLUK"];

    req_stat_positions[Maplestat::LEVEL] = {98, 48};
    req_stat_positions[Maplestat::STR] = {98, 64};
    req_stat_positions[Maplestat::DEX] = {98, 72};
    req_stat_positions[Maplestat::INT] = {173, 64};
    req_stat_positions[Maplestat::LUK] = {173, 72};

    req_set[false] = {item_tt["Equip"]["Cannot"], Charset::LEFT};
    req_set[true] = {item_tt["Equip"]["Can"], Charset::LEFT};

    jobs_back = item_tt["Equip"]["Job"]["normal"];
    jobs[false][0] = item_tt["Equip"]["Job"]["disable"]["0"];
    jobs[false][1] = item_tt["Equip"]["Job"]["disable"]["1"];
    jobs[false][2] = item_tt["Equip"]["Job"]["disable"]["2"];
    jobs[false][3] = item_tt["Equip"]["Job"]["disable"]["3"];
    jobs[false][4] = item_tt["Equip"]["Job"]["disable"]["4"];
    jobs[false][5] = item_tt["Equip"]["Job"]["disable"]["5"];
    jobs[true][0] = item_tt["Equip"]["Job"]["enable"]["0"];
    jobs[true][1] = item_tt["Equip"]["Job"]["enable"]["1"];
    jobs[true][2] = item_tt["Equip"]["Job"]["enable"]["2"];
    jobs[true][3] = item_tt["Equip"]["Job"]["enable"]["3"];
    jobs[true][4] = item_tt["Equip"]["Job"]["enable"]["4"];
    jobs[true][5] = item_tt["Equip"]["Job"]["enable"]["5"];

    inv_pos = 0;
}

void EquipTooltip::set_equip(Parent parent, std::int16_t ivp)
{
    if (ivp == inv_pos) {
        return;
    }

    inv_pos = ivp;

    const Player& player = Stage::get().get_player();

    InventoryType::Id invtype;
    switch (parent) {
    case ITEM_INVENTORY:
    case SHOP:
        invtype = InventoryType::EQUIP;
        break;
    case EQUIP_INVENTORY:
        invtype = InventoryType::EQUIPPED;
        break;
    default:
        invtype = InventoryType::NONE;
    }

    auto oequip = player.get_inventory().get_equip(invtype, ivp);
    if (!oequip) {
        return;
    }

    const Equip& equip = *oequip;

    std::int32_t item_id = equip.get_item_id();

    const EquipData& equip_data = EquipData::get(item_id);
    const ItemData& item_data = equip_data.get_item_data();
    const CharStats& stats = player.get_stats();

    height = 500;

    item_icon = item_data.get_icon(true);

    for (auto& ms : requirements) {
        can_equip[ms] = stats.get_stat(ms) >= equip_data.get_req_stat(ms);
        std::string reqstr = std::to_string(equip_data.get_req_stat(ms));
        reqstr.insert(0, 3 - reqstr.size(), '0');
        req_stat_strings[ms] = reqstr;
    }

    ok_jobs.clear();
    switch (equip_data.get_req_stat(Maplestat::JOB)) {
    case 0:
        ok_jobs.push_back(0);
        ok_jobs.push_back(1);
        ok_jobs.push_back(2);
        ok_jobs.push_back(3);
        ok_jobs.push_back(4);
        ok_jobs.push_back(5);
        can_equip[Maplestat::JOB] = true;
        break;
    case 1:
        ok_jobs.push_back(1);
        can_equip[Maplestat::JOB]
            = (stats.get_stat(Maplestat::JOB) / 100 == 1)
              || (stats.get_stat(Maplestat::JOB) / 100 >= 20);
        break;
    case 2:
        ok_jobs.push_back(2);
        can_equip[Maplestat::JOB] = stats.get_stat(Maplestat::JOB) / 100 == 2;
        break;
    case 4:
        ok_jobs.push_back(3);
        can_equip[Maplestat::JOB] = stats.get_stat(Maplestat::JOB) / 100 == 3;
        break;
    case 8:
        ok_jobs.push_back(4);
        can_equip[Maplestat::JOB] = stats.get_stat(Maplestat::JOB) / 100 == 4;
        break;
    case 16:
        ok_jobs.push_back(5);
        can_equip[Maplestat::JOB] = stats.get_stat(Maplestat::JOB) / 100 == 5;
        break;
    default:
        can_equip[Maplestat::JOB] = false;
    }

    prank = equip.get_potrank();
    switch (prank) {
    case Equip::POT_HIDDEN:
        pot_flag = Text(Text::A11M, Text::CENTER, Text::RED);
        pot_flag.change_text("(Hidden Potential)");
        break;
    case Equip::POT_RARE:
        pot_flag = Text(Text::A11M, Text::CENTER, Text::WHITE);
        pot_flag.change_text("(Rare Item)");
        break;
    case Equip::POT_EPIC:
        pot_flag = Text(Text::A11M, Text::CENTER, Text::WHITE);
        pot_flag.change_text("(Epic Item)");
        break;
    case Equip::POT_UNIQUE:
        pot_flag = Text(Text::A11M, Text::CENTER, Text::WHITE);
        pot_flag.change_text("(Unique Item)");
        break;
    case Equip::POT_LEGENDARY:
        pot_flag = Text(Text::A11M, Text::CENTER, Text::WHITE);
        pot_flag.change_text("(Legendary Item)");
        break;
    default:
        height -= 16;
    }

    Text::Color namecolor;
    switch (equip.get_quality()) {
    case EquipQuality::GREY:
        namecolor = Text::LIGHTGREY;
        break;
    case EquipQuality::ORANGE:
        namecolor = Text::ORANGE;
        break;
    case EquipQuality::BLUE:
        namecolor = Text::MEDIUMBLUE;
        break;
    case EquipQuality::VIOLET:
        namecolor = Text::VIOLET;
        break;
    case EquipQuality::GOLD:
        namecolor = Text::YELLOW;
        break;
    default:
        namecolor = Text::WHITE;
    }

    std::string name_str{item_data.get_name()};
    if (equip.get_level() > 0) {
        name_str.append(" (+");
        name_str.append(std::to_string(equip.get_level()));
        name_str.push_back(')');
    }
    name = {Text::A12B, Text::CENTER, namecolor, std::move(name_str), 400};

    std::string_view desc_text = item_data.get_desc();
    has_desc = desc_text.size() > 0;
    if (has_desc) {
        desc = {
            Text::A12M, Text::LEFT, Text::WHITE, std::string{desc_text}, 250};
        height += desc.height() + 10;
    }

    category = {Text::A11L,
                Text::LEFT,
                Text::WHITE,
                str::concat("CATEGORY: ", equip_data.get_type())};

    is_weapon = equip_data.is_weapon();
    if (is_weapon) {
        const WeaponData& weapon = WeaponData::get(item_id);
        wep_speed = {Text::A11L,
                     Text::LEFT,
                     Text::WHITE,
                     str::concat("ATTACK SPEED: ", weapon.get_speed_string())};
    } else {
        height -= 18;
    }

    has_slots = equip.get_slots() > 0 || equip.get_level() > 0;
    if (has_slots) {
        slots = {Text::A11L,
                 Text::LEFT,
                 Text::WHITE,
                 "UPGRADES AVAILABLE: " + std::to_string(equip.get_slots())};

        std::string vicious = std::to_string(equip.get_vicious());
        if (equip.get_vicious() > 1) {
            vicious.append(" (MAX) ");
        }
        hammers = {Text::A11L,
                   Text::LEFT,
                   Text::WHITE,
                   "VICIOUS HAMMERS USED: " + vicious};
    } else {
        height -= 36;
    }

    stat_labels.clear();
    for (Equipstat::Id es = Equipstat::STR; es <= Equipstat::JUMP;
         es = static_cast<Equipstat::Id>(es + 1)) {
        if (equip.get_stat(es) > 0) {
            std::int16_t delta
                = equip.get_stat(es) - equip_data.get_def_stat(es);
            std::string stat_str = std::to_string(equip.get_stat(es));
            if (delta != 0) {
                stat_str.append(" (");
                stat_str.push_back(delta < 0 ? '-' : '+');
                stat_str.append(std::to_string(std::abs(delta)));
                stat_str.push_back(')');
            }
            stat_labels[es]
                = {Text::A11L,
                   Text::LEFT,
                   Text::WHITE,
                   Equipstat::names[es] + std::string(": ") + stat_str};
        } else {
            height -= 18;
        }
    }
}

void EquipTooltip::draw(Point<std::int16_t> pos) const
{
    if (inv_pos == 0) {
        return;
    }

    top.draw({pos});
    mid.draw(
        {pos + Point<std::int16_t>{0, 13}, Point<std::int16_t>{0, height}});
    bot.draw({pos + Point<std::int16_t>{0, height + 13}});

    name.draw(pos + Point<std::int16_t>{130, 3});
    if (prank != Equip::POT_NONE) {
        pot_flag.draw(pos + Point<std::int16_t>{130, 20});
        pos.shift_y(16);
    }
    pos.shift_y(26);

    line.draw({pos});

    auto pos_plus_10 = pos + 10;
    base.draw(pos_plus_10);
    shade.draw(pos_plus_10);
    item_icon.draw({pos + Point<std::int16_t>{20, 82}, 2.0f, 2.0f});
    potential[prank].draw(pos_plus_10);
    cover.draw(pos_plus_10);

    pos.shift_y(12);

    for (Maplestat::Id ms : requirements) {
        Point<std::int16_t> req_pos = req_stat_positions[ms];
        bool req_ok = can_equip[ms];
        req_stat_textures[ms][req_ok].draw({pos + req_pos});
        req_set[req_ok].draw(req_stat_strings[ms],
                             6,
                             {pos + req_pos + Point<std::int16_t>{54, 0}});
    }

    pos.shift_y(88);

    Point<std::int16_t> job_position(pos + Point<std::int16_t>{8, 0});
    jobs_back.draw(job_position);
    for (auto& ok_job : ok_jobs) {
        jobs[can_equip[Maplestat::JOB]][ok_job].draw(job_position);
    }

    line.draw({pos + Point<std::int16_t>{0, 30}});

    pos.shift_y(32);

    category.draw(pos + Point<std::int16_t>{10, 0});

    pos.shift_y(18);

    if (is_weapon) {
        wep_speed.draw(pos + Point<std::int16_t>{10, 0});
        pos.shift_y(18);
    }

    for (const Text& label : stat_labels.values()) {
        if (label.empty()) {
            continue;
        }

        label.draw(pos + Point<std::int16_t>{10, 0});
        pos.shift_y(18);
    }

    if (has_slots) {
        slots.draw(pos + Point<std::int16_t>{10, 0});
        pos.shift_y(18);
        hammers.draw(pos + Point<std::int16_t>{10, 0});
        pos.shift_y(18);
    }

    if (has_desc) {
        line.draw({pos + Point<std::int16_t>{0, 5}});
        desc.draw({pos + Point<std::int16_t>{10, 6}});
    }
}
} // namespace jrc
