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
#include "../../Template/BoolPair.h"
#include "../Components/Textfield.h"
#include "../UIElement.h"

namespace jrc
{
class UILogin : public UIElement
{
public:
    static constexpr const Type TYPE = LOGIN;
    static constexpr const bool FOCUSED = false;
    static constexpr const bool TOGGLED = false;

    UILogin();

    void draw(float alpha) const override;
    void update() override;

    Cursor::State send_cursor(bool clicked,
                              Point<std::int16_t> cursor_pos) override;

protected:
    Button::State button_pressed(std::uint16_t id) override;

private:
    void login();

    enum Buttons {
        BT_LOGIN,
        BT_REGISTER,
        BT_HOME_PAGE,
        BT_PASS_LOST,
        BT_ID_LOST,
        BT_SAVE_ID,
        BT_QUIT,
        NUM_BUTTONS
    };

    Textfield account;
    Textfield password;
    Texture account_bg;
    Texture password_bg;
    BoolPair<Texture> checkbox;

    bool save_id;
};
} // namespace jrc
