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
#include "../Character/Player.h"
#include "../IO/KeyType.h"
#include "../Template/Singleton.h"
#include "../Template/TimedQueue.h"
#include "Camera.h"
#include "Combat/Combat.h"
#include "MapleMap/MapBackgrounds.h"
#include "MapleMap/MapChars.h"
#include "MapleMap/MapDrops.h"
#include "MapleMap/MapInfo.h"
#include "MapleMap/MapMobs.h"
#include "MapleMap/MapNpcs.h"
#include "MapleMap/MapPortals.h"
#include "MapleMap/MapReactors.h"
#include "MapleMap/MapTilesObjs.h"
#include "Physics/Physics.h"
#include "Spawn.h"

namespace jrc
{
class Stage : public Singleton<Stage>
{
public:
    Stage();

    void init();

    //! Loads the map to be displayed.
    void load(std::int32_t map_id, std::int8_t portal_id);
    //! Removes all map objects and graphics.
    void clear();

    //! Contructs the player from a character entry.
    void loadplayer(const CharEntry& entry);

    //! Calls `draw(float)` on all objects that are on this `Stage`.
    void draw(float alpha) const;
    //! Calls `update()` on all objects that are on this `Stage`.
    void update();

    //! Show a character effect.
    void show_character_effect(std::int32_t cid, CharEffect::Id effect);

    //! Send key input to the stage.
    void send_key(KeyType::Id keytype, std::int32_t keycode, bool pressed);
    //! Send mouse input to the stage.
    Cursor::State send_cursor(bool pressed, Point<std::int16_t> position);

    //! Check if the specified ID is the player's ID.
    bool is_player(std::int32_t cid) const;

    //! Returns a reference to the NPCs on the current map.
    MapNpcs& get_npcs();
    //! Returns a reference to the other characters on the current map.
    MapChars& get_chars();
    //! Returns a reference to the mobs on the current map.
    MapMobs& get_mobs();
    //! Returns a reference to the reactors on the current map.
    MapReactors& get_reactors();
    //! Returns a reference to the drops on the current map.
    MapDrops& get_drops();
    //! Returns a reference to the Player.
    Player& get_player();
    //! Return a reference to the attack and buff component.
    Combat& get_combat();

    //! Return a pointer to a character, possibly the player.
    nullable_ptr<Char> get_character(std::int32_t cid);

    //! Getter for the ID of the current world.
    [[nodiscard]] std::uint8_t get_world() const noexcept;
    //! Setter for the ID of the current world.
    void set_world(std::uint8_t world_id) noexcept;

    //! Getter for the ID of the current world.
    [[nodiscard]] std::uint8_t get_channel() const noexcept;
    //! Setter for the ID of the current world.
    void set_channel(std::uint8_t chan) noexcept;

    //! Getter for the number of channels in the current world.
    [[nodiscard]] std::uint8_t get_channel_count() const noexcept;
    //! Setter for the number of channels in the current world.
    void set_channel_count(std::uint8_t ch_count) noexcept;

private:
    void load_map(std::int32_t map_id);
    void respawn(std::int8_t portal_id);
    void check_portals();
    void check_seats();
    void check_ladders(bool up);
    void check_drops();

    enum State { INACTIVE, TRANSITION, ACTIVE };

    Camera camera;
    Physics physics;
    Player player;

    nullable_ptr<Playable> playable;

    MapInfo map_info;
    MapTilesObjs tiles_objs;
    MapBackgrounds backgrounds;
    MapPortals portals;
    MapReactors reactors;
    MapNpcs npcs;
    MapChars chars;
    MapMobs mobs;
    MapDrops drops;

    Combat combat;

    State state;
    std::uint8_t world;
    std::uint8_t channel;
    std::uint8_t channel_count;
};
} // namespace jrc
