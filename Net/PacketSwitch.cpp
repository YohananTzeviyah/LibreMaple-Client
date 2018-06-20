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
#include "PacketSwitch.h"

#include "../Console.h"
#include "Handlers/AttackHandlers.h"
#include "Handlers/CommonHandlers.h"
#include "Handlers/InventoryHandlers.h"
#include "Handlers/LoginHandlers.h"
#include "Handlers/MapObjectHandlers.h"
#include "Handlers/MessagingHandlers.h"
#include "Handlers/NpcInteractionHandlers.h"
#include "Handlers/PlayerHandlers.h"
#include "Handlers/SetfieldHandlers.h"

#include <cstdint>

namespace jrc
{
//! Opcodes for InPackets.
enum PacketSwitch::Opcode : std::uint16_t {
    // Login 1
    LOGIN_RESULT = 0x00,
    SERVERLIST = 0x0A,
    CHARLIST = 0x0B,
    SERVER_IP = 0x0C,
    CHARNAME_RESPONSE = 0x0D,
    ADD_NEWCHAR_ENTRY = 0x0E,
    DELCHAR_RESPONSE = 0x0F,
    CHANNEL_CHANGE = 0x10,
    PING = 0x11,

    // Player 1
    APPLY_BUFF = 0x14,

    // Login 2
    SELECT_WORLD = 0x1A,
    RECOMMENDED_WORLDS = 0x1B,

    // Inventory 1
    MODIFY_INVENTORY = 0x1D,

    // Player 2
    CHANGE_STATS = 0x1F,
    GIVE_BUFF = 0x20,
    CANCEL_BUFF = 0x21,
    RECALCULATE_STATS = 0x23,
    UPDATE_SKILL = 0x24,

    // Messaging 1
    SHOW_STATUS_INFO = 0x27,
    MEMO_RESULT = 0x29,
    ENABLE_REPORT = 0x2F,

    // Inventory 2
    GATHER_RESULT = 0x34,
    SORT_RESULT = 0x35,

    // Player 3
    UPDATE_GENDER = 0x3A,
    BUDDY_LIST = 0x3F,
    GUILD_OPERATION = 0x41,

    // Messaging 2
    SERVER_MESSAGE = 0x44,
    WEEK_EVENT_MESSAGE = 0x4D,

    FIELD_SET_VARIABLE = 0x5C,
    FAMILY_PRIV_LIST = 0x64,
    CANCEL_RENAME_BY_OTHER = 0x78,
    SCRIPT_PROGRESS_MESSAGE = 0x7A,
    RECEIVE_POLICE = 0x7B,
    SKILL_MACROS = 0x7C,
    SET_FIELD = 0x7D,
    FIELD_EFFECT = 0x8A,
    CLOCK = 0x93,

    // Mapobject
    SPAWN_CHAR = 0xA0,
    REMOVE_CHAR = 0xA1,

    // Messaging
    CHAT_RECEIVED = 0xA2,
    SCROLL_RESULT = 0xA7,

    // Mapobject
    SPAWN_PET = 0xA8,
    CHAR_MOVED = 0xB9,

    // Attack
    ATTACKED_CLOSE = 0xBA,
    ATTACKED_RANGED = 0xBB,
    ATTACKED_MAGIC = 0xBC,

    SHOW_ITEM_EFFECT = 0xC2,
    SHOW_CHAIR = 0xC4,
    UPDATE_CHARLOOK = 0xC5,
    SHOW_FOREIGN_EFFECT = 0xC6,
    SHOW_ITEM_GAIN_INCHAT = 0xCE, // This is terribly named.
    LOCK_UI = 0xDD,
    TOGGLE_UI = 0xDE,

    // Player
    ADD_COOLDOWN = 0xEA,

    // Mapobject
    SPAWN_MOB = 0xEC,
    KILL_MOB = 0xED,
    SPAWN_MOB_C = 0xEE,
    MOB_MOVED = 0xEF,
    MOVE_MOB_RESPONSE = 0xF0,
    SHOW_MOB_HP = 0xFA,
    SPAWN_NPC = 0x0101,
    SPAWN_NPC_C = 0x0103,
    MAKE_NPC_SCRIPTED = 0x0107,
    DROP_LOOT = 0x010C,
    REMOVE_LOOT = 0x010D,
    SPAWN_REACTOR = 0x0117,
    REMOVE_REACTOR = 0x0118,

    // NPC Interaction
    NPC_DIALOGUE = 0x0130,
    OPEN_NPC_SHOP = 0x0131,

    KEYMAP = 0x014F
};

PacketSwitch::PacketSwitch()
{
    // Common handlers
    emplace<PING, PingHandler>();

    // Login handlers
    emplace<LOGIN_RESULT, LoginResultHandler>();
    emplace<SERVERLIST, ServerlistHandler>();
    emplace<CHARLIST, CharlistHandler>();
    emplace<CHARNAME_RESPONSE, CharnameResponseHandler>();
    emplace<ADD_NEWCHAR_ENTRY, AddNewCharEntryHandler>();
    emplace<DELCHAR_RESPONSE, DeleteCharResponseHandler>();
    emplace<CHANNEL_CHANGE, ChannelChangeHandler>();
    emplace<SERVER_IP, ServerIPHandler>();

    // 'Setfield' handlers
    emplace<SET_FIELD, SetfieldHandler>();

    // MapObject handlers
    emplace<SPAWN_CHAR, SpawnCharHandler>();
    emplace<CHAR_MOVED, CharMovedHandler>();
    emplace<UPDATE_CHARLOOK, UpdateCharLookHandler>();
    emplace<SHOW_FOREIGN_EFFECT, ShowForeignEffectHandler>();
    emplace<REMOVE_CHAR, RemoveCharHandler>();
    emplace<SPAWN_PET, SpawnPetHandler>();
    emplace<SPAWN_NPC, SpawnNpcHandler>();
    emplace<SPAWN_NPC_C, SpawnNpcControllerHandler>();
    emplace<SPAWN_MOB, SpawnMobHandler>();
    emplace<SPAWN_MOB_C, SpawnMobControllerHandler>();
    emplace<MOB_MOVED, MobMovedHandler>();
    emplace<SHOW_MOB_HP, ShowMobHpHandler>();
    emplace<KILL_MOB, KillMobHandler>();
    emplace<DROP_LOOT, DropLootHandler>();
    emplace<REMOVE_LOOT, RemoveLootHandler>();
    emplace<SPAWN_REACTOR, SpawnReactorHandler>();
    emplace<REMOVE_REACTOR, RemoveReactorHandler>();

    // Attack handlers
    emplace<ATTACKED_CLOSE, CloseAttackHandler>();
    emplace<ATTACKED_RANGED, RangedAttackHandler>();
    emplace<ATTACKED_MAGIC, MagicAttackHandler>();

    // Player handlers
    emplace<KEYMAP, KeymapHandler>();
    emplace<SKILL_MACROS, SkillMacrosHandler>();
    emplace<CHANGE_STATS, ChangeStatsHandler>();
    emplace<GIVE_BUFF, ApplyBuffHandler>();
    emplace<CANCEL_BUFF, CancelBuffHandler>();
    emplace<RECALCULATE_STATS, RecalculateStatsHandler>();
    emplace<UPDATE_SKILL, UpdateSkillHandler>();
    emplace<ADD_COOLDOWN, AddCooldownHandler>();

    // Messaging handlers
    emplace<SHOW_STATUS_INFO, ShowStatusInfoHandler>();
    emplace<CHAT_RECEIVED, ChatReceivedHandler>();
    emplace<SCROLL_RESULT, ScrollResultHandler>();
    emplace<SERVER_MESSAGE, ServerMessageHandler>();
    emplace<WEEK_EVENT_MESSAGE, WeekEventMessageHandler>();
    emplace<SHOW_ITEM_GAIN_INCHAT, ShowItemGainInChatHandler>();

    // Inventory Handlers
    emplace<MODIFY_INVENTORY, ModifyInventoryHandler>();
    emplace<GATHER_RESULT, GatherResultHandler>();
    emplace<SORT_RESULT, SortResultHandler>();

    // Npc Interaction Handlers
    emplace<NPC_DIALOGUE, NpcDialogueHandler>();
    emplace<OPEN_NPC_SHOP, OpenNpcShopHandler>();

    // TODO
    emplace<MOVE_MOB_RESPONSE, NullHandler>();
    emplace<MEMO_RESULT, NullHandler>();
    emplace<ENABLE_REPORT, NullHandler>();
    emplace<BUDDY_LIST, NullHandler>();
    emplace<GUILD_OPERATION, NullHandler>();
    emplace<FAMILY_PRIV_LIST, NullHandler>();
    emplace<SCRIPT_PROGRESS_MESSAGE, NullHandler>();
    emplace<RECEIVE_POLICE, NullHandler>();
    emplace<MAKE_NPC_SCRIPTED, NullHandler>();

    // Ignored
    emplace<SELECT_WORLD, NullHandler>();
    emplace<RECOMMENDED_WORLDS, NullHandler>();
    emplace<UPDATE_GENDER, NullHandler>();
}

void PacketSwitch::forward(const std::int8_t* bytes, std::size_t length) const
{
    // Wrap the bytes with a parser.
    InPacket recv{bytes, length};
    // Read the opcode to determine handler responsible.
    auto opcode = static_cast<std::uint16_t>(recv.read_short());

    if (opcode < NUM_HANDLERS) {
        if (auto& handler = handlers[opcode]) {
            // Handler ok. Packet is passed on.
            try {
                handler->handle(recv);
            } catch (const PacketError& err) {
                // Notice about an error.
                warn(err.what(), opcode);
            }
        } else {
            // Warn about an unhandled packet.
            warn(MSG_UNHANDLED, opcode);
        }
    } else {
        // Warn about a packet with opcode out of bounds.
        warn(MSG_OUT_OF_BOUNDS, opcode);
    }
}

void PacketSwitch::warn(std::string_view message, std::size_t opcode) const
    noexcept
{
    Console::get().print(
        str::concat(message, ", Opcode: ", str::to_hex(opcode)));
}
} // namespace jrc
