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
#include "Configuration.h"

#include "Console.h"
#include "IO/UITypes/UIGameSettings.h"
#include "Util/Str.h"

#include <exception>
#include <fstream>
#include <iomanip>
#include <string_view>
#include <type_traits>

namespace jrc
{
Configuration::Configuration() noexcept
{
    try {
        load();
    } catch (const std::exception& ex) {
        Console::get().print(str::concat(
            "[settings.toml parse error] ", std::string_view{ex.what()}, '.'));
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
        if (auto ip = network_table->get_as<std::string>("ip"); ip) {
            network.ip = *ip;
        } else {
            Console::get().print(
                "No valid value for \"settings.toml:network.ip\" found; using "
                "default.");
        }

        if (auto port = network_table->get_as<std::uint16_t>("port"); port) {
            network.port = *port;
        } else {
            Console::get().print(
                "No valid value for \"settings.toml:network.port\" found; "
                "using default.");
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
                = position->get_array_of<std::int64_t>("key_config");
                key_config) {
                if (auto key_config_point
                    = vec_to_point<std::int16_t>(*key_config);
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

            if (auto stats = position->get_array_of<std::int64_t>("stats");
                stats) {
                if (auto stats_point = vec_to_point<std::int16_t>(*stats);
                    stats_point) {
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
                = position->get_array_of<std::int64_t>("inventory");
                inventory) {
                if (auto inventory_point
                    = vec_to_point<std::int16_t>(*inventory);
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
                = position->get_array_of<std::int64_t>("equip_inventory");
                equip_inventory) {
                if (auto equip_inventory_point
                    = vec_to_point<std::int16_t>(*equip_inventory);
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
                = position->get_array_of<std::int64_t>("skillbook");
                skillbook) {
                if (auto skillbook_point
                    = vec_to_point<std::int16_t>(*skillbook);
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

            if (auto change_channel
                = position->get_array_of<std::int64_t>("change_channel");
                change_channel) {
                if (auto change_channel_point
                    = vec_to_point<std::int16_t>(*change_channel);
                    change_channel_point) {
                    ui.position.change_channel = *change_channel_point;
                } else {
                    Console::get().print("No valid value for "
                                         "\"settings.toml:ui.position.change_"
                                         "channel\" found; using default.");
                }
            } else {
                Console::get().print("No valid value for "
                                     "\"settings.toml:ui.position.change_"
                                     "channel\" found; using default.");
            }

            if (auto game_settings
                = position->get_array_of<std::int64_t>("game_settings");
                game_settings) {
                if (auto game_settings_point
                    = vec_to_point<std::int16_t>(*game_settings);
                    game_settings_point) {
                    ui.position.game_settings = *game_settings_point;
                } else {
                    Console::get().print("No valid value for "
                                         "\"settings.toml:ui.position.game_"
                                         "settings\" found; using default.");
                }
            } else {
                Console::get().print("No valid value for "
                                     "\"settings.toml:ui.position.game_"
                                     "settings\" found; using default.");
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

    if (auto character_tables = settings->get_table_array("character");
        character_tables) {
        for (const auto& character_table : *character_tables) {
            Character character;
            std::string name;
            if (auto name_ = character_table->get_as<std::string>("name");
                name_) {
                name = *name_;
            } else {
                Console::get().print("No valid name found for a "
                                     "\"settings.toml:character\"; ignoring.");
                continue;
            }

            if (auto game_settings
                = character_table->get_table("game_settings");
                game_settings) {
                if (auto whispers = game_settings->get_as<bool>("whispers");
                    whispers) {
                    if (*whispers) {
                        character.game_settings.flags
                            |= 1 << UIGameSettings::WHISPERS;
                    } else {
                        character.game_settings.flags
                            &= ~(1 << UIGameSettings::WHISPERS);
                    }
                } else {
                    Console::get().print(
                        str::concat("No valid value "
                                    "\"settings.toml:character.game_settings."
                                    "whispers\" found for \"",
                                    name,
                                    "\"; using default."));
                }

                if (auto friend_invites
                    = game_settings->get_as<bool>("friend_invites");
                    friend_invites) {
                    if (*friend_invites) {
                        character.game_settings.flags
                            |= 1 << UIGameSettings::FRIEND_INVITES;
                    } else {
                        character.game_settings.flags
                            &= ~(1 << UIGameSettings::FRIEND_INVITES);
                    }
                } else {
                    Console::get().print(
                        str::concat("No valid value "
                                    "\"settings.toml:character.game_settings."
                                    "friend_invites\" found for \"",
                                    name,
                                    "\"; using default."));
                }

                if (auto chat_invites
                    = game_settings->get_as<bool>("chat_invites");
                    chat_invites) {
                    if (*chat_invites) {
                        character.game_settings.flags
                            |= 1 << UIGameSettings::CHAT_INVITES;
                    } else {
                        character.game_settings.flags
                            &= ~(1 << UIGameSettings::CHAT_INVITES);
                    }
                } else {
                    Console::get().print(
                        str::concat("No valid value "
                                    "\"settings.toml:character.game_settings."
                                    "chat_invites\" found for \"",
                                    name,
                                    "\"; using default."));
                }

                if (auto trade_requests
                    = game_settings->get_as<bool>("trade_requests");
                    trade_requests) {
                    if (*trade_requests) {
                        character.game_settings.flags
                            |= 1 << UIGameSettings::TRADE_REQUESTS;
                    } else {
                        character.game_settings.flags
                            &= ~(1 << UIGameSettings::TRADE_REQUESTS);
                    }
                } else {
                    Console::get().print(
                        str::concat("No valid value "
                                    "\"settings.toml:character.game_settings."
                                    "trade_requests\" found for \"",
                                    name,
                                    "\"; using default."));
                }

                if (auto party_invites
                    = game_settings->get_as<bool>("party_invites");
                    party_invites) {
                    if (*party_invites) {
                        character.game_settings.flags
                            |= 1 << UIGameSettings::PARTY_INVITES;
                    } else {
                        character.game_settings.flags
                            &= ~(1 << UIGameSettings::PARTY_INVITES);
                    }
                } else {
                    Console::get().print(
                        str::concat("No valid value "
                                    "\"settings.toml:character.game_settings."
                                    "party_invites\" found for \"",
                                    name,
                                    "\"; using default."));
                }

                if (auto sidekick_invites
                    = game_settings->get_as<bool>("sidekick_invites");
                    sidekick_invites) {
                    if (*sidekick_invites) {
                        character.game_settings.flags
                            |= 1 << UIGameSettings::SIDEKICK_INVITES;
                    } else {
                        character.game_settings.flags
                            &= ~(1 << UIGameSettings::SIDEKICK_INVITES);
                    }
                } else {
                    Console::get().print(
                        str::concat("No valid value "
                                    "\"settings.toml:character.game_settings."
                                    "sidekick_invites\" found for \"",
                                    name,
                                    "\"; using default."));
                }

                if (auto expedition_invites
                    = game_settings->get_as<bool>("expedition_invites");
                    expedition_invites) {
                    if (*expedition_invites) {
                        character.game_settings.flags
                            |= 1 << UIGameSettings::EXPEDITION_INVITES;
                    } else {
                        character.game_settings.flags
                            &= ~(1 << UIGameSettings::EXPEDITION_INVITES);
                    }
                } else {
                    Console::get().print(
                        str::concat("No valid value "
                                    "\"settings.toml:character.game_settings."
                                    "expedition_invites\" found for \"",
                                    name,
                                    "\"; using default."));
                }

                if (auto guild_chat
                    = game_settings->get_as<bool>("guild_chat");
                    guild_chat) {
                    if (*guild_chat) {
                        character.game_settings.flags
                            |= 1 << UIGameSettings::GUILD_CHAT;
                    } else {
                        character.game_settings.flags
                            &= ~(1 << UIGameSettings::GUILD_CHAT);
                    }
                } else {
                    Console::get().print(
                        str::concat("No valid value "
                                    "\"settings.toml:character.game_settings."
                                    "guild_chat\" found for \"",
                                    name,
                                    "\"; using default."));
                }

                if (auto guild_invites
                    = game_settings->get_as<bool>("guild_invites");
                    guild_invites) {
                    if (*guild_invites) {
                        character.game_settings.flags
                            |= 1 << UIGameSettings::GUILD_INVITES;
                    } else {
                        character.game_settings.flags
                            &= ~(1 << UIGameSettings::GUILD_INVITES);
                    }
                } else {
                    Console::get().print(
                        str::concat("No valid value "
                                    "\"settings.toml:character.game_settings."
                                    "guild_invites\" found for \"",
                                    name,
                                    "\"; using default."));
                }

                if (auto alliance_chat
                    = game_settings->get_as<bool>("alliance_chat");
                    alliance_chat) {
                    if (*alliance_chat) {
                        character.game_settings.flags
                            |= 1 << UIGameSettings::ALLIANCE_CHAT;
                    } else {
                        character.game_settings.flags
                            &= ~(1 << UIGameSettings::ALLIANCE_CHAT);
                    }
                } else {
                    Console::get().print(
                        str::concat("No valid value "
                                    "\"settings.toml:character.game_settings."
                                    "alliance_chat\" found for \"",
                                    name,
                                    "\"; using default."));
                }

                if (auto alliance_invites
                    = game_settings->get_as<bool>("alliance_invites");
                    alliance_invites) {
                    if (*alliance_invites) {
                        character.game_settings.flags
                            |= 1 << UIGameSettings::ALLIANCE_INVITES;
                    } else {
                        character.game_settings.flags
                            &= ~(1 << UIGameSettings::ALLIANCE_INVITES);
                    }
                } else {
                    Console::get().print(
                        str::concat("No valid value "
                                    "\"settings.toml:character.game_settings."
                                    "alliance_invites\" found for \"",
                                    name,
                                    "\"; using default."));
                }

                if (auto family_invites
                    = game_settings->get_as<bool>("family_invites");
                    family_invites) {
                    if (*family_invites) {
                        character.game_settings.flags
                            |= 1 << UIGameSettings::FAMILY_INVITES;
                    } else {
                        character.game_settings.flags
                            &= ~(1 << UIGameSettings::FAMILY_INVITES);
                    }
                } else {
                    Console::get().print(
                        str::concat("No valid value "
                                    "\"settings.toml:character.game_settings."
                                    "family_invites\" found for \"",
                                    name,
                                    "\"; using default."));
                }

                if (auto follow = game_settings->get_as<bool>("follow");
                    follow) {
                    if (*follow) {
                        character.game_settings.flags
                            |= 1 << UIGameSettings::FOLLOW;
                    } else {
                        character.game_settings.flags
                            &= ~(1 << UIGameSettings::FOLLOW);
                    }
                } else {
                    Console::get().print(
                        str::concat("No valid value "
                                    "\"settings.toml:character.game_settings."
                                    "follow\" found for \"",
                                    name,
                                    "\"; using default."));
                }
            } else {
                Console::get().print(str::concat(
                    "No valid table \"settings.toml:character.game_settings\" "
                    "found for \"",
                    name,
                    "\"; using default."));
            }

            characters.emplace(name, character);
        }
    }
}

void Configuration::save() const noexcept
{
    using namespace std::literals;

    static constexpr const std::string_view OUTPUT_TEMPLATE = u8R"(
# Settings for the LibreMaple client.
#
# LibreMaple client code is © 2015-2016 Daniel Allendorf, 2018-2019 LibreMaple
# Team, licensed under the GNU Affero General Public License version 3 or
# higher.

[network]
  ip = $
  port = $

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
    change_channel = $
    game_settings = $)"sv.substr(1);

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
            // TODO: Escape special characters.
            settings << std::quoted(x);
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
        } else if constexpr (
            std::is_same_v<
                x_type,
                std::uint8_t> || std::is_same_v<x_type, std::int8_t>) {
            settings << static_cast<int>(x);
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
                write(network.ip);
                break;
            case 1:
                write(network.port);
                break;
            case 2:
                write(video.fullscreen);
                break;
            case 3:
                write(video.vsync);
                break;
            case 4:
                write(fonts.normal);
                break;
            case 5:
                write(fonts.bold);
                break;
            case 6:
                write(audio.volume.sound_effects);
                break;
            case 7:
                write(audio.volume.music);
                break;
            case 8:
                write(account.save_login);
                break;
            case 9:
                write(account.account_name);
                break;
            case 10:
                write(account.world);
                break;
            case 11:
                write(account.channel);
                break;
            case 12:
                write(account.character);
                break;
            case 13:
                write(ui.position.key_config);
                break;
            case 14:
                write(ui.position.stats);
                break;
            case 15:
                write(ui.position.inventory);
                break;
            case 16:
                write(ui.position.equip_inventory);
                break;
            case 17:
                write(ui.position.skillbook);
                break;
            case 18:
                write(ui.position.change_channel);
                break;
            case 19:
                write(ui.position.game_settings);
                break;
            default:
                Console::get().print(
                    "[logic error] Number of `case` statements in "
                    "`Configuration::save()` is incorrect.");
                break;
            }

            ++entry_ix;
        } else {
            ++template_ix;
        }
    }
    settings << OUTPUT_TEMPLATE.substr(last_ix);

    static constexpr const std::string_view CHARACTER_TEMPLATE = u8R"(

[[character]]
  name = $
  [character.game_settings]
    whispers = $
    friend_invites = $
    chat_invites = $
    trade_requests = $
    party_invites = $
    sidekick_invites = $
    expedition_invites = $
    guild_chat = $
    guild_invites = $
    alliance_chat = $
    alliance_invites = $
    family_invites = $
    follow = $)";

    for (const auto& [name, ch] : characters) {
        last_ix = 0;
        template_ix = 0;
        entry_ix = 0;
        while (template_ix < CHARACTER_TEMPLATE.length()) {
            if (CHARACTER_TEMPLATE[template_ix] == '$') {
                settings << CHARACTER_TEMPLATE.substr(last_ix,
                                                      template_ix - last_ix);
                ++template_ix;
                last_ix = template_ix;

                switch (entry_ix) {
                case 0:
                    write(name);
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                    write(static_cast<bool>(ch.game_settings.flags
                                            & 1 << (entry_ix - 1)));
                    break;
                default:
                    Console::get().print(
                        "[logic error] Number of `case` statements in "
                        "`Configuration::save()` is incorrect.");
                    break;
                }

                ++entry_ix;
            } else {
                ++template_ix;
            }
        }
        settings << CHARACTER_TEMPLATE.substr(last_ix);
    }

    settings << '\n';
    settings.flush();
}

Point<std::int16_t> Configuration::get_position_of(PositionOf po) const
    noexcept
{
    switch (po) {
    case PositionOf::KEY_CONFIG:
        return ui.position.key_config;
    case PositionOf::STATS:
        return ui.position.stats;
    case PositionOf::INVENTORY:
        return ui.position.inventory;
    case PositionOf::EQUIP_INVENTORY:
        return ui.position.equip_inventory;
    case PositionOf::SKILLBOOK:
        return ui.position.skillbook;
    case PositionOf::CHANGE_CHANNEL:
        return ui.position.change_channel;
    case PositionOf::GAME_SETTINGS:
        return ui.position.game_settings;
    }

    return {};
}

void Configuration::set_position_of(PositionOf po,
                                    Point<std::int16_t> pos) noexcept
{
    switch (po) {
    case PositionOf::KEY_CONFIG:
        ui.position.key_config = pos;
        break;
    case PositionOf::STATS:
        ui.position.stats = pos;
        break;
    case PositionOf::INVENTORY:
        ui.position.inventory = pos;
        break;
    case PositionOf::EQUIP_INVENTORY:
        ui.position.equip_inventory = pos;
        break;
    case PositionOf::SKILLBOOK:
        ui.position.skillbook = pos;
        break;
    case PositionOf::CHANGE_CHANNEL:
        ui.position.change_channel = pos;
        break;
    case PositionOf::GAME_SETTINGS:
        ui.position.game_settings = pos;
        break;
    }
}

Configuration::Character&
Configuration::get_character(const std::string& name) noexcept
{
    auto [iter, inserted] = characters.try_emplace(name);
    return iter->second;
}
} // namespace jrc
