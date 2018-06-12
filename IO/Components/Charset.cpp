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
#include "Charset.h"

namespace jrc
{
Charset::Charset(nl::node src, Alignment alignment) : alignment(alignment)
{
    for (auto sub : src) {
        std::string name = sub.name();
        if (sub.data_type() != nl::node::type::bitmap || name.empty()) {
            continue;
        }

        char c = *name.begin();
        if (c == '\\') {
            c = '/';
        }
        chars.emplace(c, sub);
    }
}

Charset::Charset() noexcept : alignment(LEFT)
{
}

void Charset::draw(std::int8_t c, const DrawArgument& args) const
{
    auto iter = chars.find(c);
    if (iter != chars.end()) {
        iter->second.draw(args);
    }
}

std::int16_t Charset::get_w(std::int8_t c) const
{
    auto iter = chars.find(c);
    return static_cast<std::int16_t>(iter != chars.end() ? iter->second.width()
                                                         : 0);
}

std::int16_t Charset::draw(std::string_view text,
                           const DrawArgument& args) const
{
    std::int16_t shift = 0;
    std::int16_t total = 0;

    switch (alignment) {
    case CENTER:
        for (char c : text) {
            total += get_w(c);
        }
        shift -= total / 2;
    case LEFT:
        for (char c : text) {
            draw(c, args + Point<std::int16_t>{shift, 0});
            shift += get_w(c);
        }
        break;
    case RIGHT:
        for (auto iter = text.rbegin(); iter != text.rend(); ++iter) {
            char c = *iter;
            shift += get_w(c);
            draw(c, args - Point<std::int16_t>{shift, 0});
        }
        break;
    }
    return shift;
}

std::int16_t Charset::draw(std::string_view text,
                           std::int16_t hspace,
                           const DrawArgument& args) const
{
    std::int16_t shift = 0;

    switch (alignment) {
    case CENTER:
        shift -= hspace * static_cast<std::int16_t>(text.length()) / 2;
    case LEFT:
        for (char c : text) {
            draw(c, args + Point<std::int16_t>(shift, 0));
            shift += hspace;
        }
        break;
    case RIGHT:
        for (auto iter = text.rbegin(); iter != text.rend(); ++iter) {
            char c = *iter;
            shift += hspace;
            draw(c, args - Point<std::int16_t>(shift, 0));
        }
        break;
    }
    return shift;
}
} // namespace jrc
