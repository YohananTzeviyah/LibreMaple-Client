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
#include "../../Util/Randomizer.h"
#include "../UIElement.h"

#include <functional>
#include <string_view>

namespace jrc
{
//! Keyboard which is used via the mouse. The game uses this for pic/pin input.
class UISoftkey : public UIElement
{
public:
    using Callback = std::function<void(std::string_view)>;

    static constexpr Type TYPE = SOFT_KEYBOARD;
    static constexpr bool FOCUSED = true;
    static constexpr bool TOGGLED = false;

    UISoftkey(Callback callback);

    void draw(float alpha) const override;

protected:
    Button::State button_pressed(std::uint16_t) override;

private:
    void shufflekeys();
    Point<std::int16_t> key_pos(std::uint8_t) const;

    enum Buttons {
        BT_0,
        BT_1,
        BT_2,
        BT_3,
        BT_4,
        BT_5,
        BT_6,
        BT_7,
        BT_8,
        BT_9,
        BT_NEXT,
        BT_BACK,
        BT_CANCEL,
        BT_OK
    };

    static constexpr const std::size_t MIN_SIZE = 6;
    static constexpr const std::size_t MAX_SIZE = 12;
    static constexpr const std::uint8_t NUM_KEYS = 10;

    Callback callback;
    Text entry;
};
} // namespace jrc
