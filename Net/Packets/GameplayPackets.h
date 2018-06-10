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
#include "../../IO/Keyboard.h"
#include "MovementPacket.h"

#include <string_view>
#include <unordered_map>

namespace jrc
{
//! Requests the server to warp the player to a different map.
//! Opcode: CHANGE_MAP(38)
class ChangeMapPacket : public OutPacket
{
public:
    ChangeMapPacket(bool died,
                    std::int32_t targetid,
                    std::string_view targetp,
                    bool usewheel)
        : OutPacket(CHANGEMAP)
    {
        write_byte(died);
        write_int(targetid);
        write_string(targetp);
        skip(1);
        write_short(static_cast<std::int16_t>(usewheel ? 1 : 0));
    }
};

//! Updates the player's position with the server.
//! Opcode: MOVE_PLAYER(41)
class MovePlayerPacket : public MovementPacket
{
public:
    MovePlayerPacket(const Movement& movement) : MovementPacket(MOVE_PLAYER)
    {
        skip(9);
        write_byte(1);
        writemovement(movement);
    }
};

//! Requests one or more new keybindings to be registered.
//! Opcode: CHANGE_KEYMAP(0x87)
class ChangeKeymapPacket : public OutPacket
{
public:
    ChangeKeymapPacket(
        const std::unordered_map<std::int32_t, Keyboard::Mapping>& maplekeys)
        : OutPacket(CHANGE_KEYMAP)
    {
        // Mode
        write_int(0);

        // Number of changes
        write_int(static_cast<std::int32_t>(maplekeys.size()));

        for (const auto& [key, mapping] : maplekeys) {
            const auto& [type, action] = mapping;

            write_int(key);
            write_byte(static_cast<std::int8_t>(type));
            write_int(action);
        }
    }
};

//! Requests various party-related things.
//! Opcode: PARTY_OPERATION(124)
class PartyOperationPacket : public OutPacket
{
public:
    enum Operation : std::int8_t {
        CREATE = 1,
        LEAVE = 2,
        JOIN = 3,
        INVITE = 4,
        EXPEL = 5,
        PASS_LEADER = 6
    };

protected:
    PartyOperationPacket(Operation op) : OutPacket(PARTY_OPERATION)
    {
        write_byte(op);
    }
};

//! Creates a new party.
//! Operation: CREATE(1)
class CreatePartyPacket : public PartyOperationPacket
{
public:
    CreatePartyPacket() : PartyOperationPacket(CREATE)
    {
    }
};

//! Leaves a party.
//! Operation: LEAVE(2)
class LeavePartyPacket : public PartyOperationPacket
{
public:
    LeavePartyPacket() : PartyOperationPacket(LEAVE)
    {
    }
};

//! Joins a party.
//! Operation: JOIN(3)
class JoinPartyPacket : public PartyOperationPacket
{
public:
    JoinPartyPacket(std::int32_t party_id) : PartyOperationPacket(JOIN)
    {
        write_int(party_id);
    }
};

//! Invites a player to a party.
//! Operation: INVITE(4)
class InviteToPartyPacket : public PartyOperationPacket
{
public:
    InviteToPartyPacket(std::string_view name) : PartyOperationPacket(INVITE)
    {
        write_string(name);
    }
};

//! Expels someone from a party.
//! Operation: EXPEL(5)
class ExpelFromPartyPacket : public PartyOperationPacket
{
public:
    ExpelFromPartyPacket(std::int32_t cid) : PartyOperationPacket(EXPEL)
    {
        write_int(cid);
    }
};

//! Passes party leadership to another character.
//! Operation: PASS_LEADER(6)
class ChangePartyLeaderPacket : public PartyOperationPacket
{
public:
    ChangePartyLeaderPacket(std::int32_t cid)
        : PartyOperationPacket(PASS_LEADER)
    {
        write_int(cid);
    }
};

//! Updates a mob's position with the server.
//! Opcode: MOVE_MONSTER(188)
class MoveMobPacket : public MovementPacket
{
public:
    MoveMobPacket(std::int32_t oid,
                  std::int16_t type,
                  std::int8_t skillb,
                  std::int8_t skill0,
                  std::int8_t skill1,
                  std::int8_t skill2,
                  std::int8_t skill3,
                  std::int8_t skill4,
                  Point<std::int16_t> startpos,
                  const Movement& movement)
        : MovementPacket(MOVE_MONSTER)
    {
        write_int(oid);
        write_short(type);
        write_byte(skillb);
        write_byte(skill0);
        write_byte(skill1);
        write_byte(skill2);
        write_byte(skill3);
        write_byte(skill4);

        skip(13);

        write_point(startpos);

        write_byte(1);
        writemovement(movement);
    }
};

//! Requests picking up an item.
//! Opcode: PICKUP_ITEM(202)
class PickupItemPacket : public OutPacket
{
public:
    PickupItemPacket(std::int32_t oid, Point<std::int16_t> position)
        : OutPacket(PICKUP_ITEM)
    {
        write_int(0);
        write_byte(0);
        write_point(position);
        write_int(oid);
    }
};

//! Tells the server that we're no longer transitioning between maps,
//! including initial login.
//! Opcode: PLAYER_UPDATE(0xDF)
class PlayerUpdatePacket : public OutPacket
{
public:
    PlayerUpdatePacket() : OutPacket(PLAYER_UPDATE)
    {
    }
};
} // namespace jrc
