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
        SKILLBOOK
    };

    [[nodiscard]] Point<std::int16_t> get_position_of(PositionOf po) const
        noexcept;
    void set_position_of(PositionOf po, Point<std::int16_t> pos) noexcept;

    //////////////// Data layout ////////////////

    struct Network {
        std::string server_ip = "127.0.0.1";
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
        bool save_login = false;
        std::string account_name = "";
        std::uint8_t world = 0;
        std::uint8_t channel = 0;
        std::uint8_t character = 0;
    };

    struct Ui {
        struct Position {
            Point<std::int16_t> key_config = {150, 150};
            Point<std::int16_t> stats = {100, 150};
            Point<std::int16_t> inventory = {300, 150};
            Point<std::int16_t> equip_inventory = {250, 150};
            Point<std::int16_t> skillbook = {50, 150};
        };

        Position position;
    };

    // Data members are reordered here for compactness (blame the ABI).
    Network network;
    Fonts fonts;
    Account account;
    Video video;
    Audio audio;
    Ui ui;

private:
    //! Helper function for getting `Point`s out of TOML arrays. Converts the
    //! `std::vector`'s elements into a `Point` of another type by using
    //! `static_cast` on the elements.
    template<typename T>
    static std::optional<Point<T>>
    vec_to_point(const std::vector<std::int64_t>& vec) noexcept;
};
} // namespace jrc
