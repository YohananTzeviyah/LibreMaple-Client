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
#include "UIStatusBar.h"

#include "../../Character/ExpTable.h"
#include "../Components/MapleButton.h"
#include "../UI.h"
#include "nlnx/nx.hpp"

#include <string_view>

namespace jrc
{
constexpr Point<std::int16_t> UIStatusbar::POSITION;
constexpr Point<std::int16_t> UIStatusbar::DIMENSION;

UIStatusbar::UIStatusbar(const CharStats& st)
    : UIElement{POSITION, DIMENSION}, stats{st}, chatbar{POSITION}
{
    nl::node mainbar = nl::nx::ui["StatusBar2.img"]["mainBar"];

    sprites.emplace_back(mainbar["backgrnd"]);
    sprites.emplace_back(mainbar["gaugeBackgrd"]);
    sprites.emplace_back(mainbar["notice"]);
    sprites.emplace_back(mainbar["lvBacktrnd"]);
    sprites.emplace_back(mainbar["lvCover"]);

    auto gauge_src = mainbar["gauge"];

    auto exp_bar_src = gauge_src["exp"];
    expbar = {exp_bar_src["0"], exp_bar_src["1"], exp_bar_src["2"], 308, 0.0f};
    auto hp_bar_src = gauge_src["hp"];
    hpbar = {hp_bar_src["0"], hp_bar_src["1"], hp_bar_src["2"], 137, 0.0f};
    auto mp_bar_src = gauge_src["mp"];
    mpbar = {mp_bar_src["0"], mp_bar_src["1"], mp_bar_src["2"], 137, 0.0f};

    statset = {gauge_src["number"], Charset::RIGHT};
    levelset = {mainbar["lvNumber"], Charset::LEFT};

    joblabel = {Text::A11M, Text::LEFT, Text::YELLOW};
    namelabel = {Text::A13M, Text::LEFT, Text::WHITE};

    buttons[BT_WHISPER] = std::make_unique<MapleButton>(mainbar["BtChat"]);
    buttons[BT_CALL_GM] = std::make_unique<MapleButton>(mainbar["BtClaim"]);
    buttons[BT_NOTICE] = std::make_unique<MapleButton>(mainbar["BtNotice"]);
    buttons[BT_MAPLE_TALK]
        = std::make_unique<MapleButton>(mainbar["BtMapleTalk"]);

    buttons[BT_CASHSHOP]
        = std::make_unique<MapleButton>(mainbar["BtCashShop"]);
    buttons[BT_TRADE] = std::make_unique<MapleButton>(mainbar["BtMTS"]);
    buttons[BT_MENU] = std::make_unique<MapleButton>(mainbar["BtMenu"]);
    buttons[BT_OPTIONS] = std::make_unique<MapleButton>(mainbar["BtSystem"]);

    buttons[BT_CHARACTER]
        = std::make_unique<MapleButton>(mainbar["BtCharacter"]);
    buttons[BT_STATS] = std::make_unique<MapleButton>(mainbar["BtStat"]);
    buttons[BT_QUEST] = std::make_unique<MapleButton>(mainbar["BtQuest"]);
    buttons[BT_INVENTORY] = std::make_unique<MapleButton>(mainbar["BtInven"]);
    buttons[BT_EQUIPS] = std::make_unique<MapleButton>(mainbar["BtEquip"]);
    buttons[BT_SKILL] = std::make_unique<MapleButton>(mainbar["BtSkill"]);

    buttons[BT_KEYSETTING]
        = std::make_unique<MapleButton>(mainbar["BtKeysetting"]);
    buttons[BT_CHANNEL] = std::make_unique<MapleButton>(mainbar["BtChannel"]);
    buttons[BT_HOME] = std::make_unique<MapleButton>(mainbar["BtFarm"]);
}

void UIStatusbar::draw(float alpha) const
{
    UIElement::draw(alpha);

    expbar.draw(position + Point<std::int16_t>{-261, -15});
    hpbar.draw(position + Point<std::int16_t>{-261, -31});
    mpbar.draw(position + Point<std::int16_t>{-90, -31});

    std::int16_t level = stats.get_stat(Maplestat::LEVEL);
    std::int16_t hp = stats.get_stat(Maplestat::HP);
    std::int16_t mp = stats.get_stat(Maplestat::MP);
    std::int32_t maxhp = stats.get_total(Equipstat::HP);
    std::int32_t maxmp = stats.get_total(Equipstat::MP);
    std::int64_t exp = stats.get_exp();

    std::string exp_string = std::to_string(100 * get_exp_percent());
    statset.draw(
        str::concat(
            std::to_string(exp),
            '[',
            std::string_view(exp_string).substr(0, exp_string.find('.') + 3),
            "%]"),
        position + Point<std::int16_t>{47, -13});
    statset.draw(
        str::concat('[', std::to_string(hp), '/', std::to_string(maxhp), ']'),
        position + Point<std::int16_t>{-124, -29});
    statset.draw(
        str::concat('[', std::to_string(mp), '/', std::to_string(maxmp), ']'),
        position + Point<std::int16_t>{47, -29});
    levelset.draw(std::to_string(level),
                  position + Point<std::int16_t>{-480, -24});

    joblabel.draw(position + Point<std::int16_t>{-435, -21});
    namelabel.draw(position + Point<std::int16_t>{-435, -36});

    chatbar.draw(alpha);
}

void UIStatusbar::update()
{
    UIElement::update();

    chatbar.update();

    expbar.update(get_exp_percent());
    hpbar.update(get_hp_percent());
    mpbar.update(get_mp_percent());

    namelabel.change_text(std::string{stats.get_name()});
    joblabel.change_text(std::string{stats.get_job_name()});

    for (auto iter : message_cooldowns) {
        iter.second -= Constants::TIMESTEP;
    }
}

Button::State UIStatusbar::button_pressed(std::uint16_t id)
{
    switch (id) {
    case BT_STATS:
        UI::get().send_menu(KeyAction::CHAR_STATS);
        return Button::NORMAL;
    case BT_INVENTORY:
        UI::get().send_menu(KeyAction::INVENTORY);
        return Button::NORMAL;
    case BT_EQUIPS:
        UI::get().send_menu(KeyAction::EQUIPMENT_TAB);
        return Button::NORMAL;
    case BT_SKILL:
        UI::get().send_menu(KeyAction::SKILL_TAB);
        return Button::NORMAL;
    case BT_KEYSETTING:
        UI::get().send_menu(KeyAction::KEY_CONFIG);
        return Button::NORMAL;
    case BT_OPTIONS:
        UI::get().send_menu(KeyAction::MAIN_MENU);
        return Button::NORMAL;
    default:
        return Button::PRESSED;
    }
}

bool UIStatusbar::is_in_range(Point<std::int16_t> cursorpos) const
{
    Rectangle<std::int16_t> bounds(position - Point<std::int16_t>{512, 84},
                                   position - Point<std::int16_t>{512, 84}
                                       + dimension);

    return bounds.contains(cursorpos) || chatbar.is_in_range(cursorpos);
}

bool UIStatusbar::remove_cursor(bool clicked, Point<std::int16_t> cursorpos)
{
    if (chatbar.remove_cursor(clicked, cursorpos)) {
        return true;
    }

    return UIElement::remove_cursor(clicked, cursorpos);
}

Cursor::State UIStatusbar::send_cursor(bool pressed,
                                       Point<std::int16_t> cursorpos)
{
    if (chatbar.is_in_range(cursorpos)) {
        UIElement::send_cursor(pressed, cursorpos);
        return chatbar.send_cursor(pressed, cursorpos);
    } else {
        chatbar.send_cursor(pressed, cursorpos);
        return UIElement::send_cursor(pressed, cursorpos);
    }
}

void UIStatusbar::send_chatline(std::string&& line, UIChatbar::LineType type)
{
    chatbar.send_line(std::move(line), type);
}

void UIStatusbar::display_message(Messages::Type line,
                                  UIChatbar::LineType type)
{
    if (message_cooldowns[line] > 0) {
        return;
    }

    chatbar.send_line(std::string{Messages::messages[line]}, type);

    message_cooldowns[line] = MESSAGE_COOLDOWN;
}

float UIStatusbar::get_exp_percent() const
{
    std::int16_t level = stats.get_stat(Maplestat::LEVEL);
    if (level >= ExpTable::LEVELCAP) {
        return 0.0f;
    }

    std::int64_t exp = stats.get_exp();
    return static_cast<float>(exp)
           / static_cast<float>(ExpTable::values[level]);
}

float UIStatusbar::get_hp_percent() const
{
    std::int16_t hp = stats.get_stat(Maplestat::HP);
    std::int32_t maxhp = stats.get_total(Equipstat::HP);

    return static_cast<float>(hp) / maxhp;
}

float UIStatusbar::get_mp_percent() const
{
    std::int16_t mp = stats.get_stat(Maplestat::MP);
    std::int32_t maxmp = stats.get_total(Equipstat::MP);

    return static_cast<float>(mp) / maxmp;
}
} // namespace jrc
