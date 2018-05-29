//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include "../../Character/Look/CharLook.h"
#include "../../Graphics/Sprite.h"
#include "../../Net/Login.h"
#include "../Components/Charset.h"
#include "../Components/Nametag.h"
#include "../UIElement.h"

namespace jrc
{
// The character selection screen.
class UICharSelect : public UIElement
{
public:
    static constexpr Type TYPE = CHARSELECT;
    static constexpr bool FOCUSED = false;
    static constexpr bool TOGGLED = false;

    UICharSelect(std::vector<CharEntry> characters,
                 std::uint8_t count,
                 std::uint8_t slots,
                 std::uint8_t channel_id,
                 std::int8_t pic);

    void draw(float alpha) const override;
    void update() override;
    Button::State button_pressed(std::uint16_t id) override;

    void add_character(CharEntry&& character);
    void remove_char(std::int32_t cid);

    const CharEntry& get_character(std::int32_t cid);

private:
    void send_selection();
    void send_deletion();
    void update_selection();
    void update_counts();
    std::string get_label_string(std::size_t label) const;
    Point<std::int16_t> get_label_pos(std::size_t label) const;
    Point<std::int16_t> get_char_pos(std::size_t id) const;

    enum Buttons {
        BT_CREATECHAR,
        BT_DELETECHAR,
        BT_SELECTCHAR,
        BT_ARBEIT,
        BT_CARDS,
        BT_PAGELEFT,
        BT_PAGERIGHT,
        BT_CHAR0
    };

    static constexpr std::uint8_t PAGESIZE = 8;

    Sprite emptyslot;
    Charset levelset;
    nl::node nametag;

    Point<std::int16_t> selworldpos;
    Point<std::int16_t> charinfopos;

    std::vector<CharEntry> characters;
    std::vector<CharLook> charlooks;
    std::vector<Nametag> nametags;
    std::int8_t require_pic;

    std::uint8_t charcount_absolute;
    std::uint8_t charcount_relative;
    std::uint8_t slots_absolute;
    std::uint8_t slots_relative;
    std::uint8_t selected_absolute;
    std::uint8_t selected_relative;
    std::uint8_t page;

    struct OutlinedText {
        Text inner;
        Text l;
        Text r;
        Text t;
        Text b;

        OutlinedText(Text::Font font, Text::Alignment alignment)
        {
            inner = Text(font, alignment, Text::WHITE);
            l = Text(font, alignment, Text::DARKGREY);
            r = Text(font, alignment, Text::DARKGREY);
            t = Text(font, alignment, Text::DARKGREY);
            b = Text(font, alignment, Text::DARKGREY);
        }

        OutlinedText() = default;

        void draw(Point<std::int16_t> parentpos) const
        {
            l.draw(parentpos + Point<std::int16_t>(-1, 0));
            r.draw(parentpos + Point<std::int16_t>(1, 0));
            t.draw(parentpos + Point<std::int16_t>(0, -1));
            b.draw(parentpos + Point<std::int16_t>(0, 1));
            inner.draw(parentpos);
        }

        void change_text(const std::string& text)
        {
            inner.change_text(text);
            l.change_text(text);
            r.change_text(text);
            t.change_text(text);
            b.change_text(text);
        }
    };
    OutlinedText namelabel;

    static const std::size_t NUM_LABELS = 7;
    enum InfoLabel { JOB, WORLDRANK, JOBRANK, STR, DEX, INT, LUK };
    OutlinedText infolabels[NUM_LABELS];
};
} // namespace jrc
