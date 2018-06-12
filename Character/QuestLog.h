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
#include "boost/container/flat_map.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

namespace jrc
{
//! Class that stores information on the questlog of an individual character.
class Questlog
{
public:
    void add_started(std::int16_t, std::string&& quest_data);
    void add_in_progress(std::int16_t, std::int16_t, std::string&& quest_data);
    void add_completed(std::int16_t, std::int64_t);
    bool is_started(std::int16_t);
    std::int16_t get_last_started();

private:
    boost::container::flat_map<std::int16_t, std::string> started;
    std::unordered_map<std::int16_t, std::pair<std::int16_t, std::string>>
        in_progress;
    std::unordered_map<std::int16_t, std::int64_t> completed;
};
} // namespace jrc
