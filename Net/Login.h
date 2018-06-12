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
#include "../Character/MapleStat.h"
#include "../Template/EnumMap.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace jrc
{
struct Account {
    std::string name;
    std::int32_t acc_id;
    bool female;
    bool muted;
    std::int16_t pin;
    std::int8_t gm_level;
};

struct World {
    std::string name;
    std::string message;
    std::vector<std::int32_t> channel_loads;
    std::uint8_t channel_count;
    std::uint8_t flag;
    std::int8_t wid;
};

struct StatsEntry {
    std::string name;
    std::vector<std::int64_t> pet_ids;
    EnumMap<Maplestat::Id, std::uint16_t> stats;
    std::int64_t exp;
    std::int32_t map_id;
    std::uint8_t portal;
    std::pair<std::int32_t, std::int8_t> rank;
    std::pair<std::int32_t, std::int8_t> job_rank;
};

struct LookEntry {
    bool female;
    std::uint8_t skin;
    std::int32_t face_id;
    std::int32_t hair_id;
    std::unordered_map<std::int8_t, std::int32_t> equips;
    std::unordered_map<std::int8_t, std::int32_t> masked_equips;
    std::vector<std::int32_t> pet_ids;
};

struct CharEntry {
    StatsEntry stats;
    LookEntry look;
    std::int32_t cid;
};
} // namespace jrc
