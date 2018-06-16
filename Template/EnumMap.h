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
#include <array>
#include <type_traits>
#include <utility>

namespace jrc
{
template<typename K,
         typename V,
         K LENGTH = K::LENGTH,
         typename = std::enable_if_t<std::is_enum_v<K>>>
//! Wraps an array so that it is addressable by `enum [class]` values.
class EnumMap
{
public:
    using array_size_t = typename std::array<V, LENGTH>::size_type;

    //! Default constructor.
    EnumMap() = default;
    template<typename... Args,
             typename = std::enable_if_t<
                 std::conjunction_v<std::is_same<Args, std::pair<K, V>>...>>>
    //! Initialize with a varargs initialization list.
    constexpr EnumMap(Args&&... args) noexcept : m_values{}
    {
        ((m_values[static_cast<array_size_t>(args.first)] =
              std::forward<V>(args.second)),
         ...);
    }
    template<typename... Args>
    //! Initialize with a varargs initialization list, taking one value for
    //! every possible enumeration key.
    constexpr EnumMap(Args&&... args) noexcept
        : m_values{{std::forward<Args>(args)...}}
    {
    }
    //! Initialize with a `std::initializer_list`.
    constexpr EnumMap(std::initializer_list<std::pair<K, V>> init) noexcept
        : m_values{}
    {
        for (auto [k, v] : init) {
            m_values[static_cast<array_size_t>(k)] = v;
        }
    }

    constexpr void clear() noexcept
    {
        for (std::size_t i = 0; i < LENGTH; ++i) {
            m_values[i] = V();
        }
    }

    constexpr void erase(K key) noexcept
    {
        if (key >= 0 && key < LENGTH) {
            m_values[static_cast<array_size_t>(key)] = V();
        }
    }

    template<typename... Args>
    constexpr void emplace(K key, Args&&... args) noexcept
    {
        m_values[static_cast<array_size_t>(key)] = {
            std::forward<Args>(args)...};
    }

    constexpr V& operator[](K key) noexcept
    {
        return m_values[static_cast<array_size_t>(key)];
    }

    constexpr const V& operator[](K key) const noexcept
    {
        return m_values[static_cast<array_size_t>(key)];
    }

    template<typename T>
    class base_iterator : public std::iterator<std::forward_iterator_tag, V>
    {
    public:
        using index_type = typename std::underlying_type<K>::type;

        constexpr base_iterator(T* p, index_type i) noexcept
            : value(p), index(i)
        {
        }

        struct node {
            K first;
            T& second;

            constexpr node(K f, T& s) noexcept : first(f), second(s)
            {
            }

            constexpr node& operator=(const node&) = delete;

            constexpr void set(const T& t) noexcept
            {
                second = t;
            }
        };

        constexpr node operator*() noexcept
        {
            return node{first(), second()};
        }

        constexpr explicit operator bool() const noexcept
        {
            return index >= 0 && index < LENGTH;
        }

        constexpr K first() const noexcept
        {
            return static_cast<K>(index);
        }

        constexpr T& second() noexcept
        {
            return *(value + index);
        }

        constexpr base_iterator& operator++() noexcept
        {
            ++index;
            return *this;
        }

        constexpr bool operator!=(const base_iterator& other) const noexcept
        {
            return index != other.index;
        }

        constexpr bool operator==(const base_iterator& other) const noexcept
        {
            return index == other.index;
        }

    private:
        T* value;
        index_type index;
    };

    using iterator = base_iterator<V>;
    using const_iterator = base_iterator<const V>;
    using node = typename iterator::node;
    using cnode = typename const_iterator::node;

    constexpr iterator find(K key) noexcept
    {
        return {m_values.data(), key};
    }

    constexpr const_iterator find(K key) const noexcept
    {
        return {m_values.data(), key};
    }

    constexpr iterator begin() noexcept
    {
        return {m_values.data(), 0};
    }

    constexpr iterator end() noexcept
    {
        return {m_values.data(), LENGTH};
    }

    constexpr const_iterator begin() const noexcept
    {
        return {m_values.data(), 0};
    }

    constexpr const_iterator end() const noexcept
    {
        return {m_values.data(), LENGTH};
    }

    constexpr std::array<V, LENGTH>& values() noexcept
    {
        return m_values;
    }

    constexpr const std::array<V, LENGTH>& values() const noexcept
    {
        return m_values;
    }

private:
    std::array<V, LENGTH> m_values;
};
} // namespace jrc
