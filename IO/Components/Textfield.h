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
#include "../../Graphics/Text.h"
#include "../../Template/Rectangle.h"
#include "../Cursor.h"
#include "../Keyboard.h"

#include <functional>
#include <string_view>
#include <unordered_map>

namespace jrc
{
class Textfield
{
public:
    enum State { NORMAL, DISABLED, FOCUSED };

    Textfield(Text::Font font,
              Text::Alignment alignment,
              Text::Color color,
              Rectangle<std::int16_t> bounds,
              std::size_t limit) noexcept;
    Textfield();
    ~Textfield();

    void draw(Point<std::int16_t> position) const;
    void update(Point<std::int16_t> parentpos) noexcept;
    void send_key(KeyType::Id type, std::int32_t code, bool down) noexcept;
    void add_string(std::string_view str) noexcept;

    void set_state(State state) noexcept;
    void change_text(std::string&& text) noexcept;
    void set_crypt_char(char c) noexcept;

    void set_enter_callback(
        std::function<void(const std::string&)> on_ret) noexcept;
    void set_key_callback(std::int32_t key,
                          std::function<void()> action) noexcept;

    Cursor::State send_cursor(Point<std::int16_t> cursorpos,
                              bool clicked) noexcept;

    bool empty() const noexcept;
    State get_state() const noexcept;
    Rectangle<std::int16_t> get_bounds() const noexcept;
    const std::string& get_text() const noexcept;

private:
    void text_modified() noexcept;
    bool below_limit() const noexcept;

    Text text_label;
    std::string text;
    Text marker;
    bool show_marker;
    std::uint16_t elapsed;
    std::size_t marker_pos;
    Rectangle<std::int16_t> bounds;
    Point<std::int16_t> parent_pos;
    std::size_t limit;
    char crypt;
    State state;

    std::unordered_map<std::int32_t, std::function<void()>> callbacks;
    std::function<void(const std::string&)> on_return;
};
} // namespace jrc
