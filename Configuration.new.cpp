//////////////////////////////////////////////////////////////////////////////
// This file is part of the LibreMaple MMORPG client                        //
// Copyright © 2018-2019 LibreMaple Team                                    //
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
#include "Configuration.new.h"

#include "Console.h"

#include <exception>
#include <fstream>
#include <string_view>
#include <type_traits>

namespace jrc
{
Configuration::Configuration() noexcept
{
    try {
        load();
    } catch (const std::exception& ex) {
        Console::get().print("[settings.toml parse error] "
                             + std::string{ex.what()});
    }
}

Configuration::~Configuration() noexcept
{
    save();
}

void Configuration::load() noexcept(false)
{
    auto settings = cpptoml::parse_file("settings.toml");

    auto network_table = settings->get_table("network");
    auto video_table = settings->get_table("video");
    auto fonts_table = settings->get_table("fonts");
    auto audio_table = settings->get_table("audio");
    auto account_table = settings->get_table("account");
    auto ui_table = settings->get_table("ui");

    if (network_table) {
        if (auto server_ip = network_table->get_as<std::string>("server_ip");
            server_ip) {
            network.server_ip = *server_ip;
        } else {
            Console::get().print(
                "No valid value for \"settings.toml:network.server_ip\" "
                "found; using default.");
        }
    } else {
        Console::get().print(
            "No valid table \"settings.toml:network\" found; using default.");
    }

    if (video_table) {
        if (auto fullscreen = video_table->get_as<bool>("fullscreen");
            fullscreen) {
            video.fullscreen = *fullscreen;
        } else {
            Console::get().print(
                "No valid value for \"settings.toml:video.fullscreen\" found; "
                "using default.");
        }

        if (auto vsync = video_table->get_as<bool>("vsync"); vsync) {
            video.vsync = *vsync;
        } else {
            Console::get().print(
                "No valid value for \"settings.toml:video.vsync\" found; "
                "using default.");
        }
    } else {
        Console::get().print(
            "No valid table \"settings.toml:video\" found; using default.");
    }

    if (fonts_table) {
        if (auto normal = fonts_table->get_as<std::string>("normal"); normal) {
            fonts.normal = *normal;
        } else {
            Console::get().print(
                "No valid value for \"settings.toml:fonts.normal\" found; "
                "using default.");
        }

        if (auto bold = fonts_table->get_as<std::string>("bold"); bold) {
            fonts.bold = *bold;
        } else {
            Console::get().print(
                "No valid value for \"settings.toml:fonts.bold\" found; "
                "using default.");
        }
    } else {
        Console::get().print(
            "No valid table \"settings.toml:fonts\" found; using default.");
    }

    if (audio_table) {
        auto volume = audio_table->get_table("volume");

        if (volume) {
            if (auto sound_effects
                = volume->get_as<std::uint8_t>("sound_effects");
                sound_effects) {
                audio.volume.sound_effects = *sound_effects;
            } else {
                Console::get().print(
                    "No valid value for "
                    "\"settings.toml:audio.volume.sound_effects\" found; "
                    "using default.");
            }

            if (auto music = volume->get_as<std::uint8_t>("music"); music) {
                audio.volume.music = *music;
            } else {
                Console::get().print(
                    "No valid value for \"settings.toml:audio.volume.music\" "
                    "found; using default.");
            }
        } else {
            Console::get().print(
                "No valid table \"settings.toml:audio.volume\" found; using "
                "default.");
        }
    } else {
        Console::get().print(
            "No valid table \"settings.toml:audio\" found; using default.");
    }

    if (account_table) {
        if (auto save_login = account_table->get_as<bool>("save_login");
            save_login) {
            account.save_login = *save_login;
        } else {
            Console::get().print("No valid value for "
                                 "\"settings.toml:account.save_login\" found; "
                                 "using default.");
        }

        if (auto account_name
            = account_table->get_as<std::string>("account_name");
            account_name) {
            account.account_name = *account_name;
        } else {
            Console::get().print(
                "No valid value for \"settings.toml:account.account_name\" "
                "found; using default.");
        }

        if (auto world = account_table->get_as<std::uint8_t>("world"); world) {
            account.world = *world;
        } else {
            Console::get().print(
                "No valid value for \"settings.toml:account.world\" found; "
                "using default.");
        }

        if (auto channel = account_table->get_as<std::uint8_t>("channel");
            channel) {
            account.channel = *channel;
        } else {
            Console::get().print(
                "No valid value for \"settings.toml:account.channel\" found; "
                "using default.");
        }

        if (auto character = account_table->get_as<std::uint8_t>("character");
            character) {
            account.character = *character;
        } else {
            Console::get().print(
                "No valid value for \"settings.toml:account.character\" "
                "found; using default.");
        }
    } else {
        Console::get().print(
            "No valid table \"settings.toml:account\" found; using default.");
    }

    if (ui_table) {
        auto position = ui_table->get_table("position");

        if (position) {
            if (auto key_config
                = position->get_array_of<std::int16_t>("key_config");
                key_config) {
                if (auto key_config_point = vec_to_point(*key_config);
                    key_config_point) {
                    ui.position.key_config = *key_config_point;
                } else {
                    Console::get().print("No valid value for "
                                         "\"settings.toml:ui.position.key_"
                                         "config\" found; using default.");
                }
            } else {
                Console::get().print(
                    "No valid value for "
                    "\"settings.toml:ui.position.key_config\" "
                    "found; using default.");
            }

            if (auto stats = position->get_array_of<std::int16_t>("stats");
                stats) {
                if (auto stats_point = vec_to_point(*stats); stats_point) {
                    ui.position.stats = *stats_point;
                } else {
                    Console::get().print("No valid value for "
                                         "\"settings.toml:ui.position.stats\" "
                                         "found; using default.");
                }
            } else {
                Console::get().print(
                    "No valid value for \"settings.toml:ui.position.stats\" "
                    "found; using default.");
            }

            if (auto inventory
                = position->get_array_of<std::int16_t>("inventory");
                inventory) {
                if (auto inventory_point = vec_to_point(*inventory);
                    inventory_point) {
                    ui.position.inventory = *inventory_point;
                } else {
                    Console::get().print(
                        "No valid value for "
                        "\"settings.toml:ui.position.inventory\" "
                        "found; using default.");
                }
            } else {
                Console::get().print("No valid value for "
                                     "\"settings.toml:ui.position.inventory\" "
                                     "found; using default.");
            }

            if (auto equip_inventory
                = position->get_array_of<std::int16_t>("equip_inventory");
                equip_inventory) {
                if (auto equip_inventory_point
                    = vec_to_point(*equip_inventory);
                    equip_inventory_point) {
                    ui.position.equip_inventory = *equip_inventory_point;
                } else {
                    Console::get().print(
                        "No valid value for "
                        "\"settings.toml:ui.position.equip_inventory\" "
                        "found; using default.");
                }
            } else {
                Console::get().print(
                    "No valid value for "
                    "\"settings.toml:ui.position.equip_inventory\" "
                    "found; using default.");
            }

            if (auto skillbook
                = position->get_array_of<std::int16_t>("skillbook");
                skillbook) {
                if (auto skillbook_point = vec_to_point(*skillbook);
                    skillbook_point) {
                    ui.position.skillbook = *skillbook_point;
                } else {
                    Console::get().print(
                        "No valid value for "
                        "\"settings.toml:ui.position.skillbook\" "
                        "found; using default.");
                }
            } else {
                Console::get().print("No valid value for "
                                     "\"settings.toml:ui.position.skillbook\" "
                                     "found; using default.");
            }
        } else {
            Console::get().print(
                "No valid table \"settings.toml:ui.position\" found; using "
                "default.");
        }
    } else {
        Console::get().print(
            "No valid table \"settings.toml:ui\" found; using default.");
    }
}

void Configuration::save() const
{
    using namespace std::literals;

    static constexpr const std::string_view OUTPUT_TEMPLATE = u8R"(
# Settings for the LibreMaple client.
#
# LibreMaple client code is © 2015-2016 Daniel Allendorf, 2018-2019 LibreMaple
# Team, licensed under the GNU Affero General Public License version 3 or
# higher.

[network]
server_ip = $

[video]
fullscreen = $
vsync = $

[fonts]
normal = $
bold = $

[audio]
    [audio.volume] # Volumes are in percentages.
    sound_effects = $
    music = $

[account]
save_login = $
account_name = $
world = $
channel = $
character = $

[ui]
    [ui.position]
    key_config = $
    stats = $
    inventory = $
    equip_inventory = $
    skillbook = $
)"sv.substr(1);

    std::ofstream settings{"settings.toml"};
    if (!settings || !settings.is_open()) {
        Console::get().print("[settings.toml write error] Could not open "
                             "\"settings.toml\" for writing.");
    }

    std::size_t last_ix = 0;
    std::size_t template_ix = 0;
    std::size_t entry_ix = 0;
    auto write = [&settings](const auto& x) {
        using x_type = std::decay_t<decltype(x)>;

        if constexpr (std::is_same_v<x_type, std::string>) {
            settings.put('"');
            settings << x; // TODO: Escape special characters.
            settings.put('"');
        } else if constexpr (std::is_same_v<x_type, bool>) {
            if (x) {
                settings << "true";
            } else {
                settings << "false";
            }
        } else if constexpr (is_point<x_type>::value) {
            settings.put('[');
            settings << x.x() << ", " << x.y();
            settings.put(']');
        } else {
            settings << x;
        }
    };
    while (template_ix < OUTPUT_TEMPLATE.length()) {
        if (OUTPUT_TEMPLATE[template_ix] == '$') {
            settings << OUTPUT_TEMPLATE.substr(last_ix, template_ix - last_ix);
            ++template_ix;
            last_ix = template_ix;

            switch (entry_ix) {
            case 0:
                write(network.server_ip);
                break;
            case 1:
                write(video.fullscreen);
                break;
            case 2:
                write(video.vsync);
                break;
            case 3:
                write(fonts.normal);
                break;
            case 4:
                write(fonts.bold);
                break;
            case 5:
                write(audio.volume.sound_effects);
                break;
            case 6:
                write(audio.volume.music);
                break;
            case 7:
                write(account.save_login);
                break;
            case 8:
                write(account.account_name);
                break;
            case 9:
                write(account.world);
                break;
            case 10:
                write(account.channel);
                break;
            case 11:
                write(account.character);
                break;
            case 12:
                write(ui.position.key_config);
                break;
            case 13:
                write(ui.position.stats);
                break;
            case 14:
                write(ui.position.inventory);
                break;
            case 15:
                write(ui.position.equip_inventory);
                break;
            case 16:
                write(ui.position.skillbook);
                break;
            default:
                throw std::logic_error{
                    "[logic error] Number of `case` statements in "
                    "`Configuration::save()` is incorrect"};
                break;
            }

            ++entry_ix;
        } else {
            ++template_ix;
        }
    }
    settings << OUTPUT_TEMPLATE.substr(last_ix);

    settings.flush();
}

template<typename T>
std::optional<Point<T>>
Configuration::vec_to_point(const std::vector<T>& vec) noexcept
{
    if (vec.size() != 2) {
        return {};
    }
    return {vec[0], vec[1]};
}
} // namespace jrc