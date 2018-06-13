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
#include "../../Character/SkillBook.h"
#include "../../Graphics/Text.h"
#include "../Components/Slider.h"
#include "../UIDragElement.h"
#include "../UIElement.h"

#include <vector>

namespace jrc
{
class SkillIcon
{
public:
    SkillIcon(std::int32_t id, std::int32_t level);

    void draw(const DrawArgument& args) const;

    Cursor::State send_cursor(Point<std::int16_t> cursorpos, bool clicked);

    std::int32_t get_id() const;

private:
    enum State { NORMAL, DISABLED, MOUSE_OVER };

    Texture normal;
    Texture disabled;
    Texture mouse_over;
    Text name;
    Text level;
    std::int32_t id;

    State state;
    bool enabled;
};

class UISkillbook : public UIDragElement<PosSKILL>
{
public:
    static constexpr const Type TYPE = SKILLBOOK;
    static constexpr const bool FOCUSED = false;
    static constexpr const bool TOGGLED = true;

    UISkillbook(const CharStats& stats, const Skillbook& skillbook);

    void draw(float alpha) const override;

    void double_click(Point<std::int16_t> cursorpos) override;
    bool remove_cursor(bool clicked, Point<std::int16_t> cursorpos) override;
    Cursor::State send_cursor(bool clicked,
                              Point<std::int16_t> cursorpos) override;

    void update_stat(Maplestat::Id stat, std::int16_t value);
    void update_skills(std::int32_t skill_id);

protected:
    Button::State button_pressed(std::uint16_t id) override;

private:
    void change_job(std::uint16_t id);
    void change_sp(std::int16_t value);
    void change_tab(std::uint16_t new_tab);
    void change_offset(std::uint16_t new_offset);

    void show_skill(std::int32_t skill_id);
    void clear_tooltip();

    bool can_raise(std::int32_t skill_id) const;
    void send_spup(std::uint16_t row);

    Job::Level job_level_by_tab(std::uint16_t tab) const;
    SkillIcon* icon_by_position(Point<std::int16_t> cursor_pos);

    enum Buttons : std::uint16_t {
        BT_GUILD,
        BT_HYPER,
        BT_TAB0,
        BT_TAB1,
        BT_TAB2,
        BT_TAB3,
        BT_TAB4,
        BT_SPUP0,
        BT_SPUP1,
        BT_SPUP2,
        BT_SPUP3
    };

    static constexpr const std::int16_t ROWS = 4;
    static constexpr const std::int16_t ROW_HEIGHT = 40;
    static constexpr const Point<std::int16_t> SKILL_OFFSET{11, 93};
    static constexpr const Point<std::int16_t> ICON_OFFSET{2, 33};
    static constexpr const Point<std::int16_t> LINE_OFFSET{2, 37};

    const CharStats& stats;
    const Skillbook& skillbook;

    Slider slider;
    Texture skill_e;
    Texture skill_d;
    Texture line;
    Texture book_icon;
    Text book_text;
    Text sp_label;

    Job job;
    std::int16_t sp;

    std::uint16_t tab;
    std::uint16_t skill_count;
    std::uint16_t offset;

    std::vector<SkillIcon> icons;
};
} // namespace jrc
