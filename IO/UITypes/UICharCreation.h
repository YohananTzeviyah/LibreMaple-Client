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
#include "../../Character/Look/CharLook.h"
#include "../../Graphics/Text.h"
#include "../../Graphics/Texture.h"
#include "../../Template/BoolPair.h"
#include "../../Util/Randomizer.h"
#include "../Components/Textfield.h"
#include "../UIElement.h"

namespace jrc
{
//! Character creation screen.
class UICharCreation : public UIElement
{
public:
    static constexpr const Type TYPE = CHAR_CREATION;
    static constexpr const bool FOCUSED = false;
    static constexpr const bool TOGGLED = false;

    UICharCreation();

    void draw(float) const override;
    void update() override;

    Cursor::State send_cursor(bool, Point<std::int16_t>) override;

    void send_naming_result(bool success);

protected:
    Button::State button_pressed(std::uint16_t button_id) override;

private:
    void randomize_look();
    [[nodiscard]] std::string_view get_equip_name(Equipslot::Id slot) const
        noexcept;

    enum Buttons {
        BT_CHARC_OK,
        BT_CHARC_CANCEL,
        BT_CHARC_FACEL,
        BT_CHARC_FACER,
        BT_CHARC_HAIRL,
        BT_CHARC_HAIRR,
        BT_CHARC_HAIRCL,
        BT_CHARC_HAIRCR,
        BT_CHARC_SKINL,
        BT_CHARC_SKINR,
        BT_CHARC_TOPL,
        BT_CHARC_TOPR,
        BT_CHARC_BOTL,
        BT_CHARC_BOTR,
        BT_CHARC_SHOESL,
        BT_CHARC_SHOESR,
        BT_CHARC_WEPL,
        BT_CHARC_WEPR,
        BT_CHARC_GENDERL,
        BT_CHARC_GENDERR,
    };

    std::vector<Sprite> sprites_look_board;
    Texture sky;
    Texture cloud;
    float cloud_fx;
    Texture name_board;
    Textfield name_char;
    CharLook new_char;

    BoolPair<std::vector<std::uint8_t>> skins;
    BoolPair<std::vector<std::uint8_t>> hair_colors;
    BoolPair<std::vector<std::int32_t>> faces;
    BoolPair<std::vector<std::int32_t>> hairs;
    BoolPair<std::vector<std::int32_t>> tops;
    BoolPair<std::vector<std::int32_t>> bots;
    BoolPair<std::vector<std::int32_t>> shoes;
    BoolPair<std::vector<std::int32_t>> weapons;

    bool named;
    bool female;
    std::size_t skin;
    std::size_t hair_color;
    std::size_t face;
    std::size_t hair;
    std::size_t top;
    std::size_t bottom;
    std::size_t shoe;
    std::size_t weapon;
    Text face_name;
    Text hair_name;
    Text hair_cname;
    Text body_name;
    Text top_name;
    Text bottom_name;
    Text shoe_name;
    Text weapon_name;
    Text gender_name;
};
} // namespace jrc
