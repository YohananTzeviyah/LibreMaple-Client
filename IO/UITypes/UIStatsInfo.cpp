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
#include "UIStatsInfo.h"

#include "../../Net/Packets/PlayerPackets.h"
#include "../Components/MapleButton.h"
#include "../UI.h"
#include "nlnx/nx.hpp"

#include <cstdlib>

namespace jrc
{
UIStatsinfo::UIStatsinfo(const CharStats& st)
    : UIDragElement<Configuration::PositionOf::STATS>{{212, 20}}, stats{st}
{
    nl::node base_src = nl::nx::ui["UIWindow4.img"]["Stat"];
    nl::node src = base_src["main"];
    nl::node detail = base_src["detail"];

    sprites.emplace_back(src["backgrnd"]);
    sprites.emplace_back(src["backgrnd2"]);
    sprites.emplace_back(src["backgrnd3"]);

    textures_detail.emplace_back(detail["backgrnd"]);
    textures_detail.emplace_back(detail["backgrnd2"]);
    textures_detail.emplace_back(detail["backgrnd3"]);

    auto ability_title_src = detail["abilityTitle"];
    abilities[RARE] = ability_title_src["rare"]["0"];
    abilities[EPIC] = ability_title_src["epic"]["0"];
    abilities[UNIQUE] = ability_title_src["unique"]["0"];
    abilities[LEGENDARY] = ability_title_src["legendary"]["0"];
    abilities[NONE] = ability_title_src["normal"]["0"];

    buttons[BT_HP] = std::make_unique<MapleButton>(src["BtHpUp"]);
    buttons[BT_MP] = std::make_unique<MapleButton>(src["BtMpUp"]);
    buttons[BT_STR] = std::make_unique<MapleButton>(src["BtStrUp"]);
    buttons[BT_DEX] = std::make_unique<MapleButton>(src["BtDexUp"]);
    buttons[BT_LUK] = std::make_unique<MapleButton>(src["BtLukUp"]);
    buttons[BT_INT] = std::make_unique<MapleButton>(src["BtIntUp"]);

    buttons[BT_DETAILOPEN]
        = std::make_unique<MapleButton>(src["BtDetailOpen"]);
    buttons[BT_DETAILCLOSE]
        = std::make_unique<MapleButton>(src["BtDetailClose"]);
    buttons[BT_DETAILCLOSE]->set_active(false);

    update_ap();

    for (std::size_t i = 0; i < NUMLABELS; ++i) {
        statlabels[i] = {Text::A11M, Text::LEFT, Text::LIGHTGREY};
    }
    statoffsets[NAME] = {73, 27};
    statoffsets[JOB] = {73, 45};
    statoffsets[GUILD] = {73, 63};
    statoffsets[FAME] = {73, 81};
    statoffsets[DAMAGE] = {73, 99};
    statoffsets[HP] = {73, 117};
    statoffsets[MP] = {73, 135};
    statoffsets[AP] = {70, 177};
    statoffsets[STR] = {73, 204};
    statoffsets[DEX] = {73, 222};
    statoffsets[INT] = {73, 240};
    statoffsets[LUK] = {73, 258};
    statoffsets[ATTACK] = {73, 37};
    statoffsets[CRIT] = {73, 55};
    statoffsets[MINCRIT] = {73, 73};
    statoffsets[MAXCRIT] = {168, 73};
    statoffsets[BDM] = {73, 91};
    statoffsets[IGNOREDEF] = {168, 91};
    statoffsets[RESIST] = {73, 109};
    statoffsets[STANCE] = {168, 109};
    statoffsets[WDEF] = {73, 127};
    statoffsets[MDEF] = {73, 145};
    statoffsets[ACCURACY] = {73, 163};
    statoffsets[AVOID] = {73, 199};
    statoffsets[SPEED] = {73, 235};
    statoffsets[JUMP] = {168, 235};
    statoffsets[HONOR] = {73, 353};

    update_all_stats();
    update_stat(Maplestat::JOB);
    update_stat(Maplestat::FAME);

    dimension = {212, 318};
    showdetail = false;
}

void UIStatsinfo::draw(float alpha) const
{
    UIElement::draw(alpha);

    if (showdetail) {
        auto detail_pos = position + Point<std::int16_t>{213, 0};
        for (auto& texture : textures_detail) {
            texture.draw(detail_pos);
        }
        abilities[NONE].draw({detail_pos});
    }

    std::size_t last = showdetail ? NUMLABELS : NUMNORMAL;
    for (std::size_t i = 0; i < last; ++i) {
        auto labelpos = position + statoffsets[i];
        if (i >= NUMNORMAL) {
            labelpos.shift_x(213);
        }

        statlabels[i].draw(labelpos);
    }
}

void UIStatsinfo::update_all_stats()
{
    update_simple(AP, Maplestat::AP);
    if (hasap ^ (stats.get_stat(Maplestat::AP) > 0)) {
        update_ap();
    }

    statlabels[NAME].change_text(std::string(stats.get_name()));
    statlabels[GUILD].change_text("");

    statlabels[HP].change_text(
        str::concat(std::to_string(stats.get_stat(Maplestat::HP)),
                    " / ",
                    std::to_string(stats.get_total(Equipstat::HP))));
    statlabels[MP].change_text(
        str::concat(std::to_string(stats.get_stat(Maplestat::MP)),
                    " / ",
                    std::to_string(stats.get_total(Equipstat::MP))));

    update_basevstotal(STR, Maplestat::STR, Equipstat::STR);
    update_basevstotal(DEX, Maplestat::DEX, Equipstat::DEX);
    update_basevstotal(INT, Maplestat::INT, Equipstat::INT);
    update_basevstotal(LUK, Maplestat::LUK, Equipstat::LUK);

    statlabels[DAMAGE].change_text(
        str::concat(std::to_string(stats.get_min_damage()),
                    " ~ ",
                    std::to_string(stats.get_max_damage())));
    if (stats.is_damage_buffed()) {
        statlabels[DAMAGE].change_color(Text::RED);
    } else {
        statlabels[DAMAGE].change_color(Text::LIGHTGREY);
    }

    update_buffed(ATTACK, Equipstat::WATK);
    update_buffed(WDEF, Equipstat::WDEF);
    update_buffed(MDEF, Equipstat::MDEF);
    update_buffed(ACCURACY, Equipstat::ACC);
    update_buffed(AVOID, Equipstat::AVOID);

    statlabels[CRIT].change_text(str::concat(
        std::to_string(static_cast<std::int32_t>(stats.get_critical() * 100)),
        '%'));
    statlabels[MINCRIT].change_text(str::concat(
        std::to_string(static_cast<std::int32_t>(stats.get_min_crit() * 100)),
        '%'));
    statlabels[MAXCRIT].change_text(str::concat(
        std::to_string(static_cast<std::int32_t>(stats.get_max_crit() * 100)),
        '%'));
    statlabels[BDM].change_text(str::concat(
        std::to_string(static_cast<std::int32_t>(stats.get_boss_dmg() * 100)),
        '%'));
    statlabels[IGNOREDEF].change_text(
        str::concat(std::to_string(static_cast<std::int32_t>(
                        stats.get_ignore_def() * 100)),
                    '%'));
    statlabels[RESIST].change_text(
        str::concat(std::to_string(static_cast<std::int32_t>(
                        stats.get_resistance() * 100)),
                    '%'));
    statlabels[STANCE].change_text(str::concat(
        std::to_string(static_cast<std::int32_t>(stats.get_stance() * 100)),
        '%'));
    statlabels[SPEED].change_text(
        str::concat(std::to_string(stats.get_total(Equipstat::SPEED)), '%'));
    statlabels[JUMP].change_text(
        str::concat(std::to_string(stats.get_total(Equipstat::JUMP)), '%'));
    statlabels[HONOR].change_text(std::to_string(stats.get_honor()));
}

void UIStatsinfo::update_stat(Maplestat::Id stat)
{
    switch (stat) {
    case Maplestat::JOB:
        statlabels[JOB].change_text(std::string(stats.get_job_name()));
        break;
    case Maplestat::FAME:
        update_simple(FAME, Maplestat::FAME);
        break;
    default:
        break;
    }
}

Button::State UIStatsinfo::button_pressed(std::uint16_t id)
{
    switch (id) {
    case BT_DETAILOPEN:
        showdetail = true;
        buttons[BT_DETAILOPEN]->set_active(false);
        buttons[BT_DETAILCLOSE]->set_active(true);
        break;
    case BT_DETAILCLOSE:
        showdetail = false;
        buttons[BT_DETAILCLOSE]->set_active(false);
        buttons[BT_DETAILOPEN]->set_active(true);
        break;
    case BT_HP:
        send_apup(Maplestat::HP);
        break;
    case BT_MP:
        send_apup(Maplestat::MP);
        break;
    case BT_STR:
        send_apup(Maplestat::STR);
        break;
    case BT_DEX:
        send_apup(Maplestat::DEX);
        break;
    case BT_INT:
        send_apup(Maplestat::INT);
        break;
    case BT_LUK:
        send_apup(Maplestat::LUK);
        break;
    default:
        break;
    }
    return Button::NORMAL;
}

void UIStatsinfo::send_apup(Maplestat::Id stat) const
{
    SpendApPacket{stat}.dispatch();
    UI::get().disable();
}

void UIStatsinfo::update_ap()
{
    const bool now_ap = stats.get_stat(Maplestat::AP) > 0;
    const Button::State new_state = now_ap ? Button::NORMAL : Button::DISABLED;

    buttons[BT_HP]->set_state(new_state);
    buttons[BT_MP]->set_state(new_state);
    buttons[BT_STR]->set_state(new_state);
    buttons[BT_DEX]->set_state(new_state);
    buttons[BT_LUK]->set_state(new_state);
    buttons[BT_INT]->set_state(new_state);

    hasap = now_ap;
}

void UIStatsinfo::update_simple(StatLabel label, Maplestat::Id stat)
{
    statlabels[label].change_text(std::to_string(stats.get_stat(stat)));
}

void UIStatsinfo::update_basevstotal(StatLabel label,
                                     Maplestat::Id bstat,
                                     Equipstat::Id tstat)
{
    std::int32_t base = stats.get_stat(bstat);
    std::int32_t total = stats.get_total(tstat);
    std::int32_t delta = total - base;

    statlabels[label].change_text(
        delta ? str::concat(std::to_string(total),
                            " (",
                            std::to_string(base),
                            delta > 0 ? " + " : " - ",
                            std::to_string(std::abs(delta)),
                            ')')
              : std::to_string(total));
}

void UIStatsinfo::update_buffed(StatLabel label, Equipstat::Id stat)
{
    std::int32_t total = stats.get_total(stat);
    std::int32_t delta = stats.get_buff_delta(stat);

    statlabels[label].change_text(
        delta ? str::concat(std::to_string(total),
                            " (",
                            std::to_string(total - delta),
                            delta > 0 ? " + " : " - ",
                            std::to_string(std::abs(delta)),
                            ')')
              : std::to_string(total));

    if (delta > 0) {
        statlabels[label].change_color(Text::RED);
    } else if (delta < 0) {
        statlabels[label].change_color(Text::BLUE);
    }
}
} // namespace jrc
