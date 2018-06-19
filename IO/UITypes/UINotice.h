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
#include "../../Graphics/Texture.h"
#include "../Components/Textfield.h"
#include "../UIElement.h"

#include <functional>

namespace jrc
{
class UINotice : public UIElement
{
public:
    static constexpr const Type TYPE = NOTICE;
    static constexpr const bool FOCUSED = true;
    static constexpr const bool TOGGLED = false;

protected:
    UINotice(std::string&& question);

    void draw_notice(bool text_field) const;

    std::int16_t box2offset() const;

private:
    Texture top;
    Texture center;
    Texture center_box;
    Texture box;
    Texture box2;
    Texture bottom;
    Texture bottom_box;
    Text question;
    std::int16_t height;
};

class UIYesNo : public UINotice
{
public:
    UIYesNo(std::string&& question,
            std::function<void(bool yes)> yes_no_handler);

    void draw(float alpha) const override;

protected:
    Button::State button_pressed(std::uint16_t button_id) override;

private:
    enum Buttons : std::int16_t { YES, NO };

    std::function<void(bool yes)> yes_no_handler;
};

class UIEnterNumber : public UINotice
{
public:
    UIEnterNumber(std::string&& question,
                  std::function<void(std::int32_t number)> n_handler,
                  std::int32_t min,
                  std::int32_t max,
                  std::int32_t def);

    void draw(float alpha) const override;
    void update() override;

    Cursor::State send_cursor(bool pressed,
                              Point<std::int16_t> cursor_pos) override;

protected:
    Button::State button_pressed(std::uint16_t button_id) override;

private:
    void handle_string(const std::string& num_str);

    enum Buttons : std::int16_t { OK, CANCEL };

    std::function<void(std::int32_t number)> num_handler;
    Textfield num_field;
    std::int32_t min;
    std::int32_t max;
};
} // namespace jrc
