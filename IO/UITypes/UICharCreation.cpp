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
#include "UICharCreation.h"

#include "../../Constants.h"
#include "../../Data/ItemData.h"
#include "../../Net/Packets/CharCreationPackets.h"
#include "../Components/MapleButton.h"
#include "../UI.h"
#include "../UITypes/UICharSelect.h"
#include "../UITypes/UILoginNotice.h"
#include "nlnx/nx.hpp"

namespace jrc
{
UICharCreation::UICharCreation()
{
    nl::node src = nl::nx::ui["Login.img"];
    nl::node bg_src = nl::nx::map["Back"]["login.img"]["back"];
    nl::node cr_src = src["NewChar"];

    sky = bg_src["2"];
    cloud = bg_src["27"];

    sprites.emplace_back(bg_src["15"], Point<std::int16_t>{153, 685});
    sprites.emplace_back(bg_src["16"], Point<std::int16_t>{200, 400});
    sprites.emplace_back(bg_src["17"], Point<std::int16_t>{160, 263});
    sprites.emplace_back(bg_src["18"], Point<std::int16_t>{349, 1220});
    sprites.emplace_back(src["Common"]["frame"],
                         Point<std::int16_t>{400, 290});

    name_board = cr_src["charName"];

    sprites_look_board.emplace_back(cr_src["charSet"],
                                   Point<std::int16_t>{450, 115});
    sprites_look_board.emplace_back(cr_src["avatarSel"]["0"]["normal"],
                                   Point<std::int16_t>{461, 217});
    sprites_look_board.emplace_back(cr_src["avatarSel"]["1"]["normal"],
                                   Point<std::int16_t>{461, 236});
    sprites_look_board.emplace_back(cr_src["avatarSel"]["2"]["normal"],
                                   Point<std::int16_t>{461, 255});
    sprites_look_board.emplace_back(cr_src["avatarSel"]["3"]["normal"],
                                   Point<std::int16_t>{461, 274});
    sprites_look_board.emplace_back(cr_src["avatarSel"]["4"]["normal"],
                                   Point<std::int16_t>{461, 293});
    sprites_look_board.emplace_back(cr_src["avatarSel"]["5"]["normal"],
                                   Point<std::int16_t>{461, 312});
    sprites_look_board.emplace_back(cr_src["avatarSel"]["6"]["normal"],
                                   Point<std::int16_t>{461, 331});
    sprites_look_board.emplace_back(cr_src["avatarSel"]["7"]["normal"],
                                   Point<std::int16_t>{461, 350});
    sprites_look_board.emplace_back(cr_src["avatarSel"]["8"]["normal"],
                                   Point<std::int16_t>{461, 369});

    buttons[BT_CHARC_OK] = std::make_unique<MapleButton>(
        cr_src["BtYes"], Point<std::int16_t>{482, 292});
    buttons[BT_CHARC_CANCEL] = std::make_unique<MapleButton>(
        cr_src["BtNo"], Point<std::int16_t>{555, 292});
    buttons[BT_CHARC_FACEL] = std::make_unique<MapleButton>(
        cr_src["BtLeft"], Point<std::int16_t>{521, 216});
    buttons[BT_CHARC_FACER] = std::make_unique<MapleButton>(
        cr_src["BtRight"], Point<std::int16_t>{645, 216});
    buttons[BT_CHARC_HAIRL] = std::make_unique<MapleButton>(
        cr_src["BtLeft"], Point<std::int16_t>{521, 235});
    buttons[BT_CHARC_HAIRR] = std::make_unique<MapleButton>(
        cr_src["BtRight"], Point<std::int16_t>{645, 235});
    buttons[BT_CHARC_HAIRCL] = std::make_unique<MapleButton>(
        cr_src["BtLeft"], Point<std::int16_t>{521, 254});
    buttons[BT_CHARC_HAIRCR] = std::make_unique<MapleButton>(
        cr_src["BtRight"], Point<std::int16_t>{645, 254});
    buttons[BT_CHARC_SKINL] = std::make_unique<MapleButton>(
        cr_src["BtLeft"], Point<std::int16_t>{521, 273});
    buttons[BT_CHARC_SKINR] = std::make_unique<MapleButton>(
        cr_src["BtRight"], Point<std::int16_t>{645, 273});
    buttons[BT_CHARC_TOPL] = std::make_unique<MapleButton>(
        cr_src["BtLeft"], Point<std::int16_t>{521, 292});
    buttons[BT_CHARC_TOPR] = std::make_unique<MapleButton>(
        cr_src["BtRight"], Point<std::int16_t>{645, 292});
    buttons[BT_CHARC_BOTL] = std::make_unique<MapleButton>(
        cr_src["BtLeft"], Point<std::int16_t>{521, 311});
    buttons[BT_CHARC_BOTR] = std::make_unique<MapleButton>(
        cr_src["BtRight"], Point<std::int16_t>{645, 311});
    buttons[BT_CHARC_SHOESL] = std::make_unique<MapleButton>(
        cr_src["BtLeft"], Point<std::int16_t>{521, 330});
    buttons[BT_CHARC_SHOESR] = std::make_unique<MapleButton>(
        cr_src["BtRight"], Point<std::int16_t>{645, 330});
    buttons[BT_CHARC_WEPL] = std::make_unique<MapleButton>(
        cr_src["BtLeft"], Point<std::int16_t>{521, 349});
    buttons[BT_CHARC_WEPR] = std::make_unique<MapleButton>(
        cr_src["BtRight"], Point<std::int16_t>{645, 348});
    buttons[BT_CHARC_GENDERL] = std::make_unique<MapleButton>(
        cr_src["BtLeft"], Point<std::int16_t>{521, 368});
    buttons[BT_CHARC_GEMDERR] = std::make_unique<MapleButton>(
        cr_src["BtRight"], Point<std::int16_t>{645, 368});

    buttons[BT_CHARC_FACEL]->set_active(false);
    buttons[BT_CHARC_FACER]->set_active(false);
    buttons[BT_CHARC_HAIRL]->set_active(false);
    buttons[BT_CHARC_HAIRR]->set_active(false);
    buttons[BT_CHARC_HAIRCL]->set_active(false);
    buttons[BT_CHARC_HAIRCR]->set_active(false);
    buttons[BT_CHARC_SKINL]->set_active(false);
    buttons[BT_CHARC_SKINR]->set_active(false);
    buttons[BT_CHARC_TOPL]->set_active(false);
    buttons[BT_CHARC_TOPR]->set_active(false);
    buttons[BT_CHARC_BOTL]->set_active(false);
    buttons[BT_CHARC_BOTR]->set_active(false);
    buttons[BT_CHARC_SHOESL]->set_active(false);
    buttons[BT_CHARC_SHOESR]->set_active(false);
    buttons[BT_CHARC_WEPL]->set_active(false);
    buttons[BT_CHARC_WEPR]->set_active(false);
    buttons[BT_CHARC_GENDERL]->set_active(false);
    buttons[BT_CHARC_GEMDERR]->set_active(false);

    name_char = {
        Text::A13M, Text::LEFT, Text::WHITE, {{490, 219}, {630, 243}}, 12};
    name_char.set_state(Textfield::FOCUSED);

    face_name = {Text::A11M, Text::CENTER, Text::BLACK};
    hair_name = {Text::A11M, Text::CENTER, Text::BLACK};
    hair_cname = {Text::A11M, Text::CENTER, Text::BLACK};
    body_name = {Text::A11M, Text::CENTER, Text::BLACK};
    top_name = {Text::A11M, Text::CENTER, Text::BLACK};
    bottom_name = {Text::A11M, Text::CENTER, Text::BLACK};
    shoe_name = {Text::A11M, Text::CENTER, Text::BLACK};
    weapon_name = {Text::A11M, Text::CENTER, Text::BLACK};
    gender_name = {Text::A11M, Text::CENTER, Text::BLACK};

    nl::node mkinfo = nl::nx::etc["MakeCharInfo.img"]["Info"];
    for (auto i = 0; i < 2; ++i) {
        bool f;
        nl::node mk_n;
        if (i == 0) {
            f = true;
            mk_n = mkinfo["CharFemale"];
        } else {
            f = false;
            mk_n = mkinfo["CharMale"];
        }

        for (auto sub_node : mk_n) {
            int num = stoi(sub_node.name());
            for (auto id_node : sub_node) {
                int value = id_node;
                switch (num) {
                case 0:
                    faces[f].push_back(value);
                    break;
                case 1:
                    hairs[f].push_back(value);
                    break;
                case 2:
                    hair_colors[f].push_back(static_cast<std::uint8_t>(value));
                    break;
                case 3:
                    skins[f].push_back(static_cast<std::uint8_t>(value));
                    break;
                case 4:
                    tops[f].push_back(value);
                    break;
                case 5:
                    bots[f].push_back(value);
                    break;
                case 6:
                    shoes[f].push_back(value);
                    break;
                case 7:
                    weapons[f].push_back(value);
                    break;
                }
            }
        }
    }

    female = false;
    randomize_look();

    new_char.set_direction(true);

    position = {0, 0};
    dimension = {Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT};
    active = true;
    cloud_fx = 200.0f;
}

void UICharCreation::randomize_look()
{
    hair = Randomizer::next_int(hairs[female].size());
    face = Randomizer::next_int(faces[female].size());
    skin = Randomizer::next_int(skins[female].size());
    hair_color = Randomizer::next_int(hair_colors[female].size());
    top = Randomizer::next_int(tops[female].size());
    bottom = Randomizer::next_int(bots[female].size());
    shoe = Randomizer::next_int(shoes[female].size());
    weapon = Randomizer::next_int(weapons[female].size());

    new_char.set_body(skins[female][skin]);
    new_char.set_face(faces[female][face]);
    new_char.set_hair(hairs[female][hair] + hair_colors[female][hair_color]);
    new_char.add_equip(tops[female][top]);
    new_char.add_equip(bots[female][bottom]);
    new_char.add_equip(shoes[female][shoe]);
    new_char.add_equip(weapons[female][weapon]);

    body_name.change_text(std::string{new_char.get_body()->get_name()});
    face_name.change_text(std::string{new_char.get_face()->get_name()});
    hair_name.change_text(std::string{new_char.get_hair()->get_name()});
    hair_cname.change_text(std::string{new_char.get_hair()->get_color()});

    top_name.change_text(std::string{get_equip_name(Equipslot::TOP)});
    bottom_name.change_text(std::string{get_equip_name(Equipslot::PANTS)});
    shoe_name.change_text(std::string{get_equip_name(Equipslot::SHOES)});
    weapon_name.change_text(std::string{get_equip_name(Equipslot::WEAPON)});
    gender_name.change_text(female ? "Female" : "Male");
}

Button::State UICharCreation::button_pressed(std::uint16_t id)
{
    switch (id) {
    case BT_CHARC_OK:
        if (named) {
            std::string cname = name_char.get_text();
            std::uint16_t cjob = 1;
            std::int32_t cface = faces[female][face];
            std::int32_t chair = hairs[female][hair];
            std::uint8_t chairc = hair_colors[female][hair_color];
            std::uint8_t cskin = skins[female][skin];
            std::int32_t ctop = tops[female][top];
            std::int32_t cbot = bots[female][bottom];
            std::int32_t cshoe = shoes[female][shoe];
            std::int32_t cwep = weapons[female][weapon];
            CreateCharPacket(cname,
                             cjob,
                             cface,
                             chair,
                             chairc,
                             cskin,
                             ctop,
                             cbot,
                             cshoe,
                             cwep,
                             female)
                .dispatch();
            return Button::PRESSED;
        } else {
            std::string name = name_char.get_text();
            if (name.size() >= 4) {
                name_char.set_state(Textfield::NORMAL);

                UI::get().disable();
                UI::get().focus_text_field(nullptr);
                NameCharPacket(name).dispatch();
                return Button::PRESSED;
            } else {
                UI::get().emplace<UILoginNotice>(UILoginNotice::ILLEGAL_NAME);
                return Button::NORMAL;
            }
        }
    case BT_CHARC_CANCEL:
        if (named) {
            buttons[BT_CHARC_OK]->set_position({482, 292});
            buttons[BT_CHARC_CANCEL]->set_position({555, 292});
            buttons[BT_CHARC_FACEL]->set_active(false);
            buttons[BT_CHARC_FACER]->set_active(false);
            buttons[BT_CHARC_HAIRL]->set_active(false);
            buttons[BT_CHARC_HAIRR]->set_active(false);
            buttons[BT_CHARC_HAIRCL]->set_active(false);
            buttons[BT_CHARC_HAIRCR]->set_active(false);
            buttons[BT_CHARC_SKINL]->set_active(false);
            buttons[BT_CHARC_SKINR]->set_active(false);
            buttons[BT_CHARC_TOPL]->set_active(false);
            buttons[BT_CHARC_TOPR]->set_active(false);
            buttons[BT_CHARC_BOTL]->set_active(false);
            buttons[BT_CHARC_BOTR]->set_active(false);
            buttons[BT_CHARC_SHOESL]->set_active(false);
            buttons[BT_CHARC_SHOESR]->set_active(false);
            buttons[BT_CHARC_WEPL]->set_active(false);
            buttons[BT_CHARC_WEPR]->set_active(false);
            buttons[BT_CHARC_GENDERL]->set_active(false);
            buttons[BT_CHARC_GEMDERR]->set_active(false);
            buttons[BT_CHARC_CANCEL]->set_state(Button::NORMAL);
            name_char.set_state(Textfield::NORMAL);
            named = false;
            return Button::NORMAL;
        } else {
            active = false;
            if (auto charselect = UI::get().get_element<UICharSelect>())
                charselect->makeactive();
            return Button::PRESSED;
        }
    }

    if (id >= BT_CHARC_FACEL && id <= BT_CHARC_GEMDERR) {
        switch (id) {
        case BT_CHARC_FACEL:
            face = (face > 0) ? face - 1 : faces[female].size() - 1;
            new_char.set_face(faces[female][face]);
            face_name.change_text(std::string{new_char.get_face()->get_name()});
            break;
        case BT_CHARC_FACER:
            face = (face < faces[female].size() - 1) ? face + 1 : 0;
            new_char.set_face(faces[female][face]);
            face_name.change_text(std::string{new_char.get_face()->get_name()});
            break;
        case BT_CHARC_HAIRL:
            hair = (hair > 0) ? hair - 1 : hairs[female].size() - 1;
            new_char.set_hair(hairs[female][hair] +
                             hair_colors[female][hair_color]);
            hair_name.change_text(std::string{new_char.get_hair()->get_name()});
            break;
        case BT_CHARC_HAIRR:
            hair = (hair < hairs[female].size() - 1) ? hair + 1 : 0;
            new_char.set_hair(hairs[female][hair] +
                             hair_colors[female][hair_color]);
            hair_name.change_text(std::string{new_char.get_hair()->get_name()});
            break;
        case BT_CHARC_HAIRCL:
            hair_color = (hair_color > 0) ? hair_color - 1
                                        : hair_colors[female].size() - 1;
            new_char.set_hair(hairs[female][hair] +
                             hair_colors[female][hair_color]);
            hair_cname.change_text(
                std::string{new_char.get_hair()->get_color()});
            break;
        case BT_CHARC_HAIRCR:
            hair_color = (hair_color < hair_colors[female].size() - 1)
                            ? hair_color + 1
                            : 0;
            new_char.set_hair(hairs[female][hair] +
                             hair_colors[female][hair_color]);
            hair_cname.change_text(
                std::string{new_char.get_hair()->get_color()});
            break;
        case BT_CHARC_SKINL:
            skin = (skin > 0) ? skin - 1 : skins[female].size() - 1;
            new_char.set_body(skins[female][skin]);
            body_name.change_text(std::string{new_char.get_body()->get_name()});
            break;
        case BT_CHARC_SKINR:
            skin = (skin < skins[female].size() - 1) ? skin + 1 : 0;
            new_char.set_body(skins[female][skin]);
            body_name.change_text(std::string{new_char.get_body()->get_name()});
            break;
        case BT_CHARC_TOPL:
            top = (top > 0) ? top - 1 : tops[female].size() - 1;
            new_char.add_equip(tops[female][top]);
            top_name.change_text(std::string{get_equip_name(Equipslot::TOP)});
            break;
        case BT_CHARC_TOPR:
            top = (top < tops[female].size() - 1) ? top + 1 : 0;
            new_char.add_equip(tops[female][top]);
            top_name.change_text(std::string{get_equip_name(Equipslot::TOP)});
            break;
        case BT_CHARC_BOTL:
            bottom = (bottom > 0) ? bottom - 1 : bots[female].size() - 1;
            new_char.add_equip(bots[female][bottom]);
            bottom_name.change_text(std::string{get_equip_name(Equipslot::PANTS)});
            break;
        case BT_CHARC_BOTR:
            bottom = (bottom < bots[female].size() - 1) ? bottom + 1 : 0;
            new_char.add_equip(bots[female][bottom]);
            bottom_name.change_text(std::string{get_equip_name(Equipslot::PANTS)});
            break;
        case BT_CHARC_SHOESL:
            shoe = (shoe > 0) ? shoe - 1 : shoes[female].size() - 1;
            new_char.add_equip(shoes[female][shoe]);
            shoe_name.change_text(
                std::string{get_equip_name(Equipslot::SHOES)});
            break;
        case BT_CHARC_SHOESR:
            shoe = (shoe < shoes[female].size() - 1) ? shoe + 1 : 0;
            new_char.add_equip(shoes[female][shoe]);
            shoe_name.change_text(
                std::string{get_equip_name(Equipslot::SHOES)});
            break;
        case BT_CHARC_WEPL:
            weapon = (weapon > 0) ? weapon - 1 : weapons[female].size() - 1;
            new_char.add_equip(weapons[female][weapon]);
            weapon_name.change_text(
                std::string{get_equip_name(Equipslot::WEAPON)});
            break;
        case BT_CHARC_WEPR:
            weapon = (weapon < weapons[female].size() - 1) ? weapon + 1 : 0;
            new_char.add_equip(weapons[female][weapon]);
            weapon_name.change_text(
                std::string{get_equip_name(Equipslot::WEAPON)});
            break;
        case BT_CHARC_GENDERL:
        case BT_CHARC_GEMDERR:
            female = !female;
            randomize_look();
            break;
        }
        return Button::MOUSEOVER;
    }
    return Button::PRESSED;
}

Cursor::State UICharCreation::send_cursor(bool clicked,
                                          Point<std::int16_t> cursorpos)
{
    if (Cursor::State new_state = name_char.send_cursor(cursorpos, clicked)) {
        return new_state;
    }

    return UIElement::send_cursor(clicked, cursorpos);
}

void UICharCreation::send_naming_result(bool nameused)
{
    if (!named) {
        if (nameused) {
            name_char.change_text("");
        } else {
            named = true;
            buttons[BT_CHARC_OK]->set_position(Point<std::int16_t>{486, 445});
            buttons[BT_CHARC_CANCEL]->set_position(
                Point<std::int16_t>{560, 445});
            buttons[BT_CHARC_FACEL]->set_active(true);
            buttons[BT_CHARC_FACER]->set_active(true);
            buttons[BT_CHARC_HAIRL]->set_active(true);
            buttons[BT_CHARC_HAIRR]->set_active(true);
            buttons[BT_CHARC_HAIRCL]->set_active(true);
            buttons[BT_CHARC_HAIRCR]->set_active(true);
            buttons[BT_CHARC_SKINL]->set_active(true);
            buttons[BT_CHARC_SKINR]->set_active(true);
            buttons[BT_CHARC_TOPL]->set_active(true);
            buttons[BT_CHARC_TOPR]->set_active(true);
            buttons[BT_CHARC_BOTL]->set_active(true);
            buttons[BT_CHARC_BOTR]->set_active(true);
            buttons[BT_CHARC_SHOESL]->set_active(true);
            buttons[BT_CHARC_SHOESR]->set_active(true);
            buttons[BT_CHARC_WEPL]->set_active(true);
            buttons[BT_CHARC_WEPR]->set_active(true);
            buttons[BT_CHARC_GENDERL]->set_active(true);
            buttons[BT_CHARC_GEMDERR]->set_active(true);
            name_char.set_state(Textfield::DISABLED);
        }
        buttons[BT_CHARC_OK]->set_state(Button::NORMAL);
    }
}

void UICharCreation::draw(float alpha) const
{
    for (std::int16_t i = 0; i < 2; ++i) {
        for (std::int16_t k = 0; k < Constants::VIEW_WIDTH; k += sky.width()) {
            sky.draw(Point<std::int16_t>{
                k, static_cast<std::int16_t>((400 * i) - 100)});
        }
    }

    std::int16_t cloudx =
        static_cast<std::int16_t>(cloud_fx) % Constants::VIEW_WIDTH;
    cloud.draw(Point<std::int16_t>(cloudx - cloud.width(), 300));
    cloud.draw(Point<std::int16_t>{cloudx, 300});
    cloud.draw(Point<std::int16_t>(cloudx + cloud.width(), 300));

    if (!named) {
        name_board.draw(Point<std::int16_t>{455, 115});
        name_char.draw(position);
    } else {
        for (auto& sprite : sprites_look_board) {
            sprite.draw(position, alpha);
        }
    }

    UIElement::draw(alpha);

    new_char.draw({360, 348}, alpha);

    if (named) {
        face_name.draw(Point<std::int16_t>{591, 214});
        hair_name.draw(Point<std::int16_t>{591, 233});
        hair_cname.draw(Point<std::int16_t>{591, 252});
        body_name.draw(Point<std::int16_t>{591, 271});
        top_name.draw(Point<std::int16_t>{591, 290});
        bottom_name.draw(Point<std::int16_t>{591, 309});
        shoe_name.draw(Point<std::int16_t>{591, 328});
        weapon_name.draw(Point<std::int16_t>{591, 347});
        gender_name.draw(Point<std::int16_t>{591, 366});
    }
}

void UICharCreation::update()
{
    UIElement::update();

    if (named) {
        for (auto& sprite : sprites_look_board) {
            sprite.update();
        }
    }

    new_char.update(Constants::TIMESTEP);
    name_char.update(position);

    cloud_fx += 0.25f;
}

std::string_view UICharCreation::get_equip_name(Equipslot::Id slot) const
    noexcept
{
    if (std::int32_t item_id = new_char.get_equips().get_equip(slot)) {
        return ItemData::get(item_id).get_name();
    } else {
        static constexpr const std::string_view nullstr = "Missing name.";
        return nullstr;
    }
}
} // namespace jrc
