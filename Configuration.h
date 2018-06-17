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
#include "Template/Point.h"
#include "Template/Singleton.h"
#include "Template/TypeMap.h"
#include "Util/Misc.h"

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <type_traits>

namespace jrc
{
//! Manages the 'Settings' file which contains configurations set by user
//! behaviour.
class Configuration : public Singleton<Configuration>
{
public:
    //! Add the settings which will be used and load them.
    Configuration();
    //! Save.
    ~Configuration() override;

    //! Load all settings. If something is missing, set the default value. Can
    //! be used for reloading.
    void load();
    //! Save the current settings.
    void save() const;

    //! Base class for an entry in the settings file.
    class Entry
    {
    protected:
        Entry(std::string_view n, std::string_view v) : name(n), value(v)
        {
        }

        std::string name;
        std::string value;

    private:
        friend class Configuration;

        std::string to_string() const noexcept
        {
            return str::concat(name, " = ", value);
        }
    };

    //! Setting which converts to a bool.
    class BoolEntry : public Entry
    {
    public:
        void save(bool b);
        bool load() const;

    protected:
        using Entry::Entry;
    };

    //! Setting which uses the raw string.
    class StringEntry : public Entry
    {
    public:
        void save(std::string&& str);
        const std::string& load() const;

    protected:
        using Entry::Entry;
    };

    //! Setting which converts to a Point<std::int16_t>.
    class PointEntry : public Entry
    {
    public:
        void save(Point<std::int16_t> p);
        Point<std::int16_t> load() const;

    protected:
        using Entry::Entry;
    };

    //! Setting which converts to an integer type.
    template<class T>
    class IntegerEntry : public Entry
    {
    public:
        void save(T num)
        {
            value = std::to_string(num);
        }

        T load() const
        {
            return string_conversion::or_zero<T>(value);
        }

    protected:
        using Entry::Entry;
    };

    //! Setting which converts to a byte.
    class ByteEntry : public IntegerEntry<std::uint8_t>
    {
    protected:
        using IntegerEntry::IntegerEntry;
    };

    //! Setting which converts to a short.
    class ShortEntry : public IntegerEntry<std::uint16_t>
    {
    protected:
        using IntegerEntry::IntegerEntry;
    };

    //! Setting which converts to an int.
    class IntEntry : public IntegerEntry<std::uint32_t>
    {
    protected:
        using IntegerEntry::IntegerEntry;
    };

    //! Setting which converts to a long.
    class LongEntry : public IntegerEntry<std::uint64_t>
    {
    protected:
        using IntegerEntry::IntegerEntry;
    };

private:
    template<typename T>
    friend struct Setting;

    static constexpr char const* const FILENAME = "Settings";
    TypeMap<Entry> settings;
};

//! IP address which the client will connect to.
struct ServerIP : public Configuration::StringEntry {
    ServerIP() : StringEntry("ServerIP", "127.0.0.1")
    {
    }
};

//! Whether to start in fullscreen mode.
struct Fullscreen : public Configuration::BoolEntry {
    Fullscreen() : BoolEntry("Fullscreen", "false")
    {
    }
};

//! Whether to use vsync.
struct VSync : public Configuration::BoolEntry {
    VSync() : BoolEntry("VSync", "true")
    {
    }
};

//! The normal font which will be used.
struct FontPathNormal : public Configuration::StringEntry {
    FontPathNormal()
        : StringEntry("FontPathNormal", "../fonts/Roboto/Roboto-Regular.ttf")
    {
    }
};

//! The bold font which will be used.
struct FontPathBold : public Configuration::StringEntry {
    FontPathBold()
        : StringEntry("FontPathBold", "../fonts/Roboto/Roboto-Bold.ttf")
    {
    }
};

//! Music volume, an integer in the interval [0, 100].
struct BGMVolume : public Configuration::ByteEntry {
    BGMVolume() : ByteEntry("BGMVolume", "50")
    {
    }
};

//! Sound volume, an integer in the interval [0, 100].
struct SFXVolume : public Configuration::ByteEntry {
    SFXVolume() : ByteEntry("SFXVolume", "50")
    {
    }
};

//! Whether to save the last used account name.
struct SaveLogin : public Configuration::BoolEntry {
    SaveLogin() : BoolEntry("SaveLogin", "false")
    {
    }
};

//! The last used account name.
struct DefaultAccount : public Configuration::StringEntry {
    DefaultAccount() : StringEntry("Account", "")
    {
    }
};

//! The last used world.
struct DefaultWorld : public Configuration::ByteEntry {
    DefaultWorld() : ByteEntry("World", "0")
    {
    }
};

//! The last used channel.
struct DefaultChannel : public Configuration::ByteEntry {
    DefaultChannel() : ByteEntry("Channel", "0")
    {
    }
};

//! The last used character.
struct DefaultCharacter : public Configuration::ByteEntry {
    DefaultCharacter() : ByteEntry("Character", "0")
    {
    }
};

//! The default position of the character stats inventory.
struct PosSTATS : public Configuration::PointEntry {
    PosSTATS() : PointEntry("PosSTATS", "(100,150)")
    {
    }
};

//! The default position of the equip inventory.
struct PosEQINV : public Configuration::PointEntry {
    PosEQINV() : PointEntry("PosEQINV", "(250,150)")
    {
    }
};

//! The default position of the item inventory.
struct PosINV : public Configuration::PointEntry {
    PosINV() : PointEntry("PosINV", "(300,150)")
    {
    }
};

//! The default position of the skill inventory.
struct PosSKILL : public Configuration::PointEntry {
    PosSKILL() : PointEntry("PosSKILL", "(50,150)")
    {
    }
};

//! The default position of the keybinding configuration UI.
struct PosKEYCONFIG : public Configuration::PointEntry {
    PosKEYCONFIG() : PointEntry("PosKEYCONFIG", "(100,100)")
    {
    }
};

//! Can be used to access settings.
template<typename T>
struct Setting {
    //! Access a setting.
    static T& get()
    {
        static_assert(std::is_base_of<Configuration::Entry, T>::value,
                      "template parameter T for Setting must inherit from "
                      "Configuration::Entry.");

        auto* entry = Configuration::get().settings.get<T>();

        if (entry) {
            return *entry;
        } else {
            static T default_entry;
            return default_entry;
        }
    }
};
} // namespace jrc
