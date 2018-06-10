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
#include "BulletData.h"

#include "nlnx/nx.hpp"

namespace jrc
{
BulletData::BulletData(std::int32_t item_id)
    : item_data(ItemData::get(item_id))
{
    std::string prefix = std::to_string(item_id / 10000);
    prefix.insert(0, "0", 1);
    prefix += ".img";

    std::string str_id = std::to_string(item_id);
    str_id.insert(0, "0", 1);

    nl::node src = nl::nx::item["Consume"][prefix][str_id];

    bullet = src["bullet"];
    watk = src["info"]["incPAD"];
}

bool BulletData::is_valid() const
{
    return item_data.is_valid();
}

BulletData::operator bool() const
{
    return is_valid();
}

std::int16_t BulletData::get_watk() const
{
    return watk;
}

const Animation& BulletData::get_animation() const
{
    return bullet;
}

const ItemData& BulletData::get_item_data() const
{
    return item_data;
}
} // namespace jrc
