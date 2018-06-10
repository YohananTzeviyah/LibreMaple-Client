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
#include "Configuration.h"

#include <fstream>

namespace jrc
{
Configuration::Configuration()
{
    settings.emplace<ServerIP>();
    settings.emplace<Fullscreen>();
    settings.emplace<VSync>();
    settings.emplace<FontPathNormal>();
    settings.emplace<FontPathBold>();
    settings.emplace<BGMVolume>();
    settings.emplace<SFXVolume>();
    settings.emplace<SaveLogin>();
    settings.emplace<DefaultAccount>();
    settings.emplace<DefaultWorld>();
    settings.emplace<DefaultChannel>();
    settings.emplace<DefaultCharacter>();
    settings.emplace<PosSTATS>();
    settings.emplace<PosEQINV>();
    settings.emplace<PosINV>();
    settings.emplace<PosSKILL>();
    settings.emplace<PosKEYCONFIG>();

    load();
}

Configuration::~Configuration()
{
    save();
}

void Configuration::load()
{
    std::unordered_map<std::string, std::string> raw_settings;

    std::ifstream file{FILENAME};
    if (file.is_open()) {
        // Go through the file line for line.
        std::string line;
        while (getline(file, line)) {
            // If the setting is not empty, load the value.
            std::size_t split = line.find('=');
            if (split != std::string::npos && split + 2 < line.size()) {
                raw_settings.emplace(line.substr(0, split - 1),
                                     line.substr(split + 2));
            }
        }
    }

    // Replace default values with loaded values.
    for (auto& [_, setting] : settings) {
        auto rs_iter = raw_settings.find(setting->name);
        if (rs_iter != raw_settings.end()) {
            setting->value = rs_iter->second;
        }
    }
}

void Configuration::save() const
{
    // Open the settings file.
    std::ofstream config{FILENAME};
    if (config.is_open()) {
        // Save settings line by line.
        for (const auto& [_, setting] : settings) {
            config << setting->to_string() << '\n';
        }
    }
}

void Configuration::BoolEntry::save(bool b)
{
    value = b ? "true" : "false";
}

bool Configuration::BoolEntry::load() const
{
    return value == "true";
}

void Configuration::StringEntry::save(std::string&& str)
{
    value = std::move(str);
}

const std::string& Configuration::StringEntry::load() const
{
    return value;
}

void Configuration::PointEntry::save(Point<std::int16_t> vec)
{
    value = vec.to_string();
}

Point<std::int16_t> Configuration::PointEntry::load() const
{
    auto comma_loc = value.find(',');
    std::string x_str = value.substr(1, comma_loc - 1);
    std::string y_str =
        value.substr(comma_loc + 1, value.find(')') - comma_loc - 1);

    return {string_conversion::or_zero<std::int16_t>(x_str),
            string_conversion::or_zero<std::int16_t>(y_str)};
}
} // namespace jrc
