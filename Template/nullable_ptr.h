//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
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
#include <functional>
#include <type_traits>

namespace jrc
{
template<typename T>
class nullable_ptr
{
public:
    template<typename R,
             typename = std::enable_if_t<std::is_base_of<R, T>::value ||
                                         std::is_base_of<T, R>::value>>
    constexpr nullable_ptr(R* r_val) : val(static_cast<T*>(r_val))
    {
    }

    template<typename R,
             typename = std::enable_if_t<std::is_base_of<R, T>::value ||
                                         std::is_base_of<T, R>::value>>
    constexpr nullable_ptr(nullable_ptr<R> r_opt) : nullable_ptr(r_opt.get())
    {
    }

    constexpr nullable_ptr(T* p) : val(p)
    {
    }

    constexpr nullable_ptr(T& p) : val(&p)
    {
    }

    constexpr nullable_ptr() : val(nullptr)
    {
    }

    explicit operator bool() const
    {
        return val != nullptr;
    }

    T* get() const
    {
        return val;
    }

    T* operator->() const
    {
        return val;
    }

    T& operator*() const
    {
        return *val;
    }

private:
    T* val;
};
} // namespace jrc
