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
#include "../../Character/Char.h"
#include "Attack.h"

#include <unordered_map>

namespace jrc
{
class SkillAction
{
public:
    virtual ~SkillAction() = default;

    virtual void apply(Char& target, Attack::Type atype) const = 0;
};

class NoAction : public SkillAction
{
public:
    void apply(Char&, Attack::Type) const override
    {
    }
};

class RegularAction : public SkillAction
{
public:
    void apply(Char& target, Attack::Type atk_type) const override;
};

class SingleAction : public SkillAction
{
public:
    SingleAction(nl::node src);

    void apply(Char& target, Attack::Type atype) const override;

private:
    std::string action;
};

class TwoHAction : public SkillAction
{
public:
    TwoHAction(nl::node src);

    void apply(Char& target, Attack::Type atype) const override;

private:
    BoolPair<std::string> actions;
};

class ByLevelAction : public SkillAction
{
public:
    ByLevelAction(nl::node src, std::int32_t skillid);

    void apply(Char& target, Attack::Type atype) const override;

private:
    std::unordered_map<std::int32_t, std::string> actions;
    std::int32_t skillid;
};
} // namespace jrc
