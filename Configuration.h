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
#pragma once
#include "Template/Point.h"
#include "cpptoml.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace jrc
{
//! Manages the "settings.toml" file which contains configurations set by user
//! behavior.
//!
//! The format of this `struct` is in 1:1 correspondence with the format of the
//! "settings.toml".
//!
//! TODO: Almost all of this code can be generated based on the default
//! "settings.toml".
struct Configuration : public Singleton<Configuration> {
    //! Initialize settings using defaults, and then load as much as possible
    //! from "settings.toml".
    Configuration() noexcept;
    //! Automatically save on destruction.
    ~Configuration() noexcept override;

    //! Load all settings. If anything is missing, the defaults are used. Can
    //! be used for reloading.
    //!
    //! **Throws** cpptoml exceptions.
    void load() noexcept(false);
    //! Save the current settings.
    void save() const noexcept;

    enum class PositionOf {
        KEY_CONFIG,
        STATS,
        INVENTORY,
        EQUIP_INVENTORY,
        SKILLBOOK,
        CHANGE_CHANNEL,
        GAME_SETTINGS
    };

    [[nodiscard]] Point<std::int16_t> get_position_of(PositionOf po) const
        noexcept;
    void set_position_of(PositionOf po, Point<std::int16_t> pos) noexcept;

    //////////////// Data layout ////////////////

    struct Network {
        std::string ip = "127.0.0.1";
        std::uint16_t port = 8484;
    };

    struct Video {
        bool fullscreen = false;
        bool vsync = true;
    };

    struct Fonts {
        std::string normal = "../fonts/Roboto/Roboto-Regular.ttf";
        std::string bold = "../fonts/Roboto/Roboto-Bold.ttf";
    };

    struct Audio {
        struct Volume // Volumes are in percentages.
        {
            std::uint8_t sound_effects = 50;
            std::uint8_t music = 50;
        };

        Volume volume;
    };

    struct Account {
        std::string account_name = "";
        std::uint8_t world = 0;
        std::uint8_t channel = 0;
        std::uint8_t character = 0;
        bool save_login = false;
    };

    struct Ui {
        struct Position {
            Point<std::int16_t> key_config = {150, 150};
            Point<std::int16_t> stats = {100, 150};
            Point<std::int16_t> inventory = {300, 150};
            Point<std::int16_t> equip_inventory = {250, 150};
            Point<std::int16_t> skillbook = {50, 150};
            Point<std::int16_t> change_channel = {350, 300};
            Point<std::int16_t> game_settings = {450, 250};
        };

        Position position;
    };

    struct Character {
        struct GameSettings {
            //! whispers = true
            //! friend_invites = true
            //! chat_invites = true
            //! trade_requests = true
            //! party_invites = true
            //! sidekick_invites = true
            //! expedition_invites = true
            //! guild_chat = true
            //! guild_invites = true
            //! alliance_chat = true
            //! alliance_invites = true
            //! family_invites = true
            //! follow = true
            std::uint16_t flags = 0b00011111'11111111;
        };

        GameSettings game_settings;
    };

    // Data members are reordered here for compactness (blame the ABI).
    Network network;
    Fonts fonts;
    Account account;
    std::unordered_map<std::string, Character> characters;
    Video video;
    Audio audio;
    Ui ui;

    //! Gets a reference to the character-specific configuration for the
    //! character identified by name. **Inserts a new character with the**
    //! **default configuration if there isn't already one with the specified**
    //! **name**.
    [[nodiscard]] Character& get_character(const std::string& name) noexcept;

private:
    //! Helper function for getting `Point`s out of TOML arrays. Converts the
    //! `std::vector`'s elements into a `Point` of another type by using
    //! `static_cast` on the elements.
    template<typename T>
    static std::optional<Point<T>>
    vec_to_point(const std::vector<std::int64_t>& vec) noexcept
    {
        if (vec.size() != 2) {
            return {};
        }
        return {{static_cast<T>(vec[0]), static_cast<T>(vec[1])}};
    }
};
} // namespace jrc
