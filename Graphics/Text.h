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
#include "DrawArgument.h"

#include <cstdint>
#include <map>
#include <vector>

namespace jrc
{
class Text
{
public:
    enum Font { A11L, A11M, A11B, A12M, A12B, A13M, A13B, A18M, NUM_FONTS };

    enum Alignment { LEFT, CENTER, RIGHT };

    enum Color {
        BLACK,
        WHITE,
        YELLOW,
        BLUE,
        RED,
        DARKRED,
        BROWN,
        LIGHTGREY,
        DARKGREY,
        ORANGE,
        MEDIUMBLUE,
        VIOLET,
        NUM_COLORS
    };

    enum Background { NONE, NAMETAG };

    class Layout
    {
    public:
        struct Word {
            std::size_t first;
            std::size_t last;
            Font font;
            Color color;
        };

        struct Line {
            std::vector<Word> words;
            Point<std::int16_t> position;
        };

        Layout(const std::vector<Line>& lines,
               const std::vector<std::int16_t>& advances,
               std::int16_t width,
               std::int16_t height,
               std::int16_t endx,
               std::int16_t endy);
        Layout();

        std::int16_t width() const;
        std::int16_t height() const;
        std::int16_t advance(std::size_t index) const;
        Point<std::int16_t> get_dimensions() const;
        Point<std::int16_t> get_endoffset() const;

        using iterator = std::vector<Line>::const_iterator;
        iterator begin() const;
        iterator end() const;

    private:
        std::vector<Line> lines;
        std::vector<std::int16_t> advances;
        Point<std::int16_t> dimensions;
        Point<std::int16_t> endoffset;
    };

    Text(Font font,
         Alignment alignment,
         Color color,
         Background background,
         std::string&& text = "",
         std::uint16_t maxwidth = 0,
         bool formatted = true) noexcept;
    Text(Font font,
         Alignment alignment,
         Color color,
         std::string&& text = "",
         std::uint16_t maxwidth = 0,
         bool formatted = true) noexcept;
    Text() noexcept;

    void draw(const DrawArgument& args) const;

    void change_text(std::string&& text);
    void change_color(Color color);
    void set_background(Background background);

    bool empty() const;
    std::size_t length() const;
    std::int16_t width() const;
    std::int16_t height() const;
    std::uint16_t advance(std::size_t pos) const;
    Point<std::int16_t> dimensions() const;
    Point<std::int16_t> endoffset() const;
    std::string_view get_text() const noexcept;

private:
    void reset_layout() noexcept;

    Font font;
    Alignment alignment;
    Color color;
    Background background;
    Layout layout;
    std::uint16_t max_width;
    bool formatted;
    std::string text;
};
} // namespace jrc
