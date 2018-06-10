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
#pragma once
#include "../../Character/CharStats.h"
#include "../../Character/Inventory/Inventory.h"
#include "../../Character/Job.h"
#include "../../Graphics/Animation.h"
#include "../../Graphics/Text.h"
#include "../Components/Charset.h"
#include "../Components/Gauge.h"
#include "../Components/Textfield.h"
#include "../Messages.h"
#include "../UIElement.h"
#include "UIChatBar.h"

namespace jrc
{
class UIStatusbar : public UIElement
{
public:
    static constexpr Type TYPE = STATUSBAR;
    static constexpr bool FOCUSED = false;
    static constexpr bool TOGGLED = true;

    UIStatusbar(const CharStats& stats);

    void draw(float alpha) const override;
    void update() override;

    bool is_in_range(Point<std::int16_t> cursorpos) const override;
    bool remove_cursor(bool clicked, Point<std::int16_t> cursorpos) override;
    Cursor::State send_cursor(bool pressed,
                              Point<std::int16_t> cursorpos) override;

    void send_chatline(std::string&& line, UIChatbar::LineType type);
    void display_message(Messages::Type line, UIChatbar::LineType type);

protected:
    Button::State button_pressed(std::uint16_t buttonid) override;

private:
    float get_exp_percent() const;
    float get_hp_percent() const;
    float get_mp_percent() const;

    enum Buttons : std::uint16_t {
        BT_WHISPER,
        BT_CALLGM,
        BT_CASHSHOP,
        BT_TRADE,
        BT_MENU,
        BT_OPTIONS,
        BT_CHARACTER,
        BT_STATS,
        BT_QUEST,
        BT_INVENTORY,
        BT_EQUIPS,
        BT_SKILL,
        BT_KEYSETTING
    };

    static constexpr Point<std::int16_t> POSITION = {512, 758}; // {512, 590}
    static constexpr Point<std::int16_t> DIMENSION = {1366, 80};
    static constexpr time_t MESSAGE_COOLDOWN = 1'000;

    const CharStats& stats;

    EnumMap<Messages::Type, time_t> message_cooldowns;

    UIChatbar chatbar;
    Gauge expbar;
    Gauge hpbar;
    Gauge mpbar;
    Charset statset;
    Charset levelset;
    Text namelabel;
    Text joblabel;
    Animation hpanimation;
    Animation mpanimation;
};
} // namespace jrc
