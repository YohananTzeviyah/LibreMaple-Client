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
#include "LoginHandlers.h"

#include "../../Configuration.h"
#include "../../Gameplay/Stage.h"
#include "../../IO/UI.h"
#include "../../IO/UITypes/UICharCreation.h"
#include "../../IO/UITypes/UICharSelect.h"
#include "../../IO/UITypes/UILogin.h"
#include "../../IO/UITypes/UILoginNotice.h"
#include "../../IO/UITypes/UIWorldSelect.h"
#include "../Packets/LoginPackets.h"
#include "../Session.h"
#include "Helpers/LoginParser.h"

namespace jrc
{
void LoginResultHandler::handle(InPacket& recv) const
{
    // Remove previous UIs.
    UI::get().remove(UIElement::LOGIN_NOTICE);
    UI::get().remove(UIElement::LOGIN_WAIT);

    // The packet should contain a 'reason' integer which can signify various
    // things.
    if (std::int32_t reason = recv.read_int(); reason) {
        // Login unsuccessful. The LoginNotice displayed will contain the
        // specific information.
        switch (reason) {
        case 2:
            UI::get().emplace<UILoginNotice>(16);
            break;
        case 7:
            UI::get().emplace<UILoginNotice>(17);
            break;
        case 23:
            // The server sends a request to accept the terms of service. For
            // convenience, just auto-accept.
            TOSPacket().dispatch();
            break;
        default:
            // Other reasons.
            if (reason > 0) {
                auto reasonbyte = static_cast<std::int8_t>(reason - 1);
                UI::get().emplace<UILoginNotice>(reasonbyte);
            }
        }

        UI::get().enable();
    } else {
        Console::get().print("Login successful.");
        // Login successful. The packet contains information on the account, so
        // we initialise the account with it.
        Account account = LoginParser::parse_account(recv);

        // Save the Login ID if the box for it on the login panel is checked.
        if (Configuration::get().account.save_login) {
            Configuration::get().account.account_name
                = std::move(account.name);
        }

        // Request the list of worlds and channels online.
        ServerRequestPacket{}.dispatch();
    }
}

void ServerlistHandler::handle(InPacket& recv) const
{
    // Parse all worlds.
    std::vector<World> worlds;
    while (recv.available()) {
        if (World world = LoginParser::parse_world(recv); world.wid != -1) {
            worlds.push_back(std::move(world));
        } else {
            // "End of serverlist" packet.
            return;
        }
    }

    // Remove previous UIs.
    UI::get().remove(UIElement::LOGIN);
    UI::get().remove(UIElement::CHAR_SELECT);

    // Add the world selection screen to the UI.
    UI::get().emplace<UIWorldSelect>(std::move(worlds));
    UI::get().enable();
}

void CharlistHandler::handle(InPacket& recv) const
{
    auto channel_id = static_cast<std::uint8_t>(recv.read_byte());

    // Parse all characters.
    std::vector<CharEntry> characters;
    auto charcount = static_cast<std::uint8_t>(recv.read_byte());
    for (std::uint8_t i = 0; i < charcount; ++i) {
        characters.emplace_back(LoginParser::parse_charentry(recv));
    }
    std::int8_t pic = recv.read_byte();
    auto slots = static_cast<std::uint8_t>(recv.read_int());

    // Remove previous UIs.
    UI::get().remove(UIElement::WORLD_SELECT);
    UI::get().remove(UIElement::CHAR_CREATION);

    // Add the character selection screen.
    UI::get().emplace<UICharSelect>(
        characters, charcount, slots, channel_id, pic);
    UI::get().enable();
}

void CharnameResponseHandler::handle(InPacket& recv) const
{
    // Read the name and if it is already in use.
    std::string name = recv.read_string();
    bool used = recv.read_bool();

    if (used) {
        UI::get().emplace<UILoginNotice>(UILoginNotice::NAME_IN_USE);
    }

    // Notify the character creation screen.
    if (auto charcreation = UI::get().get_element<UICharCreation>()) {
        charcreation->send_naming_result(used);
    }

    UI::get().enable();
}

void AddNewCharEntryHandler::handle(InPacket& recv) const
{
    recv.skip(1);

    // Parse info on the new character.
    CharEntry character = LoginParser::parse_charentry(recv);

    // Remove the character creation ui.
    UI::get().remove(UIElement::CHAR_CREATION);

    // Read the updated character selection.
    if (auto char_select = UI::get().get_element<UICharSelect>();
        char_select) {
        char_select->add_character(std::move(character));
        char_select->make_active();
    }

    UI::get().enable();
}

void DeleteCharResponseHandler::handle(InPacket& recv) const
{
    // Read the character ID and if deletion was successful (PIC was correct).
    std::int32_t cid = recv.read_int();
    auto state = static_cast<std::uint8_t>(recv.read_byte());

    // Extract information from the state byte.
    if (state) {
        UILoginNotice::Message message;
        switch (state) {
        case 10:
            message = UILoginNotice::BIRTHDAY_INCORRECT;
            break;
        case 20:
            message = UILoginNotice::SECOND_PASSWORD_INCORRECT;
            break;
        default:
            message = UILoginNotice::UNKNOWN_ERROR;
            break;
        }

        UI::get().emplace<UILoginNotice>(message);
    } else {
        if (auto char_select = UI::get().get_element<UICharSelect>();
            char_select) {
            char_select->remove_char(cid);
        }
    }

    UI::get().enable();
}

void ChannelChangeHandler::handle(InPacket& recv) const
{
    recv.skip(1);

    // Read the IPv4 address into a character buffer.
    std::uint8_t addr_buf[4];
    for (auto i = 0; i < 4; ++i) {
        addr_buf[i] = static_cast<std::uint8_t>(recv.read_byte());
    }
    char addr_str[16];
    std::snprintf(addr_str,
                  16,
                  "%hhu.%hhu.%hhu.%hhu",
                  addr_buf[0],
                  addr_buf[1],
                  addr_buf[2],
                  addr_buf[3]);

    // Read the port number in a string.
    const auto port_str = std::to_string(recv.read_short());

    // Attempt to reconnect to the server, and if successful, relog into the
    // game in the new channel.
    Session::get().reconnect(addr_str, port_str.c_str());
    auto cid = Stage::get().get_player().get_oid();
    PlayerLoginPacket{cid}.dispatch();
}

void ServerIPHandler::handle(InPacket& recv) const
{
    recv.skip(2);

    // Read the IPv4 address into a character buffer.
    std::uint8_t addr_buf[4];
    for (auto i = 0; i < 4; ++i) {
        addr_buf[i] = static_cast<std::uint8_t>(recv.read_byte());
    }
    char addr_str[16];
    std::snprintf(addr_str,
                  16,
                  "%hhu.%hhu.%hhu.%hhu",
                  addr_buf[0],
                  addr_buf[1],
                  addr_buf[2],
                  addr_buf[3]);

    // Read the port number in a string.
    const auto port_str = std::to_string(recv.read_short());

    auto cid = recv.read_int();

    // Attempt to reconnect to the server, and if successful, log into the
    // game.
    Session::get().reconnect(addr_str, port_str.c_str());
    PlayerLoginPacket{cid}.dispatch();
}
} // namespace jrc
