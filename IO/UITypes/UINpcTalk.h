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
#include "../../Graphics/Texture.h"
#include "../UIElement.h"

namespace jrc
{
class UINpcTalk : public UIElement
{
public:
    static constexpr Type TYPE = NPCTALK;
    static constexpr bool FOCUSED = false;
    static constexpr bool TOGGLED = true;

    UINpcTalk();

    void draw(float inter) const override;

    void change_text(std::int32_t npcid,
                     std::int8_t msgtype,
                     std::int16_t style,
                     std::int8_t speaker,
                     std::string&& text);

protected:
    Button::State button_pressed(std::uint16_t buttonid) override;

private:
    enum Buttons { OK, NEXT, PREV, END, YES, NO };

    Texture top;
    Texture fill;
    Texture bottom;
    Texture nametag;

    Text text;
    Texture speaker;
    Text name;
    std::int16_t height;
    std::int16_t vtile;
    bool slider;

    std::int8_t type;
};
} // namespace jrc
