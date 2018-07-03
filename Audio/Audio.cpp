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
#include "Audio.h"

#include "../Configuration.h"

#define WIN32_LEAN_AND_MEAN
#include "nlnx/audio.hpp"
#include "nlnx/nx.hpp"

#include <cstddef>

namespace jrc
{
constexpr const char* Error::messages[];

Sound::Sound() noexcept : id{0}
{
}

Sound::Sound(Name name) noexcept : id{sound_ids[name]}
{
}

Sound::Sound(nl::node src) noexcept : id{add_sound(src)}
{
}

void Sound::play() const noexcept
{
    if (!initialized) {
        return;
    }

    if (auto sample_iter = samples.find(id); sample_iter != samples.end()) {
        Mix_PlayChannel(-1, sample_iter->second, 0);
    }
}

Error Sound::init()
{
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        return Error::AUDIO;
    }

    // Open 44.1kHz, signed 16-bit, system byteorder, stereo audio, using
    // 1024-byte chunks.
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        return Error::AUDIO;
    }

    // Dynamically load library for decoding ogg/vorbis audio.
    Mix_Init(MIX_INIT_OGG);

    // Allocate 16 channels for playing sound effects.
    Mix_AllocateChannels(16);

    nl::node ui_src = nl::nx::sound["UI.img"];

    add_sound(Sound::BUTTON_CLICK, ui_src["BtMouseClick"]);
    add_sound(Sound::BUTTON_OVER, ui_src["BtMouseOver"]);
    add_sound(Sound::SELECT_CHAR, ui_src["CharSelect"]);

    nl::node game_src = nl::nx::sound["Game.img"];

    add_sound(Sound::GAME_START, game_src["GameIn"]);
    add_sound(Sound::JUMP, game_src["Jump"]);
    add_sound(Sound::DROP, game_src["DropItem"]);
    add_sound(Sound::PICK_UP, game_src["PickUpItem"]);
    add_sound(Sound::PORTAL, game_src["Portal"]);
    add_sound(Sound::LEVEL_UP, game_src["LevelUp"]);

    set_sfx_volume(Configuration::get().audio.volume.sound_effects);

    return Error::NONE;
}

void Sound::init_sfx() noexcept
{
    initialized = true;
}

Mix_Music* Music::stream;

void Sound::close() noexcept
{
    if (!initialized) {
        return;
    }

    if (Music::stream) {
        Mix_HaltMusic();
        Mix_FreeMusic(Music::stream);
    }

    for (auto [_, sample] : samples) {
        Mix_FreeChunk(sample);
    }
    samples.clear();

    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();

    initialized = false;
}

void Sound::set_sfx_volume(std::uint8_t vol) noexcept
{
    if (!initialized) {
        return;
    }

    Mix_Volume(-1, MIX_MAX_VOLUME * vol / 100u);
}

std::size_t Sound::add_sound(nl::node src) noexcept
{
    if (!initialized) {
        return 0;
    }

    nl::audio ad = src;

    auto data = static_cast<const std::byte*>(ad.data());

    if (data) {
        std::size_t id = ad.id();

        samples[id] = Mix_LoadWAV_RW(
            SDL_RWFromConstMem(data + 82, ad.length() - 82), 0);

        return id;
    } else {
        return 0;
    }
}

void Sound::add_sound(Name name, nl::node src) noexcept
{
    if (!initialized) {
        return;
    }

    std::size_t id = add_sound(src);

    if (id) {
        sound_ids[name] = id;
    }
}

bool Sound::is_initialized() noexcept
{
    return initialized;
}

std::unordered_map<std::size_t, Mix_Chunk*> Sound::samples;
EnumMap<Sound::Name, std::size_t> Sound::sound_ids;
bool Sound::initialized = false;

Error Music::play(const std::string& bgm_path)
{
    if (!initialized) {
        return Error::NONE;
    }

    static std::string path;

    if (path == bgm_path) {
        return Error::NONE;
    }

    nl::audio ad = nl::nx::sound.resolve(bgm_path);
    auto data = static_cast<const std::byte*>(ad.data());

    if (data) {
        if (stream) {
            Mix_HaltMusic();
            Mix_FreeMusic(stream);
        }

        stream = Mix_LoadMUSType_RW(
            SDL_RWFromConstMem(data + 82, ad.length() - 82), MUS_OGG, 0);
        if (Mix_PlayMusic(stream, -1) == -1) {
            return Error::AUDIO;
        }

        path = bgm_path;
    }

    return Error::NONE;
}

void Music::init() noexcept
{
    stream = nullptr;
    set_bgm_volume(Configuration::get().audio.volume.music);
    initialized = true;
}

void Music::set_bgm_volume(std::uint8_t vol) noexcept
{
    if (!initialized) {
        return;
    }

    Mix_VolumeMusic(MIX_MAX_VOLUME * vol / 100u);
}

bool Music::is_initialized() noexcept
{
    return initialized;
}

bool Music::initialized = false;
} // namespace jrc
