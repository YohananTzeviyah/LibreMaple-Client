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
#include "SkillSound.h"

#include "nlnx/nx.hpp"

namespace jrc
{
SingleSkillSound::SingleSkillSound(std::string_view str_id)
{
    nl::node sound_src = nl::nx::sound["Skill.img"][str_id];

    use_sound = sound_src["Use"];
    hit_sound = sound_src["Hit"];
}

void SingleSkillSound::play_use() const
{
    use_sound.play();
}

void SingleSkillSound::play_hit() const
{
    hit_sound.play();
}
} // namespace jrc
