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
#include "../../Graphics/Geometry.h"
#include "../../Graphics/Texture.h"
#include "../Components/Slider.h"
#include "../Components/Textfield.h"
#include "../UIElement.h"

#include <unordered_map>
#include <vector>

namespace jrc
{
class UIChatbar : public UIElement
{
public:
    enum ChatTarget {
        CHT_ALL,
        CHT_BUDDY,
        CHT_GUILD,
        CHT_ALLIANCE,
        CHT_PARTY,
        CHT_SQUAD,
        NUM_TARGETS
    };

    enum LineType { UNK0, WHITE, RED, BLUE, YELLOW };

    UIChatbar(Point<std::int16_t> position);

    void draw(float inter) const override;
    void update() override;

    bool is_in_range(Point<std::int16_t> cursor_pos) const override;
    bool remove_cursor(bool clicked, Point<std::int16_t> cursor_pos) override;
    Cursor::State send_cursor(bool pressed,
                              Point<std::int16_t> cursorpos) override;

    void send_line(std::string&& line, LineType type);

protected:
    Button::State button_pressed(std::uint16_t buttonid) override;

private:
    std::int16_t get_chat_top() const;

    enum Buttons : std::uint16_t {
        BT_OPENCHAT,
        BT_CLOSECHAT,
        BT_SCROLLUP,
        BT_SCROLLDOWN,
        BT_CHATTARGETS
    };

    static constexpr const std::int16_t CHAT_Y_OFFSET = 65;
    static constexpr const std::int16_t CHAT_ROW_HEIGHT = 16;
    static constexpr const std::int16_t MAXCHATROWS = 16;
    static constexpr const std::int16_t MINCHATROWS = 1;

    Textfield chat_field;
    Texture chatspace[2];
    Texture chattargets[NUM_TARGETS];
    Texture chatenter;
    Texture chatcover;
    Texture tapbar;
    Texture tapbartop;

    bool chatopen;
    ChatTarget chattarget;

    std::vector<std::string> last_entered;
    std::size_t lastpos;

    std::unordered_map<std::int16_t, Text> row_texts;
    ColorBox chat_box;
    std::int16_t chat_rows;
    std::int16_t row_pos;
    std::int16_t row_max;
    Slider slider;
    bool drag_chat_top;
};
} // namespace jrc
